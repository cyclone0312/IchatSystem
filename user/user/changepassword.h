#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QValidator>
#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QRegularExpression>
#include <QLineEdit>
#include <QJsonObject>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr);
    ~ChangePassword();
    void setVal();//设置输入验证器
    void paintEvent(QPaintEvent *event);//窗口背景
    void mousePressEvent(QMouseEvent *event);//捕获鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    void closeEvent(QCloseEvent *event);//关闭窗口事件

private slots:
    void on_but_deletewindow_clicked();//关闭按钮
    void on_line_account_textChanged(const QString &arg1);//第一个页面判断能否点击确认按钮
    void on_line_password_textChanged(const QString &arg1);//第一个页面判断能否点击确认按钮
    void on_line_newpassword_textChanged(const QString &arg1);//第二个页面判断能否点击确认按钮
    void on_line_newpassword2_textChanged(const QString &arg1);//第二个页面判断能否点击确认按钮
    void on_but_yes_clicked();//告知主窗体发送账号密码
    void on_but_yes_2_clicked();//告知主窗体发送修改密码

public slots:
    void changePasswordAns1(const QJsonObject& json);//修改账号密码第一个申请的结果
    void changePasswordAns2(const QJsonObject& json);//修改账号密码第二个申请的结果

signals:
    void customClose();//通知主窗体要关闭了
    void changePassword1(const QJsonObject &json);//告知主窗体发送修改申请
    void changePassword2(const QJsonObject &json);//告知主窗体发送最终修改申请

private:
    Ui::ChangePassword *ui;
    int moveFlag = 0;
    QPoint dragPosition;
    QValidator *validator;
};


class LineChangePass:public QLineEdit//输入框类
{
    Q_OBJECT

public:
    LineChangePass(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件
};

#endif // CHANGEPASSWORD_H
