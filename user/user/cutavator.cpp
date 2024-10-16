#include "cutavator.h"
#include "ui_cutavator.h"
#include "registerwindow.h"

CutAvator::CutAvator(const QString &mypath,QWidget *parent)
    : QDialog(parent),ui(new Ui::CutAvator)
    , path(mypath)
{
    ui->setupUi(this);
    //处理传入的图片，确保它是有效的
    dealImage(mypath);
    //先截一次 防止图片格式出错
    on_but_cut_clicked();
    //连接截取框信号槽
    connect(ui->lab_cut, &DraggableResizableLabel::labelMoved, this, &CutAvator::moveRect);
    ui->lab_cut->resize(277,277);
    // 将窗口居中显示
    QWidget *topLevelParent = parent ? parent->window() : nullptr;
    if (topLevelParent) {
        int x = topLevelParent->x() + (topLevelParent->width() - width()) / 2;
        int y = topLevelParent->y() + (topLevelParent->height() - height()) / 2;
        move(x, y);
    }
}


CutAvator::~CutAvator()
{
    delete ui;
    delete labelRect;
}


void CutAvator::dealImage(QString path)//处理传入的图片
{
    QImage image(path);
    if (image.isNull()) {
        QMessageBox msgBox;
        QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(icon);
        msgBox.setWindowTitle("加载失败");
        msgBox.setText("您的图片有问题!");
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
        msgBox.exec();
        return;
    }
    image = image.convertToFormat(QImage::Format_ARGB32);//避免透明度问题
    path = "saved_background_image.png";
    image.save(path, "PNG");//保存为PNG格式
    setBackground(path);
}


void CutAvator::paintEvent(QPaintEvent *event)//初始化整个窗口背景
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    QPixmap pixmap(":/pictures/094 Cloudy Apple - trans.png");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
    QIcon icon(":/pictures/suliao.png");
    setWindowIcon(icon);
}


void CutAvator::setBackground(QString avapath)//设置头像
{
    QPixmap originalPixMap(avapath);
    QSize targetSize(277, 300);//子窗口大小
    QSize maxSize(277, 277);//最大图片尺寸
    QPixmap scaledPixMap = originalPixMap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap resultPixMap(targetSize);
    QPainter painter(&resultPixMap);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    painter.fillRect(resultPixMap.rect(), QColor(255, 255, 255, 255));//背景为白色
    //计算居中绘制位置
    int x = (targetSize.width() - scaledPixMap.width()) / 2;
    int y = (targetSize.height() - scaledPixMap.height()) / 2 + 11.5;
    painter.drawPixmap(x, y, scaledPixMap);
    //计算 labelRect 的位置
    if (flag > 0) {
        delete labelRect;
        labelRect = nullptr;
        flag--;
    }
    CutAvator::labelRect = new QRect(ui->lab_cut->geometry());
    flag++;
    //计算全局位置
    QPoint globalPos = ui->lab_cut->mapToGlobal(QPoint(0, 0)) + QPoint(0, 23);
    labelRect->moveTopLeft(globalPos);
    //保存 labelRect 区域的背景
    QPixmap backgroundPixmap(resultPixMap.copy(*labelRect));
    //绘制半透明层
    painter.fillRect(resultPixMap.rect(), QColor(0, 0, 0, 128));
    //在矩形区域内恢复背景
    painter.drawPixmap(labelRect->topLeft(), backgroundPixmap);
    //绘制 labelRect 边框
    painter.setPen(QColor(0, 0, 0));//黑色边框
    painter.drawRect(*labelRect);
    //设置调色板应用背景图
    QPalette palette;
    palette.setBrush(QPalette::Window, resultPixMap);
    ui->dock_avator->setAutoFillBackground(true);
    ui->dock_avator->setPalette(palette);
    ui->dock_avator->setFeatures(QDockWidget::NoDockWidgetFeatures);
}


