#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choicedialog.h"
#include "friendmessage.h"
#include "autocleartextedit.h"
#include "dialog.h"


MainWindow::MainWindow(const QString accountNumber0, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    accountNumber(accountNumber0),
    socket(new QTcpSocket(this)),
    friendModel(new QStandardItemModel(this)),
    aiChat(new AIChat(this)),
    isAIChatActive(false)
{
    ui->setupUi(this);
    //初始化窗口
    setWindowIcon(QIcon(":/pictures/suliao.png"));
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //初始化头像
    initAvatar();
    //设置好友列表模型
    setupFriendList();
    //设置聊天列表模型
    setupTalkList();
    //设置左下角按钮菜单
    setupMenu();
    //在本地建立或初始化该用户的数据库
    createUserDatabase(accountNumber);
    //连接到服务器
    tcpConnect();
    //连接 socket的信号槽
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    //连接聊天列表右键菜单和窗体的信号槽
    connect(ui->list_talks, &TalkList::choiceDone, this, &MainWindow::listtalkChoice);
    //设置聊天页面先不显示好友名字
    ui->lab_friendname->setText("");
    //设置聊天页面先不可输入
    ui->edit_input->setEnabled(false);
    //连接聊天输入框的回车和提交
    connect(ui->edit_input, &EnterTextEdit::enterKey, [this] (){
        if(ui->but_send->isEnabled() && !ui->edit_input->toPlainText().isEmpty()){
            ui->but_send->click();}
    });
    //连接保存文件完成后弹窗
    connect(this, &MainWindow::saveDone, [this] (const QString &status){
        Dialog dialog(this);
        dialog.transText(status);
        dialog.exec();
    });
    //创建右边用户信息主布局
    newLayout = new QVBoxLayout(ui->edit_show2);
    newLayout->setAlignment(Qt::AlignCenter); // 主布局居中
    //设置消息框与好友列表不显示左右的滑块
    ui->list_talks->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->list_friends->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //连接左上角头像和弹出修改信息窗口的信号槽
    connect(ui->lab_avator, &LabelAva::changeInfo, this, &MainWindow::changeInfo);
    //通过定时器延迟
    QTimer::singleShot(150, this, [this, accountNumber = this->accountNumber]() {
        //发送已经登录的信号
        havelogin(accountNumber);//收到回复后依次调用加载消息列表 加载聊天记录
        //设置按钮位置
        positionSendButton();
        //窗口操作
        raise();
        activateWindow();
    });

    setupAIChat();
}

void MainWindow::setupAIChat()
{
    // 连接AI聊天信号和槽
    connect(aiChat, &AIChat::receivedAIResponse, this, &MainWindow::handleAIResponse);
    connect(aiChat, &AIChat::errorOccurred, this, &MainWindow::handleAIError);

    // 从配置文件读取API密钥（如果有）
   // QSettings settings("settings.ini", QSettings::IniFormat);
    QString apiKey = "sk-qiqmjdxitvxseptudymlvzsipmimxbufvxeixsqiwunoapbe";
    if (!apiKey.isEmpty()) {
        aiChat->setApiKey(apiKey);
    }

    // 设置默认模型
    aiChat->setModel("deepseek-ai/DeepSeek-V3");
}

void MainWindow::toggleAIChat()
{
    isAIChatActive = true;

    if (isAIChatActive) {
        // 切换到AI聊天模式
        currentChatTarget = "AI助手";

        bool aiInList =false;
        QAbstractItemModel *model= ui->list_talks->model();
        if(model){
            int rowCount = model->rowCount();
            for(int i=0;i< rowCount;i++)
            {
                QModelIndex index = model->index(i,0);
                QString itemData = model->data(index,Qt::UserRole+1).toString();
                if (itemData == "AI助手") {
                    aiInList = true;
                    ui->list_talks->setCurrentIndex(index);
                    break;
                }
            }
        }

        // 如果AI助手不在聊天列表中，添加它
        if (!aiInList) {
            // 创建AI助手的AccountInfo
            AccountInfo aiInfo;
            aiInfo.account = "AI助手";
            aiInfo.name = "AI助手";
            aiInfo.signature = "我是您的AI助手，有什么可以帮您的？";
            aiInfo.gender = "未知";

            // 使用默认头像
            QPixmap aiAvatar(":/pictures/suliao_avator_normal.jpg");
            aiInfo.avator_base64 = pixmapToBase64(aiAvatar);

            // 添加到聊天列表  然后再次调用toggleAIChat()这样就能实现页面的切换
            addSomeoneToTalkList(aiInfo, "点击开始与AI对话", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "0");
            toggleAIChat();
        }
        // 检查AI助手是否在好友列表中
        if (!friendHash.contains("AI助手")) {
            // 如果AI助手不在好友列表中，添加它
            AccountInfo aiInfo;
            aiInfo.account = "AI助手";
            aiInfo.name = "AI助手";
            aiInfo.signature = "我是您的AI助手，有什么可以帮您的？";
            aiInfo.gender = "未知";

            // 使用默认头像
            QPixmap aiAvatar(":/pictures/suliao_avator_normal.jpg");
            aiInfo.avator_base64 = pixmapToBase64(aiAvatar);

            // 添加到好友列表
            friendHash.insert("AI助手", aiInfo);
        }
        // 创建或切换到AI聊天页面
        if (!switchPageTo("AI助手")) {
            // 如果切换失败，可能是因为聊天列表为空或其他原因
            // 可以在这里添加一些错误处理
            qDebug() << "切换到AI聊天页面失败";
            isAIChatActive = false;
            return;
        }

        // 更新UI提示用户正在AI聊天模式
        ui->but_tool4->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(0, 123, 255, 0.3);"
            "    border: none;"
            "    color: black;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(0, 123, 255, 0.5);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.7);"
            "}"
            );
    } else {
        // 切换回普通聊天模式
        ui->but_tool4->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "    color: black;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(0, 0, 0, 0.1);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 0, 0, 0.2);"
            "}"
            );
    }
}

