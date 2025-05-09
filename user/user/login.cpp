#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao_avator.png");
    setWindowIcon(icon);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //初始化头像
    setAva();
    //初始化左上角图标
    setIcon();
    //设置输入验证器
    setVal();
    //设置一些计时器 用于动态背景与动态按钮
    setTimer();
    //连接服务器
    tcpConnect();
    //连接socket的信号槽
    connect(&socket, &QTcpSocket::readyRead, this, &Login::onReadyRead);//连接读取服务器回发的信息
    //显示窗口
    this->show();
    //判断是否需要自动登录以及记住密码
    ifAutoLogin();
}


Login::~Login()
{
    delete ui;
    delete validator;
    delete timer;
    delete cancelTimer;
}

void Login::setAva()//初始化头像
{
    // 获取应用程序可执行文件所在的目录的绝对路径。
    // QCoreApplication::applicationDirPath() 是一个静态方法，返回运行中的应用程序的路径。
    // 例如，如果您的程序在 /home/user/MyApp/myapp 可执行文件，这个方法将返回 "/home/user/MyApp"。
    QString appDir = QCoreApplication::applicationDirPath();

    // 创建一个 QDir 对象，表示上面获取到的应用程序目录。
    // QDir 类提供了访问和操作目录结构的功能。
    QDir dir(appDir);

    // 这段代码检查应用程序目录是否存在，并尝试创建它（如果不存在）。
    // 对于 QCoreApplication::applicationDirPath() 返回的目录，它总是存在的（因为程序就运行在这个目录里），
    // 所以这里的 dir.exists() 检查和 dir.mkpath(".") 调用在绝大多数情况下是冗余的，但作为创建目录的通用模式是正确的。
    // dir.mkpath(".") 表示在 dir 对象所代表的路径下创建 "." 目录，即创建 appDir 本身。
    if (!dir.exists()) { // 如果应用程序目录不存在 (理论上不会发生)
        dir.mkpath("."); // 则创建它 (及必要的父目录)
    }

    // 构建上次登录头像文件的完整路径。
    // dir.filePath("laseloginavator.png") 安全地将目录路径 (appDir) 和文件名 ("laseloginavator.png") 组合起来，
    // 会自动处理不同操作系统下的路径分隔符（如 Windows 的 '\' 或 Linux/macOS 的 '/'）。
    // saveLoginAvatorPath 应该是一个成员变量，用于存储这个路径。
    saveLoginAvatorPath = dir.filePath("laseloginavator.png");

    // 检查指定路径下的文件是否存在。
    // QFile::exists() 是一个静态方法，用于判断给定路径的文件是否存在。
    if (QFile::exists(saveLoginAvatorPath)) {//如果有上次登录成功的人就加载头像
        // 如果上次登录成功的头像文件存在

        // 创建一个 QPixmap 对象，并从文件中加载图片数据。
        // QPixmap 类用于处理图片，通常用于屏幕显示，是设备相关的。
        QPixmap newpixmap(saveLoginAvatorPath);

        // 缩放加载的图片。
        // scaled() 方法创建一个新的 QPixmap，它是原图片按照指定尺寸和缩放模式缩放后的结果。
        // 100, 100 是目标宽度和高度。
        // Qt::KeepAspectRatio 是缩放模式：保持图片的原始宽高比例，使其完全放入 100x100 的矩形内，可能会在宽或高方向留白。
        newpixmap = newpixmap.scaled(100, 100, Qt::KeepAspectRatio);

        // 将缩放后的图片设置到 UI 界面的头像 QLabel 控件上。
        // ui->lab_avator 假设是一个指向 UI 文件中名为 lab_avator 的 QLabel 控件的指针。
        // setPixmap() 是 QLabel 的方法，用于显示 QPixmap 图片。
        ui->lab_avator->setPixmap(newpixmap);

        // 完成头像设置，函数返回。
        return;
    }

    // 如果上面的 if 条件不成立（即上次登录成功的头像文件不存在），则执行这里的代码。
    // 从 Qt 资源文件中加载默认头像图片。
    // ":/..." 是 Qt 资源系统的特殊前缀和路径语法，表示从编译到程序中的资源文件 (.qrc) 中加载文件。
    // suliao_avator_normal.jpg 应该是被添加到了项目的资源文件并命名为 :/pictures/suliao_avator_normal.jpg。
    QPixmap pixmap(":/pictures/suliao_avator_normal.jpg");//默认头像

    // 缩放默认头像图片，使用与上面相同的尺寸和模式。
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);

    // 将缩放后的默认头像图片设置到 UI 界面的头像 QLabel 控件上。
    ui->lab_avator->setPixmap(pixmap);
}


