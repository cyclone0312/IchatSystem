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
    friendModel(new QStandardItemModel(this))
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
    //设置列表
    ui->list_friends->setViewMode(QListView::ListMode);//设置视图模式为列表模式
    ui->list_friends->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为为选择整行
    ui->list_friends->setSelectionMode(QAbstractItemView::SingleSelection);//设置选择模式为单选
    ui->list_friends->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->list_friends->setItemDelegate(new FriendDelegate(this));//设置自定义的委托用于绘制列表项
    //设置项
    friendModel = new QStandardItemModel(this);
    ui->list_friends->setModel(friendModel);
    QStandardItem *addFriendItem0 = new QStandardItem("新的朋友");
    QStandardItem *addFriendItem = new QStandardItem("新的朋友");
    QStandardItem *friendListItem = new QStandardItem("好友列表");
    //设置不可选标志
    addFriendItem0->setFlags(Qt::NoItemFlags);
    addFriendItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    friendListItem->setFlags(Qt::NoItemFlags);
    //获取代表添加好友的图标
    //转换为 QPixmap，指定图标的大小
    QPixmap addFriendPixmap(":/pictures/addfriends.jpg"); // 32x32 像素的图标
    //将 QPixmap 转换为 Base64
    QImage image = addFriendPixmap.toImage();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); // 将图像保存为 PNG 格式
    QByteArray byteArray = buffer.data();
    QString base64String = byteArray.toBase64();
    //将 Base64 字符串设置到 addFriendItem 中
    addFriendItem->setData(base64String, Qt::UserRole + 4);
    //将项添加到模型中
    friendModel->appendRow(addFriendItem0);
    friendModel->appendRow(addFriendItem);
    friendModel->appendRow(friendListItem);
    //初始化模型
    filterProxyModel = new FilterProxyModel(this);
    filterProxyModel->setSourceModel(friendModel);
    //设置模型到列表视图
    ui->list_friends->setModel(filterProxyModel);
    //连接信号与槽
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