void MainWindow::handleAIResponse(const QString &response)
{
    // 确保我们在AI聊天模式
    if (isAIChatActive && currentChatTarget == "AI助手") {
        // 获取当前聊天页面
        QWidget *currentPage = ui->stack_talks->currentWidget();
        if (currentPage) {
            // 添加AI回复到聊天界面
            addMessageTo(currentPage,
                         "AI助手",
                         "text",
                         response,
                         QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ss"));

            // 更新聊天列表中的最新消息
            updateTalkListFree("AI助手", response, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
    }
}
void MainWindow::handleAIError(const QString &errorMessage)
{
    // 显示错误消息
    if (isAIChatActive) {
        Dialog* dialog = new Dialog(this);
        dialog->transText("AI聊天出错: " + errorMessage);
        dialog->exec();
    }
}
void MainWindow::on_but_tool4_clicked()
{
    // 切换AI聊天模式
    qDebug() << "AI聊天按钮被点击";
    toggleAIChat();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initAvatar()//初始化头像
{
    //获取运行时目录
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString saveLoginAvatorPath = dir.filePath("laseloginavator.png");
    QPixmap pixmap(saveLoginAvatorPath);
    if (pixmap.isNull()) {
        pixmap = QPixmap(":/pictures/suliao.png");
    } else {}
    ui->lab_avator->setPixmap(pixmap);
    ui->lab_avator->setScaledContents(true);
    ui->lab_avator->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void MainWindow::setupFriendList()//设置好友列表模型
{
    // //设置列表 : 设置列表视图 (QListView) 本身的属性和行为。
    // 设置视图模式为列表模式。
    // QListView::ListMode 是 QListView 的一种显示模式，它将项目排列成一个简单的垂直列表。
    ui->list_friends->setViewMode(QListView::ListMode);

    // 设置用户的选择行为。
    // QAbstractItemView::SelectRows 表示当用户点击列表中的任何位置时，选择整个行，而不是只选择单个项目或单元格。
    ui->list_friends->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 设置用户的选择模式。
    // QAbstractItemView::SingleSelection 表示用户一次只能选择一个项目。
    ui->list_friends->setSelectionMode(QAbstractItemView::SingleSelection);

    // 设置项目的编辑触发方式。
    // QAbstractItemView::NoEditTriggers 表示用户无法通过交互方式（如双击或按 F2 键）开始编辑列表中的项目。项目只能通过程序代码进行编辑。
    ui->list_friends->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置自定义的委托 (Delegate) 用于绘制列表项。
    // QAbstractItemDelegate 是负责渲染（绘制）列表中的单个项目以及管理编辑器的类。
    // FriendDelegate 是一个自定义的委托类（继承自 QAbstractItemDelegate 或其子类）。
    // 原理：由于好友列表的每个项目可能包含头像、昵称、签名等复杂布局，QLabel 默认的绘制方式无法满足需求，所以需要自定义一个委托来负责按照特定样式绘制每一个好友项目。
    ui->list_friends->setItemDelegate(new FriendDelegate(this));

    // //设置项 : 创建数据模型 (Model) 和模型中的项目 (Item)。
    // 创建一个标准的项目模型 (QStandardItemModel)。
    // QStandardItemModel 是 Qt 提供的一个简单的、基于项目的标准模型，它以树状结构存储 QStandardItem 对象。
    // 这个模型将作为数据的来源，为 QListView 提供需要显示的数据。
    // this (MainWindow) 被设置为模型的父对象，以便 Qt 的对象树管理内存。
    friendModel = new QStandardItemModel(this);

    // 将创建的 friendModel 模型设置到列表视图中。
    // 在设置代理模型之前先设置源模型，虽然通常会设置代理模型，但这步是先关联原始数据模型。
    ui->list_friends->setModel(friendModel);

    // 创建三个 QStandardItem 对象，分别代表列表中的几个固定或特殊项目。
    // “新的朋友”可能用于显示添加好友的入口。
    // “好友列表”可能用于作为下方实际好友列表的分组标题。
    QStandardItem *addFriendItem0 = new QStandardItem("新的朋友"); // 第一个“新的朋友”项
    QStandardItem *addFriendItem = new QStandardItem("新的朋友"); // 第二个“新的朋友”项
    QStandardItem *friendListItem = new QStandardItem("好友列表"); // “好友列表”项

    // //设置不可选标志 : 设置这些特殊项目的交互标志。
    // 设置 addFriendItem0 (第一个“新的朋友”) 的标志为 Qt::NoItemFlags (0)。
    // 原理：Qt::NoItemFlags 表示该项目没有任何标志位被设置，特别是 Qt::ItemIsSelectable 标志被禁用。这意味着这个项目**不能**被用户选中，它可能只作为一个静态的标题或提示。
    addFriendItem0->setFlags(Qt::NoItemFlags);

    // 设置 addFriendItem (第二个“新的朋友”) 的标志。
    // Qt::ItemIsEnabled: 项目是启用的，可以交互。
    // Qt::ItemIsSelectable: 项目可以被用户选中。
    // 原理：这个项目是可以被用户点击选中的，用于触发“添加朋友”的操作。
    addFriendItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    // 设置 friendListItem (“好友列表”) 的标志为 Qt::NoItemFlags。
    // 原理：这个项目也不能被用户选中，只作为一个静态的分组标题。
    friendListItem->setFlags(Qt::NoItemFlags);

    // //获取代表添加好友的图标 : 为可选择的“新的朋友”项准备图标数据。
    // 从 Qt 资源文件中加载一个图片作为图标。
    // ":/pictures/addfriends.jpg" 是一个资源路径，表示图片文件被嵌入到应用程序的资源中。
    QPixmap addFriendPixmap(":/pictures/addfriends.jpg"); // 加载图片到 QPixmap

    // //转换为 QPixmap，指定图标的大小 : 这行注释是错误的，它前面已经是一个 QPixmap 了。
    // 将 QPixmap 转换为 QImage。
    // QImage 是 Qt 处理图片的另一个类，通常用于像素级的访问和处理，是设备无关的。
    // 原理：某些操作（如保存到特定格式）可能需要 QImage 类型。
    QImage image = addFriendPixmap.toImage();

    // 创建一个 QBuffer 对象。QBuffer 允许你在内存中的 QByteArray 上像文件一样进行读写操作。
    QBuffer buffer;
    // 以只写模式打开缓冲区。
    buffer.open(QIODevice::WriteOnly);

    // 将 QImage 的图像数据保存到缓冲区中，并指定格式为 PNG。
    // 原理：将图片数据编码为 PNG 格式的二进制数据，存储在 QBuffer 内部关联的 QByteArray 中。
    image.save(&buffer, "PNG"); // 将图像保存为 PNG 格式

    // 从缓冲区获取保存好的 PNG 格式的二进制数据。
    // buffer.data() 返回 QBuffer 内部关联的 QByteArray 的引用。
    QByteArray byteArray = buffer.data();
    buffer.close(); // 关闭缓冲区。

    // 将 PNG 二进制数据进行 Base64 编码。
    // Base64 编码将二进制数据转换为只包含 ASCII 可打印字符的字符串，适合嵌入到文本数据（如 JSON）或某些需要在文本环境中传输的场景。
    // 原理：这里将图标数据编码为 Base64 字符串，以便存储到模型的项目中。
    QString base64String = byteArray.toBase64();

    // //将 Base64 字符串设置到 addFriendItem 中 : 将 Base64 编码后的图标数据存储到模型项目中。
    // 将 Base64 字符串数据设置到 addFriendItem (可选择的“新的朋友”) 中，使用一个自定义的角色 (Role)。
    // setData() 方法允许在 QStandardItem 中存储不同“角色”的数据。Qt 定义了标准角色（如 Qt::DisplayRole 用于文本，Qt::DecorationRole 用于图标）。
    // Qt::UserRole 是用户自定义角色的起始值。Qt::UserRole + 4 是一个应用程序自定义的角色 ID。
    // 原理：这里不使用标准的 Qt::DecorationRole 直接设置 QIcon，而是将 Base64 字符串存入一个自定义角色。这意味着前面设置的 FriendDelegate 必须能够识别 Qt::UserRole + 4 这个角色，从数据中取出 Base64 字符串，解码为图片，然后自己绘制图标。这是一个自定义的实现方式。
    addFriendItem->setData(base64String, Qt::UserRole + 4);

    // //将项添加到模型中 : 将创建的项目添加到 QStandardItemModel 中作为行。
    // 按照顺序将项目添加到模型的顶部（第一行、第二行、第三行）。
    friendModel->appendRow(addFriendItem0); // 添加第一个非可选的“新的朋友”
    friendModel->appendRow(addFriendItem);   // 添加第二个可选的“新的朋友”
    friendModel->appendRow(friendListItem); // 添加“好友列表”标题
    // 实际的好友项目会在后续代码中动态添加到 friendModel 中（可能添加到 friendListItem 下方作为子项目，或者直接添加到后面，取决于实际设计）。

    // //初始化模型 : 设置代理模型。
    // 创建一个 FilterProxyModel 的实例。
    // FilterProxyModel 看起来是继承自 QSortFilterProxyModel 的一个自定义代理模型。
    // 原理：代理模型位于源模型 (friendModel) 和视图 (ui->list_friends) 之间。它可以对源模型中的数据进行排序、过滤或分组，并将处理后的数据呈现给视图。这使得可以在不修改原始数据模型的情况下改变视图的显示内容和顺序。
    filterProxyModel = new FilterProxyModel(this);

    // 将 friendModel 设置为 filterProxyModel 的源模型。
    // 原理：filterProxyModel 现在将从 friendModel 获取数据，并在提供给视图之前对其进行潜在的处理。
    filterProxyModel->setSourceModel(friendModel);

    // //设置模型到列表视图 : 将代理模型设置到视图。
    // 将 filterProxyModel 设置到列表视图中。
    // 原理：视图现在显示的是 filterProxyModel 处理后的数据，而不是直接显示 friendModel 的原始数据。
    ui->list_friends->setModel(filterProxyModel);

    // //连接信号与槽 : 连接视图的选择模型信号到槽函数，以响应用户选择项目。
    // 获取列表视图关联的选择模型 (QItemSelectionModel)。这个模型管理视图中项目的选中状态和当前项目。
    // 连接 selectionModel 的 currentChanged 信号。这个信号在视图的“当前项目”改变时发射（例如用户点击了不同的项目）。它带有新的当前项目的模型索引。
    // 连接到 MainWindow 的 onFriendItemCurrentChanged 槽函数。
    // 原理：当用户在好友列表中选择（点击）不同的项目时，会触发这个连接，导致 onFriendItemCurrentChanged 槽函数被调用。您可以在这个槽函数中根据用户选择的项目来执行相应的操作
    //（例如，如果选择了可点击的“新的朋友”项，就弹出添加好友对话框；如果选择了实际的好友项，就打开聊天窗口）。
    connect(ui->list_friends->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onFriendItemCurrentChanged);
}

void MainWindow::setupTalkList()//设置聊天列表模型
{
    //设置列表
    ui->list_talks->setViewMode(QListView::ListMode);//设置视图模式为列表模式
    ui->list_talks->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为为选择整行
    ui->list_talks->setSelectionMode(QAbstractItemView::SingleSelection);//设置选择模式为单选
    ui->list_talks->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->list_talks->setItemDelegate(new TalkDelegate(this));//设置自定义的委托用于绘制列表项
    //设置标准模型
    talkModel = new QStandardItemModel(this);
    //初始化过滤代理模型
    talkFilterProxyModel = new TalkFilterProxyModel(this);
    talkFilterProxyModel->setSourceModel(talkModel);
    //设置模型到列表视图
    ui->list_talks->setModel(talkFilterProxyModel);
    //连接信号与槽
    connect(ui->list_talks->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onTalkItemCurrentChanged);
}


void MainWindow::setupMenu()//设置左下角按钮菜单
{
    //初始化
    toolMenu = new QMenu(this);
    toolMenu->setStyleSheet(
        "QMenu {"
        "   background-color: rgb(240, 240, 240);"
        "   border-radius: 3px;"
        "   border: 0.5px solid rgb(0, 0, 0);"
        "}"
        "QMenu::item {"
        "   padding: 10px;"
        "   text-align: left;"
        "}"
        "QMenu::item:selected {"
        "   background-color: rgb(200, 200, 200);"
        "}"
        "QMenu::item:pressed {"
        "   background-color: rgb(180, 180, 180);"
        "}");
    //添加菜单项
    QAction *action1 = toolMenu->addAction("修改密码");
    QAction *action2 = toolMenu->addAction("注销账号");
    QAction *action3 = toolMenu->addAction("退出账号");
    // 连接第一个菜单项的修改密码triggered 信号
    connect(action1, &QAction::triggered, this, [this]() {//修改密码
        if( changePassFlag == 0){
            dialogChangePass = new ChangePassword(this);
            connect(dialogChangePass,&ChangePassword::changePassword1,this,&MainWindow::changePassword1);
            connect(dialogChangePass,&ChangePassword::changePassword2,this,&MainWindow::changePassword2);
            connect(this,&MainWindow::changePasswordAnswer1,dialogChangePass,&::ChangePassword::changePasswordAns1);
            connect(this,&MainWindow::changePasswordAnswer2,dialogChangePass,&::ChangePassword::changePasswordAns2);
            connect(dialogChangePass, &ChangePassword::customClose, this, [this]() {
                qDebug()<<"修改密码窗口重置了";
                changePassFlag = 0;
                dialogChangePass->disconnect();
            });
            dialogChangePass->show();
            changePassFlag  = 1;
        }
        else if(changePassFlag  == 1){
            dialogChangePass->raise();
            if (dialogChangePass) {
                int x = this->x() + (this->width() - dialogChangePass->width()) / 2;
                int y = this->y() + (this->height() - dialogChangePass->height()) / 2;
                dialogChangePass->move(x, y);
            }
        }
    });
    // 连接第二个菜单项注销的triggered 信号
    connect(action2, &QAction::triggered, this, [this]() {//修改密码
        if( logoutFlag == 0){
            logout = new Logout(this);
            connect(logout, &Logout::logout, this, &MainWindow::goLogout);
            connect(this, &MainWindow::logoutAnswer, logout, &Logout::logoutAnswer);
            connect(logout, &Logout::customClose, this, [this]() {
                qDebug()<<"注销窗口重置了";
                logoutFlag = 0;
                logout->disconnect();
            });
            logout->show();
            logoutFlag  = 1;
        }
        else if(logoutFlag  == 1){
            logout->raise();
            if (logout) {
                int x = this->x() + (this->width() - logout->width()) / 2;
                int y = this->y() + (this->height() - logout->height()) / 2;
                logout->move(x, y);
            }
        }
    });
    // 连接第三个菜单项退出的triggered 信号
    connect(action3, &QAction::triggered, this, [this]() {//修改密码
        ChoiceDialog dialog(this);
        dialog.transText("你确定要退出账号吗？");
        dialog.transButText("确定", "取消");
        int result = dialog.exec();
        if(result == QDialog::Accepted){
            close();
        }
    });
    //连接按钮的 clicked 信号
    connect(ui->but_set, &QPushButton::clicked, [=]() {
        //获取按钮的坐标，并将菜单显示在按钮的下方
        QPoint pos = ui->but_set->mapToGlobal(QPoint(0, ui->but_set->height() - toolMenu->height()));
        toolMenu->exec(pos);
    });
}


QString MainWindow::pixmapToBase64(const QPixmap &pixmap)//图片转换成base64格式
{
    QImage image = pixmap.toImage();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray byteArray = buffer.data();
    return QString(byteArray.toBase64());
}


QPixmap MainWindow::base64ToPixmap(const QString &base64Str)//将base64转换为图片
{
    QByteArray byteArray = QByteArray::fromBase64(base64Str.toUtf8());
    QImage image;
    //尝试加载PNG格式
    if (!image.loadFromData(byteArray, "PNG")) {
        //如果PNG加载失败，尝试JPG格式
        image.loadFromData(byteArray, "JPG");
    }
    return QPixmap::fromImage(image);
}


void MainWindow::positionSendButton()//设置按钮位置
{
    ui->but_send->move(QPoint(this->width() - ui->but_send->width() - 93,
                              this->height() - ui->but_send->height() - 40));
}


void MainWindow::paintEvent(QPaintEvent *event)//窗口背景
{
    //调用基类方法以处理父类的绘图
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    //加载背景图片
    QPixmap background(":/pictures/094 Cloudy Apple - trans.png");
    //确保图片已加载
    if (background.isNull()) {
        qWarning("背景图像加载失败");
        return;//如果加载失败，直接返回
    }
    QSize newSize = QSize(this->width(), this->height());
    //将背景图缩放为新的大小
    QPixmap scaledBackground = background.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //计算绘制的位置，保持居中
    int x = (this->width() - scaledBackground.width()) / 2;
    int y = (this->height() - scaledBackground.height()) / 2;
    //设置剪裁区域以适应圆角窗口
    int radius = 10;//圆角半径
    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    painter.setClipPath(path);
    //绘制背景图片
    painter.drawPixmap(x, y, scaledBackground);
}


void MainWindow::createUserDatabase(const QString& account)//在本地建立或初始化该用户的数据库
{
    QString dbName = account + ".db";
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qDebug() << "打开数据库失败" << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS messages ("  //存储消息记录
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "sender VARCHAR(20),"
               "receiver VARCHAR(20),"
               "messagetype VARCHAR(20)," //picture,text,document
               "message LONGTEXT,"
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");
    
    /*
    QString str = pixmapToBase64(ui->lab_avator->pixmap());
    query.prepare("INSERT INTO messages (sender, receiver, messagetype, message) "
                  "VALUES (:sender, :receiver, 'picture', :message)");
    query.bindValue(":sender", "1426690995");
    query.bindValue(":receiver", "1416179884");
    query.bindValue(":message", str);
    query.exec();
*/
    
    query.exec("CREATE TABLE IF NOT EXISTS talks ("   //存储已经打开了哪些聊天
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "friend_id VARCHAR(20),"
               "unread INT DEFAULT 0,"
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)");
    qDebug() << "用户本地数据库名称:" << db.databaseName();
    QStringList tables = db.tables();
    qDebug() << "所有表名:";
    for (const QString& table : tables) {
        qDebug() << table;
    }
}


void MainWindow::setupMessageList(QListWidget *list)//设置消息页面模型
{
    //设置列表
    list->setViewMode(QListView::ListMode);//设置视图模式为列表模式
    list->setSelectionMode(QAbstractItemView::NoSelection);//不可选择
    list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);//按每次一个像素滚动
    list->setStyleSheet(
        "QListWidget { background: transparent; border: none; outline: none; } "
        "QListWidget::item { background: transparent; border: none; outline: none; } "
        "QListWidget::item:hover { background-color: transparent; } "
        "QListWidget::item:focus { outline: none; }"
        );
    list->setContentsMargins(0, 0, 0, 0);
    list->setSpacing(0);
}


bool MainWindow::tcpConnect()//连接服务器
{
    if (socket->state() != QAbstractSocket::ConnectedState){
        socket->connectToHost("127.0.0.1", 1999);//修改这个地方
        if (socket->waitForConnected(1000)) {
            qDebug() << "主窗口连接服务器成功";
            return true;
        } else {
            qDebug() << "主窗口连接服务器失败"<<socket->errorString();
            return false;
        }
    }
    return true;
}


void MainWindow::havelogin(const QString& account)//发送初始化请求
{
    if(!checkSocket()){
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "loginfirst";
    jsonObj["account"] = account;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    qDebug()<<"发送了初始化请求";
}


QPixmap MainWindow::getRoundedPixmap(const QPixmap srcPixmap, const int radius)//获得圆角图片
{
    //目标图片尺寸
    QSize desSize = srcPixmap.size();
    //新建一个目标大小的画布Qpixmap
    QPixmap desPixMap(desSize);
    //填充透明色作为背景
    desPixMap.fill(Qt::transparent);
    //以QPixmap 为绘画背景进行画笔绘制
    QPainter painter(&desPixMap);
    painter.setRenderHints(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);//平滑像素图变换
    QPainterPath path;//绘制路径
    //绘制圆角矩形，其中最后两个参数值的范围为（0-99），就是圆角的px值
    path.addRoundedRect(0, 0, desSize.width(), desSize.height(), radius, radius);
    //将绘制的圆角矩形路径中内容进行裁剪
    painter.setClipPath(path);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    //将图片绘制到desPixmap中，IgnoreAspectRatio忽视图片比例
    painter.drawPixmap(0, 0, desSize.width(), desSize.height(), srcPixmap.scaled(desSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter.setClipping(false);//关闭裁剪
    return desPixMap;
}



bool MainWindow::checkSocket()//检查是不是连接上服务器了
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox msgBox;
        QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(icon);
        msgBox.setWindowTitle("请求失败");
        msgBox.setText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
        msgBox.exec();
        return false;
    }
    return true;
}


bool MainWindow::deleteSomeoneInFriendList(const QString &account)//清理好友列表中的某个好友
{
    friendHash.remove(account);
    //获取源模型
    QAbstractItemModel *sourceModel = filterProxyModel->sourceModel();
    if (sourceModel) {
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            QModelIndex index = sourceModel->index(row, 0);
            QVariant data = sourceModel->data(index, Qt::UserRole + 1);
            QString string = data.toString();
            //匹配上则删除
            if (string == account) {
                sourceModel->removeRow(row);
                ui->list_friends->update();
                return true;
            }
        }
    }
    return false;
}

bool MainWindow::MainWindow::deleteSomeoneInTalkList(const QString &account)//删除聊天列表中的某个聊天
{
    // // 清理 hash : 删除内部缓存的数据。
    // 假设 messageListHash 是一个 QHash 或 QMap 成员变量，用于存储与每个聊天对象关联的消息列表或其他聊天数据。
    // 查找是否存在以要删除的账号 (account) 为键的条目。find() 返回一个迭代器，end() 返回一个无效迭代器。
    if(messageListHash.find(account) != messageListHash.end()){
        // 如果找到了对应的条目，将其从哈希表中移除。
        // 原理：确保内部缓存与即将删除的聊天列表项保持同步，避免内存泄漏或访问不存在的数据。
        messageListHash.remove(account);
    }

    // 检查当前聊天列表中选中的项是否是即将被删除的聊天项。
    // ui->list_talks->currentIndex() 获取 QListView 中当前选中项的模型索引 (QModelIndex)。
    // .data(Qt::UserRole + 1) 获取该选中项在自定义角色 Qt::UserRole + 1 中存储的数据（根据之前的代码，这很可能是好友的账号）。
    // == account 将选中项的账号与要删除的账号进行比较。
    if(ui->list_talks->currentIndex().data(Qt::UserRole + 1) == account){
        // 如果当前正在查看或选中的聊天就是要删除的聊天，则切换到 stacked widget 的第一个页面。
        // ui->stack_talks 假设是一个 QStackedWidget，用于在不同的聊天界面之间切换显示。
        // setCurrentIndex(0) 将 stacked widget 的显示切换到索引为 0 的子控件，这通常是一个空白页或默认提示页。
        // 原理：避免在删除当前正在查看的聊天后，界面显示混乱或崩溃。切换到一个默认状态。
        ui->stack_talks->setCurrentIndex(0);
    }

    // // 获取源模型 : 访问代理模型背后的原始数据模型。
    // 从 talkFilterProxyModel 代理模型中获取其源模型 (Source Model) 的指针。
    // talkFilterProxyModel 假设是 QSortFilterProxyModel 的子类，用于管理聊天列表 ui->list_talks 的数据。
    // 原理：QSortFilterProxyModel 本身不存储数据，数据存储在源模型中。要直接修改数据（如删除行），需要操作源模型。
    QAbstractItemModel *sourceModel = talkFilterProxyModel->sourceModel();

    // 检查获取到的源模型指针是否有效。
    if (sourceModel) {
        // 遍历源模型中的所有行。
        // sourceModel->rowCount() 获取源模型中的总行数。
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            // 获取当前行 (row) 在第一列 (0) 的模型索引。
            QModelIndex index = sourceModel->index(row, 0);

            // 从当前行模型索引中获取 Qt::UserRole + 1 角色的数据，并存储在 QVariant 中。
            // 这个角色通常存储了聊天对象的账号 ID。
            QVariant data = sourceModel->data(index, Qt::UserRole + 1);

            // 将获取到的数据（账号 ID）转换为 QString。
            QString string = data.toString();

            // // 匹配上则删除 : 检查当前行的账号是否是要删除的账号。
            if (string == account) {
                // 如果匹配上了，说明找到了要删除的聊天项。

                // 请求视图更新。这可能用于在删除前强制视图重绘一次，但通常在删除行后视图会自动更新。这里可能并非严格必要。
                ui->list_talks->update();

                // 创建一个 QSqlQuery 对象，用于执行数据库操作。
                // db 假设是已连接的数据库连接对象（可能从连接池获取）。
                QSqlQuery qry(db);
                // 准备 SQL DELETE 语句：从 talks 表中删除 friend_id 匹配指定账号 (account) 的记录。
                // 原理：删除这个好友的所有聊天记录。
                qry.prepare("DELETE FROM talks WHERE friend_id = :friend_id");
                // 绑定参数：将要删除的账号绑定到占位符。
                qry.bindValue(":friend_id", account);
                // 执行 DELETE 语句。这里的返回值未被检查，可能需要添加错误处理。
                qry.exec();

                // 再次检查当前选中的项是否是要删除的项。这段检查与函数开头的检查重复了。
                // 如果当前选中的是这个聊天，并且前面的代码已经切换了 stacked widget，这里可能只是为了额外的确认或处理。
                if(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString() == account){
                    qDebug()<<"当前聊天的人的对话框被删除"; // 打印调试信息。
                    // 清空显示当前聊天对象名称的 QLabel。
                    // ui->lab_friendname 假设是显示当前聊天对象昵称的 QLabel。
                    ui->lab_friendname->setText("");
                }

                // 从**源模型**中移除当前行 (row)。
                // 原理：这是从列表中删除项目的关键步骤。当源模型中的行被移除后，与其关联的代理模型 (talkFilterProxyModel) 会收到通知并更新其内部结构，进而通知视图 (ui->list_talks) 更新显示，从而移除列表项。
                // 注意：在一个正向循环中移除列表项后，后续循环的索引会受到影响（后面的项会前移）。由于这里在删除后立即返回，所以不会导致跳过紧邻的下一个元素的问题。
                sourceModel->removeRow(row);

                // 清除视图中的所有选择。
                // 原理：删除当前选中的项目后，清除选择状态，避免视图中仍然显示一个已删除项的“选中”状态。
                ui->list_talks->clearSelection();

                // 返回 true，表示成功找到了要删除的聊天项并已执行删除操作。
                return true;
            }
        }
        // 如果循环结束都没有找到匹配的账号
    }
    // 如果 sourceModel 无效（例如 talkFilterProxyModel 没有设置源模型）或循环结束都没有找到匹配的账号。
    // 返回 false，表示没有找到要删除的聊天项。
    return false;
}

bool MainWindow::ifTalkHaveOpened(const QString &account)//判断某个人的聊天是否已经在消息列表中了
{
    //获取源模型
    QAbstractItemModel *sourceModel = talkFilterProxyModel->sourceModel();
    if (sourceModel) {
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            QModelIndex index = sourceModel->index(row, 0);
            QVariant data = sourceModel->data(index, Qt::UserRole + 1);
            QString string = data.toString();
            //匹配上则返回
            if (string == account) {
                return true;
            }
        }
    }
    return false;
}


int MainWindow::addSomeoneToTalkList(const AccountInfo &friendMessage, QString message, QString date, QString unread)
//向聊天列表加入某个好友
{
    //检查好友是否已经存在
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *item = talkModel->item(row);
        if (item->data(Qt::UserRole + 1).toString() == friendMessage.account) {
            //好友已经存在，返回其索引
            return row;//返回已有项的索引
        }
    }
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM talks WHERE friend_id = :friend_id");
    qry.bindValue(":friend_id", friendMessage.account);
    qry.exec();
    if(!qry.next()){//如果没有记录则插入
        qry.prepare("INSERT INTO talks (friend_id) VALUES (:friend_id)");
        qry.bindValue(":friend_id", friendMessage.account);
        qry.exec();
    }
    QStandardItem *item = new QStandardItem();
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //设置标准项的显示文本为好友的昵称
    item->setText(friendMessage.name);
    //设置标准项为不可编辑状态
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);//设置为不可编辑
    //将好友的信息存储在标准项的用户角色数据中
    item->setData(friendMessage.account, Qt::UserRole + 1);//存储账号
    item->setData(friendMessage.gender, Qt::UserRole + 2);//存储性别
    item->setData(friendMessage.signature, Qt::UserRole + 3);//存储个性签名
    item->setData(friendMessage.avator_base64, Qt::UserRole + 4);//存储头像的Base64字符串
    item->setData(message, Qt::UserRole + 5);//存储上一条聊天信息
    item->setData(date, Qt::UserRole + 6);//存储上次沟通日期
    item->setData(unread, Qt::UserRole + 10);//存储未读消息个数
    //将创建的标准项添加到好友模型中
    talkModel->appendRow(item);
    //添加到消息列表的hash中
    messageListHash.insert(friendMessage.account, friendMessage);
    //返回新添加项的索引
    return talkModel->rowCount() - 1; //返回最后一项的索引
}


int MainWindow::selectSomeoneInTalkList(const QString &account)//打开某个人的聊天框
{
    ui->line_serach->setText("搜索");
    ui->line_serach->clearFocus();
    //获取源模型
    QAbstractItemModel *sourceModel = talkFilterProxyModel->sourceModel();
    if (sourceModel) {
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            QModelIndex index = sourceModel->index(row, 0);
            QVariant data = sourceModel->data(index, Qt::UserRole + 1);
            QString string = data.toString();
            //匹配上则返回
            if (string == account) {
                //获取过滤后的索引
                QModelIndex proxyIndex = talkFilterProxyModel->mapFromSource(index);
                if (proxyIndex.isValid()) {
                    ui->list_talks->setCurrentIndex(proxyIndex);
                }
                return row;
            }
        }
    }
    return -1;
}


