#ifndef DELETEFRIENDDIALOG_H
#define DELETEFRIENDDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QMainWindow>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class DeleteFriendDialog;
}


class DeleteFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteFriendDialog(QWidget *parent = nullptr);
    ~DeleteFriendDialog();
    void paintEvent(QPaintEvent *event);//窗口背景

signals:
    void deleteYes();//确定删除

private slots:
    void on_but_delete_clicked();//取消删除操作 关闭窗口
    void on_but_cacel_clicked();//点击确定发送信号

private:
    Ui::DeleteFriendDialog *ui;
};

#endif // DELETEFRIENDDIALOG_H