void Login::setIcon()//初始化左上角
{
    QPixmap pixmap_tubiao(":/pictures/suliao.png");
    ui->lab_tubiao->setPixmap(pixmap_tubiao);
    ui->lab_tubiao->setScaledContents(true);
}


void Login::setVal()//设置输入验证器
{
    QRegularExpression regExp("^[a-zA-Z0-9]{1,12}$");//给输入框设置验证器
    Login::validator = new QRegularExpressionValidator(regExp, this);
    ui->line_qqnum->setValidator(validator);
    ui->line_password->setValidator(validator);
}


void Login::setTimer()//设置一些计时器 用于动态背景与动态按钮
{
    QTimer *timer = new QTimer(this);//建立计时器 来更新动态背景
    connect(timer, &QTimer::timeout, this, &Login::updateGradient);
    timer->start(50);
    cancelTimer = new QTimer(this);//建立计时器 来发送登录请求
    connect(cancelTimer, &QTimer::timeout, this, &Login::sendLogin);
    cancelRequested = false;
    textUpdateTimer = new QTimer(this);//建立计时器 来更新登录按钮文本
    connect(textUpdateTimer, &QTimer::timeout, this, &Login::updateLoginText);
}


bool Login::tcpConnect()//连接服务器
{
    if (socket.state() != QAbstractSocket::ConnectedState){
        socket.connectToHost("127.0.0.1", 1999);//修改这个地方
        if (socket.waitForConnected(1000)) {
            qDebug() << "连接成功";
            return true;
        } else {
            qDebug() << "连接失败"<<socket.errorString();
            return false;
        }
    }
    return true;
}

void Login::remeberPassword()//记住密码
{
    // 这行代码是 QSettings 类的核心功能之一：开始一个设置组。
    // QSettings 允许以层次结构的方式组织设置，就像文件系统中的目录一样。
    // beginGroup(const QString &prefix) 方法会创建一个名为 prefix 的组。
    // 之后所有对 settings 的操作 (如 setValue, value, beginGroup, endGroup) 都会在这个组的范围内进行。
    // ui->line_qqnum->text() 获取用户在界面上输入的 QQ 号码文本。
    // 原理：这里使用 QQ 号码作为组名，意味着后面保存的设置（账号和密码）将保存在以这个 QQ 号码命名的分组下。
    // 这样做的好处是可以在同一台电脑上为多个不同的用户账号保存各自的密码。
    // 例如，如果用户 A 输入 12345，组名就是 "12345"；用户 B 输入 67890，组名就是 "67890"。
    settings.beginGroup(ui->line_qqnum->text());

    // 这行代码将用户的 QQ 号码保存到当前组（即以 QQ 号码命名的组）下的一个键为 "qq_num" 的位置。
    // setValue(const QString &key, const QVariant &value) 是 QSettings 的方法，用于将一个值与一个键关联并存储。
    // QVariant 是一个通用类型，可以存储多种数据类型，QString 会自动转换为 QVariant。
    // "qq_num" 是在这个组内的键名。
    // ui->line_qqnum->text() 是要保存的值，也就是用户输入的 QQ 号码本身。
    // 原理：将 QQ 号码保存起来。虽然组名已经是 QQ 号码，但显式保存一次可以方便后续读取时获取账号信息。
    // 例如，如果组名是 "12345"，这一行会保存 "12345/qq_num" = "12345"。
    settings.setValue("qq_num", ui->line_qqnum->text());

    // 这行代码将用户的密码保存到当前组（即以 QQ 号码命名的组）下的一个键为 "password" 的位置。
    // ui->line_password->text() 获取用户在界面上输入的密码文本。
    // 原理：将密码保存起来。后续登录时，可以通过 QQ 号码找到对应的组，然后读取 "password" 键的值来自动填充密码框。
    // 例如，如果组名是 "12345"，这一行会保存 "12345/password" = 用户输入的密码。
    // !! 重要安全警告：QSettings 默认以明文存储数据（例如在 Windows 注册表、macOS 的 Plist 文件或 Linux 的 INI 文件中）。
    // !! 直接将用户密码以明文形式保存在 QSettings 中是**非常不安全**的。任何有权限访问这些设置文件的用户或程序都可以轻易读取到密码。
    // !! 如果需要保存密码，应该使用更安全的存储方式，例如加密后再保存，或者使用操作系统提供的安全凭据存储机制（如 Windows Credential Manager, macOS Keychain）。
    settings.setValue("password", ui->line_password->text());

    // 这行代码结束当前设置组。
    // endGroup() 方法退出当前的组范围，使后续的 setValue 或 value 操作回到上一级组（如果存在的话），或者回到顶级范围。
    // 原理：标记以当前 QQ 号码为名的设置组的结束。
    settings.endGroup();
}