void MainWindow::sendMessageToFriend(const QString &account)//给某个好友发送消息
{
    if(friendHash.find(account) != friendHash.end()){//确定有这个好友
        AccountInfo info = friendHash[account];
        addSomeoneToTalkList(info, NULL, NULL, NULL);
        selectSomeoneInTalkList(account);
        ui->but_chat->click();
    }
    else{//没有这个好友
        Dialog dialog(this);
        dialog.transText("他不在您的好友列表当中!");
        dialog.exec();
        return;
    }
}
bool MainWindow::switchPageTo(const QString &friendId)
{
    // 1. 基本有效性检查
    if(friendId.isEmpty() || !ui->list_talks->currentIndex().isValid()){
        return false;
    }

    // 2. 查找是否已存在该好友的聊天页面
    QWidget *targetPage = ui->stack_talks->findChild<QWidget*>(friendId);

    // 3. 如果找到了已存在的聊天页面
    if (targetPage) {
        int index = ui->stack_talks->indexOf(targetPage);
        ui->stack_talks->setCurrentIndex(index);

        QListWidget *list = targetPage->findChild<QListWidget *>();
        if (list) {
            list->scrollToBottom();
        }

        qDebug()<<"已经有这个聊天窗口了";
        return true;
    }

    // 4. 特殊处理AI助手或检查是否是好友
    if(friendId == "AI助手" || friendHash.contains(friendId)){
        // 创建一个新的聊天页面
        QWidget *newPage = new QWidget();
        newPage->setObjectName(friendId);

        // 创建聊天消息列表
        QListWidget *messageList = new QListWidget(newPage);
        messageList->setFrameShape(QFrame::NoFrame);
        messageList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        messageList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        messageList->setSelectionMode(QAbstractItemView::NoSelection);

        // 设置布局
        QVBoxLayout *layout = new QVBoxLayout(newPage);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(messageList);
        newPage->setLayout(layout);

        // 将新页面添加到stack_talks
        int newIndex = ui->stack_talks->addWidget(newPage);
        ui->stack_talks->setCurrentIndex(newIndex);

        // 设置当前聊天对象
        currentChatTarget = friendId;

        // 设置好友名称
        if (friendId == "AI助手") {
            ui->lab_friendname->setText("AI助手");

            // 添加欢迎消息
            addMessageTo(newPage,
                         "AI助手",
                         "text",
                         "您好，我是AI助手，有什么可以帮您的？",
                         QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ss"));
        } else {
            ui->lab_friendname->setText(friendHash[friendId].name);
        }

        // 启用输入框
        ui->edit_input->setEnabled(true);

        return true;
    } else {
        qDebug()<<"没有这个好友";
        return false;
    }
}

void MainWindow::addMessageTo(const QWidget *page, const QString &sender, const QString &messageType,
                              const QString &message, const QString &timestamp)
//把聊天记录添加到某个聊天页面
{
    QString msg;
    if(messageType == "picture"){
        msg = "[图片消息]";
    }else if(messageType == "document"){
        msg = "[文件消息]";
    }else{
        msg = message;
    }
    QDateTime dateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");//处理时间戳问题
    dateTime = dateTime.addSecs(8 * 3600); //加八个小时
    QString newTimestamp = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    updateTalkListFree(page->objectName(), msg, newTimestamp);
    QListWidget *list = page->findChild<QListWidget *>();
    //创建新的QListWidgetItem
    QListWidgetItem *item = new QListWidgetItem();
    //创建一个QWidget来显示消息
    QWidget *itemWidget = new QWidget();
    QHBoxLayout *messageLayout = new QHBoxLayout(itemWidget);
    messageLayout->setContentsMargins(0, 0, 0, 0);
    QString time;
    //处理时间戳
    if (printTimeOrNot(timestamp, list->objectName(), time)) {
        list->setObjectName(timestamp);
        //创建新的QListWidgetItem
        QListWidgetItem *item = new QListWidgetItem();
        //创建一个QWidget 来显示时间戳
        QWidget *itemWidget = new QWidget();
        QHBoxLayout *messageLayout = new QHBoxLayout(itemWidget);
        //创建时间标签
        QLabel *timeLab = new QLabel(time);
        timeLab->setStyleSheet("QLabel {font-weight: bold;font-size:12pt; text-align: center; border: none; color: rgb(30, 30, 30); }");
        timeLab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        timeLab->setFixedSize(200,25);
        messageLayout->addWidget(timeLab,Qt::AlignCenter);
        item->setSizeHint(QSize(210, 38)); //设置每个项的最小高度
        list->addItem(item);
        list->setItemWidget(item, itemWidget);
    }
    list->setObjectName(timestamp);
    //创建发送者头像
    LabelFriendAvaInMessage *senderAva = new LabelFriendAvaInMessage;
    senderAva->setScaledContents(true);
    senderAva->setFixedSize(40, 40);//头像大小
    senderAva->setCursor(Qt::PointingHandCursor);
    if (sender == accountNumber){
        connect(senderAva, &LabelFriendAvaInMessage::showMessage, [=](){
            changeInfo();
        });
    }
    else{
        connect(senderAva, &LabelFriendAvaInMessage::showMessage, [=](){
            QJsonObject json;
            json["tag"] = "message";
            listtalkChoice(json);
        });
    }
    //获取相应的头像
    QPixmap pix = loadAvator(sender).scaled(senderAva->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pix = getRoundedPixmap(pix, 5);//圆角
    senderAva->setPixmap(pix);
    int messageHeight ;
    if (messageType == "text") {
        QLabel *messageLabel = new QLabel();
        messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        messageLabel->setStyleSheet("background-color: rgb(167, 214, 255); "
                                    "color: black; border-radius: 10px; "
                                    "padding: 5px; line-height: 2; text-align: center; "
                                    "font: 450 12pt 'Microsoft YaHei UI Light';"); //行高
        messageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        messageLabel->setContentsMargins(5,5,5,5);
        messageLabel->setWordWrap(true);//允许换行
        messageLabel->setMaximumWidth(500);//设置最大宽度，防止消息过宽
        messageLabel->setText(message);
        //根据sender进行布局判断
        if (sender == accountNumber) {
            messageLayout->addStretch(); //左侧留空
            messageLayout->addWidget(messageLabel);
            messageLayout->addWidget(senderAva);
            messageLayout->addSpacing(15);
        } else {
            messageLayout->addWidget(senderAva);
            messageLayout->addWidget(messageLabel);
            messageLayout->addStretch(); //右侧留空
        }
        messageHeight = messageLabel->sizeHint().height();
    }
    else if (messageType == "picture") {
        QPixmap pix = base64ToPixmap(message);
        ImageLabel *picLabel = new ImageLabel(pix);
        picLabel->setMaximumSize(250, 250);
        picLabel->setStyleSheet("background-color: rgb(167, 214, 255); "
                                "border-radius: 10px; ");
        picLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        picLabel->setContentsMargins(5,5,5,5);
        pix = pix.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        picLabel->setPixmap(pix);
        //根据sender进行布局判断
        if (sender == accountNumber) {
            messageLayout->addStretch(); //左侧留空
            messageLayout->addWidget(picLabel);
            messageLayout->addWidget(senderAva);
            messageLayout->addSpacing(15);
        } else {
            messageLayout->addWidget(senderAva);
            messageLayout->addWidget(picLabel);
            messageLayout->addStretch(); //右侧留空
        }
        messageHeight = picLabel->sizeHint().height();
    }
    else if (messageType == "document") {
        QPushButton *document = new QPushButton();
        document->setFixedSize(200, 60);
        document->setStyleSheet("background-color: rgb(185, 185, 185); "
                                "color: black; border-radius: 10px; "
                                "line-height: 2; text-align: left; "
                                "font: 480 13pt 'Microsoft YaHei UI Light';"
                                "padding: 7px;");
        document->setIcon(QIcon::fromTheme("document-save"));
        document->setText(message.left(14));
        //根据sender进行布局判断
        if (sender == accountNumber) {
            messageLayout->addStretch();//左侧留空
            messageLayout->addWidget(document);
            messageLayout->addWidget(senderAva);
            messageLayout->addSpacing(13);
        } else {
            document->setCursor(Qt::PointingHandCursor);//对方发的则可点击
            connect(document, &QPushButton::clicked, [message = QString(message), timestamp = timestamp, this] {
                if (!checkSocket()) {
                    return;
                }
                QJsonObject jsonObj;
                jsonObj["tag"] = "askfordocument";
                jsonObj["timestamp"] = timestamp;
                jsonObj["filename"] = message;
                QJsonDocument jsonDoc(jsonObj);
                QByteArray jsonData = jsonDoc.toJson() + "END";
                socket->write(jsonData);
                socket->flush();
            });
            messageLayout->addWidget(senderAva);
            messageLayout->addWidget(document);
            messageLayout->addStretch();//右侧留空
        }
        messageHeight = 80;
    }
    //设置布局并让其垂直居中
    messageLayout->setAlignment(Qt::AlignVCenter);
    //计算高度
    int senderHeight = senderAva->height();
    int totalHeight = qMax(senderHeight, messageHeight) + 5;
    item->setSizeHint(QSize(0, totalHeight)); //设置每个项的最小高度
    list->addItem(item);
    list->setItemWidget(item, itemWidget);
    list->scrollToBottom();
}


void MainWindow::addMessageToDatabase(const QString &sender, const QString &receiver,const QString &messageType,
                                      const QString &message, const QString &timestamp)
//把聊天记录添加到本地数据库
{
    QSqlQuery qry(db);
    qry.prepare("INSERT INTO messages (sender,receiver,message,messagetype)VALUES(:sender,:receiver,:message,:messagetype)");
    qry.bindValue(":sender", sender);
    qry.bindValue(":receiver", receiver);
    qry.bindValue(":message", message);
    qry.bindValue(":messagetype", messageType);
    qry.exec();
}


QPixmap& MainWindow::loadAvator(const QString& friendId)//获得某人头像
{
    if (friendId.isEmpty()) {
        return defaultAva;
    }
    auto it = avatorHash.find(friendId);
    if (it == avatorHash.end()) {
        auto it1 = friendHash.find(friendId);
        if (it1 != friendHash.end()) {
            QString avator = it1.value().avator_base64;
            QPixmap pix = base64ToPixmap(avator);
            if (!pix.isNull()) {
                avatorHash.insert(friendId, pix);
                return avatorHash.find(friendId).value();
            }
        }
    }
    else{
        return it.value();
    }
    return defaultAva;
}

bool MainWindow::printTimeOrNot(const QString& messageTime, const QString& preMessageTime, QString& result)//判断聊天时是否需要加载新的时间
{
    //解析时间戳字符串
    QDateTime messageTimestamp = QDateTime::fromString(messageTime, "yyyy-MM-dd hh:mm:ss");
    QDateTime preMessageTimestamp = QDateTime::fromString(preMessageTime, "yyyy-MM-dd hh:mm:ss");
    messageTimestamp.setTimeZone(QTimeZone::utc());
    preMessageTimestamp.setTimeZone(QTimeZone::utc());
    QTimeZone targetTimeZone("Asia/Shanghai");
    messageTimestamp = messageTimestamp.toTimeZone(targetTimeZone);
    preMessageTimestamp = preMessageTimestamp.toTimeZone(targetTimeZone);
    //计算时间差（以秒为单位）
    qint64 secondsDifference = preMessageTimestamp.secsTo(messageTimestamp);
    //判断差值是否大于三分钟（180秒）
    if (secondsDifference < 180 && preMessageTimestamp.isValid()) {
        return false;
    }
    //计算日期
    QDateTime current = QDateTime::currentDateTime();
    QDate dateToday = current.date();
    QDate dateTimestamp = messageTimestamp.date();
    //如果是今天
    if (dateToday == dateTimestamp) {
        QString timePart = messageTimestamp.time().toString("hh:mm");
        QString period = messageTimestamp.time().hour() < 12 ? "上午" : "下午";
        result = QString("           %1 %2").arg(period).arg(timePart);
    }
    //如果是昨天
    else if (dateToday == dateTimestamp.addDays(1)) {
        QString timePart = messageTimestamp.time().toString("hh:mm");
        QString period = messageTimestamp.time().hour() < 12 ? "上午" : "下午";
        result = QString("           昨天 %1 %2").arg(period).arg(timePart);
    }
    //如果早于昨天
    else {
        result = dateTimestamp.toString("           yyyy-MM-dd");
    }
    return true;
}


void MainWindow::checkFriendInTalk(const QString& friendId)//检查这个好友是不是已经有打开的聊天窗口了 如果没有 则打开一个
{
    QWidget* page = ui->stack_talks->findChild<QWidget*>(friendId);
    if(!page){
        qDebug()<<"没有窗口新建一个";
        //创建一个新的页面
        QWidget *newPage = new QWidget();
        //为新页面设置对象名称
        newPage->setObjectName(friendId);
        QListWidget *list = new QListWidget();
        setupMessageList(list);
        list->setObjectName("notime");
        QVBoxLayout *layout0 = new QVBoxLayout(newPage);
        layout0->addWidget(list);
        newPage->setLayout(layout0);
        //设置newPage的内容
        QSqlQuery qry(db);
        qry.prepare("SELECT * FROM messages WHERE sender = :friend OR receiver = :friend ORDER BY timestamp ASC");
        qry.bindValue(":friend", friendId);
        qry.exec();
        while(qry.next()){//遍历所有消息记录
            QString sender = qry.value("sender").toString();
            QString messageType = qry.value("messagetype").toString();
            QString message = qry.value("message").toString();
            QString timestamp = qry.value("timestamp").toString();
            addMessageTo(newPage, sender, messageType, message, timestamp);
        }
        //将新页面添加到stackedWidget 中
        ui->stack_talks->addWidget(newPage);
    }
}


void MainWindow::updateTalkList(const QString& friendId)//更新某人的消息列表信息
{
    QStandardItem *item = nullptr;
    //查找对应 friendId 的项
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *itemTmp = talkModel->item(row);
        if (itemTmp->data(Qt::UserRole + 1).toString() == friendId) {
            item = itemTmp;
            break;
        }
    }
    if (!item) {
        return;
    }
    //查询talks表以获取该好友的信息
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM talks WHERE friend_id = :friend");
    qry.bindValue(":friend", friendId);
    if (!qry.exec()) {
        return;
    }
    if (!qry.next()) {
        return;
    }
    QString unread = qry.value("unread").toString();
    QString latestMessage, timestamp;
    //如果好友存在，则继续查询消息
    auto it = friendHash.find(friendId);
    if (it != friendHash.end()) {
        QSqlQuery msgQuery(db);
        msgQuery.prepare("SELECT message, messagetype, timestamp FROM messages "
                         "WHERE sender = :friendAccount OR receiver = :friendAccount "
                         "ORDER BY timestamp DESC LIMIT 1");
        msgQuery.bindValue(":friendAccount", friendId);
        if (msgQuery.exec() && msgQuery.next()) {
            if (msgQuery.value("messagetype").toString() == "picture") {
                latestMessage = "[图片消息]";
            } else if (msgQuery.value("messagetype").toString() == "document") {
                latestMessage = "[文件消息]";
            } else {
                latestMessage = msgQuery.value("message").toString();
            }
            timestamp = msgQuery.value("timestamp").toString();
            QDateTime dateTime = QDateTime::fromString(timestamp, Qt::ISODate);
            dateTime = dateTime.addSecs(8 * 3600);//处理时区问题
            //将调整后的时间转回字符串
            timestamp = dateTime.toString("yyyy-MM-dd HH:mm:ss");
        } else {
            qDebug() << "执行失败:" << msgQuery.lastError();
        }
        //将数据存储到项中
        item->setData(latestMessage, Qt::UserRole + 5);
        item->setData(timestamp, Qt::UserRole + 6);
        item->setData(unread, Qt::UserRole + 10);
    }
}


void MainWindow::updateTalkListFree(const QString& friendId, const QString& latestMessage, const QString& timestamp)//更新某人的消息列表信息 指定更新
{
    QStandardItem *item = nullptr;
    //查找对应 friendId 的项
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *itemTmp = talkModel->item(row);
        if (itemTmp->data(Qt::UserRole + 1).toString() == friendId) {
            item = itemTmp;
            break;
        }
    }
    if (!item) {
        return;
    }
    //将数据存储到项中
    item->setData(latestMessage, Qt::UserRole + 5);
    item->setData(timestamp, Qt::UserRole + 6);
}


