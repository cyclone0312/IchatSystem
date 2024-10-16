#include "logout.h"
#include "ui_logout.h"
#include "dialog.h"
#include "choicedialog.h"

Logout::Logout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Logout)
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


Logout::~Logout()
{
    delete ui;
}


void Logout::on_but_deletewindow_clicked()//关闭窗口
{
    this->close();
}


void Logout::setVal()//设置输入验证器
{
    QRegularExpression regExp("^[a-zA-Z0-9]{1,12}$");//给输入框设置验证器
    validator = new QRegularExpressionValidator(regExp, this);
    ui->line_account->setValidator(validator);
    ui->line_password->setValidator(validator);
}


void Logout::paintEvent(QPaintEvent *event)//窗口背景
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


void Logout::mousePressEvent(QMouseEvent *event)//捕获鼠标按下事件
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


void Logout::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果
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


void Logout::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


void Logout:: closeEvent(QCloseEvent *event)//关闭窗口事件
{
    emit customClose();//this->disconnect();放在主窗口了
    this->deleteLater();
    event->accept();
}


void Logout::on_line_account_textChanged(const QString &arg1)//判断确认是否可以点击
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


void Logout::on_line_password_textChanged(const QString &arg1)//判断确认是否可以点击
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


void Logout::on_but_yes_clicked()//是否确定注销
{
    ChoiceDialog dialog;
    dialog.transText("你确定永久注销此账号吗？");
    dialog.transButText("取消", "确定");
    int result = dialog.exec();
    if(result == QDialog::Rejected){
        close();
    }
    else if(result == QDialog::Accepted){
        QJsonObject json;
        json["tag"] = "logout";
        json["account"] = ui->line_account->text();
        json["password"] = ui->line_password->text();
        emit logout(json);
    }
}


void Logout::logoutAnswer(const QJsonObject& json)//注销的结果
{
    if(json["answer"] == "user_not_found"){
        Dialog dialog;
        dialog.transText("账号或密码错误,请检查!");
        dialog.exec();
    }
    else if(json["answer"] == "fail"){
        Dialog dialog;
        dialog.transText("注销失败!");
        dialog.exec();
    }
}


void LineLogout::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    if(text() == "请输入您的账号" || text() == "请输入您的密码"){
        setText("");
        setStyleSheet("font: 12pt 'Microsoft YaHei UI';"
                      "border: 1px solid rgba(0, 0, 0, 0.1);"
                      "border-radius: 8px;"
                      "padding: 5px;"
                      "color: black;");
    }
    if(objectName() == "line_password"){
        this->setEchoMode(Password);
    }
    QLineEdit::focusInEvent(event);
}


void LineLogout::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
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
    }
    QLineEdit::focusOutEvent(event);
}


