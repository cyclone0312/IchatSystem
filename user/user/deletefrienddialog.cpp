#include "deletefrienddialog.h"
#include "ui_deletefrienddialog.h"

DeleteFriendDialog::DeleteFriendDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteFriendDialog)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
    setWindowFlags( Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);//允许背景透明
    //将窗口居中显示
    if (parent) {
        int x = parent->x() + (parent->width() - width()) / 2;
        int y = parent->y() + (parent->height() - height()) / 2;
        move(x, y);
    }
}


DeleteFriendDialog::~DeleteFriendDialog()
{
    delete ui;
    this->disconnect();
}


void DeleteFriendDialog::paintEvent(QPaintEvent *event)//窗口背景
{
    //调用基类方法以处理父类的绘图
    QDialog::paintEvent(event);
    QPainter painter(this);
    //设置圆角半径
    int radius = 4;//圆角半径
    //设置背景颜色
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    //绘制圆角矩形背景
    painter.drawRoundedRect(rect(), radius, radius);
}


void DeleteFriendDialog::on_but_delete_clicked()//取消删除操作 关闭窗口
{
    this->disconnect();
    this->close();
}


void DeleteFriendDialog::on_but_cacel_clicked()//点击确定发送信号
{
    emit deleteYes();
}

