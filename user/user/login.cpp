#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login),settings("settings.ini", QSettings::IniFormat),set("set.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
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
    QPixmap pixmap(":/pictures/suliao_avator_normal.jpg");//默认头像
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);
    ui->lab_avator->setPixmap(pixmap);
    //获取应用程序的专用目录
    QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDir);
    //创建目录（如果不存在）
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    saveLoginAvatorPath = dir.filePath("laseloginavator.png");
    if (QFile::exists(saveLoginAvatorPath)) {//如果有上次登录成功的人就加载头像
        QPixmap newpixmap(saveLoginAvatorPath);
        newpixmap = newpixmap.scaled(100, 100, Qt::KeepAspectRatio);
        ui->lab_avator->setPixmap(newpixmap);
    }
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
        socket.connectToHost("192.168.220.1", 1999);//修改这个地方
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
    settings.beginGroup(ui->line_qqnum->text());
    settings.setValue("qq_num", ui->line_qqnum->text());
    settings.setValue("password", ui->line_password->text());
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
    //计算颜色变化
    int r1 = 255;//固定红色分量（白色）
    int g1 = 220 + (sin(offset * 0.06) * 30);//增加绿色分量变化
    int r2 = 172;//固定红色分量（淡蓝色）
    int g2 = 224;//固定绿色分量（淡蓝色）
    int b2 = 249;//固定蓝色分量（淡蓝色）
    gradient.setColorAt(0.0, QColor(r1, g1, 235));//动态渐变开始颜色
    gradient.setColorAt(1.0, QColor(r2, g2, b2));//动态渐变结束颜色
    painter.setBrush(gradient);//使用渐变作为背景
    painter.drawRect(rect());
}


void Login::mouseMoveEvent(QMouseEvent *event)//拖拽移动窗口位置
{
    if (event->buttons() & Qt::LeftButton&&this->rect().contains(event->pos()) && moveFlag ==1) {
        this->move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
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
    tcpConnect();
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
    regis = new RegisterWindow(&socket,this);
    connect(this,&Login::sendAvator,regis,&RegisterWindow::sendAva);
    connect(this,&Login::zhuCeChengGong,regis,&RegisterWindow::chengGong);
    connect(this,&Login::zhuCeShiBai,regis,&RegisterWindow::shiBai);
    regis->show();
}


void Login::on_fgt_pbt_clicked()//找回密码
{
    tcpConnect();
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
    setClearButtonEnabled(true);
    if(text() == "请输入你的密码"||text() == "请输入你的账号"){
        setText("");
        setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                      "border: 1px solid rgba(0, 0, 0, 0.3); "
                      "border-radius: 10px; "
                      "color: black;");
    }
    if(this->objectName() == "line_password")
        this->setEchoMode(QLineEdit::Password);
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



