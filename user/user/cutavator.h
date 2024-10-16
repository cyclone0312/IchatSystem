#ifndef CUTAVATOR_H
#define CUTAVATOR_H

#include <QDialog>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QString>
#include <QMessageBox>
#include <QPalette>
#include <QDebug>
#include <QTimer>
#include <QStyle>

namespace Ui {
class CutAvator;
}


class CutAvator : public QDialog
{
    Q_OBJECT

public:
    explicit CutAvator(const QString &mypath,QWidget *parent = nullptr);
    ~CutAvator();
    void dealImage(QString path);//处理传入的图片
    void paintEvent(QPaintEvent *event);//初始化整个窗口背景
    void setBackground(QString avapath);//设置头像

public slots:
    void moveRect(const QPoint &newPosition);//移动明亮矩形更新头像

signals:
    void cutOk(const QPixmap &pixmap);//裁剪完了发送信号

private slots:
    void on_but_cut_clicked();//点击后剪裁头像
    void on_but_yes_clicked();//点击后上传头像并关闭窗口

private:
    Ui::CutAvator *ui;
    QString path;
    QRect *labelRect;
    QPainter painter;
    int flag = 0;
};

class DraggableResizableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DraggableResizableLabel(QWidget *parent = nullptr) : QLabel(parent)
    {}

signals:
    void labelMoved(const QPoint &newPosition);//标签发生任何变化时发送信号

protected:
    void mousePressEvent(QMouseEvent *event) override;//点击判断是要干什么
    void mouseMoveEvent(QMouseEvent *event) override;//移动时判断是移动还是变动大小
    void mouseReleaseEvent(QMouseEvent *event) override;//释放重置状态

private:
    QPoint myOffset;
    bool myResizing = false;//标志是否正在调整大小
};

#endif // CUTAVATOR_H