void CutAvator::moveRect(const QPoint &newPosition)//移动明亮矩形更新头像
{
    QSize targetSize(277, 300);//子窗口大小
    QSize maxSize(277, 277);//最大图片尺寸
    QPixmap originalPixMap(path);
    QPixmap scaledPixMap = originalPixMap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap resultPixMap(targetSize);
    QPainter painter(&resultPixMap);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿
    painter.fillRect(resultPixMap.rect(), QColor(255, 255, 255, 255));//背景为白色
    //更新 labelRect 的大小以匹配 ui->lab_cut
    QSize newSize = ui->lab_cut->size();
    labelRect->setSize(newSize);
    labelRect->moveTopLeft(newPosition + QPoint(0, 23));
    //计算居中绘制位置
    int x = (targetSize.width() - scaledPixMap.width()) / 2;
    int y = (targetSize.height() - scaledPixMap.height()) / 2 + 11.5;
    painter.drawPixmap(x, y, scaledPixMap);
    //保存 labelRect 区域的背景
    QPixmap backgroundPixmap(resultPixMap.copy(*labelRect));
    //绘制半透明层
    painter.fillRect(resultPixMap.rect(), QColor(0, 0, 0, 128));
    //在矩形区域内恢复背景
    painter.drawPixmap(labelRect->topLeft(), backgroundPixmap);
    //绘制 labelRect 边框
    painter.setPen(QColor(0, 0, 0, 0));//黑色边框
    painter.drawRect(*labelRect);
    //更新调色板以使用新的背景图像
    QPalette palette;
    palette.setBrush(QPalette::Window, resultPixMap);
    ui->dock_avator->setPalette(palette);
}


void CutAvator::on_but_cut_clicked()//点击后剪裁头像
{
    ui->lab_cut->setVisible(false);//防止截到边框黑边
    //获取dock窗口的背景并拉伸
    QPixmap dockPixmap = ui->dock_avator->grab();
    QPixmap dockPixmapResize = dockPixmap.scaled(277, 300,
                                                  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //获取 lab_cut 在 dock_avator 中的位置和大小
    QRect cutrect = ui->lab_cut->geometry();//获取位置和大小
    QPoint cutPosInDock = ui->lab_cut->mapTo(ui->dock_avator, QPoint(0, 0));
    cutrect.moveTo(cutPosInDock);//移动到 dock_avator 的坐标
    //确保 cutrect 在 dockpixmap_resize 内
    cutrect.intersects(QRect(0, 0, dockPixmapResize.width(), dockPixmapResize.height()));
    //从调整大小后的 dockpixmap 中复制对应的区域
    QPixmap finalpixmap = dockPixmapResize.copy(cutrect);
    finalpixmap.save("saved_background_image.png", "PNG");
    path = "saved_background_image.png";
    moveRect(ui->lab_cut->mapToParent(QPoint(0, 0)));
    ui->lab_cut->setVisible(true);
}


void CutAvator::on_but_yes_clicked()//上传头像并关闭窗口
{
    emit cutOk(path);
    close();
}


void  DraggableResizableLabel::mousePressEvent(QMouseEvent *event)//点击判断是要干什么
{
    if (event->button() == Qt::LeftButton)
    {
        myOffset = event->pos();
        myResizing = (event->pos().x() > width() - 10 && event->pos().y() > height() - 10);
    }
}


void DraggableResizableLabel::mouseMoveEvent(QMouseEvent *event)//移动时判断是移动还是变动大小
{
    //检测鼠标是否在右下角区域
    if (event->pos().x() > width() - 10 && event->pos().y() > height() - 10)
    {
        setCursor(Qt::SizeFDiagCursor);//设置光标为调整大小
        myResizing = true;
    }
    else
    {
        setCursor(Qt::ArrowCursor);//设置光标为箭头
        myResizing = false;
    }
    if (event->buttons() & Qt::LeftButton)
    {
        if (myResizing)
        {
            //调整大小，保持正方形
            int newSize = qMax(50, qMin(event->pos().x(), event->pos().y()));
            QRect parentRect = parentWidget()->rect();
            //确保新大小不超出父窗口的尺寸
            int maxWidth = parentRect.width() - pos().x();
            int maxHeight = parentRect.height() - pos().y();
            newSize = qMin(newSize, qMin(maxWidth, maxHeight));
            setFixedSize(newSize, newSize);
            emit labelMoved(mapToParent(QPoint(0, 0)));//发送当前标签的位置
        }
        else
        {
            //移动标签
            QPoint newPosition = mapToParent(event->pos() - myOffset);
            //限制移动范围，确保不超出父窗口
            QRect parentRect = parentWidget()->rect();
            QSize labelSize = size();
            newPosition.setX(qMax(parentRect.left(), qMin(newPosition.x(), parentRect.right() - labelSize.width())));
            newPosition.setY(qMax(parentRect.top(), qMin(newPosition.y(), parentRect.bottom() - labelSize.height())));

            move(newPosition);
            emit labelMoved(newPosition);//发送新位置
        }
    }
}


void DraggableResizableLabel::mouseReleaseEvent(QMouseEvent *event)//释放重置状态
{
    Q_UNUSED(event);
    myResizing = false;//重置状态
    unsetCursor();//恢复光标
}

