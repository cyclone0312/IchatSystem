#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "cutavator.h"
#include "dialog.h"

RegisterWindow::RegisterWindow(QTcpSocket *msocket,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow),socket(msocket)
{
    ui->setupUi(this);
    //初始化窗口
    setWindowFlags(Qt::Dialog);
    //给输入框设置验证器
    setVal();
    //设置默认头像
    setAva();
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


RegisterWindow::~RegisterWindow()
{
    delete ui;
}


void RegisterWindow::setVal()//给输入框设置验证器
{
    QRegularExpression regExp("^[a-zA-Z0-9]{1,12}$");
    RegisterWindow::validator = new QRegularExpressionValidator(regExp, this);
    ui->line_password->setValidator(validator);
    ui->line_password2->setValidator(validator);
}


void RegisterWindow::setAva()//设置默认头像
{
    QPixmap pixmap(":/pictures/suliao_avator_normal.jpg");//初始化默认头像
    ui->lab_avator->setPixmap(pixmap);
    ui->lab_avator->setScaledContents(true);
}


QPixmap RegisterWindow::createCircularPixmap(const QString &imagePath)//给图片裁剪成圆形
{
    QPixmap pixmap(imagePath);
    double width = pixmap.width();
    double height = pixmap.height();
    double radius = std::min(width, height) / 2;
    QBitmap mask(pixmap.size());
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    painter.setBrush(Qt::color1);
    double centerX = width / 2;
    double centerY = height / 2;
    painter.drawEllipse((centerX - radius)-5, (centerY - radius)-5, (2 * radius)-2, (2 * radius)-2);
    pixmap.setMask(mask);
    return pixmap;
}


void RegisterWindow::paintEvent(QPaintEvent *event)//初始化背景
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    QPixmap pixmap(":/pictures/094 Cloudy Apple - trans.png");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}


