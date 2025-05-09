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
    // 检查传入的模型索引 (index) 是否有效。如果索引无效，表示没有对应的项目数据，直接返回，不进行绘制。
    // index.isValid() 如果索引指向模型中一个有效的项目，返回 true。
    if (!index.isValid()) return;

    // //保存绘图状态 : 保存 QPainter 的当前状态。
    // 将 painter 的当前状态（包括坐标变换、剪裁区域、画笔、画刷、字体、渲染提示等）保存到内部堆栈中。
    // 原理：这允许你在接下来的代码中自由地修改 painter 的设置（如设置抗锯齿、画笔、画刷等），而不用担心影响到在此函数调用之前 painter 的状态，或者影响到绘制下一个项目时的状态。
    painter->save();

    // 设置渲染提示，开启反锯齿，使绘制的图形边缘更平滑。
    painter->setRenderHint(QPainter::Antialiasing);
    // 设置渲染提示，开启文本反锯齿，使绘制的文字边缘更平滑。
    painter->setRenderHint(QPainter::TextAntialiasing);

    // //获取名称 : 从模型中获取项目的显示文本数据。
    // 从模型索引 index 中获取 Qt::DisplayRole 角色的数据（通常是项目的显示文本），并转换为 QString。
    // 原理：这是项目的主要文本内容，比如好友的昵称或“新的朋友”/“好友列表”的文字。
    QString name = index.data(Qt::DisplayRole).toString();

    // //不同条件下绘制背景 : 根据项目的状态或数据，确定背景颜色。
    QColor backgroundColor; // 声明一个 QColor 变量用于存储背景颜色。

    // 检查项目的状态 (option.state)。option.state 是一个包含多个状态标志的位掩码。
    // QStyle::State_Selected: 项目是否被选中。
    // 如果项目被选中，设置背景颜色为粉色。
    if (option.state & QStyle::State_Selected) {
        backgroundColor = QColor(255, 220, 240);//选择则粉色
    }
    // 否则，检查项目是否被鼠标悬停 (QStyle::State_MouseOver)，并且不是第一行 (索引 0) 或第三行 (索引 2)。
    // index.row() 返回项目所在的行号（从 0 开始）。
    // 原理：为非标题行（即实际可交互的行）在鼠标悬停时设置一个灰色背景，提供视觉反馈。
    else if (option.state & QStyle::State_MouseOver && index.row() != 0 && index.row() != 2) {
        backgroundColor = QColor(240, 240, 240);//鼠标悬浮则灰色
    }
    // 如果项目既没有被选中也没有被鼠标悬停，则使用系统默认的窗口背景颜色。
    // option.palette.window().color() 获取当前样式下窗口的标准背景色。
    else {
        backgroundColor = option.palette.window().color();
    }

    // 检查是否是第二行 (索引 1)，并且该项目在自定义的 Qt::UserRole + 20 角色中存储的数据为 true。
    // 原理：这部分逻辑是为“新的朋友”这个可选项（在 setupFriendList 中位于索引 1）定制的，如果它在用户自定义的某个角色中被标记为有未读状态（例如有新的好友申请），则设置一个特定的粉色背景来高亮提示。
    if(index.row() == 1 && index.data(Qt::UserRole + 20) == true){//表示有未读好友申请
        backgroundColor = QColor(255, 105, 180);
    }

    // //绘制带圆角的背景 : 使用上面确定的背景颜色绘制项目的背景矩形。
    // 设置 painter 的画刷 (brush) 颜色，画刷用于填充图形的内部区域。
    painter->setBrush(backgroundColor);
    // 设置 painter 的画笔 (pen)。Qt::NoPen 表示不绘制图形的轮廓线或边框。
    painter->setPen(Qt::NoPen);//不绘制边框
    // 在 option.rect 区域内绘制一个带圆角的矩形。
    // option.rect 是 View 提供给委托的矩形区域，表示当前项目应该绘制在这个范围内。
    // 10, 10 是圆角在 x 和 y 方向的半径。
    painter->drawRoundedRect(option.rect, 10, 10);//10是圆角半径

    // //获取图标 : 获取并处理项目的图标数据。
    // 从模型索引 index 中获取自定义的 Qt::UserRole + 4 角色的数据（在 setupFriendList 中存储了 Base64 编码的图标字符串），并转换为 QString。
    QString base64avator = index.data(Qt::UserRole + 4).toString();

    // 将 Base64 编码的字符串解码回原始的二进制数据 (QByteArray)。
    // toUtf8() 将 QString 转换为 UTF-8 编码的 QByteArray，fromBase64() 对其进行 Base64 解码。
    QByteArray imageData = QByteArray::fromBase64(base64avator.toUtf8());

    QImage image; // 声明一个 QImage 对象。
    // 尝试从解码后的二进制数据中加载图像。loadFromData() 会尝试自动检测图像格式（这里期望是 PNG）。
    bool imageLoaded = image.loadFromData(imageData); // 记录加载是否成功。

    // 如果图像成功加载
    if (imageLoaded) {
        // 缩放图像到指定尺寸 (40x40)，保持宽高比例，并使用平滑转换算法以获得更好的缩放质量。
        image = image.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 将 QImage 转换回 QPixmap，QPixmap 更适合在屏幕上高效绘制。
        QPixmap pixmap = QPixmap::fromImage(image);
        // 调用自定义函数 getRoundedPixmap (未在此显示) 对 QPixmap 进行处理，使其 corners 变成圆角。
        pixmap = getRoundedPixmap(pixmap, 5);

        // //绘制图标 : 绘制处理好的圆角图标。
        // 计算图标的垂直 (Y) 坐标，使其在项目的整个高度 (option.rect.height()) 中垂直居中。
        // option.rect.y() 是项目区域的顶部 Y 坐标。
        // (option.rect.height() - 40) / 2 是项目高度减去图标高度后剩余空间的一半。
        int y = option.rect.y() + (option.rect.height() - 40) / 2; // 计算Y坐标

        // 定义图标的绘制区域矩形。
        // 矩形从项目左侧边缘向右偏移 5 像素开始 (option.rect.x() + 5)。
        // 垂直位置是计算出的 y。
        // 宽度和高度是缩放后的图标尺寸 40x40。
        QRect iconRect(option.rect.x() + 5, y, 40, 40);//调整图标矩形的大小

        // 在 iconRect 区域内绘制 QPixmap。
        painter->drawPixmap(iconRect, pixmap);
    }

    // //设置字体和颜色 : 设置绘制文本的字体和颜色。
    // 从项目的样式选项中获取默认的文本颜色。
    QColor textColor = option.palette.text().color();//默认文字颜色
    // 获取 painter 当前使用的字体（通常继承自 View 的字体设置）。
    QFont font = painter->font();//获取默认字体

    // //检查索引 : 根据项目索引定制文本样式（为标题行设置不同的样式）。
    // 如果项目是第一行 (索引 0) 或第三行 (索引 2)（即“新的朋友”非可选和“好友列表”标题行）。
    if (index.row() == 0 || index.row() == 2) {
        textColor = Qt::gray;//设置为灰色
        font.setPointSize(10);//设置字号为 10 点
    }
    // 如果不是标题行，则使用默认的文本颜色和字体大小（或者 View 设置的）。

    // 将确定好的字体应用到 painter。
    painter->setFont(font);//应用字体设置
    // 将确定好的文本颜色设置给 painter 的画笔。画笔的颜色用于绘制文本。
    painter->setPen(textColor);//应用文字颜色

    // //绘制文字，留出图标空间 : 绘制项目的文本。
    // 检查图标是否成功加载。
    if (imageLoaded) {
        // 如果加载了图标，则绘制文本到图标右侧的一个调整过的矩形区域内。
        // option.rect.adjusted(50, 0, 0, 0) 创建一个新的矩形，其左边界比 option.rect 的左边界向右移动 50 像素。
        // 原理：这在图标绘制区域（40px 宽）右侧留出了一定的空间（图标左侧 5px 偏移 + 图标 40px 宽 + 图标右侧到文本开始的 5px 间距 = 50px）。
        // Qt::AlignVCenter: 文本在绘制区域内垂直居中。
        // Qt::TextSingleLine: 将文本视为单行，不进行换行处理。
        // name: 要绘制的文本内容。
        painter->drawText(option.rect.adjusted(50, 0, 0, 0), Qt::AlignVCenter | Qt::TextSingleLine, name);//有头像
    } else {
        // 如果没有加载图标，则绘制文本到项目左侧边缘向右偏移 5 像素开始的区域。
        painter->drawText(option.rect.adjusted(5, 0, 0, 0), Qt::AlignVCenter | Qt::TextSingleLine, name);//无头像
    }

    // //恢复绘图状态 : 恢复 QPainter 的状态。
    // 从内部堆栈中弹出最近保存的状态，将 painter 恢复到调用 save() 之前的设置。
    // 原理：确保本次 paint 调用对 painter 设置的修改不会影响到绘制下一个项目。
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