void Login::rememberAvator()//记住头像
{
    qDebug()<<"要发送索要头像的请求了";
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray jsonData;
    jsonObj["tag"] = "askforavator";//向服务器索要头像
    jsonObj["qq_number"] = ui->line_qqnum->text();
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket.write(messageWithSeparator);
    socket.flush();
    jsonData.clear();
}


void Login::ifAutoLogin()//检查自动登录设置
{
    if (set.value("autologin", false).toBool()) {//获取自动登录用户名并设置到输入框
        qDebug()<<"要自动登录了";
        ui->auto_ckbox->setChecked(true);
        QString tmp = set.value("autologinuser", "").toString();
        ui->line_qqnum->setText(tmp);
        ui->line_password->setText(settings.value(tmp+"/password").toString());
        setSty();
        on_gologin_clicked();//登录
    }
    else
        qDebug()<<"没有自动登录";
    ifRememberPassword();//没有自动登录 判断是不是有人登录成功记住密码了 加载账号
}


void Login::ifRememberPassword()//如果没有自动登录的检查是否有人记住密码
{
    QString tmp = settings.value("lastlogin",false).toString();
    if(settings.value("lastlogin",false).toBool()){//上次有人记住密码 加载账号
        ui->line_qqnum->setText(settings.value("lastlogin").toString());
        if(settings.value(tmp+"/password").toBool()){//判断这个账号有没有记录的密码
            ui->line_password->setText(settings.value(tmp+"/password").toString());
            ui->ckbox_remeber->setChecked(true);
        }
        setSty();
    }
}


void Login::paintEvent(QPaintEvent *event)//绘画渐变背景
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    QLinearGradient gradient(0, 0, width(), height());//创建渐变
    int g1 = 220 + (sin(offset * 0.06) * 30);//增加绿色分量变化
    gradient.setColorAt(0.0, QColor(255, g1, 235));//动态渐变开始颜色
    gradient.setColorAt(1.0, QColor(172, 224, 249));//动态渐变结束颜色
    painter.setBrush(gradient);//使用渐变作为背景
    painter.drawRect(rect());
}

void Login:: mousePressEvent(QMouseEvent *event)//点击窗口空白让输入框失去焦点 并且判断拖拽窗口
{
    QPoint pos = event->pos();
    int margin = 30;
    ui->line_password->clearFocus();
    ui->line_qqnum->clearFocus();
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - this->geometry().topLeft();
        if (pos.x() <= 30 || pos.x() >= width() - 30 ||
            pos.y() <= 30 || pos.y() >= height() - 30){//如果点击在边缘
            qDebug() << "点击在边缘";
            moveFlag = 1;
        }
        event->accept();
    }
}
void Login::mouseMoveEvent(QMouseEvent *event)//拖拽移动窗口位置
{
    // 这是一个条件判断，只有当满足所有条件时，才会执行窗口移动的逻辑。
    // 它是实现“只有在特定条件下（例如按住左键）才能拖拽”的关键。
    if (event->buttons() & Qt::LeftButton // 条件1: 检查当前鼠标左键是否被按下
        && this->rect().contains(event->pos()) // 条件2: 检查鼠标光标位置是否在窗口的有效区域内 (相对于窗口左上角)
        && moveFlag == 1) // 条件3: 检查自定义的移动标志是否被设置 (通常在 mousePressEvent 中设置)
    {
        // 如果所有条件都满足，执行窗口移动操作。
        // event->globalPosition().toPoint() 获取当前鼠标光标的屏幕全局坐标 (相对于整个屏幕的左上角)。
        // dragPosition 是一个成员变量 (应该在 mousePressEvent 中被设置)，记录了鼠标按下时，光标相对于窗口左上角的偏移量。
        // 原理：新的窗口位置 = 当前鼠标全局位置 - 鼠标按下时相对于窗口左上角的偏移量。
        // 这样计算可以确保鼠标光标相对于窗口的相对位置在拖拽过程中保持不变，实现平滑拖拽的效果。
        this->move(event->globalPosition().toPoint() - dragPosition);

        // 标记此事件已被接收和处理。
        // 这告诉 Qt 事件系统，这个 mouseMoveEvent 已经被当前控件处理了，不需要再将其传播给父控件或执行默认处理。
        event->accept();
    }
    // 如果条件不满足（例如没有按住左键，或 moveFlag 未设置），则不执行窗口移动，事件可能继续传播或执行默认处理。
}