int MainWindow::updateUnread(const QString& friendId)//使某人的未读消息条数加一 如果正在聊天 则返回
{
    if(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString() == friendId){
        return 0;
    }
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM talks WHERE friend_id = :friend");
    qry.bindValue(":friend", friendId);
    if (!qry.exec()) {
        return 0;
    }
    if (!qry.next()) {
        return 0;
    }
    int unread = qry.value("unread").toInt();
    unread += 1;
    qry.prepare("UPDATE talks SET unread = :unread WHERE friend_id = :friend;");
    qry.bindValue(":friend", friendId);
    qry.bindValue(":unread", unread);
    qry.exec();
    QStandardItem *item = nullptr;
    //查找对应 friendId 的项
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *itemTmp = talkModel->item(row);
        if (itemTmp->data(Qt::UserRole + 1).toString() == friendId) {
            item = itemTmp;
            break;
        }
    }
    if (!item) {
        return 0;
    }
    //将数据存储到项中
    item->setData(unread, Qt::UserRole + 10);
    return unread;
}


void MainWindow::clearUnread(const QString& friendId)//清空某人的未读消息并更新左边消息列表视图
{
    if(friendId.isEmpty()){
        return;
    }
    QSqlQuery qry(db);
    qry.prepare("UPDATE talks SET unread = 0 WHERE friend_id = :friend;");
    qry.bindValue(":friend", friendId);
    qry.exec();
    QStandardItem *item = nullptr;
    //查找对应 friendId 的项
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *itemTmp = talkModel->item(row);
        if (itemTmp->data(Qt::UserRole + 1).toString() == friendId) {
            item = itemTmp;
            break;
        }
    }
    if (!item) {
        return;
    }
    item->setData(0, Qt::UserRole + 10);
}


void MainWindow::liftSomebody(const QString& friendId)//把某人从消息列表提升到最上面
{
    QStandardItem *item0 = talkModel->item(0);
    if (item0->data(Qt::UserRole + 1).toString() == friendId) {
        return;
    }
    if (friendId.isEmpty()) {
        return;
    }
    QStandardItem *item = nullptr;
    //查找对应 friendId 的项
    for (int row = 0; row < talkModel->rowCount(); ++row) {
        QStandardItem *itemTmp = talkModel->item(row);
        if (itemTmp->data(Qt::UserRole + 1).toString() == friendId) {
            item = itemTmp;
            break;
        }
    }
    if (!item) {
        return;
    }
    bool flag = false;
    if(ui->list_talks->currentIndex().data(Qt::UserRole + 1 ) == friendId){
        flag = true;
    }
    //从模型中移除该项
    int currentRow = item->row();
    QStandardItem *movedItem = talkModel->takeItem(currentRow);
    //复制用户角色数据
    for (int role = 1; role <= 10; ++role) {
        QVariant value = item->data(Qt::UserRole + role);
        movedItem->setData(value, Qt::UserRole + role);
    }
    //移除原来的项
    talkModel->removeRow(currentRow);
    //将该项插入到模型的最上面
    talkModel->insertRow(0, movedItem);
    if(flag){
        QModelIndex newIndex = talkFilterProxyModel->mapFromSource(talkModel->index(0, 0));
        ui->list_talks->setCurrentIndex(newIndex);
    }
}


void MainWindow::on_but_minwindow_clicked()//最小化按钮
{
    setWindowState(windowState() | Qt::WindowMinimized);
}


void MainWindow::on_but_maxwindow_clicked()//最大化按钮
{
    if(maxFlag == 0){
        this->showMaximized();
        maxFlag = 1;
    }
    else{
        this->showNormal();
        maxFlag = 0;
    }
    QTimer::singleShot(0, this, [this]() {
        //延迟按钮的位移设置
        ui->but_send->move(QPoint(this->width() - ui->but_send->width() - 93, this->height() - ui->but_send->height() - 40));
    });
}


void MainWindow::on_but_deletewindow_clicked()//关闭按钮
{
    this->close();
}


void MainWindow::on_but_minwindow2_clicked()//页面2的最小化按钮
{
    setWindowState(windowState() | Qt::WindowMinimized);
    // 获取所有键并输出
    QList<QString> keys = friendHash.keys();
    for (const QString &key : keys) {
        qDebug() << key;
    }
}


void MainWindow::on_but_maxwindow2_clicked()//页面2的最大化按钮
{
    if(maxFlag == 0){
        this->showMaximized();
        maxFlag = 1;
    }
    else{
        this->showNormal();
        maxFlag = 0;
    }
}


void MainWindow::on_but_deletewindow2_clicked()//页面3的关闭按钮
{
    this->close();
}


void MainWindow::on_but_chat_clicked()//跳转到聊天窗口页面
{
    ui->pages->setCurrentIndex(0);
}


void MainWindow::on_but_friends_clicked()//跳转到联系人页面
{
    ui->pages->setCurrentIndex(1);
}


void MainWindow::on_edit_input_textChanged()//输入框改变内容判断能否发送消息
{
    if(ui->edit_input->toPlainText().trimmed().isEmpty()){
        ui->but_send->setEnabled(false);
        ui->but_send->setCursor(Qt::ArrowCursor);
        ui->but_send->setStyleSheet(
            "QPushButton {"
            "    font: 11pt 'Microsoft YaHei UI';"
            "    background-color: rgb(167, 214, 255);"
            "    color: white;"
            "    border-radius: 5px;"
            "}");
    }
    else{
        ui->but_send->setEnabled(true);
        ui->but_send->setCursor((Qt::PointingHandCursor));
        ui->but_send->setStyleSheet(
            "QPushButton {"
            "    font: 11pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 5px;"
            "}"
            );
    }
}


void MainWindow::on_pages_currentChanged(int arg1)//页面发生改变 修改左边菜单栏图标的背景颜色
{
    if (arg1 == 0) {
        ui->but_chat->setStyleSheet("QPushButton {"
                                    "background-color: rgb(187, 214, 255); "
                                    "border: none;"
                                    "}");
        ui->but_friends->setStyleSheet("QPushButton {"
                                       "background: transparent; "
                                       "border: none; "
                                       "}"
                                       "QPushButton:hover {"
                                       "background: rgba(0, 0, 0, 0.2); "
                                       "border-radius: 10px;"
                                       "}");
    }
    if (arg1 == 1) {
        ui->but_chat->setStyleSheet("QPushButton {"
                                    "background: transparent; "
                                    "border: none; "
                                    "}"
                                    "QPushButton:hover {"
                                    "background: rgba(0, 0, 0, 0.2); "
                                    "border-radius: 10px;"
                                    "}");
        ui->but_friends->setStyleSheet("QPushButton {"
                                       "background-color: rgb(187, 214, 255); "
                                       "border: none;"
                                       "}");
    }
}


void MainWindow::on_but_addfriends_clicked()//添加好友按钮
{
    if(addFriendsFlag == 0){
        dialogAdd = new AddFriends(this);
        connect(dialogAdd,&AddFriends::serachFriends,this,&MainWindow::goSerachFriends);
        connect(dialogAdd,&AddFriends::addFriend,this,&MainWindow::goAddFriends);
        connect(this,&MainWindow::serachResult,dialogAdd,&AddFriends::serachResult);
        connect(dialogAdd, &AddFriends::customClose, this, [this]() {
            qDebug()<<"添加好友窗口重置了";
            addFriendsFlag = 0;
            dialogAdd->disconnect();
        });
        dialogAdd->show();
        addFriendsFlag = 1;
    }
    else if(addFriendsFlag == 1){
        dialogAdd->raise();
        if (dialogAdd) {
            int x = this->x() + (this->width() - dialogAdd->width()) / 2;
            int y = this->y() + (this->height() - dialogAdd->height()) / 2;
            dialogAdd->move(x, y);
        }
    }
}


void MainWindow::on_but_add0_clicked()//添加好友按钮
{
    if(addFriendsFlag == 0){
        dialogAdd = new AddFriends(this);
        connect(dialogAdd,&AddFriends::serachFriends,this,&MainWindow::goSerachFriends);
        connect(dialogAdd,&AddFriends::addFriend,this,&MainWindow::goAddFriends);
        connect(this,&MainWindow::serachResult,dialogAdd,&AddFriends::serachResult);
        connect(dialogAdd, &AddFriends::customClose, this, [this]() {
            qDebug()<<"添加好友窗口重置了";
            addFriendsFlag = 0;
            dialogAdd->disconnect();
        });
        dialogAdd->show();
        addFriendsFlag = 1;
    }
    else if(addFriendsFlag == 1){
        dialogAdd->raise();
        if (dialogAdd) {
            int x = this->x() + (this->width() - dialogAdd->width()) / 2;
            int y = this->y() + (this->height() - dialogAdd->height()) / 2;
            dialogAdd->move(x, y);
        }
    }
}


void MainWindow::goSerachFriends(const QString &account)//向服务器发送搜索用户信息申请
{
    if(!checkSocket()){
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "serachfriend";
    jsonObj["account"] = account;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    qDebug()<<"发送了搜索好友信息请求";
}


void MainWindow::clearEditShow2()//清理右边好友信息视图
{
    //清理旧的布局和控件
    QLayout *existingLayout = ui->edit_show2->layout();
    if (existingLayout) {
        QLayoutItem *item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            QWidget *widget = item->widget();
            if (widget) {
                widget->deleteLater();//删除控件
            }
            delete item;//删除布局项
        }
    }
    for (QWidget *widget : ui->edit_show2->findChildren<QWidget*>()) {
        if (qobject_cast<QPushButton*>(widget) || qobject_cast<QLabel*>(widget)) {
            //检查是否是 QPushButton 或 QLabel
            widget->deleteLater();//删除按钮或标签
        }
    }
}


void MainWindow::updateEditShow2Normal(const QModelIndex &index)//更新右边好友信息视图
{
    clearEditShow2();//清理旧控件
    //从模型中获取好友的信息
    deleteFriendNum = ui->list_friends->currentIndex().data(Qt::UserRole+1).toString();
    QString nickname = index.data(Qt::DisplayRole).toString();
    QString accountNum = index.data(Qt::UserRole + 1).toString();
    QString gender = index.data(Qt::UserRole + 2).toString();
    QString signature = index.data(Qt::UserRole + 3).toString();
    QString avator = index.data(Qt::UserRole + 4).toString();//base64
    QByteArray byteArray = QByteArray::fromBase64(avator.toUtf8());
    QImage image;
    image.loadFromData(byteArray);
    QPixmap pixmap = QPixmap::fromImage(image);
    //把右边的edit背景重绘
    ui->edit_show2->background = pixmap;
    ui->edit_show2->repaint();
    //添加间隔器
    QSpacerItem *spacertop = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    newLayout->addItem(spacertop);
    //显示头像
    QLabel *avatorLabel = new QLabel;
    avatorLabel->setPixmap(pixmap);
    avatorLabel->setScaledContents(true);
    avatorLabel->setFixedSize(120, 120);
    avatorLabel->setAlignment(Qt::AlignCenter);
    avatorLabel->setStyleSheet("border: 3px solid black; border-radius: 8px;");
    newLayout->addWidget(avatorLabel, 0, Qt::AlignCenter);
    //添加信息标签
    auto addTextEditWithStyle = [&](const QString &prefix, const QString &text) {
        AutoClearTextEdit *textEdit = new AutoClearTextEdit(prefix + text);
        textEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        textEdit->setFixedSize(300, 50);
        textEdit->setStyleSheet(
            "font-size: 16px; "
            "font-weight: bold; "
            "color: #333333; "
            "padding: 5px; "
            "background: rgba(255, 255, 255, 0); "
            "border: 2px solid black; "
            "border-radius: 5px;");
        textEdit->setAlignment(Qt::AlignCenter);
        textEdit->setReadOnly(true);//设置为只读
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用垂直滚动条
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用水平滚动条
        return textEdit;
    };
    //添加间隔器
    QSpacerItem *spacer0 = new QSpacerItem(0, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);
    newLayout->addItem(spacer0);
    //添加信息
    if (!nickname.isEmpty()) newLayout->addWidget(addTextEditWithStyle("昵称: ", nickname));
    if (!accountNum.isEmpty()) newLayout->addWidget(addTextEditWithStyle("账号: ", accountNum));
    if (!gender.isEmpty()) newLayout->addWidget(addTextEditWithStyle("性别: ", gender));
    if (!signature.isEmpty()) newLayout->addWidget(addTextEditWithStyle("签名: ", signature));
    //添加间隔器
    QSpacerItem *spacer1 = new QSpacerItem(0, 5, QSizePolicy::Fixed, QSizePolicy::Minimum);
    newLayout->addItem(spacer1);
    //创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);//按钮布局居中
    //添加发送信息按钮
    QPushButton *sendButton = new QPushButton("发送信息");
    sendButton->setCursor(Qt::PointingHandCursor);
    sendButton->setFixedSize(140, 55);
    sendButton->setStyleSheet(
        "QPushButton {"
        "    font: 12pt 'Microsoft YaHei UI';"
        "    background-color: rgb(5, 186, 251);"
        "    color: white;"
        "    font-weight: bold; "
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(5, 186, 251, 0.7);"
        "    font-weight: bold; "
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 123, 255, 0.8);"
        "    font-weight: bold; "
        "    color: rgba(255, 255, 255, 0.9);"
        "    border-radius: 15px;"
        "}"
        );
    buttonLayout->addWidget(sendButton);
    //添加间隔器
    QSpacerItem *spacer = new QSpacerItem(25, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    buttonLayout->addItem(spacer);
    //添加删除好友按钮
    QPushButton *deleteButton = new QPushButton("删除好友");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setFixedSize(140, 55);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "    font: 12pt 'Microsoft YaHei UI';"
        "    background-color: rgb(255, 0, 0);"
        "    color: white;"
        "    font-weight: bold; "
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 0, 0, 0.7);"
        "    font-weight: bold; "
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(200, 0, 0, 0.8);"
        "    font-weight: bold; "
        "    color: rgba(255, 255, 255, 0.9);"
        "    border-radius: 15px;"
        "}"
        );
    buttonLayout->addWidget(deleteButton);
    //将按钮布局添加到主布局中
    newLayout->addLayout(buttonLayout);
    //添加间隔器
    QSpacerItem *spacer2 = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    newLayout->addItem(spacer2);
    //增加间距
    newLayout->setSpacing(3);
    newLayout->setContentsMargins(5, 5, 5, 5);
    //为按钮添加点击事件
    connect(sendButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "发送信息按钮被点击";
        sendMessageToFriend(accountNum);
    });
    connect(deleteButton, &QPushButton::clicked, this, [&]() {
        qDebug() << "删除好友按钮被点击";
        if(friendHash.find(deleteFriendNum) == friendHash.end()){
            Dialog dialog(this);
            dialog.transText("他不在您的好友列表当中!");
            dialog.exec();
            return;
        }
        ChoiceDialog dialog(this);
        dialog.transText("你确定要删除好友吗?");
        dialog.transButText("确认","取消");
        int result = dialog.exec();
        if(result == QDialog::Accepted){
            deleteFriend();
        }
    });
    //设置布局
    ui->edit_show2->setLayout(newLayout);
}


