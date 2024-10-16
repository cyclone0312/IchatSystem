#include "findpassword.h"
#include "qevent.h"
#include "ui_findpassword.h"
#include "dialog.h"

FindPassword::FindPassword(QTcpSocket* msocket,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FindPassword),socket(msocket)
{
    ui->setupUi(this);
    //初始化窗口
    setWindowFlags(Qt::Dialog);
    //给输入框设置验证器
    setVal();
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


FindPassword::~FindPassword()
{
    delete ui;
    delete validator;
}


void FindPassword::setVal()//给输入框设置验证器
{
    QRegularExpression regExp("^[a-zA-Z0-9]{1,12}$");
    FindPassword::validator = new QRegularExpressionValidator(regExp, this);
    ui->line_pages1->setValidator(validator);
    ui->line_pages3->setValidator(validator);
    ui->line_pages3_2->setValidator(validator);
}


void FindPassword::paintEvent(QPaintEvent *event)//初始化背景
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    QPixmap pixmap(":/pictures/094 Cloudy Apple - trans.png");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}


void FindPassword::closeEvent(QCloseEvent *event)//关闭窗口事件
{
    this->disconnect();
    //在关闭时调用 deleteLater
    this->deleteLater();
    //接受关闭事件
    event->accept();
}


void FindPassword::mousePressEvent(QMouseEvent *event)//点击窗口空白让所有输入框失去焦点
{
    ui->line_pages1->clearFocus();
    ui->line_pages2->clearFocus();
    ui->line_pages3->clearFocus();
    ui->line_pages3_2->clearFocus();
}


void FindPassword::on_but_pages1_clicked()//切换页面
{
    if(socket->state() != QAbstractSocket::ConnectedState){//没连接到服务器
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    jsonObj = QJsonObject();
    jsonObj["tag"] = "findpassword1";
    jsonObj["qq_number"] = ui->line_pages1->text();
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket->write(messageWithSeparator);
    socket->flush();
    jsonData.clear();
    qDebug() << "发送要找回的账号了";
}


void FindPassword::on_but_pages2_clicked()//切换页面
{
    if(socket->state() != QAbstractSocket::ConnectedState){//没连接到服务器
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    jsonObj = QJsonObject();
    jsonObj["tag"] = "findpassword2";
    jsonObj["qq_number"] = ui->line_pages1->text();
    jsonObj["theanswer"] = ui->line_pages2->text();
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket->write(messageWithSeparator);
    socket->flush();
    jsonData.clear();
    qDebug() << "发送密保答案了"<<messageWithSeparator;
}


void FindPassword::on_but_pages3_clicked()//提交最后的修改密码请求
{
    if(socket->state() != QAbstractSocket::ConnectedState){//没连接到服务器
        Dialog msgBox(this);
        msgBox.transText("请检查您的网络连接!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    if(ui->line_pages3->text() != ui->line_pages3_2->text()){
        Dialog msgBox(this);
        msgBox.transText("您的两次密码不一致!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
        return;
    }
    jsonObj = QJsonObject();
    jsonObj["tag"] = "findpassword3";
    jsonObj["qq_number"]= ui->line_pages1->text();
    jsonObj["password"] = ui->line_pages3_2->text();
    //发送消息
    jsonDoc = QJsonDocument(jsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket->write(messageWithSeparator);
    socket->flush();
    jsonData.clear();
    qDebug() << "发送修改后的密码了"<<messageWithSeparator;
}


void FindPassword::findPassword1(const QJsonObject &jsonObj)//看看有没有这个账号 对应切换页面或弹窗
{
    if(jsonObj["answer"] == "no"){
        Dialog msgBox(this);
        msgBox.transText("请检查您的账号!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
    }
    if(jsonObj["answer"] == "yes"){
        ui->page_find->setCurrentIndex(1);
        ui->lab_pages2->setText(jsonObj["question"].toString());
    }
}


void FindPassword::findPassword2(const QJsonObject &jsonObj)//看看问题回答对了没有 对应切换页面或弹窗
{
    if(jsonObj["answer"] == "no"){
        Dialog msgBox(this);
        msgBox.transText("您的答案不正确!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
    }
    if(jsonObj["answer"] == "yes"){
        ui->page_find->setCurrentIndex(2);
    }
}


void FindPassword::findPassword3(const QJsonObject &jsonObj)//修改密码成功没有 对应弹窗
{
    if(jsonObj["answer"] == "no"){
        Dialog msgBox(this);
        msgBox.transText("请求失败!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        msgBox.exec();
    }
    if(jsonObj["answer"] == "yes"){
        QString text =  "找回密码成功";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog |Qt::FramelessWindowHint);
        dialog->exec();
        this->disconnect();
        this->close();
    }
}


void LineFind::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    if(text() == "请输入你的账号" || text() == "请输入密保答案" || text() == "请输入新的密码" || text() == "请确认密码"){
        setText("");
    }
    setStyleSheet("font: 13pt 'Microsoft YaHei UI'; "
                  "border: 1px solid rgba(0, 0, 0, 0.3); "
                  "border-radius: 10px; "
                  "color: black;");
    QLineEdit::focusInEvent(event);
}


void LineFind::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    if(text() == ""){
        if(objectName() == "line_pages1"){
            setStyleSheet(
                "font: 13pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入你的账号");
        }
        else if(objectName() == "line_pages2"){
            setStyleSheet(
                "font: 13pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入密保答案");
        }
        else if(objectName() == "line_pages3_2"){
            setStyleSheet(
                "font: 13pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请输入新的密码");
        }
        else if(objectName() == "line_pages3"){
            setStyleSheet(
                "font: 13pt 'Microsoft YaHei UI'; "
                "border: 1px solid rgba(0, 0, 0, 0.3); "
                "border-radius: 8px; "
                "color: grey;"
                );
            setText("请确认密码");
        }
    }
    QLineEdit::focusOutEvent(event);
}
