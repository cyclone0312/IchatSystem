#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QLabel>
#include <QTcpSocket>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QPainterPath>
#include <QStandardItemModel>
#include <QBrush>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QTextEdit>

namespace Ui {
class RegisterWindow;
}


class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QTcpSocket *msocket,QWidget *parent = nullptr);
    ~RegisterWindow();
    void setVal();//给输入框设置验证器
    void setAva();//设置默认头像
    QPixmap createCircularPixmap(const QString &imagePath);//设置图片的圆形掩码
    void paintEvent(QPaintEvent *event);//初始化背景
    QPixmap getRoundedPixmap(const QPixmap srcPixmap, const float radiusRatio);//切割圆角图片
    void closeEvent(QCloseEvent *event);//关闭事件
    void mousePressEvent(QMouseEvent *event);//点击窗口空白让所有输入框失去焦点

private slots:
    void on_line_answer_textChanged(const QString &arg1);//文本改变 判断能不能提交
    //下面这几个槽函数都是判断能否点击提交按钮的
    void on_cbbox_sex_currentIndexChanged(int index);//性别
    void on_line_password_textChanged(const QString &arg1);//密码
    void on_line_password2_textChanged(const QString &arg1);//确认密码
    void on_line_question_textChanged(const QString &arg1);//问题
    void on_line_nickname_textChanged(const QString &arg1);//昵称
    void on_submit_but_clicked(bool checked);//提交注册申请 两个密码不一致则弹窗

public slots:
    void receiveAvator(const QPixmap &pixmap);//收到用户选择的头像然后做相应处理
    void sendAva();//发送头像给服务器
    void chengGong(QString qqnum);//注册成功
    void shiBai();//注册失败

private:
    Ui::RegisterWindow *ui;
    QTcpSocket *socket;
    QRegularExpressionValidator *validator;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray jsonData;
};

class LineRegis :public QLineEdit{
    Q_OBJECT

public:
    LineRegis(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件
};

class LabelRegis:public QLabel//点击头像更换头像
{
    Q_OBJECT

public:
    LabelRegis(QWidget *parent=nullptr):QLabel(parent){}
    void mousePressEvent(QMouseEvent *event);//点击头像更换头像
};

#endif // REGISTERWINDOW_H
