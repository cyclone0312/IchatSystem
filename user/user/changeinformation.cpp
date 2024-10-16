#include "changeinformation.h"
#include "qpainterpath.h"
#include "ui_changeinformation.h"
#include "dialog.h"
#include "cutavator.h"

ChangeInformation::ChangeInformation(MyAccountInfo info, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeInformation)
    , actInfo(info)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    //设置当前页面各信息
    setMessage(actInfo);
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


ChangeInformation::~ChangeInformation()
{
    delete ui;
    qDebug()<<"修改个人信息窗口析构函数被调用";
    disconnect();
}


void ChangeInformation::paintEvent(QPaintEvent *event)//窗口背景
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


void ChangeInformation::mousePressEvent(QMouseEvent *event)//捕获鼠标按下事件
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


void ChangeInformation::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果
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


void ChangeInformation::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


QString ChangeInformation::pixmapToBase64(const QPixmap &pixmap)//图片转换成base64格式
{
    QImage image = pixmap.toImage();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray byteArray = buffer.data();
    return QString(byteArray.toBase64());
}


QPixmap ChangeInformation::base64ToPixmap(const QString &base64Str)//将base64转换为图片
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


void ChangeInformation::setMessage(const MyAccountInfo &info)//设置当前页面各信息
{
    ui->textEdit_account->setText(info.account);
    ui->line_nickname->setText(info.name);
    ui->line_sig->setText(info.signature);
    QPixmap avator = base64ToPixmap(info.avator_base64);
    ui->lab_avator->setScaledContents(true);
    ui->lab_avator->setPixmap(avator);
    if(info.gender == "其他"){
        ui->cbbox_sex->setCurrentIndex(0);
    }
    else if(info.gender == "男"){
        ui->cbbox_sex->setCurrentIndex(1);
    }
    else{
        ui->cbbox_sex->setCurrentIndex(2);
    }
}


void ChangeInformation::judgeMessage()//判断能否点击确认
{
    int flag = 0;//初始不可点击
    if(avaFlag == 1){//头像修改了 则标记可点击
        flag = 1;
    }
    if(ui->line_nickname->text() != actInfo.name || ui->line_sig->text() != actInfo.signature || ui->cbbox_sex->currentText() != actInfo.gender){
        flag = 1;//有信息修改过 则标记可点击
    }
    if(ui->line_nickname->text() == ""){//任何情况昵称为空 则不可修改
        flag = 0;
    }
    if(flag == 0){
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(167, 214, 255); "
            "color: white;"
            "border-radius: 10px;"
            "}"
            );
        ui->but_yes->setEnabled(false);
    }
    else{
        ui->but_yes->setStyleSheet(
            "QPushButton {"
            "font: 12pt 'Microsoft YaHei UI';"
            "background-color: rgb(5, 186, 251);"
            "color: white;"
            "border-radius: 10px;"
            "border: 0.5px solid rgb(220, 220, 220);"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(5, 186, 251, 0.7);"
            "color: white;"
            "border-radius: 10px;"
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(0, 123, 255, 0.8);"
            "color: rgba(255, 255, 255, 0.9);"
            "border-radius: 10px;"
            "}"
            );
        ui->but_yes->setEnabled(true);
    }
}


void ChangeInformation::on_but_cancelwindow_clicked()//关闭窗口
{
    emit customClose();
    this->close();
    this->deleteLater();
}


void ChangeInformation::on_but_no_clicked()//点击取消 也关闭窗口
{
    on_but_cancelwindow_clicked();
}


void ChangeInformation::on_cbbox_sex_currentIndexChanged(int index)//改变性别 判断能否点击确认
{
    judgeMessage();
}


void ChangeInformation::on_line_nickname_textChanged(const QString &arg1)//昵称文本框文字改变 判断是否能点击确认
{
    judgeMessage();
}


void ChangeInformation::on_line_sig_textChanged(const QString &arg1)//个性签名文本框改变 判断是否能点击确认
{
    judgeMessage();
}


void ChangeInformation::on_but_yes_clicked()//发送修改申请
{
    MyAccountInfo tmpInfo;
    tmpInfo.account = ui->textEdit_account->toPlainText();
    tmpInfo.signature = ui->line_sig->text();
    tmpInfo.gender = ui->cbbox_sex->currentText();
    tmpInfo.name = ui->line_nickname->text();
    tmpInfo.avator_base64 = pixmapToBase64(ui->lab_avator->pixmap());
    sendMessage(tmpInfo);
    ui->but_yes->setEnabled(false);
}


void ChangeInformation::getAva(const QPixmap &pixmap)//子窗口剪切好头像 收到头像 更新并判断能否点击确认
{
    ui->lab_avator->setPixmap(pixmap);
    avaFlag = 1;
    judgeMessage();
}


void ChangeInformation::dealResult(const QJsonObject &json)//处理修改资料的结果
{
    if(json["answer"] == "succeed"){//成功则修改个人资料
        actInfo.account = json["account"].toString();
        actInfo.name = json["nickname"].toString();
        actInfo.gender = json["gender"].toString();
        actInfo.signature = json["signature"].toString();
        actInfo.avator_base64 = json["avator"].toString();
        setMessage(actInfo);//重新设置个人资料
        avaFlag = 0;
        judgeMessage();//判断是否可以点击
        QString text = "修改成功!";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        emit customClose();
        close();
    }
    else if(json["answer"] == "fail"){
        qDebug()<<"修改资料失败";
        QString text = "修改失败!";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        judgeMessage();//判断是否可以点击
    }
}


void LineMessage::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    if(flag == 0){
        setText("");
        flag = 1;
    }
    QLineEdit::focusInEvent(event);
}


void LineMessage::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    QLineEdit::focusOutEvent(event);
}


void LabelChooseAva::mousePressEvent(QMouseEvent *event)//点击头像更换头像
{
    qDebug() << "点击头像了";
    QString filename = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (!filename.isEmpty()) {
        qDebug() << "头像路径有效";
        QPixmap pixmap(filename);
        if (pixmap.isNull()) {
            qDebug() << "加载图片失败";
            return;
        }
        //打开头像编辑器
        CutAvator *cutter = new CutAvator(filename);
        cutter->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Dialog);
        connect(cutter,&CutAvator::cutOk,qobject_cast<ChangeInformation*>(this->parentWidget()->parentWidget()),&ChangeInformation::getAva);
        cutter->exec();
        cutter->deleteLater();
    }
}







