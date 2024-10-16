#include "mainwindow-else.h"
#include "friendmessage.h"

void TalkList::contextMenuEvent(QContextMenuEvent *event)//菜单
{
    //获取当前点击的索引
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        //设置当前选中的项
        setCurrentIndex(index);
    }
    //创建上下文菜单
    QMenu menu(this);
    QJsonObject json;
    emit choiceDone(json);//发送空 先触发点击事件
    menu.setStyleSheet(
        "QMenu {"
        "   background-color: rgb(240, 240, 240);"
        "   border-radius: 3px;"
        "   border: 0.5px solid rgb(0, 0, 0);"
        "}"
        "QMenu::item {"
        "   padding: 10px;"
        "   text-align: left;"
        "}"
        "QMenu::item:selected {"
        "   background-color: rgb(200, 200, 200);"
        "}"
        "QMenu::item:pressed {"
        "   background-color: rgb(180, 180, 180);"
        "}");
    //添加菜单项
    QAction *action1 = menu.addAction("发送消息");
    QAction *action2 = menu.addAction("查看资料");
    QAction *action3 = menu.addAction("删除好友");
    QAction *action4 = menu.addAction("移除消息列表");
    QAction *action5 = menu.addAction("清空聊天记录");
    // 执行菜单并获取用户选择
    QAction *selectedAction = menu.exec(event->globalPos());
    if (selectedAction) {
        if (selectedAction == action1) {
            //处理发送消息的逻辑
            json["tag"] = "sendmessage";
        } else if (selectedAction == action2) {
            //处理查看资料的逻辑
            json["tag"] = "message";
        } else if (selectedAction == action3) {
            //处理删除好友的逻辑
            json["tag"] = "deletefriend";
        } else if (selectedAction == action4) {
            //处理移除消息列表的逻辑
            json["tag"] = "deletetalk";
        } else if (selectedAction == action5) {
            //处理清空聊天记录的逻辑
            json["tag"] = "clearmessage";
        }
        emit choiceDone(json);
    }
}


void FriendList::mousePressEvent(QMouseEvent *event)//实现左右键都可选择
{
    QListView::mousePressEvent(event);
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
            setCurrentIndex(index);
            clicked(index);
        }
    }
}


void FriendDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//好友列表项委托绘图
{
    if (!index.isValid()) return;
    //保存绘图状态
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);//反锯齿
    painter->setRenderHint(QPainter::TextAntialiasing);//文本反锯齿
    //获取名称
    QString name = index.data(Qt::DisplayRole).toString();
    //不同条件下绘制背景
    QColor backgroundColor;
    if (option.state & QStyle::State_Selected) {
        backgroundColor = QColor(255, 220, 240);//选择则粉色
    } else if (option.state & QStyle::State_MouseOver &&index.row() != 0 && index.row() != 2) {
        backgroundColor = QColor(240, 240, 240);//鼠标悬浮则灰色
    } else {
        backgroundColor = option.palette.window().color();
    }
    if(index.row() == 1 && index.data(Qt::UserRole + 20) == true){//表示有未读好友申请
        backgroundColor = QColor(255, 105, 180);
    }
    //绘制带圆角的背景
    painter->setBrush(backgroundColor);
    painter->setPen(Qt::NoPen);//不绘制边框
    painter->drawRoundedRect(option.rect, 10, 10);//10是圆角半径
    //获取图标
    QString base64avator = index.data(Qt::UserRole + 4).toString();
    QByteArray imageData = QByteArray::fromBase64(base64avator.toUtf8());
    QImage image;
    bool imageLoaded = image.loadFromData(imageData);
    if (imageLoaded) {
        image = image.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap pixmap = QPixmap::fromImage(image);
        pixmap = getRoundedPixmap(pixmap, 5);
        //绘制图标
        int y = option.rect.y() + (option.rect.height() - 40) / 2;//计算Y坐标
        QRect iconRect(option.rect.x() + 5, y, 40, 40);//调整图标矩形的大小
        painter->drawPixmap(iconRect, pixmap);
    }
    //设置字体和颜色
    QColor textColor = option.palette.text().color();//默认文字颜色
    QFont font = painter->font();//获取默认字体
    //检查索引
    if (index.row() == 0 || index.row() == 2) {
        textColor = Qt::gray;//设置为灰色
        font.setPointSize(10);//设置字号
    }
    painter->setFont(font);//应用字体设置
    painter->setPen(textColor);//应用文字颜色
    //绘制文字，留出图标空间
    if (imageLoaded) {
        painter->drawText(option.rect.adjusted(50, 0, 0, 0), Qt::AlignVCenter | Qt::TextSingleLine, name);//有头像
    } else {
        painter->drawText(option.rect.adjusted(5, 0, 0, 0), Qt::AlignVCenter | Qt::TextSingleLine, name);//无头像
    }
    //恢复绘图状态
    painter->restore();
}


