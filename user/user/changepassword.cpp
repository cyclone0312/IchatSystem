#include "changepassword.h"
#include "ui_changepassword.h"
#include "dialog.h"

ChangePassword::ChangePassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


ChangePassword::~ChangePassword()
{
    delete ui;
}


void ChangePassword::setVal()//设置输入验证器
{
    QRegularExpression regExp("^[a-zA-Z0-9]{1,12}$");//给输入框设置验证器
    validator = new QRegularExpressionValidator(regExp, this);
    ui->line_account->setValidator(validator);
    ui->line_password->setValidator(validator);
    ui->line_newpassword->setValidator(validator);
    ui->line_newpassword2->setValidator(validator);
}


void ChangePassword::paintEvent(QPaintEvent *event)//窗口背景
{
    //调用基类方法以处理父类的绘图
    QDialog::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    //加载背景图片
    QPixmap background(":/pictures/094 Cloudy Apple - trans.png");
    //确保图片已加载
    if (background.isNull()) {
        qWarning("背景图像加载失败");
        return;
    }
    QSize newSize = QSize(this->width(), this->height());
    //将背景图缩放为新的大小
    QPixmap scaledBackground = background.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //计算绘制的位置，保持居中
    int x = (this->width() - scaledBackground.width()) / 2;
    int y = (this->height() - scaledBackground.height()) / 2;
    //设置剪裁区域以适应圆角窗口
    int radius = 10;
    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius);
    painter.setClipPath(path);
    //绘制背景图片
    painter.drawPixmap(x, y, scaledBackground);
    //绘制边框
    painter.setPen(QPen(Qt::gray, 3.5));
    painter.drawRoundedRect(rect(), radius, radius);
}


void ChangePassword::mousePressEvent(QMouseEvent *event)//捕获鼠标按下事件
{
    //清除焦点
    QList<QWidget*> widgets = this->findChildren<QWidget*>();
    for (QWidget* widget : widgets) {
        widget->clearFocus();
    }
    QPoint pos = event->pos();
    int margin = 30;
    if (pos.x() <= 30 || pos.x() >= width() - 30 ||
        pos.y() <= 30 || pos.y() >= height() - 30){//如果点击在边缘
        qDebug() << "点击在边缘";
        moveFlag = 1;
        dragPosition = event->globalPosition().toPoint() - this->frameGeometry().topLeft();//记录鼠标相对于窗口的初始偏移
        event->accept();
    }
}


void ChangePassword::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果
{
    qDebug()<<"鼠标位移了";
    QPoint globalPos = event->globalPosition().toPoint();//获取当前的鼠标位置
    QPoint delta = globalPos - (this->frameGeometry().topLeft() + dragPosition);//计算当前鼠标位置与窗口之间的相对偏移
    if (event->buttons() & Qt::LeftButton) {
        if (moveFlag == 1) {
            this->move(globalPos - dragPosition);
            event->accept();
        }
        //更新拖动位置
        dragPosition = globalPos - this->frameGeometry().topLeft();
    }
}


void ChangePassword::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


void ChangePassword:: closeEvent(QCloseEvent *event)//关闭窗口事件
{
    emit customClose();//this->disconnect();放在主窗口的槽函数了
    this->deleteLater();
    event->accept();
}


void ChangePassword::on_but_deletewindow_clicked()//关闭按钮
{
    this->close();
}


void ChangePassword::on_line_account_textChanged(const QString &arg1)
{
    if(ui->line_account->text() != "" && ui->line_account->text() != "请输入您的账号"
        && ui->line_password->text() != "" && ui->line_password->text() != "请输入您的密码"){
        ui->but_yes->setEnabled(true);
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(5, 186, 251);"
            "color: white;"
            "border-radius: 10px;"
            "border: 0.5px solid rgb(220, 220, 220);"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(5, 186, 251, 0.7);"
            "color: white;"
            "border-radius: 10px;"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(0, 123, 255, 0.8);"
            "color: rgba(255, 255, 255, 0.9);"
            "border-radius: 10px;"
            "}"
            );
    }
    else{
        ui->but_yes->setEnabled(false);
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(167, 214, 255); "
            "color: white;"
            "border-radius: 10px;"
            "}"
            );
    }
}


void ChangePassword::on_line_password_textChanged(const QString &arg1)//第一个页面判断能否点击确认按钮
{
    if(ui->line_account->text() != "" && ui->line_account->text() != "请输入您的账号"
        && ui->line_password->text() != "" && ui->line_password->text() != "请输入您的密码"){
        ui->but_yes->setEnabled(true);
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(5, 186, 251);"
            "color: white;"
            "border-radius: 10px;"
            "border: 0.5px solid rgb(220, 220, 220);"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(5, 186, 251, 0.7);"
            "color: white;"
            "border-radius: 10px;"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(0, 123, 255, 0.8);"
            "color: rgba(255, 255, 255, 0.9);"
            "border-radius: 10px;"
            "}"
            );
    }
    else{
        ui->but_yes->setEnabled(false);
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(167, 214, 255); "
            "color: white;"
            "border-radius: 10px;"
            "}"
            );
    }
}