QPixmap RegisterWindow::getRoundedPixmap(const QPixmap srcPixmap, const float radiusRatio)//切割圆角图片
{
    //目标图片尺寸
    QSize desSize = srcPixmap.size();
    //新建一个目标大小的画布QPixmap
    QPixmap desPixMap(desSize);
    //填充透明色作为背景
    desPixMap.fill(Qt::transparent);
    //以QPixmap 为绘画背景进行画笔绘制
    QPainter painter(&desPixMap);
    painter.setRenderHints(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);//平滑像素图变换
    QPainterPath path;//绘制路径
    //计算圆角半径，基于图片尺寸的比例
    int radius = qMin(desSize.width(), desSize.height()) * radiusRatio;
    //绘制圆角矩形
    path.addRoundedRect(0, 0, desSize.width(), desSize.height(), radius, radius);
    //将绘制的圆角矩形路径中内容进行裁剪
    painter.setClipPath(path);
    //将图片绘制到 desPixmap 中，忽视图片比例
    painter.drawPixmap(0, 0, desSize.width(), desSize.height(),
                       srcPixmap.scaled(desSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter.setClipping(false);//关闭裁剪
    return desPixMap;
}


void RegisterWindow::closeEvent(QCloseEvent *event)//关闭事件
{
    this->disconnect();
    //在关闭时调用 deleteLater
    this->deleteLater();
    //接受关闭事件
    event->accept();
}


void RegisterWindow::mousePressEvent(QMouseEvent *event)//点击窗口空白让所有输入框失去焦点
{
    ui->line_answer->clearFocus();
    ui->line_nickname->clearFocus();
    ui->line_password->clearFocus();
    ui->line_password2->clearFocus();
    ui->line_question->clearFocus();
}


void RegisterWindow::on_line_answer_textChanged(const QString &arg1)//文本改变 判断能不能提交
{
    if(!ui->line_nickname->text().isEmpty()&&!ui->line_password->text().isEmpty()\
        &&!ui->line_password2->text().isEmpty()&&!ui->line_question->text().isEmpty()\
        &&!ui->line_answer->text().isEmpty()&&ui->line_nickname->text() != "请输入昵称"\
        &&ui->line_password->text() !="请输入密码" &&ui->line_password2->text() != "请确认密码"\
        &&ui->line_question->text() !="请输入问题" &&ui->line_answer->text() !=" 请输入答案"){
        ui->submit_but->setEnabled(true);
        ui->submit_but->setCursor(Qt::PointingHandCursor);
        ui->submit_but->setStyleSheet(
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
        ui->submit_but->setEnabled(false);
        ui->submit_but->setCursor(Qt::ArrowCursor);
        ui->submit_but->setStyleSheet(
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


void RegisterWindow::on_cbbox_sex_currentIndexChanged(int index)//性别
{
    on_line_answer_textChanged("arg");
}


void RegisterWindow::on_line_password_textChanged(const QString &arg1)//密码
{
    on_line_answer_textChanged(arg1);
}


void RegisterWindow::on_line_password2_textChanged(const QString &arg1)//确认密码
{
    on_line_answer_textChanged(arg1);
}


void RegisterWindow::on_line_question_textChanged(const QString &arg1)//问题
{
    on_line_answer_textChanged(arg1);
}


void RegisterWindow::on_line_nickname_textChanged(const QString &arg1)//昵称
{
    on_line_answer_textChanged(arg1);
}


void RegisterWindow::on_submit_but_clicked(bool checked)//提交注册申请 两个密码不一致则弹窗
{

    if(ui->line_password->text() != ui->line_password2->text()){
        Dialog msgBox(this);
        QStyle *style = QApplication::style();
        QIcon icon = style->standardIcon(QStyle::SP_MessageBoxWarning);
        msgBox.setWindowIcon(icon);
        msgBox.setWindowTitle("注册失败");
        msgBox.transText("您的两次密码不一致!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Dialog | Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    if(socket == nullptr){//没连接到服务器
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;}
    qDebug() << "准备发送注册请求";
    ui->submit_but->setEnabled(false);
    jsonObj = QJsonObject();
    jsonObj["tag"] = "register";
    jsonObj["nickname"] = ui->line_nickname->text();
    jsonObj["gender"] = ui->cbbox_sex->currentText();
    jsonObj["password"] = ui->line_password->text();
    jsonObj["question"] = ui->line_question->text();
    jsonObj["answer"] = ui->line_answer->text();
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket->write(messageWithSeparator);
    socket->flush();
    jsonData.clear();
    qDebug() << "发送注册请求";
}


void RegisterWindow:: receiveAvator(const QPixmap &pixmap)//收到用户选择的头像然后做相应处理
{
    //处理圆角并更新图片
    ui->lab_avator->setPixmap(pixmap);
    ui->lab_avator->setScaledContents(true);
}


void RegisterWindow::sendAva()//发送头像给服务器
{
    QPixmap pixmap = ui->lab_avator->pixmap();
    if (pixmap.isNull()) {
        qDebug() << "头像不存在，无法发送";
        return;
    }
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    if (!buffer.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开缓冲区，用于保存pixmap";
        return;
    }
    if (!pixmap.save(&buffer, "PNG")) {
        qDebug() << "保存pixmap到缓冲区失败";
        return;
    }
    QString avator = byteArray.toBase64();
    jsonObj = QJsonObject();
    jsonObj["tag"] = "myavator";
    jsonObj["avator"] = avator;
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(messageWithSeparator);
        socket->flush();
    } else {
        qDebug() << "Socket未连接，无法发送头像信息";
    }
    qDebug() << "发送头像信息了";
    jsonData.clear();
}


void RegisterWindow::chengGong(QString qqnum)//注册成功
{
    QString text =  "<p style='text-align: center;'>您的账号是:<br>" + qqnum + "," + "<br>请牢记!</p>";
    Dialog* dialog = new Dialog(this);
    dialog->transText(text);
    dialog->exec();
    this->disconnect();
    this->close();
}


void RegisterWindow::shiBai()//注册失败
{
    Dialog msgBox(this);
    msgBox.transText("注册失败!");
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
    msgBox.exec();
}


void LineRegis::focusInEvent(QFocusEvent *event)
{
    if(text() == "请输入昵称" || text() == "请输入问题" || text() == "请输入答案"){
        setText("");
    }
    setStyleSheet("font: 12pt 'Microsoft YaHei UI'; "
                  "border: 1px solid rgba(0, 0, 0, 0.3); "
                  "border-radius: 10px; "
                  "color: black;");
    if(this->text() == "请输入密码"||this->text() == "请确认密码"){
        this->setEchoMode(QLineEdit::Password);
        setText("");
    }
    QLineEdit::focusInEvent(event);
}


void LineRegis::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    if(text() == ""){
        if(objectName() == "line_nickname"){
            setStyleSheet(
                "font: 12pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入昵称");
        }
        else if(objectName() == "line_password"){
            setStyleSheet(
                "font: 12pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入密码");
            setEchoMode(QLineEdit::Normal);
        }
        else if(objectName() == "line_password2"){
            setStyleSheet(
                "font: 12pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请确认密码");
            setEchoMode(QLineEdit::Normal);
        }
        else if(objectName() == "line_question"){
            setStyleSheet(
                "font: 12pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入问题");
        }
        else if(objectName() == "line_answer"){
            setStyleSheet(
                "font: 12pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入答案");
        }
    }
    QLineEdit::focusOutEvent(event);
}


void LabelRegis::mousePressEvent(QMouseEvent *event)//点击按钮更换头像
{
    qDebug() << "点击头像了";
    QString filename = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (!filename.isEmpty()) {
        qDebug() << "头像路径有效";
        QPixmap pixmap(filename);
        if (pixmap.isNull()) {
            qDebug() << "加载图片失败";
            return;
        }
        //打开头像编辑器
        CutAvator *cutter = new CutAvator(filename);
        cutter->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Dialog);
        connect(cutter,&CutAvator::cutOk,qobject_cast<RegisterWindow*>(this->parentWidget()->parentWidget()),&RegisterWindow::receiveAvator);
        cutter->exec();
        cutter->deleteLater();
    }
}

