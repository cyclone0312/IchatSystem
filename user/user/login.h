#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QLineEdit>
#include <QFocusEvent>
#include <QSettings>
#include <QMessageBox>
#include <QStyle>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QIcon>
#include <QBrush>
#include <QTimer>
#include <QLinearGradient>
#include <QStandardPaths>
#include "registerwindow.h"
#include "findpassword.h"
#include "mainwindow.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE


class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    void setAva();//初始化头像
    void setIcon();//初始化左上角图标
    void setVal();//设置输入验证器
    void setTimer();//设置一些计时器 用于动态背景与动态按钮
    bool tcpConnect();//连接服务器
    void remeberPassword();//记住密码
    void rememberAvator();//记住头像
    void ifAutoLogin();//检查自动登录设置
    void ifRememberPassword();//如果没有自动登录的检查是否有人记住密码
    void paintEvent(QPaintEvent *event);//绘制渐变背景
    void mouseMoveEvent(QMouseEvent *event);//拖拽移动窗口位置
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    void saveAvator(QString avator);//保存头像
    void dengLuChengGong();//登录
    void dengLuShiBai();//登录失败
    QPixmap createCircularPixmap(const QString &imagePath);//设置一个调整好掩码的圆形头像
    QPixmap getRoundedPixmap(const QPixmap srcPixmap, const int radius);//获得圆角图片
    void closeEvent(QCloseEvent *event);//关闭窗口事件

signals:
    void sendAvator();//通知子窗口发送头像给服务器
    void zhuCeShiBai();//通知子窗口注册失败
    void zhuCeChengGong(QString qqnum);//通知子窗口注册成功
    void findPass1(const QJsonObject &jsonObj);//通知子窗口找回密码
    void findPass2(const QJsonObject &jsonObj);//通知子窗口回答问题的结果
    void findPass3(const QJsonObject &jsonObj);//通知子窗口找回密码的结果
    void loginSucceed(const QString account);//登录成功通知main新建一个主窗口

private slots:
    void onReadyRead();//读取服务器发送的消息
    void on_gologin_clicked();//点击登录发送请求
    void sendLogin();//发送登录信息
    void updateLoginText();//更新取消按钮的文本 模拟动态效果
    void on_line_password_returnPressed();//回车登录也会发送请求
    void on_line_qqnum_returnPressed();//回车登录也会发送请求
    void mousePressEvent(QMouseEvent *event);//点击窗口空白让输入框失去焦点 并且判断拖拽窗口
    void on_line_password_textChanged(const QString &arg1);//改变密码文本是否判断登录是否可点击
    void on_line_qqnum_textChanged(const QString &arg1);//改变登录文本是否判断登录是否可点击
    void on_auto_ckbox_toggled(bool checked);//勾选自动登录自动勾选记住密码
    void on_ckbox_remeber_toggled(bool checked);//记住密码取消勾选时 如果自动登录仍勾选 则取消
    void setSty();//初始设置输入框格式
    void on_rgs_pbt_clicked();//注册账号
    void on_fgt_pbt_clicked();//找回密码
    void updateGradient() ;//更新渐变背景
    void on_but_deletewindow_clicked();//关闭按钮

private:
    Ui::Login *ui;
    QTcpSocket socket;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray jsonData;
    QRegularExpressionValidator *validator;
    QSettings settings = QSettings("settings.ini", QSettings::IniFormat);
    /*存储
    /  lastlogin: 上次成功登录的用户名。如果有说明则有人记住密码了 没有则说明没人记住密码 不必理会
    /  账号/password: 与特定用户相关联的密码。分组存储 组名是账号
    */
    QSettings set = QSettings("set.ini", QSettings::IniFormat);
    /*存储
    /  autologin: 是否启用自动登录的布尔值。
    /  autologinuser: 自动登录的用户名。加载后去setting中找密码
    */
    RegisterWindow* regis;//注册账号窗口对象
    FindPassword* finddPassword;//找回密码窗口对象
    QTimer *timer;
    QPoint dragPosition;
    QString saveLoginAvatorPath;//保存上次登录成功的人的头像 用来初始化界面
    MainWindow *mainwind;
    QTimer *cancelTimer;
    bool cancelRequested;
    QTimer *textUpdateTimer;//用于更新按钮文本的计时器
    int dotCount = 0;//用于更新文本的点计数
    bool loginFlag = false;//是不是正在登录
    int moveFlag = 0;//用于判断是否要拖动窗口
    int offset = 0 ;//渐变尺度;
};

class LineInput :public QLineEdit//输入框类
{
    Q_OBJECT

public:
    LineInput(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件
};

#endif // LOGIN_H