void MainWindow::updateEditShow2New()//更新右边好友信息视图(好友申请)
{
    if(ui->list_friends->currentIndex().row() == 1){
        QModelIndex secondItemIndex = ui->list_friends->model()->index(1, 0);
        ui->list_friends->model()->setData(secondItemIndex, false, Qt::UserRole + 20);
        ui->list_friends->model()->dataChanged(secondItemIndex, secondItemIndex, {Qt::UserRole + 20});
    }
    clearEditShow2();//清理旧控件
    // 把右边的edit背景重绘为默认
    QPixmap pix(":/pictures/andan_touming.jpg");
    ui->edit_show2->background = pix;
    ui->edit_show2->repaint();
    if(newFriendArray.empty()){
        QLabel *label = new QLabel(this);
        label->setText("目前没有好友申请哦。");
        label->setStyleSheet("font-size: 22px; color: #333333; font-weight: bold;");
        newLayout->addWidget(label, Qt::AlignCenter);
    }
    for (const auto &friendRequest : newFriendArray) {//遍历添加好友申请用户
        QString friendNickname = friendRequest.name;
        QString friendavatorBase64 = friendRequest.avator_base64;
        QByteArray avatarByteArray = QByteArray::fromBase64(friendavatorBase64.toUtf8());
        QImage avatarImage;
        avatarImage.loadFromData(avatarByteArray);
        QPixmap avatarPixmap = QPixmap::fromImage(avatarImage);
        avatarPixmap = getRoundedPixmap(avatarPixmap, 5);
        QHBoxLayout *requestLayout = new QHBoxLayout();
        requestLayout->setContentsMargins(0, 0, 0, 0);
        requestLayout->setSpacing(10);
        requestLayout->setSizeConstraint(QLayout::SetMinimumSize);
        //添加头像
        LabelFriendAva *avatorLabel = new LabelFriendAva(this);
        avatorLabel->setPixmap(avatarPixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        avatorLabel->setFixedSize(60, 60);
        avatorLabel->setScaledContents(true);
        avatorLabel->setCursor(Qt::PointingHandCursor);
        requestLayout->addWidget(avatorLabel);
        requestLayout->setAlignment(avatorLabel, Qt::AlignLeft);
        //给头像添加申请人信息
        avatorLabel->setProperty("avator",QVariant(friendRequest.avator_base64));
        avatorLabel->setProperty("account",QVariant(friendRequest.account));
        avatorLabel->setProperty("nickname",QVariant(friendRequest.name));
        avatorLabel->setProperty("signature",QVariant(friendRequest.signature));
        avatorLabel->setProperty("gender",QVariant(friendRequest.gender));
        //添加名称
        QLabel *nicknameLabel = new QLabel(friendNickname);
        nicknameLabel->setStyleSheet("font-size: 16px; color: #333333;");
        requestLayout->addWidget(nicknameLabel);
        requestLayout->setAlignment(nicknameLabel, Qt::AlignLeft);
        QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        requestLayout->addItem(spacer);
        //接受按钮
        QPushButton *acceptButton = new QPushButton("接受");
        acceptButton->setCursor(Qt::PointingHandCursor);
        // 存储数据到按钮
        acceptButton->setProperty("sender", friendRequest.account);
        acceptButton->setStyleSheet(
            "QPushButton {"
            "    font: 14pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    border-radius: 5px;"
            "    min-width: 80px;"
            "    min-height: 30px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(255, 0, 0, 0.5);"
            "}"
            );
        connect(acceptButton, &QPushButton::clicked, this, [this, acceptButton]() {
            ChoiceDialog dialog(this);
            dialog.transText("你确定要接受好友申请吗？");
            dialog.transButText("确定", "取消");
            int result = dialog.exec();
            if (result == QDialog::Accepted) {
                QString senderId = acceptButton->property("sender").toString();
                if (!senderId.isEmpty()) {
                    acceptAddFriends(myInfo.account, senderId);
                }
            }
        });
        requestLayout->addWidget(acceptButton);
        requestLayout->setAlignment(acceptButton, Qt::AlignRight);
        //拒绝按钮
        QPushButton *rejectButton = new QPushButton("拒绝");
        rejectButton->setCursor(Qt::PointingHandCursor);
        // 存储数据到按钮
        rejectButton->setProperty("sender", friendRequest.account);
        rejectButton->setStyleSheet(
            "QPushButton {"
            "    font: 14pt 'Microsoft YaHei UI';"
            "    background-color: rgb(255, 0, 0);"
            "    color: white;"
            "    border-radius: 5px;"
            "    min-width: 80px;"
            "    min-height: 30px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(255, 0, 0, 0.7);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(255, 0, 0, 0.5);"
            "}"
            );
        connect(rejectButton, &QPushButton::clicked, this, [this, rejectButton]() {
            ChoiceDialog dialog(this);
            dialog.transText("你确定要拒绝好友申请吗？");
            dialog.transButText("确定", "取消");
            int result = dialog.exec();
            if (result == QDialog::Accepted) {
                QString senderId = rejectButton->property("sender").toString();
                if (!senderId.isEmpty()) {
                    rejectAddFriends(myInfo.account, senderId);
                }
            }
        });
        requestLayout->addWidget(rejectButton);
        requestLayout->setAlignment(rejectButton, Qt::AlignRight);
        newLayout->addLayout(requestLayout, Qt::AlignTop);
    }
    //最下方添加一个伸缩器
    QSpacerItem *bottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    newLayout->addItem(bottomSpacer);
}


void MainWindow::on_line_serach2_textChanged(const QString &arg1)//搜索输入 更新好友列表视图
{
    if(arg1 == "搜索")return;
    QRegularExpression regex(arg1, QRegularExpression::CaseInsensitiveOption);
    filterProxyModel->setFilterRegularExpression(regex);
    filterProxyModel->invalidate();
    ui->list_friends->viewport()->update();
}


void MainWindow::on_line_serach_textChanged(const QString &arg1)//搜索输入 更新聊天列表视图
{
    if(arg1 == "搜索")return;
    QRegularExpression regex(arg1, QRegularExpression::CaseInsensitiveOption);
    talkFilterProxyModel->setFilterRegularExpression(regex);
    talkFilterProxyModel->invalidate();
    ui->list_friends->viewport()->update();
}


void MainWindow::deleteFriend()//删除好友
{
    if(!checkSocket()){
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "deletefriend";
    jsonObj["account"] = myInfo.account;
    jsonObj["friend"] = deleteFriendNum;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    qDebug()<<"发送了删除好友请求";
}


void MainWindow::goAddFriends(const QString &friendAccount)//添加好友
{
    if(!checkSocket()){
        return;
    }
    //查看是否是本人的账号
    if(friendAccount == myInfo.account){
        //弹窗
        QString text =  "你不能添加自己为好友!";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;
    }
    //检查是否已经添加好友了
    if(friendHash.find(friendAccount) != friendHash.end()){
        //弹窗
        QString text =  "对方已经是你的好友了";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "addfriend";
    jsonObj["account"] = myInfo.account;//谁要加（当前用户）
    jsonObj["friend"] = friendAccount;//要加谁
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    qDebug()<<"发送了添加好友请求";
    //弹窗
    QString text =  "已发送好友申请";
    Dialog* dialog = new Dialog(this);
    dialog->transText(text);
    dialog->exec();
}

void MainWindow::changeInfo()//弹出修改个人资料窗口
{
    // // 弹出修改个人资料窗口 : 函数签名。这是一个槽函数，通常连接到菜单项或按钮的点击信号。

    // 检查自定义标志 changeInfoFlag 的值。
    // changeInfoFlag 假设是 MainWindow 的一个成员变量，用于记录修改资料窗口的状态，例如 0 表示未打开，1 表示已打开。
    if (changeInfoFlag == 0) { // 如果标志为 0，表示修改资料窗口当前未打开
        // // 未打开时：创建新的窗口并初始化

        // 声明一个 MyAccountInfo 结构体变量。
        // MyAccountInfo 假设是用于存储用户账号信息的结构体，myInfo 是存储当前用户信息的成员变量。
        MyAccountInfo tmpInfo;
        // 将当前用户的信息从 myInfo 复制到临时结构体 tmpInfo 中。
        // 原理：创建一个用户信息的副本，用于传递给修改资料对话框进行显示和编辑。这样做避免了对话框直接操作 myInfo 成员变量，提高了安全性，即使对话框被取消，也不会影响 myInfo 的原始数据。
        tmpInfo.account = myInfo.account;
        tmpInfo.name = myInfo.name;
        tmpInfo.avator_base64 = myInfo.avator_base64;
        tmpInfo.gender = myInfo.gender;
        tmpInfo.signature = myInfo.signature;

        // 创建一个新的 ChangeInformation 对话框实例。
        // ChangeInformation 假设是用于修改个人资料的自定义对话框类。
        // new ChangeInformation(tmpInfo, this) 在堆上分配内存创建一个 ChangeInformation 对象。
        // 传入参数：tmpInfo 结构体（初始化对话框显示内容），this（将 MainWindow 设置为对话框的父窗口）。
        // 原理：创建一个具体的修改资料窗口。设置父窗口有助于 Qt 的对象树管理内存（父窗口销毁时子窗口也会被删除）以及影响窗口的层叠关系。dialogChangeInfo 假设是 MainWindow 的一个成员变量，用于存储这个对话框的指针。
        dialogChangeInfo = new ChangeInformation(tmpInfo, this);

        // // 建立信号与槽连接：在窗口关闭时重置标志
        // 连接 dialogChangeInfo 发出的 customClose 信号到 MainWindow 的一个 Lambda 槽函数。
        // customClose 假设是 ChangeInformation 对话框中自定义的一个信号，在对话框关闭时发射。
        // [this]() { ... } 是一个 Lambda 表达式，作为槽函数。它捕获 this 指针，可以在 Lambda 内部访问 MainWindow 的成员。
        // Lambda 函数体：打印调试信息，并将 changeInfoFlag 重置为 0。
        // 原理：当修改资料对话框关闭时，通过这个连接将 MainWindow 中的 changeInfoFlag 标志改回 0，表示窗口已关闭，下次再点击时可以重新创建。Lambda 函数适合用于简短、局部性的槽逻辑。
        connect(dialogChangeInfo, &ChangeInformation::customClose, this, [this]() {
            qDebug() << "修改信息窗口重置了"; // 打印调试信息。
            changeInfoFlag = 0; // 重置标志为 0。
        });

        // // 建立信号与槽连接：对话框需要发送网络请求时通知 MainWindow
        // 连接 dialogChangeInfo 发出的 sendMessage 信号到 MainWindow 的 sendMessage 槽。
        // sendMessage 假设是 ChangeInformation 对话框中的一个信号，在用户确认修改并需要将新资料发送给服务器时发射。
        // MainWindow::sendMessage 假设是 MainWindow 中负责实际处理网络发送（使用 socket）的槽函数。
        // 原理：将修改资料对话框发送网络请求的任务“委托”给 MainWindow 来完成。对话框只负责收集用户输入并发出“请发送这个数据”的信号，MainWindow 负责处理网络通信细节，实现了职责分离。
        connect(dialogChangeInfo, &ChangeInformation::sendMessage, this, &MainWindow::sendMessage);

        // // 建立信号与槽连接：MainWindow 接收服务器响应后通知对话框结果
        // 连接 MainWindow 发出的 changeResult 信号到 dialogChangeInfo 的 dealResult 槽。
        // changeResult 假设是 MainWindow 中的一个信号，在 MainWindow 接收并处理了服务器关于修改资料请求的响应（成功或失败）后发射。
        // dealResult 假设是 ChangeInformation 对话框中的一个槽函数，用于接收服务器响应结果并更新对话框界面（如显示“修改成功”或“修改失败”提示）。
        // 原理：将服务器响应结果从处理网络通信的 MainWindow 传递回需要显示结果的修改资料对话框。实现了结果的传递和界面的更新。
        connect(this, &MainWindow::changeResult, dialogChangeInfo, &ChangeInformation::dealResult);

        // 显示修改资料对话框。
        // show() 方法使窗口可见。对于 QDialog，通常使用 exec() 方法以模态方式显示（阻塞其他窗口），但这里使用 show() 可能表示它被设计为非模态对话框，允许在修改资料时同时操作主窗口。
        dialogChangeInfo->show();

        // 将标志设置为 1，表示修改资料窗口已打开。
        changeInfoFlag = 1;
    }
    // 否则，如果标志不是 0（即 changeInfoFlag == 1），表示修改资料窗口已经打开
    else if (changeInfoFlag == 1) {
        // // 已打开时：将现有窗口带到前面并居中

        // 将已存在的对话框窗口带到其父窗口（或桌面，如果是顶级窗口）的堆叠顺序的最前面。
        // 原理：确保窗口即使被其他窗口遮挡，也能被用户看到。
        dialogChangeInfo->raise();

        // 检查 dialogChangeInfo 指针是否有效。
        // 原理：这行检查在这里是多余的，因为 if (changeInfoFlag == 1) 的前提就是 dialogChangeInfo 已经被创建且标志被设置为 1。如果程序其他地方错误地将 changeInfoFlag 设置为 1 但没有创建对话框，这个检查可以避免崩溃，但更应检查 changeInfoFlag 的设置逻辑。
        if (dialogChangeInfo) {
            // 计算将对话框窗口在其父窗口 (MainWindow) 中居中的位置。
            // this->x(), this->y(): 获取 MainWindow 左上角的屏幕全局坐标。
            // this->width(), this->height(): 获取 MainWindow 的尺寸。
            // dialogChangeInfo->width(), dialogChangeInfo->height(): 获取对话框的尺寸。
            // 原理：计算公式 (父窗口位置 + (父窗口尺寸 - 子窗口尺寸) / 2) 可以得出子窗口左上角应放置的位置，使其相对于父窗口居中。
            int x = this->x() + (this->width() - dialogChangeInfo->width()) / 2;
            int y = this->y() + (this->height() - dialogChangeInfo->height()) / 2;

            // 将已存在的对话框窗口移动到计算出的居中位置。
            // move(x, y) 将窗口的左上角移动到指定的屏幕全局坐标 (x, y)。
            dialogChangeInfo->move(x, y);
            // 原理：将对话框移动到主窗口中央，提升用户体验。
        }
    }
    // 函数在此结束。
}

void MainWindow::listtalkChoice(const QJsonObject& json)//聊天列表的菜单选择完毕
{
    // // 聊天列表的菜单选择完毕 : 函数签名，接收一个 QJsonObject。

    // 检查接收到的 JSON 对象是否为空。
    // 如果菜单系统在用户关闭菜单但未选择任何特定项时发送一个空 JSON，可能会进入此分支。
    if(json.empty()){//如果是空 触发点击事件
        // 以编程方式发射 ui->list_talks 控件的 clicked() 信号，并传入当前选中项的模型索引。
        // ui->list_talks->clicked() 是 QListView 的一个信号，通常在用户点击列表项时由 Qt 自动发射。
        // ui->list_talks->currentIndex() 获取当前选中项的模型索引。
        // 原理：这部分逻辑可能旨在模拟用户对当前选中项进行了一次常规点击。这可能会触发连接到 ui->list_talks 的 clicked() 信号的其他槽函数，例如打开与该好友的聊天窗口。这是一种将菜单默认行为映射到常规点击行为的方式。
        ui->list_talks->clicked(ui->list_talks->currentIndex());
    }
    // 如果 JSON 不为空，则根据 JSON 中的 "tag" 字段来判断用户选择了哪个菜单项。
    else if(json["tag"] == "sendmessage"){//发送消息 (通常是右键菜单项“发送消息”)
        // 设置输入文本框 (ui->edit_input) 获得键盘输入焦点。
        // 原理：让用户可以直接开始输入消息，提高用户体验。
        ui->edit_input->setFocus();
        // 将鼠标光标移动到输入框内的一个固定位置 (左上角偏移 30, 30)。
        // QPoint(30, 30) 定义相对于 ui->edit_input 左上角的局部坐标点。
        // ui->edit_input->mapToGlobal(...) 将局部坐标点转换为屏幕全局坐标。
        // QCursor::setPos(...) 设置鼠标光标的全局位置。
        // 原理：进一步引导用户输入，让鼠标光标直接位于输入区域内，方便点击或开始输入。
        QCursor::setPos(ui->edit_input->mapToGlobal(QPoint(30, 30)));//移动鼠标到edit_input内
    }
    else if(json["tag"] == "message"){//好友信息 (通常是右键菜单项“查看资料/好友信息”)
        // 声明一个 FriendMessage::FriendInfo 结构体变量，用于存储好友详细信息。
        // FriendMessage 假设是一个自定义的显示好友资料的对话框类，FriendInfo 是其内部定义的结构体。
        FriendMessage::FriendInfo info;
        // 从聊天列表当前选中项的模型数据中获取好友的各项信息，填充到 info 结构体中。
        // ui->list_talks->currentIndex() 获取选中项索引。
        // .data(Role) 从模型中获取指定角色的数据。
        // Qt::UserRole + 4: 获取 Base64 编码的头像字符串。
        // Qt::UserRole + 1: 获取账号。
        // Qt::DisplayRole: 获取昵称。
        // Qt::UserRole + 3: 获取签名。
        // Qt::UserRole + 2: 获取性别。
        info.avator_base64 = ui->list_talks->currentIndex().data(Qt::UserRole + 4).toString();
        info.account = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();
        info.name = ui->list_talks->currentIndex().data(Qt::DisplayRole).toString();
        info.signature = ui->list_talks->currentIndex().data(Qt::UserRole + 3).toString();
        info.gender = ui->list_talks->currentIndex().data(Qt::UserRole + 2).toString();
        // 原理：从列表项关联的模型数据中提取好友的各种详细属性，因为这些数据可能已经被加载并存储在模型中了。

        // 创建一个 FriendMessage 对话框实例，传入好友信息 info 和当前窗口作为父窗口。
        FriendMessage *dialog = new FriendMessage(info,this);
        // 以模态方式显示好友信息对话框。用户必须关闭此对话框后才能操作主窗口。
        dialog->exec();
        // 原理：显示一个独立的窗口，向用户展示选中好友的详细资料。
    }
    else if(json["tag"] == "deletefriend"){//删除好友 (通常是右键菜单项“删除好友”)
        // 在执行删除操作前，检查该账号是否存在于好友列表的 hash 缓存中。
        // friendHash 假设是一个存储好友信息的 QHash 或 QMap。
        // find(account).end() == end() 表示在 hash 中未找到该账号。
        if(friendHash.find(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString()) == friendHash.end()){
            // 如果账号不在好友列表 hash 中，则显示错误提示框。
            Dialog dialog(this); // 创建自定义消息框。
            dialog.transText("他不在您的好友列表当中!"); // 设置文本。
            dialog.exec(); // 模态显示。
            // 原理：防止用户尝试删除一个实际不在好友列表中的聊天项，提供友好的错误提示。
            return; // 函数结束。
        }

        // 如果账号在好友列表 hash 中，弹出确认对话框询问用户是否确定删除。
        ChoiceDialog dialog(this); // 创建自定义选择对话框。
        dialog.transText("你确定要删除好友吗?"); // 设置提示文本。
        dialog.transButText("确认","取消"); // 设置按钮文本。
        int result = dialog.exec(); // 模态显示并等待用户点击按钮，返回点击结果 (QDialog::Accepted 或 QDialog::Rejected)。
        // 原理：删除好友是敏感操作，需要用户二次确认。

        // 检查用户是否点击了“确认”按钮 (QDialog::Accepted)。
        if(result == QDialog::Accepted){
            // 如果用户确认删除，则检查网络连接。
            // checkSocket() 假设是检查 socket 连接状态的函数，如果未连接可能显示错误并返回 false。
            if(!checkSocket()){
                return; // 网络未连接则结束函数。
            }
            // 原理：确保网络连接可用，才能向服务器发送删除请求。

            // 构建删除好友请求的 JSON 对象。
            QJsonObject jsonObj;
            jsonObj["tag"] = "deletefriend"; // 请求类型标签。
            jsonObj["account"] = myInfo.account; // 当前用户的账号 (myInfo 假设存储当前用户信息的成员变量)。
            // 获取要删除的好友账号 (从选中项模型数据中获取 Qt::UserRole + 1 的值)。
            jsonObj["friend"] = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();;
            // 原理：准备包含删除请求类型、操作者账号、被操作对象账号信息的 JSON 数据，用于发送给服务器。

            // 将 JSON 对象转换为 QJsonDocument 并序列化为 QByteArray，添加结束标识符 "END"。
            QJsonDocument jsonDoc(jsonObj);
            QByteArray jsonData = jsonDoc.toJson() + "END";

            // 通过 socket 将删除好友请求发送给服务器。
            // socket 假设是连接服务器的 QTcpSocket 成员变量。
            socket->write(jsonData); // 写入数据到缓冲区。
            socket->flush(); // 强制发送缓冲区数据。
            // 原理：向服务器发起删除好友的网络请求。服务器接收到此请求后会处理数据库中的好友关系。

            qDebug()<<"发送了删除好友请求"<<jsonObj["friend"]; // 打印调试信息。
        }
        // 注意：这里的代码只负责发送删除请求。实际的界面更新（从列表移除项）和本地数据库清理通常在收到服务器返回的“删除成功”响应后由另一个槽函数处理。
    }
    else if(json["tag"] == "deletetalk"){//从消息列表移除 (通常是右键菜单项“移除该聊天”)
        // 调用 deleteSomeoneInTalkList() 函数，传入当前选中项的账号。
        // deleteSomeoneInTalkList() 函数（前面已经讲解过）负责从聊天列表中删除该项，并清理相关缓存和本地数据库中的聊天记录。
        deleteSomeoneInTalkList(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString());
        // 原理：执行本地的“移除聊天”操作，只影响当前用户的聊天列表和本地记录，不影响好友关系或对方的聊天列表。
    }
    else if(json["tag"] == "clearmessage"){//清空聊天记录 (通常是右键菜单项“清空聊天记录”)
        // 弹出确认对话框询问用户是否确定清空聊天记录。
        ChoiceDialog dialog(this); // 创建自定义选择对话框。
        dialog.transText("你确定要清空聊天记录吗？"); // 设置提示文本。
        dialog.transButText("确定", "取消"); // 设置按钮文本。
        int result = dialog.exec(); // 模态显示并等待用户点击按钮。
        // 原理：清空记录是不可恢复操作，需要用户二次确认。

        // 检查用户是否点击了“确定”按钮 (QDialog::Accepted)。
        if(result == QDialog::Accepted){
            // 如果用户确认清空。

            // 清空本地数据库中与该好友的聊天记录。
            QSqlQuery qry(db); // 创建数据库查询对象。db 假设是数据库连接。
            // 准备 SQL DELETE 语句：删除 messages 表中 sender 是该好友 或 receiver 是该好友 的所有记录。
            qry.prepare("DELETE FROM messages WHERE sender = :friend OR receiver = :friend");
            // 绑定参数：将要清空记录的好友账号绑定到占位符。
            qry.bindValue(":friend", ui->list_talks->currentIndex().data(Qt::UserRole + 1));
            // 执行 DELETE 语句。这里的返回值未被检查，可能需要添加错误处理。
            qry.exec();
            // 原理：从本地数据库中物理删除所有与该好友相关的聊天消息。

            // 获取当前选中项的模型索引。
            auto currentIndex = ui->list_talks->currentIndex();

            // 更新聊天列表项模型中的数据，清除与最后一条消息预览、时间戳等相关的信息。
            // talkFilterProxyModel->setData(index, value, role) 在代理模型上设置数据，代理模型会转发到源模型。
            // 设置 Qt::UserRole + 5, Qt::UserRole + 6, Qt::UserRole + 10 等角色的数据为空字符串。
            // 原理：这些自定义角色可能存储了最近一条消息的预览文本、时间戳、未读计数等信息。清空聊天记录后，这些信息也应被清除，列表项不再显示最后一条消息的预览。
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 5); // 假设是最后一条消息预览
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 6); // 假设是最后一条消息时间戳
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 10); // 假设是未读计数或草稿

            // 手动发射 dataChanged 信号，通知视图模型中指定范围和角色的数据已改变。
            // talkFilterProxyModel->dataChanged(topLeft, bottomRight, roles)
            // 原理：虽然 setData 通常会自动触发 dataChanged，但显式发射可以确保视图（特别是委托）知道这些特定角色的数据发生了变化，从而重新绘制列表项，更新显示（例如清除未读红点和最后一条消息预览）。
            talkFilterProxyModel->dataChanged(currentIndex, currentIndex, {Qt::UserRole + 10}); // 通知未读计数/草稿变化

            qDebug()<<ui->list_talks->currentIndex().data(Qt::UserRole + 10); // 打印更新后的 UserRole + 10 数据（应该为空）。

            // 清空**当前显示的**聊天界面中的消息显示区域。
            // 查找与当前选中聊天项对应的聊天界面 QWidget (使用账号作为 objectName)。
            QWidget* page = ui->stack_talks->findChild<QWidget *>(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString());
            // 在找到的聊天界面 QWidget 中查找用于显示消息的 QListWidget 控件。
            QListWidget *list = page->findChild<QListWidget *>();
            // 清空 QListWidget 中的所有项目。
            list->clear();
            // 设置 QListWidget 的 objectName 为 "notime"。注释 "10.18修改" 可能表示这与时间显示逻辑相关。
            list->setObjectName("notime");//10.18修改
            // 原理：从聊天界面中移除所有已显示的消息泡泡或文本。

            // 显示清空成功的提示框。
            Dialog dialog(this);
            dialog.transText("清空聊天记录成功!");
            dialog.exec();
            // 原理：告知用户操作已完成。
        }
    }
    // 如果 json 的 tag 不属于以上任何一种情况，函数执行到这里结束。
}


