#ifndef AddFriendDialog_H
#define AddFriendDialog_H

#include <QDialog>
#include <QPainter>
#include "dialog.h"

namespace Ui {
class AddFriendDialog;
}


class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = nullptr);
    ~AddFriendDialog();
    void paintEvent(QPaintEvent *event);//窗口背景

private slots:
    void on_but_no_clicked();//取消添加好友操作 关闭窗口
    void on_but_yes_clicked();//点击确定发送信号 要求发送好友申请

private:
    Ui::AddFriendDialog *ui;
};

#endif // AddFriendDialog_H