bool FilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
//好友列表过滤代理模型判断是否接收
{
    // // 好友列表过滤代理模型判断是否接收 : 函数签名。
    // 参数:
    // sourceRow: 正在被判断是否接受的行号，是源模型 (friendModel) 中的行号。
    // sourceParent: 正在被判断的行的父项的模型索引。对于列表模型 (如 QStandardItemModel 用作列表)，通常是无效的索引 (代表根)。
    // 返回值: bool。返回 true 表示这一行应该被接受 (显示)，返回 false 表示应该被过滤掉 (隐藏)。
    // const: 表示这个方法不会修改 FilterProxyModel 对象的状态。

    // 检查行号。如果源模型中的行号小于 3（即第 0, 1, 2 行）。
    // 原理：根据之前 setupFriendList 的代码，第 0 行是不可选的“新的朋友”标题，第 1 行是可选择的“新的朋友”入口，第 2 行是“好友列表”标题。
    // 这段代码明确规定：前 3 行（索引 0, 1, 2）无论过滤条件是什么，都**总是**被接受（显示）。
    if (sourceRow < 3) {
        return true; // 直接返回 true，接受这三行。
    }

    // 如果行号大于或等于 3，开始进行文本过滤检查。
    // 获取当前代理模型使用的过滤正则表达式的模式字符串。
    // filterRegularExpression() 继承自 QSortFilterProxyModel，返回当前设置的过滤正则表达式对象。
    // .pattern() 返回正则表达式的字符串模式。这就是用户在搜索框中输入的过滤文本。
    const QString filter = filterRegularExpression().pattern();

    // 如果 filter 字符串是空的，通常应该接受所有行（除了前面已经处理的头）。
    // 这段代码**缺少**对 filter 为空的判断。如果 filter 为空，contains() 总是返回 true，但如果需要更明确地处理空过滤器，可以在这里添加检查，例如：
    // if (filter.isEmpty()) return true;

    // 获取当前源模型行 (sourceRow) 在第一列 (0) 对应的模型索引。
    // sourceModel() 继承自 QAbstractProxyModel，返回指向源模型的指针。
    // index() 方法根据行号、列号和父项索引创建模型索引。
    // 原理：获取一个 QModelIndex 对象，用于访问源模型中该行的具体数据。
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    // 从模型索引中获取 Qt::DisplayRole 角色的数据（即项目的显示文本，通常是昵称），并转换为 QString。
    // 原理：获取好友的昵称数据。
    QString nickname = index.data(Qt::DisplayRole).toString();

    // 从模型索引中获取 Qt::UserRole + 1 角色的数据（在 setupFriendList 中可能存储了账号），并转换为 QString。
    // 原理：获取好友的账号数据。
    QString account = index.data(Qt::UserRole + 1).toString();

    // // 执行过滤逻辑：检查过滤字符串是否包含在昵称或账号中。
    // 检查 nickname 字符串是否包含 filter 字符串，不区分大小写。
    // contains() 是 QString 的方法，用于检查字符串是否包含另一个字符串。
    // Qt::CaseInsensitive 是一个枚举值，表示比较时忽略字母大小写。
    // || 是逻辑或运算符。
    // 检查 account 字符串是否包含 filter 字符串，不区分大小写。
    bool accepts = nickname.contains(filter, Qt::CaseInsensitive) ||
                   account.contains(filter, Qt::CaseInsensitive);
    // 原理：如果过滤字符串在昵称或账号中的任何一个里面出现（不分大小写），则 accepts 为 true。

    // 返回过滤结果。
    // 如果 filter 字符串包含在昵称或账号中，返回 true，该行将被显示。
    // 否则，返回 false，该行将被隐藏。
    return accepts;
}