void MainWindow::sendMessage(const MyAccountInfo& info)//收到修改资料窗口的信号 发送信息
{
    if(!checkSocket()){
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "changeinformation";
    jsonObj["account"] = info.account;
    jsonObj["nickname"] = info.name;
    jsonObj["gender"] = info.gender;
    jsonObj["signature"] = info.signature;
    jsonObj["avator"] = info.avator_base64;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
}


void MainWindow::changePassword1(const QJsonObject &json)//收到修改密码窗口的信号 发送信息
{
    if(!checkSocket()){
        return;
    }
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
}


void MainWindow::changePassword2(const QJsonObject &json)//收到修改密码窗口的信号 发送最终的信息
{
    if(!checkSocket()){
        return;
    }
    if(json["answer"] == "succeed"){
        this->close();
    }
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
}


void MainWindow::goLogout(const QJsonObject &json)//收到注销窗口发送的信号 发送注销申请
{
    if(!checkSocket()){
        return;
    }
    if(json["account"] != myInfo.account){
        Dialog dialog(this);
        dialog.transText("请输入您当前登录的账号!");
        dialog.exec();
        return;
    }
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
}


void MainWindow::rejectAddFriends(const QString &account, const QString &sender)//拒绝好友申请
{
    if(!checkSocket()){
        return;
    }
    QJsonObject json;
    json["tag"] = "newfriends";
    json["answer"] = "reject";
    json["account"] = account;
    json["sender"] = sender;
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    for (int i = newFriendArray.size() - 1; i >= 0; --i) {
        if (newFriendArray[i].account == json["sender"].toString()) {
            newFriendArray.removeAt(i);
        }
    }
    updateEditShow2New();
}


void MainWindow::acceptAddFriends(const QString &account, const QString &sender)//接受好友申请
{
    if(!checkSocket()){
        return;
    }
    QJsonObject json;
    json["tag"] = "newfriends";
    json["answer"] = "accept";
    json["account"] = account;
    json["sender"] = sender;
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
}


void MainWindow::on_but_send_clicked()//发送信息
{
    if(!checkSocket()){
        return;
    }
    // 检查是否在AI聊天模式
    if (isAIChatActive && currentChatTarget == "AI助手") {
        qDebug()<<"AI聊天模式发送消息";
        QString message = ui->edit_input->toPlainText();
        if (!message.isEmpty()) {
            // 清空输入框
            ui->edit_input->clear();

            // 添加用户消息到聊天界面
            addMessageTo(ui->stack_talks->currentWidget(),
                         accountNumber,
                         "text",
                         message,
                         QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ss"));

            // 更新聊天列表中的最新消息
            updateTalkListFree("AI助手", "正在等待AI回复...", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            // 发送消息到AI
            aiChat->sendMessageToAI(message);
        }
        return; // 不执行后续的普通聊天发送逻辑
    }
    else
    {qDebug()<<"不在AI聊天模式";
    }
    if(friendHash.find(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString()) == friendHash.end()){
        Dialog dialog(this);
        dialog.transText("他不是您的好友!");
        dialog.exec();
    }
    QJsonObject json;
    QDateTime time = QDateTime::currentDateTime();
    QTimeZone targetTimeZone("Asia/Shanghai");
    time.setTimeZone(targetTimeZone);
    time = time.toTimeZone(QTimeZone::utc());
    json["tag"] = "messages";
    json["messages"] = ui->edit_input->toPlainText();
    json["sender"] = myInfo.account;
    json["messagetype"] = "text";
    json["receiver"] = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();
    json["timestamp"] = time.toString("yyyy-MM-dd HH:mm:ss");
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    ui->edit_input->clear();
    QWidget *page = ui->stack_talks->currentWidget();
    //把聊天记录添加到本地数据库
    addMessageToDatabase(myInfo.account, json["receiver"].toString(), "text",
                         json["messages"].toString(), QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    //把聊天记录添加到当前聊天页面
    addMessageTo(page, myInfo.account, "text",
                 json["messages"].toString(), time.toString("yyyy-MM-dd HH:mm:ss"));
    liftSomebody(json["receiver"].toString());
}


void MainWindow::on_but_tool1_clicked()//发送图片
{
    if(!checkSocket()){
        return;
    }
    if(friendHash.find(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString()) == friendHash.end()){
        Dialog dialog(this);
        dialog.transText("他不是您的好友!");
        dialog.exec();
    }
    //弹出文件对话框让用户选择图片
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
    //检查用户是否选择了文件
    if (fileName.isEmpty()) {
        return; //用户取消了选择
    }
    //创建QPixmap并加载选择的图片
    QPixmap pixmap(fileName);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("错误"), tr("无法加载图片: %1").arg(fileName));
        return;
    }
    QString pixmapString = pixmapToBase64(pixmap);
    QJsonObject json;
    QDateTime time = QDateTime::currentDateTime();
    QTimeZone targetTimeZone("Asia/Shanghai");
    time.setTimeZone(targetTimeZone);
    time = time.toTimeZone(QTimeZone::utc());
    json["tag"] = "messages";
    json["messages"] = pixmapString;
    json["sender"] = myInfo.account;
    json["messagetype"] = "picture";
    json["receiver"] = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();
    json["timestamp"] = time.toString("yyyy-MM-dd HH:mm:ss");
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson() + "END";
    socket->write(jsonData);
    socket->flush();
    QWidget *page = ui->stack_talks->currentWidget();
    //把聊天记录添加到本地数据库
    addMessageToDatabase(myInfo.account, json["receiver"].toString(), "picture",
                         json["messages"].toString(), QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    //把聊天记录添加到当前聊天页面
    addMessageTo(page, myInfo.account, "picture",
                 json["messages"].toString(), time.toString("yyyy-MM-dd HH:mm:ss"));
    liftSomebody(json["receiver"].toString());
}


void MainWindow::on_but_tool2_clicked()//发送文件
{
    if(!checkSocket()) {
        return;
    }
    QString receiver = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();
    if (friendHash.find(receiver) == friendHash.end()) {
        Dialog dialog(this);
        dialog.transText("他不是您的好友!");
        dialog.exec();
        return;
    }
    //出文件对话框选择文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择要发送的文件"), QString(), tr("所有文件 (*)"));
    if (fileName.isEmpty()) {
        return; // 用户取消选择
    }
    //创建文件，检查有效性
    QFile *file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly)) {
        Dialog dialog(this);
        dialog.transText("无法打开文件!");
        dialog.exec();
        delete file;
        return;
    }
    //检查文件大小，如果太大则返回
    qint64 fileSize = file->size();
    const qint64 maxFileSize = 10LL * 1024 * 1024 * 1024 /4; //0.25g
    if (fileSize > maxFileSize) {
        Dialog dialog(this);
        dialog.transText("文件大小超过限制!");
        dialog.exec();
        file->close();
        delete file;
        return;
    }
    ui->edit_input->setEnabled(false);
    ui->but_send->setEnabled(false);
    //将读取和发送操作放入线程池
    QThreadPool::globalInstance()->start([this, file = std::move(file), receiver, fileName]() mutable {
        QByteArray fileData = file->readAll();
        file->close();
        QString fileBase64 = fileData.toBase64();
        QJsonObject json;
        json["tag"] = "messages";
        json["messages"] = fileBase64;
        json["sender"] = myInfo.account;
        json["messagetype"] = "document";
        json["receiver"] = receiver;
        json["filename"] = QFileInfo(fileName).fileName();
        QDateTime time = QDateTime::currentDateTime();
        time.setTimeZone(QTimeZone("Asia/Shanghai"));
        time = time.toTimeZone(QTimeZone::utc());
        json["timestamp"] = time.toString("yyyy-MM-dd HH:mm:ss");
        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson() + "END";
        //切换回主线程发送数据
        QMetaObject::invokeMethod(this, "sendDoc", Qt::QueuedConnection,
                                  Q_ARG(QByteArray, jsonData),
                                  Q_ARG(QString, json["filename"].toString()),
                                  Q_ARG(QString, json["timestamp"].toString()),
                                  Q_ARG(QString, json["receiver"].toString()));
    });
}


void MainWindow::sendDoc(const QByteArray &jsonData, const QString &filename,
                         const QString &timestamp, const QString &receiver)//发送文件
{
    socket->write(jsonData);
    socket->flush();
    QWidget *page = ui->stack_talks->currentWidget();
    if (page) {
        addMessageToDatabase(myInfo.account, receiver, "document", filename, timestamp);
        addMessageTo(page, myInfo.account, "document", filename, timestamp);
    }
    ui->edit_input->setEnabled(true);
    ui->but_send->setEnabled(true);
    liftSomebody(receiver);
}


void MainWindow::onTalkItemCurrentChanged()//聊天页面项切换项更新消息框视图
{
    qDebug()<<"聊天窗口切换了";
    // 获取当前选中的聊天项
    QModelIndex currentIndex = ui->list_talks->currentIndex();
    if (currentIndex.isValid()) {
        QString itemData = currentIndex.data(Qt::UserRole+1).toString();

        // 检查是否是AI助手
        if (itemData == "AI助手") {
            isAIChatActive = true;
            currentChatTarget = "AI助手";

            // 更新UI提示用户正在AI聊天模式
            ui->but_tool4->setStyleSheet(
                "QPushButton {"
                "    background-color: rgba(0, 123, 255, 0.3);"
                "    border: none;"
                "    color: black;"
                "}"
                "QPushButton:hover {"
                "    background-color: rgba(0, 123, 255, 0.5);"
                "}"
                "QPushButton:pressed {"
                "    background-color: rgba(0, 123, 255, 0.7);"
                "}"
                );
        } else {
            isAIChatActive = false;
            currentChatTarget = itemData;

            // 切换回普通聊天模式
            ui->but_tool4->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    border: none;"
                "    color: black;"
                "}"
                "QPushButton:hover {"
                "    background-color: rgba(0, 0, 0, 0.1);"
                "}"
                "QPushButton:pressed {"
                "    background-color: rgba(0, 0, 0, 0.2);"
                "}"
                );
        }
    }
    ui->lab_friendname->setText(ui->list_talks->currentIndex().data(Qt::DisplayRole).toString());
    ui->edit_input->setEnabled(true);
    ui->but_tool1->setEnabled(true);
    ui->but_tool2->setEnabled(true);
    ui->but_tool3->setEnabled(true);
    clearUnread(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString());
    if(!switchPageTo(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString())){
        ui->stack_talks->setCurrentIndex(0);
        qDebug()<<"找不到聊天页面";
        ui->edit_input->setEnabled(false);
        ui->but_tool1->setEnabled(false);
        ui->but_tool2->setEnabled(false);
        ui->but_tool3->setEnabled(false);
    }
}


