#include "friendmessage.h"
#include "ui_friendmessage.h"

FriendMessage::FriendMessage(FriendInfo info, QWidget *parent)
    :QDialog(parent),  ui(new Ui::FriendMessage)
    , friendInfo(info)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    //设置当前页面各信息
    setMessage(friendInfo);
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


FriendMessage::~FriendMessage()
{
    delete ui;
    this->disconnect();
    qDebug()<<"好友信息被析构";
}


void FriendMessage::paintEvent(QPaintEvent *event)//窗口背景
{
    //调用基类方法以处理父类的绘图
    QDialog::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    //加载背景图片
    QPixmap background(":/pictures/094 Cloudy Apple - trans.png");
    //确保图片已加载
    if (background.isNull()) {
        qWarning("背景图像加载失败");
        return;
    }
    QSize newSize = QSize(this->width(), this->height());
    //将背景图缩放为新的大小
    QPixmap scaledBackground = background.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //计算绘制的位置，保持居中
    int x = (this->width() - scaledBackground.width()) / 2;
    int y = (this->height() - scaledBackground.height()) / 2;
    //设置剪裁区域以适应圆角窗口
    int radius = 10;
    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius);
    painter.setClipPath(path);
    //绘制背景图片
    painter.drawPixmap(x, y, scaledBackground);
    //绘制边框
    painter.setPen(QPen(Qt::gray, 3.5));
    painter.drawRoundedRect(rect(), radius, radius);
}


void FriendMessage::mousePressEvent(QMouseEvent *event)//捕获鼠标按下事件
{
    //清除焦点
    QList<QWidget*> widgets = this->findChildren<QWidget*>();
    for (QWidget* widget : widgets) {
        widget->clearFocus();
    }
    QPoint pos = event->pos();
    int margin = 30;
    if (pos.x() <= 30 || pos.x() >= width() - 30 ||
        pos.y() <= 30 || pos.y() >= height() - 30){//如果点击在边缘
        qDebug() << "点击在边缘";
        moveFlag = 1;
        dragPosition = event->globalPosition().toPoint() - this->frameGeometry().topLeft();//记录鼠标相对于窗口的初始偏移
        event->accept();
    }
}


void FriendMessage::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果
{
    qDebug()<<"鼠标位移了";
    QPoint globalPos = event->globalPosition().toPoint();//获取当前的鼠标位置
    QPoint delta = globalPos - (this->frameGeometry().topLeft() + dragPosition);//计算当前鼠标位置与窗口之间的相对偏移
    if (event->buttons() & Qt::LeftButton) {
        if (moveFlag == 1) {
            this->move(globalPos - dragPosition);
            event->accept();
        }
        //更新拖动位置
        dragPosition = globalPos - this->frameGeometry().topLeft();
    }
}


void FriendMessage::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


QString FriendMessage::pixmapToBase64(const QPixmap &pixmap)//图片转换成base64格式
{
    QImage image = pixmap.toImage();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray byteArray = buffer.data();
    return QString(byteArray.toBase64());
}


QPixmap FriendMessage::base64ToPixmap(const QString &base64Str)//将base64转换为图片
{
    QByteArray byteArray = QByteArray::fromBase64(base64Str.toUtf8());
    QImage image;
    //尝试加载PNG格式
    if (!image.loadFromData(byteArray, "PNG")) {
        //如果PNG加载失败，尝试JPG格式
        image.loadFromData(byteArray, "JPG");
    }
    return QPixmap::fromImage(image);
}


void FriendMessage::setMessage(const FriendInfo &info)//设置当前页面各信息
{
    ui->textEdit_account->setText(info.account);
    ui->textEdit_nickname->setText(info.name);
    ui->textEdit_sig->setText(info.signature);
    ui->textEdit_gender->setText(info.gender);
    QPixmap avator = base64ToPixmap(info.avator_base64);
    ui->lab_avator->setScaledContents(true);
    ui->lab_avator->setPixmap(avator);
}


void FriendMessage::on_but_cancelwindow_clicked()//关闭窗口
{
    this->close();
    this->deleteLater();
}

