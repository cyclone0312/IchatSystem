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
    // 从 UI 界面上的 lab_avator 控件中获取当前显示的 QPixmap 图片。
    // ui->lab_avator 假设是一个指向 QLabel 控件的指针，该控件用于显示头像图片。
    // pixmap() 方法返回当前设置在 QLabel 上的 QPixmap。如果未设置图片，或者设置的是一个无效图片，则返回一个空的 QPixmap。
    QPixmap pixmap = ui->lab_avator->pixmap();

    // 检查获取到的 QPixmap 是否是空的或无效的。
    // isNull() 方法对于 QPixmap 而言，如果它不包含有效的图像数据（例如，文件加载失败或从未设置），则返回 true。
    if (pixmap.isNull()) {
        qDebug() << "头像不存在，无法发送"; // 如果头像无效，打印调试信息。
        return; // 函数结束，不发送头像。
    }

    // 声明一个 QByteArray 对象。QByteArray 用于存储原始的字节数据，例如文件内容或网络数据包。
    // 这里将用于存储将 QPixmap 编码后的二进制图像数据（例如 PNG 格式）。
    QByteArray byteArray;

    // 创建一个 QBuffer 对象，并将其与上面声明的 byteArray 关联。
    // QBuffer 是一个特殊的 QIODevice（输入输出设备），它允许你像操作文件或网络连接一样，对 QByteArray 进行读写操作。
    QBuffer buffer(&byteArray);

    // 尝试以只写模式打开 QBuffer。准备向这个缓冲区写入数据。
    // QIODevice::WriteOnly 是打开模式标志，表示只允许向设备写入数据。
    if (!buffer.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开缓冲区，用于保存pixmap"; // 如果打开失败，打印错误信息。
        return; // 函数结束。
    }

    // 将 QPixmap 的图像数据保存到 QBuffer 中，并指定保存格式为 PNG。
    // save() 方法是 QPixmap 的方法，它将图片数据按照指定格式写入到一个 QIODevice 设备。
    // PNG 是一个无损的图像格式，适合传输图片数据。
    if (!pixmap.save(&buffer, "PNG")) { // 如果保存失败
        qDebug() << "保存pixmap到缓冲区失败"; // 打印错误信息。
        buffer.close(); // 关闭缓冲区。
        return; // 函数结束。
    }
    buffer.close(); // 保存成功后，关闭缓冲区。

    // 将存储在 byteArray 中的原始二进制图像数据（PNG格式）进行 Base64 编码。
    // Base64 编码是一种将任意二进制数据转换为只包含 ASCII 可打印字符的字符串的编码方式。
    // 这是因为 JSON 等数据格式通常只处理文本，直接传输二进制数据可能会有问题。Base64 编码后的字符串可以安全地嵌入到 JSON 中。
    // toBase64() 是 QByteArray 的方法，返回一个 Base64 编码后的 QByteArray。
    // 这里的代码紧接着将其转换为 QString，方便放入 QJsonObject。
    QString avator = byteArray.toBase64();

    // 创建或清空一个 QJsonObject 对象。QJsonObject 表示 JSON 数据结构中的一个对象（键值对的集合）。
    // jsonObj 假设是一个成员变量。
    jsonObj = QJsonObject();

    // 向 JSON 对象中添加一个键值对："tag" 为键，"myavator" 为值。这通常用于告诉服务器消息的类型。
    jsonObj["tag"] = "myavator";

    // 向 JSON 对象中添加另一个键值对："avator" 为键，将 Base64 编码后的头像字符串作为值。
    jsonObj["avator"] = avator;

    // ...接下来的代码是发送 JSON 数据，这部分在之前的讲解中多次出现。

    // 将 QJsonObject 封装到 QJsonDocument 中。QJsonDocument 代表一个完整的 JSON 文档。
    // jsonObj 是一个 JSON 对象，所以用 QJsonDocument(jsonObj) 构建文档。jsonDoc 假设是一个成员变量。
    jsonDoc = QJsonDocument(jsonObj);

    // 将 QJsonDocument 序列化为 JSON 格式的 QByteArray。这是实际要发送的网络数据。jsonData 假设是一个成员变量。
    jsonData = jsonDoc.toJson();

    // 添加自定义的消息结束标识符 "END"。在基于流的协议（如 TCP）中，使用特定的标识符来划分不同的消息包是一种常见的做法。
    QByteArray messageWithSeparator = jsonData + "END";

    // 检查 socket 对象是否有效且处于连接状态。
    // socket 假设是一个指向 QTcpSocket 或 QAbstractSocket 子类的成员变量。
    // socket && socket->state() == QAbstractSocket::ConnectedState 确保 socket 已被创建且已经成功连接到服务器。
    // QAbstractSocket::ConnectedState 是 QAbstractSocket 的枚举值之一，表示套接字已连接。
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        // 如果 socket 有效且已连接，则将带有标识符的 JSON 数据写入 socket 的发送缓冲区。
        // write() 方法将数据放入缓冲区，但不保证立即发送。
        socket->write(messageWithSeparator);
        // 强制将发送缓冲区中的数据立即发送出去。
        socket->flush();
    } else {
        // 如果 socket 无效或未连接，打印警告信息。
        qDebug() << "Socket未连接，无法发送头像信息";
    }

    qDebug() << "发送头像信息了"; // 打印调试信息，表明已尝试发送（不代表发送成功）。

    // 清空用于存储 JSON 数据的 QByteArray。释放内存资源。
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