void MainWindow::onFriendItemCurrentChanged()//好友列表页面项切换项更新右边视图
{
    auto index = ui->list_friends->currentIndex();
    if (!index.isValid()) {
        clearEditShow2();
        QPixmap pix(":/pictures/andan_touming.jpg");
        ui->edit_show2->background = pix;
        ui->edit_show2->repaint();
        return;
    }
    int row = index.row();
    if (row == 0 || row == 2) return;
    else if (row == 1) {//选择新的好友的操作 查看未处理的好友申请
        updateEditShow2New();
        return;
    }
    updateEditShow2Normal(index);//普通好友
}



void MainWindow::mousePressEvent(QMouseEvent *event)//鼠标点击事件
{
    //清除焦点
    QList<QWidget*> widgets = this->findChildren<QWidget*>();
    for (QWidget* widget : widgets) {
        widget->clearFocus();
    }
    QPoint pos = event->pos();
    int margin = 30;
    if (event->button() == Qt::LeftButton) {
        if (pos.x() > width() - frame || pos.y() > height() - frame) {//如果点击在边框
            qDebug() << "点击在边框";
            dragPosition = event->globalPosition().toPoint() - this->frameGeometry().topLeft();//记录鼠标相对于窗口的初始偏移
            resizeFlag = 1;
            event->accept();
        }
        else if (pos.x() > width() - margin && pos.y() > height() - margin) {//右下角
            qDebug() << "点击在角落";
            resizeFlag = 1;
            dragPosition = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
            event->accept();
        }
        else if (pos.x() <= 30 || pos.x() >= width() - 30 ||
                 pos.y() <= 30 || pos.y() >= height() - 30){//如果点击在边缘
            qDebug() << "点击在边缘";
            moveFlag = 1;
            dragPosition = event->globalPosition().toPoint() - this->frameGeometry().topLeft();//记录鼠标相对于窗口的初始偏移
            event->accept();
        }
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果与移动到边框显示不同的图标效果
{
    QPoint globalPos = event->globalPosition().toPoint();//获取当前的鼠标位置
    QPoint delta = globalPos - (this->frameGeometry().topLeft() + dragPosition);//计算当前鼠标位置与窗口之间的相对偏移
    if (event->buttons() & Qt::LeftButton) {
        if (resizeFlag == 1) {
            QSize newSize = this->size();
            //根据鼠标位置调整窗口大小
            if (dragPosition.x() > width() - frame) {//右边缘
                newSize.setWidth(this->width() + delta.x());
                setCursor(Qt::SizeHorCursor);
            }
            else if (dragPosition.y() > height() - frame) {//下边缘
                newSize.setHeight(this->height() + delta.y());
                setCursor(Qt::SizeVerCursor);//垂直调整大小光标
            }
            //角落调整
            if (dragPosition.x() > width() - frame && dragPosition.y() > height() - frame) {//右下角
                newSize.setWidth(this->width() + delta.x());
                newSize.setHeight(this->height() + delta.y());
                setCursor(Qt::SizeFDiagCursor);
            }
            //确保新尺寸不小于最小尺寸
            QSize minSize(100, 100);//设置最小尺寸
            newSize.setWidth(qMax(newSize.width(), minSize.width()));
            newSize.setHeight(qMax(newSize.height(), minSize.height()));
            //设置新的窗口大小
            this->resize(newSize);
            //更新按钮位置
            QPoint newButtonPos = QPoint(newSize.width() - ui->but_send->width() - 93, newSize.height() - ui->but_send->height() - 40);
            ui->but_send->move(newButtonPos);
            event->accept();
        }
        else if (moveFlag == 1) {
            this->move(globalPos - dragPosition);
            event->accept();
        }
        //更新拖动位置
        dragPosition = globalPos - this->frameGeometry().topLeft();
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
    resizeFlag = 0;
    unsetCursor();
}


void MainWindow::onReadyRead()//读取服务器回复数据
{
    jsonData += socket->readAll();//使用 += 追加接收的数据
    //检查数据是否以END结尾
    while (jsonData.endsWith("END")) {
        jsonData.chop(3);//移除结尾的"END"
        //确保 jsonData 完整，尝试将其解析为 JSON 文档
        jsonDoc = QJsonDocument::fromJson(jsonData);
        jsonData.clear();
        if (jsonDoc.isNull()) {
            qDebug() << "未能解析 JSON 数据，当前数据是:" << jsonData;
            return;//有可能数据未完整，提前返回
        }
        //处理解析成功的 JSON 对象
        jsonObj = jsonDoc.object();
        qDebug()<<jsonObj["tag"];
        //根据 "answer" 处理不同的请求
        if (jsonObj["tag"] == "friendmessage") {//好友信息
            uploadAll(jsonObj);
        }
        else if (jsonObj["tag"] == "deletefriendsucceed") {//删除好友成功
            deleteSucceed(jsonObj);
        }
        else if (jsonObj["tag"] == "deletefriendfail") {//删除好友失败
            deleteFail(jsonObj);
        }
        else if (jsonObj["tag"] == "serchaccount") {//获得了查找用户的信息
            sendSerach(jsonObj);//搜索好友的结果
        }
        else if (jsonObj["tag"] == "changeinformation") {//修改用户信息完成
            changeMyInfo(jsonObj);//修改用户信息的结果
        }
        else if (jsonObj["tag"] == "changepassword1") {//修改账号密码第一个申请的结果
            changePasswordAns1(jsonObj);//修改账号密码第一个申请的结果
        }
        else if (jsonObj["tag"] == "changepassword2") {//修改账号密码第一个申请的结果
            changePasswordAns2(jsonObj);//修改账号密码第二个申请的结果
        }
        else if (jsonObj["tag"] == "logout") {//注销账号的结果
            logoutAns(jsonObj);
        }
        else if (jsonObj["tag"] == "updatefriendship") {//处理好友申请的结果
            dealFriendsRequest(jsonObj);
        }
        else if (jsonObj["tag"] == "newaddrequest") {//接收到新的好友申请 将其加入好友申请列表
            dealNewAddRequest(jsonObj);
        }
        else if (jsonObj["tag"] == "addrequestpass") {//发送的好友申请通过 将其加入好友列表
            dealAddRequestPass(jsonObj);
        }
        else if (jsonObj["tag"] == "youaredeleted") {//处理被删除好友
            dealYouAreDeleted(jsonObj);
        }
        else if (jsonObj["tag"] == "youarekickedoffline") {//处理被挤下线
            dealYouAreKickedOffline(jsonObj);
        }
        else if (jsonObj["tag"] == "yourmessages") {//处理接收到的聊天消息
            dealMessages(jsonObj);
        }
        else if (jsonObj["tag"] == "document") {//处理接收到的文件
            dealDocument(jsonObj);
        }
    }
}


void MainWindow::uploadAll(const QJsonObject& json)//加载好友数据(包含聊天记录 好友申请等)
{
    //删除除了前三个索引之外的所有项
    int rowCount = friendModel->rowCount();
    for (int i = 3; i < rowCount; ++i) {
        friendModel->removeRow(i);
    }
    //从传入的 JSON 对象中获取好友数组
    QJsonArray friendsArray = json["friends"].toArray();
    //加载好友列表
    uploadFriedList(friendsArray);
    //加载未读好友申请
    //从传入的JSON对象中获取好友申请数组
    QJsonArray friendsRequestsArray= json["newfriends"].toArray();
    uploadFriedRequest(friendsRequestsArray);
    //加载消息列表
    uploadListMessages(accountNumber);
    //加载消息记录
    uploadMessages(accountNumber);
    //加载未读消息记录
    QJsonArray friendsRequestsArray1= json["unreadmessages"].toArray();
    uploadUnreadMessages(friendsRequestsArray1);
    return;
}


void MainWindow::uploadFriedList(const QJsonArray &friendsArray)//加载好友列表
{
    //遍历每个好友的 JSON 对象
    for (const QJsonValue &value : friendsArray) {
        QJsonObject friendObject = value.toObject();//将当前值转换为 QJsonObject
        if(friendObject["accountflag"] == true){//如果是本人的信息 那么就不加载到好友列表中 改为加载到成员变量中
            myInfo.account = friendObject["account_num"].toString();
            myInfo.gender = friendObject["gender"].toString();
            myInfo.name = friendObject["nickname"].toString();
            myInfo.signature = friendObject["signature"].toString();
            myInfo.avator_base64 = friendObject["avator"].toString();
            friendHash.insert(myInfo.account, myInfo);
            continue;
            qDebug()<<"用户信息加载完毕"<<myInfo.account<<myInfo.name;
        }
        /*    struct AccountInfo {//存储好友信息
        QString account;//账号
        QString name;//名称
        QString avator_base64;//头像的 Base64 编码字符串
        QString gender;//性别
        QString signature;//签名
         };
        */
        //读取到好友列表中
        AccountInfo friendInfo;
        friendInfo.account = friendObject["account_num"].toString();
        friendInfo.gender = friendObject["gender"].toString();
        friendInfo.avator_base64 = friendObject["avator"].toString();
        friendInfo.name = friendObject["nickname"].toString();
        friendInfo.signature = friendObject["signature"].toString();
        friendHash.insert(friendInfo.account, friendInfo);
        //给listview创建一个新的标准项，用于表示好友
        QStandardItem *item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        //设置标准项的显示文本为好友的昵称
        item->setText(friendObject["nickname"].toString());
        //设置标准项为不可编辑状态
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);//设置为不可编辑
        //将好友的信息存储在标准项的用户角色数据中
        item->setData(friendObject["account_num"].toString(), Qt::UserRole + 1);//存储账号
        item->setData(friendObject["gender"].toString(), Qt::UserRole + 2);//存储性别
        item->setData(friendObject["signature"].toString(), Qt::UserRole + 3);//存储个性签名
        item->setData(friendObject["avator"].toString(), Qt::UserRole + 4);//存储头像的Base64字符串
        //将创建的标准项添加到好友模型中
        friendModel->appendRow(item);//将 item 添加到模型
    }
}


void MainWindow::uploadFriedRequest(const QJsonArray &friendsRequestArray)//加载好友申请
{
    bool flag = false;
    //遍历每个申请者
    for (const QJsonValue &value : friendsRequestArray) {
        QJsonObject newFriendObject = value.toObject();//将当前值转换为 QJsonObject
        qDebug()<<"发送好友申请的人有"<<newFriendObject["account_num"];
        //创建一个AccountInfo 结构体实例
        AccountInfo newFriend;
        newFriend.account = newFriendObject["account_num"].toString();
        newFriend.name = newFriendObject["nickname"].toString();
        newFriend.avator_base64 = newFriendObject["avator"].toString();
        newFriend.gender = newFriendObject["gender"].toString();
        newFriend.signature = newFriendObject["signature"].toString();
        //将新好友信息添加到好友申请列表中
        newFriendArray.append(newFriend);
        flag = true;
    }
    if(flag){
        QModelIndex secondItemIndex = ui->list_friends->model()->index(1, 0);
        ui->list_friends->model()->setData(secondItemIndex, true, Qt::UserRole + 20);
        ui->list_friends->model()->dataChanged(secondItemIndex, secondItemIndex, {Qt::UserRole + 20});
    }
}


void MainWindow::uploadListMessages(const QString& account)//加载消息列表
{
    QSqlQuery qry(db);
    qry.prepare("SELECT * FROM talks");
    if (qry.exec()){
        qDebug() << "查找消息列表要加载什么";
        while(qry.next()){
            QString account = qry.value("friend_id").toString();
            auto it = friendHash.find(account);
            if(it == friendHash.end()){continue;}
            AccountInfo friendinfo = it.value();//用户信息
            QString unread = qry.value("unread").toString();//未读数量
            QString latestMessage, timestamp ;//最近的消息 消息发送时间
            if(it != friendHash.end()){//如果好友列表确实有这个账号的话
                //查询 messages 表，找到该好友的最近一条消息
                QSqlQuery msgQuery(db);
                msgQuery.prepare("SELECT message, messagetype, timestamp FROM messages "
                                 "WHERE sender = :friendAccount OR receiver = :friendAccount "
                                 "ORDER BY timestamp DESC LIMIT 1");
                msgQuery.bindValue(":friendAccount", account);
                if (msgQuery.exec() && msgQuery.next()) {
                    if(msgQuery.value("messagetype").toString() == "picture"){
                        latestMessage = "[图片消息]";
                    }
                    else if(msgQuery.value("messagetype").toString() == "document"){
                        latestMessage = "[文件消息]";
                    }
                    else {
                        latestMessage = msgQuery.value("message").toString();
                    }
                    //处理时间戳
                    timestamp = msgQuery.value("timestamp").toString();
                    QDateTime messageDateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");
                    messageDateTime.setTimeZone(QTimeZone::utc());//设置为utc
                    //设置时区为本地标准时间
                    QTimeZone timeZone = QTimeZone::systemTimeZone();//使用系统时区
                    messageDateTime = messageDateTime.toTimeZone(timeZone);
                    timestamp = messageDateTime.toString("yyyy-MM-dd HH:mm:ss");
                }
            }
            addSomeoneToTalkList(friendinfo, latestMessage, timestamp, unread);
            if(unread != "0"){
                liftSomebody(friendinfo.account);
            }
        }
    }
}


void MainWindow::uploadMessages(const QString& account)//加载消息记录
{
    ui->but_tool1->setEnabled(false);
    ui->but_tool2->setEnabled(false);
    ui->but_tool3->setEnabled(false);
    //遍历聊天列表
    for(auto it : friendHash){//c++标准容器这里遍历的是键值对
        qDebug()<<"遍历到"<<it.account;
        if(it.account == accountNumber){
            continue;
        }
        //创建一个新的页面
        QWidget *newPage = new QWidget();
        //为新页面设置对象名称
        newPage->setObjectName(it.account);
        QListWidget *list = new QListWidget();
        setupMessageList(list);
        list->setObjectName("notime");
        QVBoxLayout *layout0 = new QVBoxLayout(newPage);
        layout0->addWidget(list);
        newPage->setLayout(layout0);
        //设置newPage的内容
        QSqlQuery qry(db);
        qry.prepare("SELECT * FROM messages WHERE sender = :friend OR receiver = :friend ORDER BY timestamp ASC");
        qry.bindValue(":friend", it.account);
        qry.exec();
        while(qry.next()){//遍历所有消息记录
            QString sender = qry.value("sender").toString();
            QString messageType = qry.value("messagetype").toString();
            QString message = qry.value("message").toString();
            QString timestamp = qry.value("timestamp").toString();
            addMessageTo(newPage, sender, messageType, message, timestamp);
        }
        //将新页面添加到stackedWidget 中
        ui->stack_talks->addWidget(newPage);
    }
}


void MainWindow::uploadUnreadMessages(const QJsonArray &messagesArray)//加载未读聊天记录
{
    //遍历消息
    for(const QJsonValue &value : messagesArray){
        QJsonObject json = value.toObject();//将当前值转换为 QJsonObject
        addSomeoneToTalkList(friendHash[json["sender"].toString()], "", "", "");
        updateUnread(json["sender"].toString());
        QWidget *page = ui->stack_talks->findChild<QWidget*>(json["sender"].toString());
        addMessageTo(page, json["sender"].toString(), json["message_type"].toString(), json["content"].toString(),
                     json["timestamp"].toString());
        addMessageToDatabase(json["sender"].toString(), json["receiver"].toString(), json["message_type"].toString(),
                             json["content"].toString(),
                             json["timestamp"].toString());
        liftSomebody(json["sender"].toString());
    }
}


void MainWindow::deleteSucceed(const QJsonObject& json)//删除好友成功
{
    emit deleteDone();
    Dialog* dialog = new Dialog(this);
    dialog->transText("删除好友成功!");
    dialog->exec();
    deleteSomeoneInFriendList(json["account"].toString());
    deleteSomeoneInTalkList(json["account"].toString());
    //更新视图
    //清理旧的布局和控件
    clearEditShow2();
    QPixmap pix(":/pictures/andan_touming.jpg");
    ui->edit_show2->background = pix;
    ui->edit_show2->repaint();
    ui->list_friends->clearSelection();
}


void MainWindow::deleteFail(const QJsonObject& json)//删除失败
{
    emit deleteDone();
    Dialog* dialog = new Dialog(this);
    dialog->transText("删除好友失败!");
    dialog->exec();
}


void MainWindow::sendSerach(const QJsonObject& json)//搜索好友的结果
{
    emit serachResult(json);
}


void MainWindow::changeMyInfo(const QJsonObject& json)//修改用户信息的结果
{
    emit changeResult(json);//发送信号 告知修改个人资料窗口结果
    if(json["answer"] == "succeed"){//成功则修改个人资料
        myInfo.account = json["account"].toString();
        myInfo.name = json["nickname"].toString();
        myInfo.gender = json["gender"].toString();
        myInfo.signature = json["signature"].toString();
        myInfo.avator_base64 = json["avator"].toString();
        ui->lab_avator->setPixmap(base64ToPixmap(myInfo.avator_base64));
        QString avator = myInfo.avator_base64;
        //保存头像 让下次登录可以加载
        avator = avator.simplified();
        QByteArray byteArray = QByteArray::fromBase64(avator.toUtf8());
        QImage image;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::ReadOnly);
        if (!image.load(&buffer, "PNG")) {
            qDebug() << "无法从数据加载图像";
            return;
        }
        //获取运行时目录
        QString appDir = QCoreApplication::applicationDirPath();
        QDir dir(appDir);
        //创建目录（如果不存在）
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QString saveLoginAvatorPath = dir.filePath("laseloginavator.png");
        if (image.save(saveLoginAvatorPath)) {
            qDebug() << "头像已成功保存到" << saveLoginAvatorPath;
        } else {
            qDebug() << "保存头像失败";
        }
    }
}


void MainWindow::changePasswordAns1(const QJsonObject& json)//修改账号密码第一个申请的结果
{
    emit changePasswordAnswer1(json);
}


void MainWindow::changePasswordAns2(const QJsonObject& json)//修改账号密码第二个申请的结果
{
    emit changePasswordAnswer2(json);
}


void MainWindow::logoutAns(const QJsonObject& json)//注销的结果
{
    if(json["answer"] == "success"){
        QSettings settings("settings.ini", QSettings::IniFormat); // 记住的上次登录的账号
        QSettings set("set.ini", QSettings::IniFormat); // 是否记住账号密码
        settings.remove(myInfo.account);
        settings.setValue("lastlogin", "");
        set.setValue("autologin", false);
        set.setValue("autologinuser", "");
        //获取运行时目录目录
        QString appDir = QCoreApplication::applicationDirPath();
        QDir dir(appDir);
        //创建目录（如果不存在）
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QString saveLoginAvatorPath = dir.filePath("laseloginavator.png");
        QFile::remove(saveLoginAvatorPath);
        Dialog dialog(this);
        dialog.transText("注销成功!");
        dialog.exec();
        this->close();
    }
    else {
        emit logoutAnswer(json);
    }
}


void MainWindow::dealFriendsRequest(const QJsonObject& json)//处理好友申请的结果
{
    if(json["answer"] == "succeed"){
        if(json["type"] == "accept"){
            for (int i = newFriendArray.size() - 1; i >= 0; --i) {
                if (newFriendArray[i].account == json["sender"].toString()) {
                    //添加到好友列表
                    AccountInfo friendInfo;
                    friendInfo.account = newFriendArray[i].account;
                    friendInfo.gender = newFriendArray[i].gender;
                    friendInfo.avator_base64 = newFriendArray[i].avator_base64;
                    friendInfo.name = newFriendArray[i].name;
                    friendInfo.signature = newFriendArray[i].signature;
                    friendHash.insert(friendInfo.account, friendInfo);
                    checkFriendInTalk(friendInfo.account);
                    //创建一个新的标准项，用于表示好友
                    QStandardItem *item = new QStandardItem();
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                    //设置标准项的显示文本为好友的昵称
                    item->setText(newFriendArray[i].name);
                    //设置标准项为不可编辑状态
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);//设置为不可编辑
                    //将好友的信息存储在标准项的用户角色数据中
                    item->setData(newFriendArray[i].account, Qt::UserRole + 1);
                    item->setData(newFriendArray[i].gender, Qt::UserRole + 2);
                    item->setData(newFriendArray[i].signature, Qt::UserRole + 3);
                    item->setData(newFriendArray[i].avator_base64, Qt::UserRole + 4);
                    //将创建的标准项添加到好友模型中
                    friendModel->appendRow(item);//将 item 添加到模型
                    newFriendArray.removeAt(i);
                    updateEditShow2New();
                }
            }
        }
        else if(json["type"] == "reject"){
            for (int i = newFriendArray.size() - 1; i >= 0; --i) {
                if (newFriendArray[i].account == json["sender"].toString()) {
                    newFriendArray.removeAt(i);
                    updateEditShow2New();
                }
            }
        }
    }
    else if(json["answer"] == "fail"){
    }
}