void ChangePassword::on_line_newpassword_textChanged(const QString &arg1)//第二个页面判断能否点击确认按钮
{
    if(ui->line_newpassword->text() != "" && ui->line_newpassword->text() != "请输入新的密码"
        && ui->line_newpassword2->text() != "" && ui->line_newpassword2->text() != "请确认您的密码"){
        ui->but_yes_2->setEnabled(true);
        ui->but_yes_2->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(5, 186, 251);"
            "color: white;"
            "border-radius: 10px;"
            "border: 0.5px solid rgb(220, 220, 220);"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(5, 186, 251, 0.7);"
            "color: white;"
            "border-radius: 10px;"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(0, 123, 255, 0.8);"
            "color: rgba(255, 255, 255, 0.9);"
            "border-radius: 10px;"
            "}"
            );
    }
    else{
        ui->but_yes_2->setEnabled(false);
        ui->but_yes_2->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(167, 214, 255); "
            "color: white;"
            "border-radius: 10px;"
            "}"
            );
    }
}


void ChangePassword::on_line_newpassword2_textChanged(const QString &arg1)//第二个页面判断能否点击确认按钮
{
    if(ui->line_newpassword->text() != "" && ui->line_newpassword->text() != "请输入新的密码"
        && ui->line_newpassword2->text() != "" && ui->line_newpassword2->text() != "请确认您的密码"){
        ui->but_yes_2->setEnabled(true);
        ui->but_yes_2->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(5, 186, 251);"
            "color: white;"
            "border-radius: 10px;"
            "border: 0.5px solid rgb(220, 220, 220);"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(5, 186, 251, 0.7);"
            "color: white;"
            "border-radius: 10px;"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(0, 123, 255, 0.8);"
            "color: rgba(255, 255, 255, 0.9);"
            "border-radius: 10px;"
            "}"
            );
    }
    else{
        ui->but_yes_2->setEnabled(false);
        ui->but_yes_2->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(167, 214, 255); "
            "color: white;"
            "border-radius: 10px;"
            "}"
            );
    }
}


void ChangePassword::on_but_yes_clicked()//告知主窗体发送账号密码
{
    QJsonObject jsonObj;
    jsonObj["tag"] = "changepassword1";
    jsonObj["account"] = ui->line_account->text();
    jsonObj["password"] = ui->line_password->text();
    emit changePassword1(jsonObj);
    ui->line_account->setEnabled(false);
    ui->line_password->setEnabled(false);
    ui->but_yes->setEnabled(false);
}


void ChangePassword::on_but_yes_2_clicked()//告知主窗体发送修改密码
{
    if(ui->line_newpassword->text() != ui->line_newpassword2->text()){
        QString text =  "您的两次密码不一致。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;
    }
    QJsonObject jsonObj;
    jsonObj["tag"] = "changepassword2";
    jsonObj["account"] = ui->line_account->text();
    jsonObj["password"] = ui->line_newpassword->text();
    emit changePassword2(jsonObj);
    ui->line_newpassword->setEnabled(false);
    ui->line_newpassword2->setEnabled(false);
    ui->but_yes_2->setEnabled(false);
}


void ChangePassword::changePasswordAns1(const QJsonObject& json)//修改账号密码第一个申请的结果
{
    if(json ["answer"] == "fail"){
        ui->line_account->setEnabled(true);
        ui->line_password->setEnabled(true);
        ui->but_yes->setEnabled(true);
        QString text =  "请求失败。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;

    }
    else if(json ["answer"] == "user_not_found"){
        ui->line_account->setEnabled(true);
        ui->line_password->setEnabled(true);
        ui->but_yes->setEnabled(true);
        QString text =  "账号或密码错误，请检查。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;
    }
    else if(json ["answer"] == "succeed"){
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void ChangePassword::changePasswordAns2(const QJsonObject& json)//修改账号密码第二个申请的结果
{
    if(json ["answer"] == "fail"){
        QString text =  "请求失败。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        this->close();
        return;

    }
    else if(json ["answer"] == "succeed"){
        QString text =  "修改成功!";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        this->close();
        return;
    }
}


void LineChangePass::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    if(text() == "请输入您的账号" || text() == "请输入您的密码" || text() == "请输入新的密码" || text() == "请确认您的密码"){
        setText("");
        setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                      "border: 1px solid rgba(0, 0, 0, 0.1);"
                      "border-radius: 8px;"
                      "padding: 5px;"
                      "color: black;");
    }
    if(objectName() == "line_newpassword" || objectName() == "line_newpassword2" || objectName() == "line_password"){
        this->setEchoMode(Password);
    }
    QLineEdit::focusInEvent(event);
}


void LineChangePass::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    if(text() == ""){
        if(objectName() == "line_accont")
        {
            setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                          "border: 1px solid rgba(0, 0, 0, 0.1);"
                          "border-radius: 8px;"
                          "padding: 5px;"
                          "color: grey;");
            setText("请输入您的账号");
        }
        if(objectName() == "line_password")
        {
            setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                          "border: 1px solid rgba(0, 0, 0, 0.1);"
                          "border-radius: 8px;"
                          "padding: 5px;"
                          "color: grey;");
            setText("请输入您的密码");
            setEchoMode(Normal);
        }
        if(objectName() == "line_newpassword")
        {
            setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                          "border: 1px solid rgba(0, 0, 0, 0.1);"
                          "border-radius: 8px;"
                          "padding: 5px;"
                          "color: grey;");
            setText("请输入新的密码");
            setEchoMode(Normal);
        }
        if(objectName() == "line_newpassword2")
        {
            setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                          "border: 1px solid rgba(0, 0, 0, 0.1);"
                          "border-radius: 8px;"
                          "padding: 5px;"
                          "color: grey;");
            setText("请确认您的密码");
            setEchoMode(Normal);
        }
    }
    QLineEdit::focusOutEvent(event);
}


