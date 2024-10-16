#ifndef LOGOUT_H
#define LOGOUT_H

#include <QValidator>
#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QRegularExpression>
#include <QLineEdit>
#include <QJsonObject>

namespace Ui {
class Logout;
}

class Logout : public QDialog//注销类
{
    Q_OBJECT

public:
    explicit Logout(QWidget *parent = nullptr);
    ~Logout();
    void setVal();//设置输入验证器
    void paintEvent(QPaintEvent *event);//窗口背景
    void mousePressEvent(QMouseEvent *event);//捕获鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    void closeEvent(QCloseEvent *event);//关闭窗口事件

private slots:
    void on_but_deletewindow_clicked();//关闭窗口
    void on_line_account_textChanged(const QString &arg1);//判断确认是否可以点击
    void on_line_password_textChanged(const QString &arg1);//判断确认是否可以点击
    void on_but_yes_clicked();//是否确定注销

public slots:
    void logoutAnswer(const QJsonObject& json);//注销的结果

signals:
    void customClose();//发出退出的信号
    void logout(const QJsonObject &json);//告知主窗体发送注销申请

private:
    Ui::Logout *ui;
    int moveFlag = 0;
    QPoint dragPosition;
    QValidator *validator;
};


class LineLogout:public QLineEdit//输入框类
{
    Q_OBJECT

public:
    LineLogout(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件
};

#endif // LOGOUT_H
