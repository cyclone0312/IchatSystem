#ifndef CHANGEINFORMATION_H
#define CHANGEINFORMATION_H

#include "qlineedit.h"
#include <QLabel>
#include <QDialog>
#include <QString>
#include <QPainter>
#include <QBuffer>
#include <Qpixmap>
#include <QFileDialog>
#include <QJsonObject>

namespace Ui {
class ChangeInformation;
}


struct MyAccountInfo {
    QString account;
    QString name;
    QString avator_base64;
    QString gender;
    QString signature;
};


class ChangeInformation : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeInformation(MyAccountInfo info, QWidget *parent = nullptr);
    ~ChangeInformation();
    void paintEvent(QPaintEvent *event);//窗口背景
    void mousePressEvent(QMouseEvent *event);//捕获鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    QString pixmapToBase64(const QPixmap &pixmap);//图片转换成base64格式
    QPixmap base64ToPixmap(const QString &base64Str);//将base64转换为图片
    void setMessage(const MyAccountInfo &info);//设置当前页面各信息
    void judgeMessage();//判断能否点击确认

private slots:
    void on_but_cancelwindow_clicked();//关闭窗口
    void on_but_no_clicked();//点击取消 也关闭窗口
    void on_cbbox_sex_currentIndexChanged(int index);//改变性别 判断能否点击确认
    void on_line_nickname_textChanged(const QString &arg1);//昵称文本框文字改变 判断是否能点击确认
    void on_line_sig_textChanged(const QString &arg1);//个性签名文本框改变 判断是否能点击确认
    void on_but_yes_clicked();//发送修改申请

public slots:
    void getAva(const QPixmap &pixmap);//子窗口剪切好头像 收到头像 更新并判断能否点击确认
    void dealResult(const QJsonObject &json);//处理修改资料的结果

signals:
    void customClose();//告诉主窗口要关闭了
    void sendMessage(const MyAccountInfo& info);//告诉主窗口 发送新的信息

private:
    Ui::ChangeInformation *ui;
    MyAccountInfo actInfo; // 存储账号信息
    QPoint dragPosition;
    int moveFlag = 0;
    int avaFlag = 0;//判断头像是不是更新过
};


class LineMessage:public QLineEdit//输入框类
{
    Q_OBJECT

public:
    LineMessage(QWidget *parent = nullptr) : QLineEdit(parent) {}

public slots:
    void focusInEvent(QFocusEvent *event);//获得焦点处理事件
    void focusOutEvent(QFocusEvent *event);//失去焦点处理事件

private:
    int flag = 0;//标记是不是第一次点击
};


class LabelChooseAva:public QLabel//点击头像更换头像
{
    Q_OBJECT

public:
    LabelChooseAva(QWidget *parent=nullptr):QLabel(parent){}
    void mousePressEvent(QMouseEvent *event);//点击头像更换头像
};
#endif // CHANGEINFORMATION_H