void Login::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


void Login::saveAvator(QString avator)//保存登录成功的头像
{
    qDebug() << "正在保存登录成功的用户的头像";
    avator = avator.simplified();
    QByteArray byteArray = QByteArray::fromBase64(avator.toUtf8());
    QImage image;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadOnly);
    if (!image.load(&buffer, "PNG")) {
        qDebug() << "无法从数据加载图像";
        return;
    }
    //保存到用户的文档目录
    if (image.save(saveLoginAvatorPath)) {
        qDebug() << "头像已成功保存到" << saveLoginAvatorPath;
    } else {
        qDebug() << "保存头像失败";
    }
    this->close();
}


void Login::dengLuChengGong()//登录成功
{
    qDebug() << "登录成功";
    loginFlag = true;
    settings.setValue("lastlogin", ui->line_qqnum->text());//记录上次登录账号
    if (ui->ckbox_remeber->isChecked() && ui->auto_ckbox->isChecked()) {
        remeberPassword();//记住密码
        rememberAvator();//记住头像
        set.setValue("autologin", true);
        set.setValue("autologinuser", ui->line_qqnum->text());
        qDebug() << "记住密码和头像了,下次自动登录";
    } else if (ui->ckbox_remeber->isChecked()) {
        remeberPassword();//记住密码
        rememberAvator();//记住头像
        set.setValue("autologin", false);
        set.setValue("autologinuser", "");
        qDebug() << "记住密码和头像了,下次不自动登录";
    } else {
        rememberAvator();//记住头像
        settings.remove(ui->line_qqnum->text());
        QFile::remove(saveLoginAvatorPath);
        settings.setValue("lastlogin", "");
        set.setValue("autologin", false);
        set.setValue("autologinuser", "");
        qDebug() << "移除记住的账号与密码与头像了";
    }
}