bool TalkFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
//聊天列表过滤代理模型判断是否接收
{
    // // 聊天列表过滤代理模型判断是否接收 : 函数签名。
    // 参数:
    // sourceRow: 当前正在被判断的行号，它是**源模型**中的行号。
    // sourceParent: 当前行的父项的模型索引（对于列表或简单表格通常是无效的索引）。
    // 返回值: bool。返回 true 表示这一行应该被接受 (显示)，返回 false 表示应该被过滤掉 (隐藏)。
    // const: 表示这个方法不会修改 TalkFilterProxyModel 对象的状态。

    // 获取当前设置在代理模型上的过滤正则表达式的模式字符串。
    // filterRegularExpression() 继承自 QSortFilterProxyModel，返回用于过滤的 QRegularExpression 对象。
    // .pattern() 获取这个正则表达式对象对应的字符串模式。这个字符串通常就是用户在搜索框等地方输入的过滤文本。
    const QString filter = filterRegularExpression().pattern();

    // 如果 filter 字符串是空的，通常应该接受所有行。
    // **注意：** 这段代码**没有**对 filter 为空进行显式判断。如果 filter 为空，contains() 总是返回 true，所以实际上空过滤器会接受所有行，行为是正确的，但显式判断 (if (filter.isEmpty()) return true;) 可以使意图更明确。

    // 获取当前源模型行 (sourceRow) 在**第一列** (0) 对应的模型索引。
    // sourceModel() 继承自 QAbstractProxyModel，返回指向源模型的指针。
    // index() 方法根据行号、列号和父项索引创建模型索引。
    // 原理：获取一个 QModelIndex 对象，用于访问源模型中该行第一列的具体数据。对于聊天列表，通常每行就代表一个聊天对象，主要数据都在第一列。
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    // 从上面获取的模型索引 (index) 中，获取 Qt::DisplayRole 角色的数据（即项目的显示文本，对于聊天列表通常是好友的昵称），并转换为 QString。
    // 原理：获取聊天对象的昵称数据，用于后续的过滤匹配。
    QString nickname = index.data(Qt::DisplayRole).toString();

    // 从同一个模型索引 (index) 中，获取 Qt::UserRole + 1 角色的数据（在您的代码中约定俗成是存储账号 ID 的角色），并转换为 QString。
    // 原理：获取聊天对象的账号 ID 数据，用于后续的过滤匹配。
    QString account = index.data(Qt::UserRole + 1).toString();

    // // 执行过滤逻辑：检查过滤字符串是否包含在昵称或账号中。
    // 检查 nickname 字符串是否包含 filter 字符串，不区分大小写 (Qt::CaseInsensitive)。
    // || 是逻辑或运算符。
    // 检查 account 字符串是否包含 filter 字符串，不区分大小写 (Qt::CaseInsensitive)。
    bool accepts = nickname.contains(filter, Qt::CaseInsensitive) ||
                   account.contains(filter, Qt::CaseInsensitive);
    // 原理：如果过滤字符串在聊天对象的昵称或者账号中的任何一个里面出现（不分大小写），那么这个逻辑表达式 evaluates 为 true。

    // 返回过滤检查的结果。
    // 如果 accepts 为 true，表示该行符合过滤条件，代理模型将接受并显示该行。
    // 如果 accepts 为 false，表示该行不符合过滤条件，代理模型将过滤掉该行，视图中将不显示它。
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
