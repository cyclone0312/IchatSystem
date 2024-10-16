#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriendDialog)
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


AddFriendDialog::~AddFriendDialog()
{
    delete ui;
    this->disconnect();
}


void AddFriendDialog::paintEvent(QPaintEvent *event)//窗口背景
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


void AddFriendDialog::on_but_no_clicked()//取消添加好友操作 关闭窗口
{
    QDialog::reject();
    this->deleteLater();
}


void AddFriendDialog::on_but_yes_clicked()//点击确定发送信号 要求发送好友申请
{
    QDialog::accept();
    this->deleteLater();
}

