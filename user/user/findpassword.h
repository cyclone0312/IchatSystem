#ifndef FINDPASSWORD_H
#define FINDPASSWORD_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QMessageBox>
#include <QStyle>

namespace Ui {
class FindPassword;
}


class FindPassword : public QMainWindow
{
    Q_OBJECT

public:
    explicit FindPassword(QTcpSocket* msocket,QWidget *parent = nullptr);
    ~FindPassword();
    void setVal();//给输入框设置验证器
    void paintEvent(QPaintEvent *event);//初始化背景
    void closeEvent(QCloseEvent *event);//关闭窗口事件
    void mousePressEvent(QMouseEvent *event);//点击窗口空白让所有输入框失去焦点

private slots:
    void on_but_pages1_clicked();//切换页面
    void on_but_pages2_clicked();//切换页面
    void on_but_pages3_clicked();//提交最后的修改密码请求

public slots:
    void findPassword1(const QJsonObject &jsonObj);//看看有没有这个账号 对应切换页面或弹窗
    void findPassword2(const QJsonObject &jsonObj);//看看问题回答对了没有 对应切换页面或弹窗
    void findPassword3(const QJsonObject &jsonObj);//看看修改密码的结果 对应弹窗

private:
    Ui::FindPassword *ui;
    QTcpSocket *socket;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray jsonData;
    QRegularExpressionValidator *validator;
};


class LineFind :public QLineEdit//输入文本框
{
    Q_OBJECT

public:
    LineFind(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件
};

#endif // FINDPASSWORD_H