void Login::dengLuShiBai()//登录失败
{
    settings.remove(ui->line_qqnum->text());
    QFile::remove(saveLoginAvatorPath);
    settings.setValue("lastlogin", "");
    set.setValue("autologin", false);
    set.setValue("autologinuser", "");
    qDebug() << "移除记住的账号与密码与头像了";
    if (socket.state() == QAbstractSocket::ConnectedState) {
        Dialog msgBox(this);
        msgBox.transText("请检查您的账号密码!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
    }
}


QPixmap Login::createCircularPixmap(const QString &imagePath)//设置一个调整好掩码的圆形图片
{
    QPixmap pixmap(imagePath);
    //直接缩放为 100x100 的正方形
    QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    double radius = scaledPixmap.width() / 2 - 2;//半径为 50
    QBitmap mask(scaledPixmap.size());
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    painter.setBrush(Qt::color1);
    double centerX = scaledPixmap.width() / 2;
    double centerY = scaledPixmap.height() / 2;
    //绘制圆形掩码
    painter.drawEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
    scaledPixmap.setMask(mask);
    return scaledPixmap;
}


QPixmap Login::getRoundedPixmap(const QPixmap srcPixmap, const int radius)//获得圆角图片
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
    //将图片绘制到desPixmap中，IgnoreAspectRatio忽视图片比例
    painter.drawPixmap(0, 0, desSize.width(), desSize.height(), srcPixmap.scaled(desSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter.setClipping(false);//关闭裁剪
    return desPixMap;
}


void Login::closeEvent(QCloseEvent *event)//关闭窗口事件
{
    if (loginFlag == true) {
        qDebug() << "登录成功 弹出新窗口";
        socket.disconnectFromHost();
        qDebug() << "登录窗口已断开和服务器的连接";
        //发送登录成功的信号
        emit loginSucceed(ui->line_qqnum->text());
    } else {
        //如果没有登录，直接关闭窗口
        qDebug()<<"正常关闭";
        socket.disconnectFromHost();
        event->accept();
    }
}


void Login::onReadyRead()//读取服务器回复数据
{
    jsonData += socket.readAll();//使用 += 追加接收的数据
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
        if (jsonObj["answer"] == "dengluchenggong") {//登录成功
            dengLuChengGong();
        }
        else if (jsonObj["answer"] == "denglushibai") {//登录失败
            qDebug() << "登录失败";
            dengLuShiBai();
        }
        else if (jsonObj["answer"] == "askforavator") {
            qDebug() << "要发送头像了";
            emit sendAvator();//发送头像请求
        }
        else if (jsonObj["answer"] == "zhucechenggong") {
            qDebug() << "注册成功了";
            emit zhuCeChengGong(jsonObj["qq_number"].toString());
        }
        else if (jsonObj["answer"] == "zhuAccountInfo") {
            qDebug() << "注册失败了";
            emit zhuCeShiBai();
        }
        else if (jsonObj["tag"] == "youravator") {
            qDebug() << "要保存登录成功的头像了";
            saveAvator(jsonObj["avator"].toString());
        }
        else if (jsonObj["tag"] == "findpassword1_answer") {
            qDebug() << "收到找回密码有没有这个账号了";
            emit findPass1(jsonObj);
        }
        else if (jsonObj["tag"] == "findpassword2_answer") {
            qDebug() << "收到回发的密保问题结果了";
            emit findPass2(jsonObj);
        }
        else if (jsonObj["tag"] == "findpassword3_answer") {
            qDebug() << "收到修改密码结果了";
            emit findPass3(jsonObj);
        }
        //清空 jsonData，准备接收下一个数据块
        jsonData.clear();
    }
}


void Login::on_gologin_clicked()//点击登录发送请求
{
    tcpConnect();
    if (socket.state() != QAbstractSocket::ConnectedState) {
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    ui->line_password->setEnabled(false);
    ui->line_qqnum->setEnabled(false);
    ui->gologin->setStyleSheet(//取消登录红色风格
        "QPushButton {"
        "    font: 12pt 'Microsoft YaHei UI';"
        "    background-color: rgb(255, 0, 0);"
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 0, 0, 0.7);"
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(200, 0, 0, 0.8);"
        "    color: rgba(255, 255, 255, 0.9);"
        "    border-radius: 15px;"
        "}"
        );
    if (cancelRequested) {
        //如果请求取消，重置状态并返回
        ui->line_password->setEnabled(true);
        ui->line_qqnum->setEnabled(true);
        ui->gologin->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );
        cancelRequested = false;
        dotCount = 1;
        cancelTimer->stop();
        textUpdateTimer->stop();
        ui->gologin->setText("安全登录");//重置按钮文本
        return;
    }
    //更改按钮文本为“正在登录.”
    ui->gologin->setText("正在登录.");
    cancelRequested = true;
    //启动定时器，4.5秒后登录(通过信号和槽)
    cancelTimer->start(4500);
    //启动定时器更新文本
    textUpdateTimer->start(350);
    //如果计时器还在运行，直接返回
    if (cancelTimer->isActive()) {
        return;
    }
}