QSize FriendDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const//返回尺寸
{
    int row = index.row();
    if(row == 2 || row == 0) {
        return QSize(170, 20);
    }
    else
        return QSize(170, 56);
}


QPixmap FriendDelegate::getRoundedPixmap(QPixmap srcPixmap, const int radius)const//获得圆角图片
{
    //目标图片尺寸
    QSize desSize = srcPixmap.size();
    //新建一个目标大小的画布Qpixmap
    QPixmap desPixMap(desSize);
    //填充透明色作为背景
    desPixMap.fill(Qt::transparent);
    //以QPixmap 为绘画背景进行画笔绘制
    QPainter painter(&desPixMap);
    painter.setRenderHints(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);//平滑像素图变换
    QPainterPath path;//绘制路径
    //绘制圆角矩形，其中最后两个参数值的范围为（0-99），就是圆角的px值
    path.addRoundedRect(0, 0, desSize.width(), desSize.height(), radius, radius);
    //将绘制的圆角矩形路径中内容进行裁剪
    painter.setClipPath(path);
    //将图片绘制到desPixmap中，IgnoreAspectRatio忽视图片比例
    painter.drawPixmap(0, 0, desSize.width(), desSize.height(), srcPixmap.scaled(desSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter.setClipping(false);//关闭裁剪
    return desPixMap;
}


void TalkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//聊天列表项委托绘图
{
    if (!index.isValid()) return;
    //保存绘图状态
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing); //反锯齿
    painter->setRenderHint(QPainter::TextAntialiasing); //文本反锯齿
    //获取名称
    QString name = index.data(Qt::DisplayRole).toString();
    //不同条件下绘制背景
    QColor backgroundColor;
    if (option.state & QStyle::State_Selected) {
        backgroundColor = QColor(255, 220, 240);//选择则粉色
    } else if (option.state & QStyle::State_MouseOver) {
        backgroundColor = QColor(240, 240, 240);//鼠标悬浮则灰色
    } else {
        backgroundColor = option.palette.window().color();
    }
    //绘制带圆角的背景
    painter->setBrush(backgroundColor);
    painter->setPen(Qt::NoPen);//不绘制边框
    painter->drawRoundedRect(option.rect, 10, 10);//10是圆角半径
    //获取头像
    QString base64avator = index.data(Qt::UserRole + 4).toString();
    QByteArray imageData = QByteArray::fromBase64(base64avator.toUtf8());
    QImage image;
    bool imageLoaded = image.loadFromData(imageData);
    if (imageLoaded) {
        image = image.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap pixmap = QPixmap::fromImage(image);
        pixmap = getRoundedPixmap(pixmap, 5);
        // 绘制图标
        int y = option.rect.y() + (option.rect.height() - 40) / 2; // 计算Y坐标
        QRect iconRect(option.rect.x() + 5, y, 40, 40); // 调整图标矩形的大小
        painter->drawPixmap(iconRect, pixmap);
    }
    //设置字体和颜色
    QColor textColor = Qt::black;
    QFont font("Microsoft YaHei UI Light", 9);
    font.setWeight(QFont::Normal);
    painter->setFont(font);
    painter->setPen(textColor);
    //计算文本绘制的起始位置
    int textX = 49; // 头像右侧
    int textY = option.rect.y() + 10;
    //绘制名字
    painter->drawText(QRect(textX, textY, option.rect.width() - textX - 50, option.rect.height() / 2),
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, name);
    QColor textColor2 = QColor(100,100,100);
    painter->setPen(textColor2);
    //获取并绘制上条消息文字
    QString messageInfo = index.data(Qt::UserRole + 5).toString();
    if (messageInfo.length() > 7) {
        messageInfo = messageInfo.left(6) + "....";
    }
    font.setPointSize(font.pointSize());
    painter->setFont(font);
    painter->setPen(QColor(130, 130, 130));
    painter->drawText(QRect(textX, textY + font.pointSize() + 3, option.rect.width() - textX - 50, option.rect.height() / 2),
                      Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, messageInfo);
    //获取并绘制日期文字
    QString dataInfo = index.data(Qt::UserRole + 6).toString();
    dataInfo = displayTimeComparison(dataInfo);
    QRect dateRect(option.rect.x() + option.rect.width() - 106, textY - 5, 98, option.rect.height() / 2);
    font.setPointSize(font.pointSize() - 3);
    bool flag = false;
    if(dataInfo.size() <8){
        font.setPointSize(font.pointSize() + 3);
        flag = true;
    }
    painter->setFont(font);
    painter->setPen(QColor(100, 100, 100));
    painter->drawText(dateRect, Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, dataInfo);
    //获取未读消息个数并绘图
    if(flag){
        font.setPointSize(font.pointSize() - 3);
    }
    QString newMessage = index.data(Qt::UserRole + 10).toString();
    QColor bgColor(255, 100, 100);//背景颜色
    int cornerRadius = 10;//圆角半径
    int rectWidth = 16;//矩形的宽度
    int rectHeight = 16;//矩形的高度
    int num = newMessage.toInt();
    if (num == 0){
        //恢复绘图状态
        painter->restore();
        return;
    } else if(num > 99){
        newMessage = "99+";
        rectWidth = 24;//矩形的宽度
    }
    //计算带圆角的矩形的位置
    QRect roundedRect(option.rect.x() + 2, option.rect.y() + 2, rectWidth, rectHeight);
    painter->setBrush(bgColor);//设置填充颜色
    painter->setPen(Qt::NoPen);//不绘制边框
    painter->drawRoundedRect(roundedRect, cornerRadius, cornerRadius);//绘制带圆角的矩形
    //在矩形上绘制消息数量文本内容
    painter->setPen(Qt::white);
    font.setBold(true);
    font.setPointSize(font.pointSize() + 1);
    painter->setFont(font);
    painter->drawText(roundedRect, Qt::AlignCenter, newMessage);
    //恢复绘图状态
    painter->restore();
}


QSize TalkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const//返回尺寸
{
    return QSize(188, 56);
}


QPixmap TalkDelegate::getRoundedPixmap(QPixmap srcPixmap, const int radius)const//获得圆角图片
{
    //目标图片尺寸
    QSize desSize = srcPixmap.size();
    //新建一个目标大小的画布Qpixmap
    QPixmap desPixMap(desSize);
    //填充透明色作为背景
    desPixMap.fill(Qt::transparent);
    //以QPixmap 为绘画背景进行画笔绘制
    QPainter painter(&desPixMap);
    painter.setRenderHints(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);//平滑像素图变换
    QPainterPath path;//绘制路径
    //绘制圆角矩形，其中最后两个参数值的范围为（0-99），就是圆角的px值
    path.addRoundedRect(0, 0, desSize.width(), desSize.height(), radius, radius);
    //将绘制的圆角矩形路径中内容进行裁剪
    painter.setClipPath(path);
    //将图片绘制到desPixmap中，IgnoreAspectRatio忽视图片比例
    painter.drawPixmap(0, 0, desSize.width(), desSize.height(), srcPixmap.scaled(desSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    painter.setClipping(false);//关闭裁剪
    return desPixMap;
}


QString TalkDelegate::displayTimeComparison(const QString &timestampStr)const//解析时间戳
{
    //解析时间戳字符串
    QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy-MM-dd hh:mm:ss");
    if (!timestamp.isValid()) {
        return "";
    }
    //获取当前时间
    QDateTime current = QDateTime::currentDateTime();
    //计算日期
    QDate dateToday = current.date();
    QDate dateTimestamp = timestamp.date();
    QString result;
    //如果是今天
    if (dateToday == dateTimestamp) {
        QString timePart = timestamp.time().toString("hh:mm");
        QString period = timestamp.time().hour() < 12 ? "上午" : "下午";
        result = QString("%1 %2").arg(period).arg(timePart);
    }
    //如果是昨天
    else if (dateToday == dateTimestamp.addDays(1)) {
        QString timePart = timestamp.time().toString("hh:mm");
        QString period = timestamp.time().hour() < 12 ? "上午" : "下午";
        result = QString("昨天%1 %2").arg(period).arg(timePart);
    }
    //如果早于昨天
    else {
        result = dateTimestamp.toString("yyyy-MM-dd");
    }
    return result;
}


bool FilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const//好友列表过滤代理模型判断是否接收
{
    if (sourceRow < 3) {
        return true;
    }
    const QString filter = filterRegularExpression().pattern();
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString nickname = index.data(Qt::DisplayRole).toString();
    QString account = index.data(Qt::UserRole + 1).toString();
    bool accepts = nickname.contains(filter, Qt::CaseInsensitive) ||
                   account.contains(filter, Qt::CaseInsensitive);
    return accepts;
}


bool TalkFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const//聊天列表过滤代理模型判断是否接收
{
    const QString filter = filterRegularExpression().pattern();
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString nickname = index.data(Qt::DisplayRole).toString();
    QString account = index.data(Qt::UserRole + 1).toString();
    bool accepts = nickname.contains(filter, Qt::CaseInsensitive) ||
                   account.contains(filter, Qt::CaseInsensitive);
    return accepts;
}


void LineSerach::focusInEvent(QFocusEvent *event)//获得焦点处理事件
{
    setStyleSheet("font: 10pt 'Microsoft YaHei UI'; "
                  "border-radius: 3px; "
                  "background:white;"
                  "color: black;"
                  "padding: 5px;");
    if(text() == "搜索"){
        setText("");
    }
    QLineEdit::focusInEvent(event);
}


void LineSerach::focusOutEvent(QFocusEvent *event)//失去焦点处理事件
{
    setStyleSheet("font: 10pt 'Microsoft YaHei UI'; "
                  "border-radius: 3px; "
                  "background:rgb(245, 245, 245);"
                  "color: grey;"
                  "padding: 5px;");
    if (text().isEmpty()) {
        setText("搜索");
    }
    QLineEdit::focusOutEvent(event);
}


void LabelAva::mousePressEvent(QMouseEvent *event)//点击头像发出弹出修改个人信息窗口的信号
{
    emit changeInfo();
}


void LabelFriendAva::mousePressEvent(QMouseEvent *event)//点击头像弹出好友个人信息
{
    FriendMessage::FriendInfo info;
    info.avator_base64 = property("avator").toString();
    info.account = property("account").toString();
    info.name = property("nickname").toString();
    info.signature = property("signature").toString();
    info.gender = property("gender").toString();
    FriendMessage *dialog = new FriendMessage(info,this);
    dialog->exec();
}


void LabelFriendAvaInMessage::mousePressEvent(QMouseEvent *event)//点击头像弹出好友个人信息
{
    emit showMessage();
}


void CustomTextEdit::paintEvent(QPaintEvent *event)//右边edit的背景
{
    //调用基类方法
    QTextEdit::paintEvent(event);
    //创建QPainter对象
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    //计算窗口的最大边长和背景的最小边长
    int maxSize = qMax(viewport()->width(), viewport()->height());
    //将背景图缩放为正方形的大小
    QPixmap scaledBackground = background.scaled(maxSize, maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //计算将背景图居中的位置
    int x = (viewport()->width() - scaledBackground.width()) / 2;
    int y = (viewport()->height() - scaledBackground.height()) / 2;
    //绘制背景图片
    painter.drawPixmap(x, y, scaledBackground);
    //创建一个蒙版，设置透明度
    painter.setPen(Qt::transparent);
    painter.setBrush(QColor(255, 255, 255, 150));
    painter.drawRect(0, 0, viewport()->width(), viewport()->height());
}


void TalkStacked::paintEvent(QPaintEvent *event)//右边edit的背景
{
    //调用基类方法
    QStackedWidget::paintEvent(event);
    //创建 QPainter 对象
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    //计算窗口的最大边长和背景的最小边长
    int maxSize = qMin(width(), height());
    //将背景图缩放为正方形的大小
    QPixmap scaledBackground = background.scaled(maxSize - 5, maxSize - 5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //计算将背景图居中的位置
    int x = (width() - scaledBackground.width()) / 2;
    int y = (height() - scaledBackground.height()) / 2;
    //绘制背景图片
    painter.drawPixmap(x, y, scaledBackground);
}


ImageLabel::ImageLabel(const QPixmap &pixmap, QWidget *parent)
    : QLabel(parent), myPixmap(pixmap) {
    setPixmap(myPixmap);
    setAlignment(Qt::AlignCenter);
    setCursor(Qt::PointingHandCursor);
}


void ImageLabel::mousePressEvent(QMouseEvent *event)//鼠标按下事件打开图片
{
    if (event->button() == Qt::LeftButton) {
        QString tempFilePath = QDir::temp().filePath("temp_image.png");
        myPixmap.save(tempFilePath);
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
    }
    QLabel::mousePressEvent(event);
}


void ImageLabel::contextMenuEvent(QContextMenuEvent *event)//用户右键点击时显示菜单
{
    QMenu contextMenu(this);
    contextMenu.setStyleSheet(
        "QMenu {"
        "   background-color: rgb(240, 240, 240);"
        "   border-radius: 3px;"
        "   border: 0.5px solid rgb(0, 0, 0);"
        "}"
        "QMenu::item {"
        "   padding: 10px;"
        "   text-align: left;"
        "   color: rgb(0, 0, 0);"
        "}"
        "QMenu::item:selected {"
        "   background-color: rgb(200, 200, 200);"
        "   color: rgb(0, 0, 0);"
        "}"
        "QMenu::item:pressed {"
        "   background-color: rgb(180, 180, 180);"
        "   color: rgb(0, 0, 0);"
        "}"
        );
    QAction *saveAction = contextMenu.addAction("另存为...");
    QAction *selectedAction = contextMenu.exec(event->globalPos());
    if (selectedAction == saveAction) {
        QString fileName = QFileDialog::getSaveFileName(this, "另存为", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            myPixmap.save(fileName);
        }
    }
}


void EnterTextEdit::keyPressEvent(QKeyEvent *event)//按下判断是不是回车加ctrl
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        //如果同时按下了Ctrl，则只接受回车
        if (event->modifiers() & Qt::ControlModifier) {
            this->insertPlainText("\n");
            return;
        } else {
            emit enterKey();//只有回车则发送消息
            return;
        }
    }
    //调用基类的处理函数
    QTextEdit::keyPressEvent(event);
}
