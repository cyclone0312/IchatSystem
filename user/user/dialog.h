#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QPainter>

namespace Ui {
class Dialog;
}


class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setEdit();//设置文本框
    void paintEvent(QPaintEvent *event);//窗口背景
    void transText(QString text);//改变文字

private slots:
    void on_but_delete_clicked();//点击确定关闭窗口

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