void Login::sendLogin()//发送登录信息
{
    ui->gologin->setText("安全登录");
    ui->gologin->setStyleSheet(
        "QPushButton {"
        "    font: 12pt 'Microsoft YaHei UI';"
        "    background-color: rgb(5, 186, 251);"
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(5, 186, 251, 0.7);"
        "    color: white;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 123, 255, 0.8);"
        "    color: rgba(255, 255, 255, 0.9);"
        "    border-radius: 15px;"
        "}"
        );
    cancelRequested = false;
    dotCount = 1;
    cancelTimer->stop();
    textUpdateTimer->stop();
    ui->line_password->setEnabled(true);
    ui->line_qqnum->setEnabled(true);
    tcpConnect();
    if (socket.state() != QAbstractSocket::ConnectedState) {
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    jsonObj["tag"] = "login";
    jsonObj["qq_number"] = ui->line_qqnum->text();
    jsonObj["password"] = ui->line_password->text();
    QJsonDocument jsondoc(jsonObj);
    QByteArray jsonData = jsondoc.toJson() + "END";
    //发送消息
    socket.write(jsonData);
    socket.flush();
}


void Login::updateLoginText()//更新取消按钮的文本 模拟动态效果
{
    QString text = "正在登录.";
    if (dotCount < 5) {
        text.append(QString(".").repeated(dotCount));
        dotCount++;
    } else {
        dotCount = 1;//重置点计数
    }
    ui->gologin->setText(text);
}


void Login::on_line_password_returnPressed()//回车登录也会发送请求
{
    on_gologin_clicked();
}


void Login::on_line_qqnum_returnPressed()
{
    on_gologin_clicked();
}




void Login::on_line_password_textChanged(const QString &arg1)//改变密码文本是否判断登录是否可点击
{
    if(!arg1.isEmpty()&&arg1 != "请输入你的密码"&&\
                                                      !ui->line_qqnum->text().isEmpty()&&ui->line_qqnum->text() != "请输入你的账号"){
        ui->gologin->setEnabled(true);
        ui->gologin->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );}
    else{
        ui->gologin->setEnabled(false);
        ui->gologin->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(167, 214, 255);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );
    }
}


void Login::on_line_qqnum_textChanged(const QString &arg1)//改变登录文本是否判断登录是否可点击
{
    if(!arg1.isEmpty()&&arg1 != "请输入你的密码"&&\
                                                      !ui->line_qqnum->text().isEmpty()&&ui->line_qqnum->text() != "请输入你的账号"){
        ui->gologin->setEnabled(true);
        ui->gologin->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );}
    else{
        ui->gologin->setEnabled(false);
        ui->gologin->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(167, 214, 255);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );
    }
}


void Login::on_auto_ckbox_toggled(bool checked)//勾选自动登录自动勾选记住密码
{
    if(ui->auto_ckbox->isChecked())
        ui->ckbox_remeber->setChecked(checked);
}


void Login::on_ckbox_remeber_toggled(bool checked)//记住密码取消勾选时 如果自动登录仍勾选 则取消
{
    if (!checked && ui->auto_ckbox->isChecked()) {
        ui->auto_ckbox->setChecked(false);
    }
}


void Login::setSty()//初始设置输入框格式
{
    if(ui->line_qqnum->text() != "请输入你的账号"){
        ui->line_qqnum->setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                                      "border: 1px solid rgba(0, 0, 0, 0.3); "
                                      "border-radius: 10px; "
                                      "color: black;");}
    if(ui->line_password->text() != "请输入你的密码"){
        ui->line_password->setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                                         "border: 1px solid rgba(0, 0, 0, 0.3); "
                                         "border-radius: 10px; "
                                         "color: black;");
        ui->line_password->setEchoMode(QLineEdit::Password);}
}

