#ifndef MAINWINDOW-ELSE_H
#define MAINWINDOW-ELSE_H

#include <QStackedWidget>
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
#include <QMouseEvent>
#include <QTimer>
#include <QMutex>
#include <QJsonArray>
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QMenu>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QListView>
#include <QPushButton>
#include <QSqlError>
#include <QDesktopServices>
#include <QClipboard>


class TalkFilterProxyModel;


class TalkList : public QListView//消息列表类
{
    Q_OBJECT

public:
    TalkList(QWidget *parent = nullptr) : QListView(parent) {
    }

protected:
    void contextMenuEvent(QContextMenuEvent *event);//菜单

signals:
    void choiceDone(const QJsonObject &json);
};


class FriendList : public QListView//好友列表类
{
    Q_OBJECT

public:
    FriendList(QWidget *parent = nullptr) : QListView(parent) {
    }

    void mousePressEvent(QMouseEvent *event);//实现左右键都可选择
};


class FriendDelegate : public QStyledItemDelegate//好友列表委托绘图类
{
    Q_OBJECT
public:
    FriendDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;//好友列表项委托绘图
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;//返回尺寸
    QPixmap getRoundedPixmap(QPixmap srcPixmap, const int radius)const;//获得圆角图片
};


class TalkDelegate : public QStyledItemDelegate//聊天列表委托绘图类
{
    Q_OBJECT
public:
    TalkDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;//聊天列表项委托绘图
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;//返回尺寸
    QPixmap getRoundedPixmap(QPixmap srcPixmap, const int radius)const;//获得圆角图片
    QString displayTimeComparison(const QString &timestampStr)const;//解析时间戳
};


class FilterProxyModel : public QSortFilterProxyModel//好友列表过滤代理模型
{
    Q_OBJECT
public:
    explicit FilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent) {
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent)const;//好友列表过滤代理模型判断是否接收
};


class TalkFilterProxyModel : public QSortFilterProxyModel//聊天列表过滤代理模型
{
    Q_OBJECT
public:
    explicit TalkFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent) {
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent)const;//聊天列表过滤代理模型判断是否接收
};


class LineSerach :public QLineEdit//搜索框类
{
    Q_OBJECT

public:
    LineSerach(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件

};


class LabelAva:public QLabel//实现点击头像弹出修改信息窗口
{
    Q_OBJECT

public:
    LabelAva(QWidget *parent=nullptr):QLabel(parent){}
    void mousePressEvent(QMouseEvent *event);//点击头像发出弹出修改个人信息窗口的信号

signals:
    void changeInfo();
};


class LabelFriendAva:public QLabel//实现点击头像弹出好友信息窗口(好友申请内的)
{
    Q_OBJECT

public:
    LabelFriendAva(QWidget *parent=nullptr):QLabel(parent){}
    void mousePressEvent(QMouseEvent *event);//点击头像弹出好友个人信息
};


class LabelFriendAvaInMessage:public QLabel//实现点击头像弹出好友信息窗口（消息页面内的）
{
    Q_OBJECT

public:
    LabelFriendAvaInMessage(QWidget *parent=nullptr):QLabel(parent){}
    void mousePressEvent(QMouseEvent *event);//点击头像弹出好友个人信息

signals:
    void showMessage();
};


class CustomTextEdit : public QTextEdit//显示好友信息的edit类
{
public:
    CustomTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) , background(":/pictures/andan_touming.jpg"){}
    QPixmap background;//背景图

protected:
    void paintEvent(QPaintEvent *event);//右边edit的背景
};


class TalkStacked : public QStackedWidget//显示聊天信息的类
{
public:
    TalkStacked(QWidget *parent = nullptr): QStackedWidget(parent), background(":/pictures/andan_touming.jpg"){};
    QPixmap background;//背景图

protected:
    void paintEvent(QPaintEvent *event);//右边edit的背景

};


class ImageLabel : public QLabel//图片消息框
{
    Q_OBJECT

public:
    explicit ImageLabel(const QPixmap &pixmap, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件打开图片
    void contextMenuEvent(QContextMenuEvent *event) override;//用户右键点击时显示菜单

private:
    QPixmap myPixmap;
};


class EnterTextEdit : public QTextEdit {
    Q_OBJECT

public:
    EnterTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override;//按下判断是不是回车加ctrl

signals:
    void enterKey();//回车和ctrl键被按下
};
#endif // MAINWINDOW-ELSE_H