void MainWindow::dealNewAddRequest(const QJsonObject& json)//接收到新的好友申请 将其加入好友申请列表
{
    //创建一个AccountInfo 结构体实例
    AccountInfo newFriend;
    newFriend.account = json["account_num"].toString();
    newFriend.name = json["nickname"].toString();
    newFriend.avator_base64 = json["avator"].toString();
    newFriend.gender = json["gender"].toString();
    newFriend.signature = json["signature"].toString();
    //将新好友信息添加到好友申请列表中
    newFriendArray.append(newFriend);
    QModelIndex index = ui->list_friends->model()->index(1,0);
    ui->list_friends->model()->setData(index, true, Qt::UserRole + 20);
    ui->list_friends->model()->dataChanged(index, index, {Qt::UserRole + 20});
    if(ui->list_friends->currentIndex().row() == 1){
        ui->list_friends->setCurrentIndex(index);
        updateEditShow2New();
    }
}


void MainWindow::dealAddRequestPass(const QJsonObject& json)//发送的好友申请通过 将其加入好友列表
{
    qDebug()<<"你发送的好友申请通过了";
    //添加到好友列表
    AccountInfo friendInfo;
    friendInfo.account = json["account_num"].toString();
    friendInfo.gender = json["gender"].toString();
    friendInfo.avator_base64 = json["avator"].toString();
    friendInfo.name = json["nickname"].toString();
    friendInfo.signature = json["signature"].toString();
    friendHash.insert(friendInfo.account, friendInfo);
    checkFriendInTalk(friendInfo.account);
    //创建一个新的标准项，用于表示好友
    QStandardItem *item = new QStandardItem();
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //设置标准项的显示文本为好友的昵称
    item->setText(json["nickname"].toString());
    //设置标准项为不可编辑状态
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);//设置为不可编辑
    //将好友的信息存储在标准项的用户角色数据中
    item->setData(json["account_num"], Qt::UserRole + 1);
    item->setData(json["gender"], Qt::UserRole + 2);
    item->setData(json["signature"], Qt::UserRole + 3);
    item->setData(json["avator"], Qt::UserRole + 4);
    //将创建的标准项添加到好友模型中
    friendModel->appendRow(item);
}


void MainWindow::dealYouAreDeleted(const QJsonObject& json)//处理被删除好友
{
    deleteSomeoneInFriendList(json["account"].toString());
    deleteSomeoneInTalkList(json["account"].toString());
}


void MainWindow::dealYouAreKickedOffline(const QJsonObject& json)//处理被挤下线
{
    Dialog dialog(this);
    dialog.transText("您的账号在别处登录,您已下线!");
    socket = nullptr;
    dialog.exec();
    this->close();
}

void MainWindow::dealMessages(const QJsonObject& json)//处理接收到的聊天消息
{
    // // 处理接收到的聊天消息 : 函数签名。接收一个 const 引用类型的 QJsonObject。
    // QJsonObject 是 Qt 处理 JSON 数据结构的核心类，代表一个 JSON 对象（键值对集合）。

    // // 确保发送者在聊天列表中显示或更新其信息
    // json["sender"].toString() 从接收到的 JSON 中获取消息发送者的账号，并转换为 QString。
    // friendHash[账号] 假设 friendHash 是一个 QHash 或 QMap 成员变量，存储了好友账号到好友信息的映射（如昵称、头像等）。这里根据发送者账号查找到其好友信息。
    // addSomeoneToTalkList() 假设是 MainWindow 的一个成员函数，用于将某个对象添加到聊天列表 (ui->list_talks) 中。
    // 原理：调用这个函数是为了确保发送消息的人在“最近聊天”列表 ui->list_talks 中有对应的条目。如果他是第一次给你发消息且不在列表里，这个函数会创建并添加到列表；如果已经在列表里，可能只是更新一下信息（例如将他对应的列表项移到顶部表示最近联系）。
    // 后续的 "", "", "" 参数，根据函数名猜测，可能是用于设置列表项的最后一条消息预览、时间戳、或草稿等，这里传入空字符串表示默认处理。
    addSomeoneToTalkList(friendHash[json["sender"].toString()], "", "", "");

    // // 查找对应聊天对象的聊天界面
    // ui->stack_talks 假设是一个 QStackedWidget，用于在不同好友的聊天窗口之间切换显示。每个好友的聊天界面可能是一个独立的 QWidget，被添加到了这个 QStackedWidget 中，并以好友账号作为 objectName。
    // findChild<QWidget*>() 是 QObject 的一个方法，用于在其子对象树中查找指定类型和名称的子对象。
    // <QWidget*> 指定查找类型为 QWidget 指针。
    // json["sender"].toString() 提供要查找的子控件的 objectName，这里是发送者的账号。
    // 原理：尝试在 stacked widget 中找到与消息发送者对应的那个聊天界面（假设每个聊天界面控件都将其 objectName 设置为了对方的账号）。如果找到，page 指针将指向该控件，否则为 nullptr。
    QWidget *page = ui->stack_talks->findChild<QWidget*>(json["sender"].toString());

    // // 更新未读消息状态
    // updateUnread() 假设是 MainWindow 的一个成员函数，用于更新某个对象的未读消息计数或视觉标记。
    // json["sender"].toString() 传入发送者的账号。
    // 原理：调用这个函数是为了在 ui->list_talks 中更新发送者对应的列表项的未读状态，例如增加未读消息数量显示，或者显示一个红点。
    updateUnread(json["sender"].toString());

    // // 将消息存储到数据库
    // addMessageToDatabase() 假设是 MainWindow 的一个成员函数，用于将消息记录保存到数据库。
    // 传入消息的详细信息：发送者账号、接收者账号、消息类型、消息内容、时间戳。
    // 原理：确保消息的持久化存储，以便下次用户登录或查看历史记录时能够加载。
    addMessageToDatabase(json["sender"].toString(), json["receiver"].toString(), json["messagetype"].toString(),
                         json["messages"].toString(), // json["messages"] 是消息内容的字段
                         json["timestamp"].toString()); // json["timestamp"] 是消息时间戳的字段

    // // 将消息添加到对应的聊天界面显示
    // addMessageTo() 假设是 MainWindow 的一个成员函数，用于将接收到的消息添加到指定的聊天界面控件中显示出来。
    // 传入参数：聊天界面控件指针 (page)、发送者账号、消息类型、消息内容、时间戳。
    // 原理：如果 page 有效（即找到了对应的聊天界面），这个函数会将消息内容格式化后添加到该界面（例如添加到文本框或聊天记录视图中），让用户在当前打开这个聊天时能看到新消息。如果 page 是 nullptr，这个函数应该能处理这种情况（比如不做任何界面更新）。
    addMessageTo(page, json["sender"].toString(), json["messagetype"].toString(),
                 json["messages"].toString(), json["timestamp"].toString());

    // // 提升聊天界面到最前
    // liftSomebody() 假设是 MainWindow 的一个成员函数，用于将某个对象的聊天界面提升到最前面显示。
    // 传入发送者的账号。
    // 原理：调用这个函数是为了在接收到新消息时，自动将发送者对应的聊天界面在 ui->stack_talks 中显示出来，让用户可以直接看到新消息的对话窗口。
    liftSomebody(json["sender"].toString());

    // 函数在此结束。
}

void MainWindow::dealDocument(const QJsonObject& json)//处理接收到的文件
{
    QString filename = json["filename"].toString();
    //选择保存路径
    QString savePath = QFileDialog::getSaveFileName(this, tr("Save File"), filename);
    if (savePath.isEmpty()) {
        qDebug() << "保存文件被取消";
        return;
    }
    Dialog dialog(this);
    dialog.transText("正在下载文件，请勿关闭窗口!");
    dialog.exec();
    //开启一个新线程处理文件保存
    QtConcurrent::run([=]() {
        QByteArray fileData = QByteArray::fromBase64(json["data"].toString().toUtf8());
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(fileData);
            file.close();
            qDebug() << "文件保存成功:" << savePath;
            //在主线程中发送信号
            QMetaObject::invokeMethod(this, "handleSaveDone", Qt::QueuedConnection, Q_ARG(QString, "保存成功"));
        } else {
            qDebug() << "文件保存失败:" << file.errorString();
            //在主线程中发送信号
            QMetaObject::invokeMethod(this, "handleSaveDone", Qt::QueuedConnection, Q_ARG(QString, "保存失败"));
        }
    });
}


void MainWindow::handleSaveDone(const QString &status)//发送文件保存完毕的信号
{
    emit saveDone(status);
}
