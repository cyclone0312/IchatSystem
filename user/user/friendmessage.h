#ifndef FRIENDMESSAGE_H
#define FRIENDMESSAGE_H

#include <QDialog>
#include <QLabel>
#include <QDialog>
#include <QString>
#include <QPainter>
#include <QBuffer>
#include <Qpixmap>
#include <QFileDialog>
#include <QPainterPath>
#include <QJsonObject>
#include <QMouseEvent>

namespace Ui {
class FriendMessage;
}

class FriendMessage : public QDialog
{
    Q_OBJECT

public:
    struct FriendInfo {//存储好友信息
        QString account;
        QString name;
        QString avator_base64;
        QString gender;
        QString signature;
    };

public:
    explicit FriendMessage(FriendInfo info, QWidget *parent = nullptr);
    ~FriendMessage();
    void paintEvent(QPaintEvent *event);//窗口背景
    void mousePressEvent(QMouseEvent *event);//捕获鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    QString pixmapToBase64(const QPixmap &pixmap);//图片转换成base64格式
    QPixmap base64ToPixmap(const QString &base64Str);//将base64转换为图片
    void setMessage(const FriendInfo &info);//设置当前页面各信息

private slots:
    void on_but_cancelwindow_clicked();//关闭窗口

private:
    Ui::FriendMessage *ui;
    FriendInfo friendInfo;//存储好友信息
    int moveFlag = 0;
    QPoint dragPosition;
};

#endif // FRIENDMESSAGE_H