bool MainWindow::deleteSomeoneInTalkList(const QString &account)//删除聊天列表中的某个聊天
{
    //清理hash
    if(messageListHash.find(account) != messageListHash.end()){
        messageListHash.remove(account);
    }
    if(ui->list_talks->currentIndex().data(Qt::UserRole + 1) == account){
        ui->stack_talks->setCurrentIndex(0);
    }
    //获取源模型
    QAbstractItemModel *sourceModel = talkFilterProxyModel->sourceModel();
    if (sourceModel) {
        for (int row = 0; row < sourceModel->rowCount(); ++row) {
            QModelIndex index = sourceModel->index(row, 0);
            QVariant data = sourceModel->data(index, Qt::UserRole + 1);
            QString string = data.toString();
            //匹配上则删除
            if (string == account) {
                ui->list_talks->update();
                QSqlQuery qry(db);
                qry.prepare("DELETE FROM talks WHERE friend_id = :friend_id");
                qry.bindValue(":friend_id", account);
                qry.exec();
                if(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString() == account){
                    qDebug()<<"当前聊天的人的对话框被删除";
                    ui->lab_friendname->setText("");
                }
                sourceModel->removeRow(row);
                ui->list_talks->clearSelection();
                return true;
            }
        }
    }
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


bool MainWindow::switchPageTo(const QString &friendId)//将聊天页面切换到账号为account的页面
{
    if(friendId.isEmpty() || !ui->list_talks->currentIndex().isValid()){
        return false;
    }
    QWidget *targetPage = ui->stack_talks->findChild<QWidget*>(friendId);
    if (targetPage) {
        int index = ui->stack_talks->indexOf(targetPage);
        ui->stack_talks->setCurrentIndex(index);
        QListWidget *list = targetPage->findChild<QListWidget *>();
        list->scrollToBottom();
        qDebug()<<"已经有这个聊天窗口了";
        return true;
    }
    if(friendHash.find((friendId)) == friendHash.end()){//没有这个好友
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


QPixmap MainWindow::loadAvator(const QString& friendId)//获得某人头像
{
    if (friendId.isEmpty()) {
        return QPixmap();
    }
    auto it = avatorHash。find(friendId);
    if (it != avatorHash。end()) {
        return it.value();
    } else {
        auto it1 = friendHash.find(friendId);
        if (it1 != friendHash.end()) {
            QString avator = it1.value().avator_base64;
            QPixmap pix = base64ToPixmap(avator);
            if (!pix.isNull()) {
                avatorHash。insert(friendId， pix);
            }
            return pix;
        } else {
            //没有时返回默认头像
            return QPixmap(":/pictures/suliao_avator_normal.jpg");
        }
    }
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
        result = dateTimestamp.toString("           yyyy-MM-dd hh:mm");
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
    if (changeInfoFlag == 0) {
        MyAccountInfo tmpInfo;
        tmpInfo.account = myInfo.account;
        tmpInfo.name = myInfo.name;
        tmpInfo.avator_base64 = myInfo.avator_base64;
        tmpInfo.gender = myInfo.gender;
        tmpInfo.signature = myInfo.signature;
        dialogChangeInfo = new ChangeInformation(tmpInfo, this);
        connect(dialogChangeInfo, &ChangeInformation::customClose, this, [this]() {
            qDebug() << "修改信息窗口重置了";
            changeInfoFlag = 0;
        });
        connect(dialogChangeInfo, &ChangeInformation::sendMessage, this, &MainWindow::sendMessage);
        connect(this, &MainWindow::changeResult, dialogChangeInfo, &ChangeInformation::dealResult);
        dialogChangeInfo->show();
        changeInfoFlag = 1;
    } else if (changeInfoFlag == 1) {
        dialogChangeInfo->raise();
        if (dialogChangeInfo) {
            int x = this->x() + (this->width() - dialogChangeInfo->width()) / 2;
            int y = this->y() + (this->height() - dialogChangeInfo->height()) / 2;
            dialogChangeInfo->move(x, y);
        }
    }
}


void MainWindow::listtalkChoice(const QJsonObject& json)//聊天列表的菜单选择完毕
{
    if(json.empty()){//如果是空 触发点击事件
        ui->list_talks->clicked(ui->list_talks->currentIndex());
    }
    else if(json["tag"] == "sendmessage"){//发送消息
        ui->edit_input->setFocus();
        QCursor::setPos(ui->edit_input->mapToGlobal(QPoint(30, 30)));//移动鼠标到edit_input内
    }
    else if(json["tag"] == "message"){//好友信息
        FriendMessage::FriendInfo info;
        info.avator_base64 = ui->list_talks->currentIndex().data(Qt::UserRole + 4).toString();
        info.account = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();
        info.name = ui->list_talks->currentIndex().data(Qt::DisplayRole).toString();
        info.signature = ui->list_talks->currentIndex().data(Qt::UserRole + 3).toString();
        info.gender = ui->list_talks->currentIndex().data(Qt::UserRole + 2).toString();
        FriendMessage *dialog = new FriendMessage(info,this);
        dialog->exec();
    }
    else if(json["tag"] == "deletefriend"){//删除好友
        if(friendHash.find(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString()) == friendHash.end()){
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
            if(!checkSocket()){
                return;
            }
            QJsonObject jsonObj;
            jsonObj["tag"] = "deletefriend";
            jsonObj["account"] = myInfo.account;
            jsonObj["friend"] = ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString();;
            QJsonDocument jsonDoc(jsonObj);
            QByteArray jsonData = jsonDoc.toJson() + "END";
            socket->write(jsonData);
            socket->flush();
            qDebug()<<"发送了删除好友请求"<<jsonObj["friend"];
        }
    }
    else if(json["tag"] == "deletetalk"){//从消息列表移除
        deleteSomeoneInTalkList(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString());
    }
    else if(json["tag"] == "clearmessage"){//清空聊天记录
        ChoiceDialog dialog(this);
        dialog.transText("你确定要清空聊天记录吗？");
        dialog.transButText("确定", "取消");
        int result = dialog.exec();
        if(result == QDialog::Accepted){
            QSqlQuery qry(db);
            qry.prepare("DELETE FROM messages WHERE sender = :friend OR receiver = :friend");
            qry.bindValue(":friend", ui->list_talks->currentIndex().data(Qt::UserRole + 1));
            qry.exec();
            auto currentIndex = ui->list_talks->currentIndex();
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 5);
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 6);
            talkFilterProxyModel->setData(currentIndex, "", Qt::UserRole + 10);
            talkFilterProxyModel->dataChanged(currentIndex, currentIndex, {Qt::UserRole + 10});
            qDebug()<<ui->list_talks->currentIndex().data(Qt::UserRole + 10);
            QWidget* page = ui->stack_talks->findChild<QWidget *>(ui->list_talks->currentIndex().data(Qt::UserRole + 1).toString());
            QListWidget *list = page->findChild<QListWidget *>();
            list->clear();
            list->setObjectName("notime");//10.18修改
            Dialog dialog(this);
            dialog.transText("清空聊天记录成功!");
            dialog.exec();
        }
    }
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
    addSomeoneToTalkList(friendHash[json["sender"].toString()], "", "", "");
    QWidget *page = ui->stack_talks->findChild<QWidget*>(json["sender"].toString());
    updateUnread(json["sender"].toString());
    addMessageToDatabase(json["sender"].toString(), json["receiver"].toString(), json["messagetype"].toString(),
                         json["messages"].toString(),
                         json["timestamp"].toString());
    addMessageTo(page, json["sender"].toString(), json["messagetype"].toString(),
                 json["messages"].toString(), json["timestamp"].toString());
    liftSomebody(json["sender"].toString());
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
            文件。write(fileData);
            文件。close();
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
