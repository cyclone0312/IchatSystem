#include "addfriends.h"
#include "ui_addfriends.h"
#include "autocleartextedit.h"
#include "dialog.h"
#include "choicedialog.h"

AddFriends::AddFriends(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFriends)
{
    ui->setupUi(this);
    //初始化窗口
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
    setWindowFlags( Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);//允许背景透明
    //清除输入框焦点
    ui->line_serach->clearFocus();
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


AddFriends::~AddFriends()
{
    delete ui;
    //this->disconnect();放在主窗口的槽函数中了
    qDebug()<<"添加好友窗口析构函数执行";
}


void AddFriends::paintEvent(QPaintEvent *event)//窗口背景
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


void AddFriends::mousePressEvent(QMouseEvent *event)//捕获鼠标按下事件
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


void AddFriends::mouseMoveEvent(QMouseEvent *event)//实现拖拽移动效果与调整大小效果
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


void AddFriends::mouseReleaseEvent(QMouseEvent *event)//重置移动状态
{
    moveFlag = 0;
}


void AddFriends:: closeEvent(QCloseEvent *event)//关闭窗口事件
{
    emit customClose();//this->disconnect();放在主窗口的槽函数中了
    this->deleteLater();
    event->accept();
}


void AddFriends::on_but_deletewindow_clicked()//关闭窗口按钮
{
    this->close();
}


void AddFriends::on_line_serach_returnPressed()//搜索好友
{
    if(ui->line_serach->text() == "输入对方账号搜索" || ui->line_serach->text().length() != 10){
        QString text =  "请输入正确的账号。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
        return;
    }
    emit serachFriends(ui->line_serach->text());
    ui->line_serach->clearFocus();
}


void AddFriends::on_but_fangdajing_clicked()//点击放大镜也是搜索
{
    on_line_serach_returnPressed();
}


void AddFriends::serachResult(const QJsonObject &json)//查找账号的结果
{
    if(json["answer"] == "fail"){
        QString text =  "没有找到用户，请检查您输入的账号。";
        Dialog* dialog = new Dialog(this);
        dialog->transText(text);
        dialog->exec();
    }
    else if(json["answer"] == "succeed"){
        //删除旧控件
        QLayoutItem *item;
        while ((item = ui->layout_frame->takeAt(0)) != nullptr) {
            delete item->widget();//删除控件
            delete item;//删除布局项
        }
        //添加间隔器
        QSpacerItem *spacertop = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
        ui->layout_frame->addItem(spacertop);
        //添加信息标签
        auto addTextEditWithStyle = [&](const QString &prefix, const QString &text) {
            AutoClearTextEdit *textEdit = new AutoClearTextEdit(prefix + text);
            textEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
            textEdit->setFixedSize(300, 42);
            textEdit->setStyleSheet(
                "font-size: 15px; "
                "color: #333333; "
                "font-weight: bold; "
                "padding: 5px; "
                "background: rgba(255, 255, 255 , 10); "
                "border: 1.5px solid black; "
                "border-radius: 5px;");
            textEdit->setAlignment(Qt::AlignCenter);
            textEdit->setReadOnly(true);//设置为只读
            textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用垂直滚动条
            textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//禁用水平滚动条
            return textEdit;
        };
        //获取用户信息
        accountNum = json["account_num"].toString();
        QString nickname = json["nickname"].toString();
        QString gender = json["gender"].toString();
        QString birthday = json["birthday"].toString();
        QString signature = json["signature"].toString();
        QString avator = json["avator"].toString();//base64
        QByteArray byteArray = QByteArray::fromBase64(avator.toUtf8());
        QImage image;
        image.loadFromData(byteArray);
        QPixmap pixmap = QPixmap::fromImage(image);
        //显示头像
        QLabel *avatorLabel = new QLabel;
        avatorLabel->setPixmap(pixmap);
        avatorLabel->setScaledContents(true);
        avatorLabel->setFixedSize(120, 120);
        avatorLabel->setAlignment(Qt::AlignCenter);
        avatorLabel->setStyleSheet("border: 3px solid black; border-radius: 8px;");
        ui->layout_frame->addWidget(avatorLabel, 0, Qt::AlignCenter);
        //添加间隔器
        QSpacerItem *spacer0 = new QSpacerItem(0, 3, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->layout_frame->addItem(spacer0);
        //添加信息
        if (!nickname.isEmpty()) ui->layout_frame->addWidget(addTextEditWithStyle("昵称: ", nickname), 0, Qt::AlignCenter);
        if (!accountNum.isEmpty()) ui->layout_frame->addWidget(addTextEditWithStyle("账号: ", accountNum), 0, Qt::AlignCenter);
        if (!gender.isEmpty()) ui->layout_frame->addWidget(addTextEditWithStyle("性别: ", gender), 0, Qt::AlignCenter);
        if (!birthday.isEmpty()) ui->layout_frame->addWidget(addTextEditWithStyle("生日: ", birthday), 0, Qt::AlignCenter);
        if (!signature.isEmpty()) ui->layout_frame->addWidget(addTextEditWithStyle("签名: ", signature), 0, Qt::AlignCenter);
        //添加间隔器
        QSpacerItem *spacer1 = new QSpacerItem(0, 3, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->layout_frame->addItem(spacer1);
        //添加添加好友按钮
        QPushButton *addButton = new QPushButton("添加好友");
        addButton->setCursor(Qt::PointingHandCursor);
        addButton->setFixedSize(180, 55);
        addButton->setStyleSheet(
            "QPushButton {"
            "    font: 12pt 'Microsoft YaHei UI';"
            "    background-color: rgb(5, 186, 251);"
            "    color: white;"
            "    font-weight: bold; "
            "    border-radius: 15px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(5, 186, 251, 0.7);"
            "    font-weight: bold; "
            "    color: white;"
            "    border-radius: 15px;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 123, 255, 0.8);"
            "    font-weight: bold; "
            "    color: rgba(255, 255, 255, 0.9);"
            "    border-radius: 15px;"
            "}"
            );
        ui->layout_frame->addWidget(addButton, 0, Qt::AlignCenter);
        //增加间距
        ui->layout_frame->setSpacing(3);
        ui->layout_frame->setContentsMargins(5, 5, 5, 5);
        //添加间隔器
        QSpacerItem *spacer2 = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
        ui->layout_frame->addItem(spacer2);
        addButton->setFocusPolicy(Qt::ClickFocus);
        //为按钮添加点击事件
        connect(addButton, &QPushButton::clicked, this, [&]() {
            qDebug() << "添加好友按钮被点击";
            ChoiceDialog dialog(this);
            dialog.transText("你确定要添加好友吗?");
            dialog.transButText("确定","取消");
            int result = dialog.exec();
            if (result == QDialog::Accepted) {
                emit addFriend(accountNum);
            }
        });
    }
}


void LineSerach_2::focusInEvent(QFocusEvent *event)//搜索框获得焦点
{
    setStyleSheet("font: 450 11pt 'Microsoft YaHei UI Light'; "
                  "border: 1px solid rgba(0, 0, 0, 0.3); "
                  "border-radius: 5px; "
                  "border: none;"
                  "padding:5px;"
                  "color: black;");
    if(text() == "输入对方账号搜索"){
        setText("");
    }
    QLineEdit::focusInEvent(event);
}


void LineSerach_2::focusOutEvent(QFocusEvent *event)//搜索失去焦点
{
    setStyleSheet("font: 450 11pt 'Microsoft YaHei UI Light'; "
                  "border: 1px solid rgba(0, 0, 0, 0.3); "
                  "border-radius: 5px; "
                  "border: none;"
                  "padding:5px;"
                  "color: grey;");
    if (text().isEmpty()) {
        setText("输入对方账号搜索");
        setStyleSheet("font: 400 11pt 'Microsoft YaHei UI Light'; "
                      "border: 1px solid rgba(0, 0, 0, 0.3); "
                      "border-radius: 5px; "
                      "border: none;"
                      "padding:5px;"
                      "color: grey;");
    }
    QLineEdit::focusOutEvent(event);
}




