#ifndef ADDFRIENDS_H
#define ADDFRIENDS_H

#include <QDialog>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QJsonObject>
#include <QTextEdit>
#include <QLabel>

namespace Ui {
class AddFriends;
}


class AddFriends : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriends(QWidget *parent = nullptr);
    ~AddFriends();
    void paintEvent(QPaintEvent *event);//窗口背景
    void mousePressEvent(QMouseEvent *event);//捕获鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态

protected:
    void closeEvent(QCloseEvent *event);//关闭窗口事件

private slots:
    void on_but_deletewindow_clicked();//关闭窗口按钮
    void on_line_serach_returnPressed();//搜索好友
    void on_but_fangdajing_clicked();//点击放大镜也是搜索

public slots:
    void serachResult(const QJsonObject &json);//查找账号的结果

signals:
    void serachFriends(const QString &account);//向主窗体发送搜索账号信息
    void customClose();//向主窗体发送信号 表示已经关闭了
    void addFriend(const QString &friendAccount);//向主窗体发送信号 表示添加好友

private:
    Ui::AddFriends *ui;
    QString accountNum;//搜索到的账号
    QPoint dragPosition;
    int moveFlag = 0;
};


class LineSerach_2 :public QLineEdit//搜索框类
{
    Q_OBJECT

public:
    LineSerach_2(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//搜索框获得焦点
    void focusOutEvent(QFocusEvent *event);//搜索失去焦点
};

#endif // ADDFRIENDS_H