void Login::on_rgs_pbt_clicked()//注册账号
{
    // 调用 tcpConnect() 函数。这个函数（没有在这里显示）应该包含了尝试建立 TCP 网络连接到服务器的逻辑。
    // 它可能会创建 socket 对象（如果尚未创建）并调用 socket->connectToHost() 方法。
    // 原理：这是开始与服务器通信以进行注册的第一步，确保与服务器的网络连接处于尝试或建立中。
    tcpConnect();

    // 检查 UI 元素 ui->gologin 的文本是否不是“安全登录”。
    // ui->gologin 可能是一个用于切换登录和注册界面的按钮或标签。
    // 如果文本不是“安全登录”（可能表示当前是注册界面或者其他状态），则调用 on_gologin_clicked() 槽函数。
    // 原理：这部分逻辑似乎是为了确保在启动注册流程之前，UI 界面处于一个特定的“准备注册”状态，或者从其他状态切换到登录/注册的初始界面状态。
    if(ui->gologin->text() != "安全登录"){
        // 调用 on_gologin_clicked() 槽函数。这个槽函数（没有在这里显示）应该负责切换 Login 窗口的 UI 状态，
        // 例如在登录表单和注册表单之间切换显示，或者改变某些控件的可见性/文本。
        on_gologin_clicked();
    }

    // 检查 socket 连接的状态。socket 应该是一个 QTcpSocket 或 QAbstractSocket 子类的成员变量。
    // state() 方法返回 socket 的当前连接状态。
    // QAbstractSocket::ConnectedState 是一个枚举值，表示套接字已成功连接到服务器。
    if (socket.state() != QAbstractSocket::ConnectedState) {
        // 如果 socket 的状态不是“已连接”，说明与服务器的网络连接没有成功建立。

        // 创建一个 Dialog 类的实例作为模态消息框，并指定 Login 窗口作为其父窗口。
        // Dialog 看起来是一个自定义的消息框类。指定父窗口有助于管理子窗口的生命周期和层叠关系。
        Dialog msgBox(this);

        // 调用 msgBox 的 transText 方法，设置消息框要显示的文本内容为“请检查您的网络连接!”。
        msgBox.transText("请检查您的网络连接!");

        // 设置消息框的窗口标志。这些标志影响窗口的外观和行为。
        // Qt::WindowStaysOnTopHint: 提示窗口管理器让这个窗口保持在其他窗口的最上方。
        // Qt::Dialog: 提示窗口管理器这是一个对话框窗口，通常会影响窗口的边框和按钮。
        // Qt::FramelessWindowHint: 移除窗口的原生边框和标题栏，使窗口看起来是自定义的。
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);

        // 以模态方式显示消息框。
        // exec() 方法会显示对话框，并进入一个新的局部事件循环。在对话框关闭之前，用户无法与 Login 窗口或应用程序的其他窗口交互。
        msgBox.exec();

        // 显示错误消息后，函数结束，不继续执行后续的注册窗口创建和显示流程。
        return;
    }

    // 如果网络连接检查通过（socket 已连接）

    // 创建一个 RegisterWindow 类的新实例，用于处理注册界面的显示和交互。
    // new RegisterWindow(...) 在堆上分配内存创建一个 RegisterWindow 对象。
    // &socket: 将 socket 成员变量的地址（即 socket 对象的指针）传递给 RegisterWindow 的构造函数。
    // 原理：这意味着 RegisterWindow 需要使用这个现有的 socket 连接与服务器进行通信来完成注册过程。
    // this: 将当前的 Login 窗口对象的指针作为父对象传递。这建立了对象树的父子关系，有利于内存管理（当父对象被删除时，会自动删除子对象）。
    regis = new RegisterWindow(&socket,this);

    // 建立信号与槽的连接。
    // 连接当前 Login 窗口的 sendAvator 信号到 regis (RegisterWindow 实例) 的 sendAva 槽。
    // 原理：这允许 Login 窗口在合适的时机（例如，用户在 RegisterWindow 中上传了头像或点击了发送头像按钮后，由 Login 窗口协调）发射 sendAvator 信号，触发 RegisterWindow 中的 sendAva 槽函数执行，从而发送头像数据给服务器。
    connect(this,&Login::sendAvator,regis,&RegisterWindow::sendAva);

    // 连接 Login 窗口的 zhuCeChengGong 信号到 regis 的 chengGong 槽。
    // 原理：当 Login 窗口（或其他地方处理服务器响应的逻辑）检测到注册成功时，发射 zhuCeChengGong 信号，通知 RegisterWindow 显示注册成功的界面或进行后续操作。
    connect(this,&Login::zhuCeChengGong,regis,&RegisterWindow::chengGong);

    // 连接 Login 窗口的 zhuCeShiBai 信号到 regis 的 shiBai 槽。
    // 原理：当 Login 窗口检测到注册失败时，发射 zhuCeShiBai 信号，通知 RegisterWindow 显示注册失败的信息或进行相应处理。
    connect(this,&Login::zhuCeShiBai,regis,&RegisterWindow::shiBai);

    // 显示新创建的 RegisterWindow 窗口。
    // show() 方法使隐藏的窗口变为可见。
    regis->show();
}


void Login::on_fgt_pbt_clicked()//找回密码
{
    tcpConnect();
    //- 检查登录按钮的文本状态 ：判断当前登录按钮上显示的文本是否不是"安全登录"
    //- 重置登录状态 ：如果按钮文本不是"安全登录"（例如显示"正在登录..."），说明当前正在进行登录操作或登录状态异常
    //    - 取消当前登录过程 ：调用 on_gologin_clicked() 会触发取消当前登录过程的逻辑
    if(ui->gologin->text() != "安全登录"){
        on_gologin_clicked();
    }
    if (socket.state() != QAbstractSocket::ConnectedState) {
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    finddPassword = new FindPassword(&socket,this);
    connect(this,&Login::findPass1,finddPassword,&FindPassword::findPassword1);
    connect(this,&Login::findPass2,finddPassword,&FindPassword::findPassword2);
    connect(this,&Login::findPass3,finddPassword,&FindPassword::findPassword3);
    finddPassword->show();
}


void Login::updateGradient()//更新渐变背景
{
    offset += 1;//不用考虑上限 计算得到几年之后才会上限
    update();
}


void Login::on_but_deletewindow_clicked()//关闭按钮
{
    this->close();
}

void LineInput::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    // 启用 QLineEdit 内置的清除按钮（在文本框末尾显示一个小“x”图标，点击可清除文本）。
    // 原理：用户获得焦点准备输入时，如果文本框有内容，启用清除按钮方便用户快速清空。
    setClearButtonEnabled(true);

    // 检查当前文本内容是否是预设的占位符文本（“请输入你的密码”或“请输入你的账号”）。
    // text() 方法获取 QLineEdit 当前的文本内容。
    if(text() == "请输入你的密码"||text() == "请输入你的账号"){
        // 如果当前是占位符文本，将其清空。
        // 原理：用户获得焦点准备输入实际内容时，应清空占位符，以便用户直接输入。
        setText("");

        // 设置控件的样式表（使用 Qt 的 CSS-like 语法）。
        // 原理：改变文本框的视觉样式。这里设置字体、边框、圆角，并将文本颜色设置为黑色。
        // 这通常是为了区分占位符（可能是灰色）和用户输入的正常文本（黑色）。
        setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                      "border: 1px solid rgba(0, 0, 0, 0.3); "
                      "border-radius: 10px; "
                      "color: black;");
    }

    // 检查控件的 objectName 是否是“line_password”。
    // objectName() 方法返回在 Qt Designer 中或代码中给 QObject 设置的名称。
    // 原理：LineInput 是一个自定义类，可能用于多个不同的输入框（如账号和密码）。通过检查 objectName，可以在同一个自定义类中为不同用途的实例实现不同的行为。
    if(this->objectName() == "line_password")
        // 如果是密码输入框，设置其回显模式为密码模式。
        // setEchoMode(QLineEdit::EchoMode) 控制文本框内容的显示方式。
        // QLineEdit::Password 模式会用圆点或星号隐藏用户输入的字符。
        // 原理：当密码框获得焦点准备输入密码时，切换到密码回显模式以保护用户输入的隐私。
        this->setEchoMode(QLineEdit::Password);

    // 调用基类 QLineEdit 的 focusInEvent 方法。
    // 原理：执行 QLineEdit 默认的获得焦点时的处理逻辑。这是非常重要的一步，确保 Qt 标准的焦点处理机制能够正常工作，例如更新内部状态、触发默认信号等。在重写事件处理函数时，通常都需要调用基类的实现，除非你明确知道不需要并想完全替代基类的行为。
    QLineEdit::focusInEvent(event);
}


void LineInput::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    setClearButtonEnabled(false);
    if (objectName() == "line_password") {
        if (text().isEmpty()) {
            setText("请输入你的密码");
            setEchoMode(QLineEdit::Normal);
            setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                          "border: 1px solid rgba(0, 0, 0, 0.3); "
                          "border-radius: 10px; "
                          "color: grey;");
        }
    } else if (objectName() == "line_qqnum") {
        if (text().isEmpty()) {
            setText("请输入你的账号");
            setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                          "border: 1px solid rgba(0, 0, 0, 0.3); "
                          "border-radius: 10px; "
                          "color: grey;");
        }
    }
    QLineEdit::focusOutEvent(event);
}



