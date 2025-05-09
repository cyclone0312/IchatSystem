/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mainwindow-else.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_5;
    LabelAva *lab_avator;
    QSpacerItem *verticalSpacer;
    QPushButton *but_chat;
    QPushButton *but_friends;
    QSpacerItem *verticalSpacer_4;
    QPushButton *but_set;
    QSpacerItem *verticalSpacer_3;
    QStackedWidget *pages;
    QWidget *page_talk;
    QPushButton *but_send;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_friendname;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_minwindow;
    QPushButton *but_maxwindow;
    QPushButton *but_deletewindow;
    TalkStacked *stack_talks;
    QWidget *page0;
    QGridLayout *gridLayout_3;
    QWidget *page_;
    EnterTextEdit *edit_input;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *but_tool1;
    QPushButton *but_tool2;
    QPushButton *but_tool3;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *but_fangdajing;
    LineSerach *line_serach;
    QPushButton *but_add0;
    TalkList *list_talks;
    QWidget *page_friends;
    QGridLayout *gridLayout_4;
    CustomTextEdit *edit_show2;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *but_fangdajing2;
    LineSerach *line_serach2;
    QPushButton *but_addfriends;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *but_minwindow2;
    QPushButton *but_maxwindow2;
    QPushButton *but_deletewindow2;
    FriendList *list_friends;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(934, 650);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(900, 650));
        MainWindow->setMouseTracking(false);
        MainWindow->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        MainWindow->setStyleSheet(QString::fromUtf8("border-radius: 10px;\n"
"border: 0.5px solid rgb(0, 0, 0);\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_5 = new QSpacerItem(19, 9, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_5);

        lab_avator = new LabelAva(centralwidget);
        lab_avator->setObjectName("lab_avator");
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_avator->sizePolicy().hasHeightForWidth());
        lab_avator->setSizePolicy(sizePolicy1);
        lab_avator->setMinimumSize(QSize(40, 40));
        lab_avator->setMaximumSize(QSize(40, 40));
        lab_avator->setCursor(QCursor(Qt::PointingHandCursor));
        lab_avator->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border: 2px solid black;\n"
"border-radius: 3px;\n"
""));

        verticalLayout->addWidget(lab_avator, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        verticalSpacer = new QSpacerItem(18, 50, QSizePolicy::Policy::Preferred, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);

        but_chat = new QPushButton(centralwidget);
        but_chat->setObjectName("but_chat");
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(but_chat->sizePolicy().hasHeightForWidth());
        but_chat->setSizePolicy(sizePolicy2);
        but_chat->setMinimumSize(QSize(40, 30));
        but_chat->setMaximumSize(QSize(40, 30));
        but_chat->setCursor(QCursor(Qt::PointingHandCursor));
        but_chat->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" \n"
"    border: none;            \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: rgba(0, 0, 0, 0.2);  \n"
"    border-radius: 10px;\n"
"}"));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::MailSend")));
        but_chat->setIcon(icon);
        but_chat->setIconSize(QSize(18, 18));

        verticalLayout->addWidget(but_chat, 0, Qt::AlignmentFlag::AlignHCenter);

        but_friends = new QPushButton(centralwidget);
        but_friends->setObjectName("but_friends");
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(but_friends->sizePolicy().hasHeightForWidth());
        but_friends->setSizePolicy(sizePolicy3);
        but_friends->setMinimumSize(QSize(40, 30));
        but_friends->setMaximumSize(QSize(40, 30));
        but_friends->setCursor(QCursor(Qt::PointingHandCursor));
        but_friends->setMouseTracking(true);
        but_friends->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: transparent; \n"
"    border: none;            \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: rgba(0, 0, 0, 0.2);  \n"
"    border-radius: 10px;\n"
"}"));
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::CameraWeb")));
        but_friends->setIcon(icon1);
        but_friends->setIconSize(QSize(18, 18));

        verticalLayout->addWidget(but_friends, 0, Qt::AlignmentFlag::AlignHCenter);

        verticalSpacer_4 = new QSpacerItem(20, 63, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_4);

        but_set = new QPushButton(centralwidget);
        but_set->setObjectName("but_set");
        sizePolicy1.setHeightForWidth(but_set->sizePolicy().hasHeightForWidth());
        but_set->setSizePolicy(sizePolicy1);
        but_set->setMinimumSize(QSize(40, 30));
        but_set->setMaximumSize(QSize(40, 30));
        but_set->setCursor(QCursor(Qt::PointingHandCursor));
        but_set->setMouseTracking(true);
        but_set->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: transparent; \n"
"    border: none;            \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: rgba(0, 0, 0, 0.2);  \n"
"    border-radius: 10px;\n"
"}"));
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::DocumentProperties")));
        but_set->setIcon(icon2);
        but_set->setIconSize(QSize(18, 18));

        verticalLayout->addWidget(but_set, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignBottom);

        verticalSpacer_3 = new QSpacerItem(20, 16, QSizePolicy::Policy::Maximum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_3);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        pages = new QStackedWidget(centralwidget);
        pages->setObjectName("pages");
        pages->setStyleSheet(QString::fromUtf8("border: transparent;"));
        page_talk = new QWidget();
        page_talk->setObjectName("page_talk");
        but_send = new QPushButton(page_talk);
        but_send->setObjectName("but_send");
        but_send->setGeometry(QRect(740, 462, 70, 23));
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(but_send->sizePolicy().hasHeightForWidth());
        but_send->setSizePolicy(sizePolicy4);
        but_send->setCursor(QCursor(Qt::ArrowCursor));
        but_send->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 11pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;; \n"
"   border-radius: 5px;\n"
"}\n"
""));
        gridLayout = new QGridLayout(page_talk);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lab_friendname = new QLabel(page_talk);
        lab_friendname->setObjectName("lab_friendname");
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lab_friendname->sizePolicy().hasHeightForWidth());
        lab_friendname->setSizePolicy(sizePolicy5);
        lab_friendname->setCursor(QCursor(Qt::ArrowCursor));
        lab_friendname->setStyleSheet(QString::fromUtf8("font: 520 11pt \"Microsoft YaHei UI\";\n"
"border: transparent;"));

        horizontalLayout_4->addWidget(lab_friendname);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(500, 25, QSizePolicy::Policy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        but_minwindow = new QPushButton(page_talk);
        but_minwindow->setObjectName("but_minwindow");
        sizePolicy5.setHeightForWidth(but_minwindow->sizePolicy().hasHeightForWidth());
        but_minwindow->setSizePolicy(sizePolicy5);
        but_minwindow->setMinimumSize(QSize(20, 20));
        but_minwindow->setMaximumSize(QSize(20, 20));
        but_minwindow->setCursor(QCursor(Qt::PointingHandCursor));
        but_minwindow->setMouseTracking(true);
        but_minwindow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    background: grey;\n"
"    border-radius: 3px;\n"
"}"));
        QIcon icon3(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::ListRemove")));
        but_minwindow->setIcon(icon3);
        but_minwindow->setIconSize(QSize(12, 10));

        horizontalLayout->addWidget(but_minwindow, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        but_maxwindow = new QPushButton(page_talk);
        but_maxwindow->setObjectName("but_maxwindow");
        sizePolicy5.setHeightForWidth(but_maxwindow->sizePolicy().hasHeightForWidth());
        but_maxwindow->setSizePolicy(sizePolicy5);
        but_maxwindow->setMinimumSize(QSize(20, 20));
        but_maxwindow->setMaximumSize(QSize(20, 20));
        but_maxwindow->setCursor(QCursor(Qt::PointingHandCursor));
        but_maxwindow->setMouseTracking(true);
        but_maxwindow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    background: grey;\n"
"    border-radius: 3px;\n"
"}"));
        QIcon icon4(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::MediaPlaybackStop")));
        but_maxwindow->setIcon(icon4);
        but_maxwindow->setIconSize(QSize(12, 12));

        horizontalLayout->addWidget(but_maxwindow, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        but_deletewindow = new QPushButton(page_talk);
        but_deletewindow->setObjectName("but_deletewindow");
        sizePolicy5.setHeightForWidth(but_deletewindow->sizePolicy().hasHeightForWidth());
        but_deletewindow->setSizePolicy(sizePolicy5);
        but_deletewindow->setMinimumSize(QSize(20, 20));
        but_deletewindow->setMaximumSize(QSize(20, 20));
        but_deletewindow->setCursor(QCursor(Qt::PointingHandCursor));
        but_deletewindow->setMouseTracking(true);
        but_deletewindow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"    font:  16pt 'Microsoft New Tai Lue';\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    font:  16pt 'Microsoft New Tai Lue';\n"
"    background: red;\n"
"    border-radius: 3px;\n"
"}"));
        but_deletewindow->setIconSize(QSize(15, 15));

        horizontalLayout->addWidget(but_deletewindow, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_4->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        stack_talks = new TalkStacked(page_talk);
        stack_talks->setObjectName("stack_talks");
        sizePolicy.setHeightForWidth(stack_talks->sizePolicy().hasHeightForWidth());
        stack_talks->setSizePolicy(sizePolicy);
        stack_talks->setMinimumSize(QSize(0, 314));
        stack_talks->setStyleSheet(QString::fromUtf8("QStackedWidget {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    border: 0.5px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 10px;\n"
"    background: white;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"    width: 8px;                /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"    background: #f0f0f0;      /* \346\273\232\345\212\250\346\235\241\350\203\214\346\231\257 */\n"
"    border-radius: 8px;       /* \346\233\264\346\230\216\346\230\276\347\232\204\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #888;          /* \346\273\232\345\212\250\346\235\241\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"    min-height: 40px;          /* \346\273\221\345\235\227\346\234\200\345\260\217\351\253\230\345\272\246 */\n"
"    border-radius: 10px;       /* \345\242\236\345\244\247\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #555;          /* \346\273\221\345\235\227"
                        "\346\202\254\345\201\234\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    background: none;          /* \344\270\212\344\270\213\347\256\255\345\244\264\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"    background: none;          /* \351\241\265\351\235\242\347\232\204\351\242\234\350\211\262 */\n"
"}"));
        page0 = new QWidget();
        page0->setObjectName("page0");
        gridLayout_3 = new QGridLayout(page0);
        gridLayout_3->setObjectName("gridLayout_3");
        stack_talks->addWidget(page0);
        page_ = new QWidget();
        page_->setObjectName("page_");
        stack_talks->addWidget(page_);

        gridLayout->addWidget(stack_talks, 1, 1, 1, 1);

        edit_input = new EnterTextEdit(page_talk);
        edit_input->setObjectName("edit_input");
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(10);
        sizePolicy6.setHeightForWidth(edit_input->sizePolicy().hasHeightForWidth());
        edit_input->setSizePolicy(sizePolicy6);
        edit_input->setMaximumSize(QSize(16777215, 160));
        edit_input->setMouseTracking(false);
        edit_input->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        edit_input->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    border: 0.5px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 10px;\n"
"    background: white;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"    width: 7px;                /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"    background: #f0f0f0;      /* \346\273\232\345\212\250\346\235\241\350\203\214\346\231\257 */\n"
"    border-radius: 8px;       /* \346\233\264\346\230\216\346\230\276\347\232\204\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #888;          /* \346\273\232\345\212\250\346\235\241\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"    min-height: 40px;          /* \346\273\221\345\235\227\346\234\200\345\260\217\351\253\230\345\272\246 */\n"
"    border-radius: 10px;       /* \345\242\236\345\244\247\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #555;          /* \346\273\221\345\235\227\346"
                        "\202\254\345\201\234\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    background: none;          /* \344\270\212\344\270\213\347\256\255\345\244\264\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"    background: none;          /* \351\241\265\351\235\242\347\232\204\351\242\234\350\211\262 */\n"
"}"));

        gridLayout->addWidget(edit_input, 3, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        but_tool1 = new QPushButton(page_talk);
        but_tool1->setObjectName("but_tool1");
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(but_tool1->sizePolicy().hasHeightForWidth());
        but_tool1->setSizePolicy(sizePolicy7);
        but_tool1->setMinimumSize(QSize(80, 30));
        but_tool1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    color: black;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(0, 0, 0, 0.1); \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 0, 0, 0.2); \n"
"}"));
        QIcon icon5(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::ZoomFitBest")));
        but_tool1->setIcon(icon5);

        horizontalLayout_2->addWidget(but_tool1, 0, Qt::AlignmentFlag::AlignVCenter);

        but_tool2 = new QPushButton(page_talk);
        but_tool2->setObjectName("but_tool2");
        sizePolicy5.setHeightForWidth(but_tool2->sizePolicy().hasHeightForWidth());
        but_tool2->setSizePolicy(sizePolicy5);
        but_tool2->setMinimumSize(QSize(80, 30));
        but_tool2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    color: black;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(0, 0, 0, 0.1); \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 0, 0, 0.2); \n"
"}"));
        QIcon icon6(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::DocumentOpen")));
        but_tool2->setIcon(icon6);

        horizontalLayout_2->addWidget(but_tool2, 0, Qt::AlignmentFlag::AlignVCenter);

        but_tool3 = new QPushButton(page_talk);
        but_tool3->setObjectName("but_tool3");
        sizePolicy5.setHeightForWidth(but_tool3->sizePolicy().hasHeightForWidth());
        but_tool3->setSizePolicy(sizePolicy5);
        but_tool3->setMinimumSize(QSize(80, 30));
        but_tool3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    color: black;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(0, 0, 0, 0.1); \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 0, 0, 0.2); \n"
"}"));
        QIcon icon7(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::CallStart")));
        but_tool3->setIcon(icon7);

        horizontalLayout_2->addWidget(but_tool3, 0, Qt::AlignmentFlag::AlignVCenter);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        but_fangdajing = new QPushButton(page_talk);
        but_fangdajing->setObjectName("but_fangdajing");
        sizePolicy5.setHeightForWidth(but_fangdajing->sizePolicy().hasHeightForWidth());
        but_fangdajing->setSizePolicy(sizePolicy5);
        but_fangdajing->setStyleSheet(QString::fromUtf8("background:transparent;\n"
" border: none;\n"
""));
        QIcon icon8(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditFind")));
        but_fangdajing->setIcon(icon8);

        horizontalLayout_17->addWidget(but_fangdajing, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignVCenter);

        line_serach = new LineSerach(page_talk);
        line_serach->setObjectName("line_serach");
        sizePolicy5.setHeightForWidth(line_serach->sizePolicy().hasHeightForWidth());
        line_serach->setSizePolicy(sizePolicy5);
        line_serach->setMinimumSize(QSize(140, 26));
        line_serach->setMaximumSize(QSize(140, 26));
        line_serach->setMouseTracking(false);
        line_serach->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_serach->setStyleSheet(QString::fromUtf8("font: 10pt \"Microsoft YaHei UI\";\n"
"border-radius: 3px;\n"
"background:rgb(245, 245, 245);\n"
"color:grey;\n"
"padding: 5px;\n"
""));
        line_serach->setMaxLength(20);

        horizontalLayout_17->addWidget(line_serach, 0, Qt::AlignmentFlag::AlignLeft);

        but_add0 = new QPushButton(page_talk);
        but_add0->setObjectName("but_add0");
        sizePolicy5.setHeightForWidth(but_add0->sizePolicy().hasHeightForWidth());
        but_add0->setSizePolicy(sizePolicy5);
        but_add0->setMinimumSize(QSize(0, 40));
        but_add0->setCursor(QCursor(Qt::PointingHandCursor));
        but_add0->setStyleSheet(QString::fromUtf8("background:transparent;\n"
" border: none;\n"
""));
        QIcon icon9(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::ContactNew")));
        but_add0->setIcon(icon9);

        horizontalLayout_17->addWidget(but_add0, 0, Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);


        gridLayout->addLayout(horizontalLayout_17, 0, 0, 1, 1);

        list_talks = new TalkList(page_talk);
        list_talks->setObjectName("list_talks");
        QSizePolicy sizePolicy8(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(185);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(list_talks->sizePolicy().hasHeightForWidth());
        list_talks->setSizePolicy(sizePolicy8);
        list_talks->setMinimumSize(QSize(185, 0));
        list_talks->setMaximumSize(QSize(185, 16777215));
        list_talks->setMouseTracking(true);
        list_talks->setStyleSheet(QString::fromUtf8("QListView {\n"
"font: 450 11pt \"Microsoft YaHei UI Light\";\n"
"border: 0.5px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 10px;\n"
"background: rgb(255, 248, 253);\n"
"padding: 5px;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"width: 7px; /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"background: #f0f0f0; /* \346\273\232\345\212\250\346\235\241\350\203\214\346\231\257 */\n"
"border-radius: 8px; /* \346\233\264\346\230\216\346\230\276\347\232\204\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"background: #888; /* \346\273\232\345\212\250\346\235\241\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"min-height: 40px; /* \346\273\221\345\235\227\346\234\200\345\260\217\351\253\230\345\272\246 */\n"
"border-radius: 10px; /* \345\242\236\345\244\247\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"background: #555; /* \346\273\221\345\235\227\346\202\254\345\201\234\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line"
                        ":vertical, QScrollBar::sub-line:vertical {\n"
"background: none; /* \344\270\212\344\270\213\347\256\255\345\244\264\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"background: none; /* \351\241\265\351\235\242\347\232\204\351\242\234\350\211\262 */\n"
"}"));
        list_talks->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

        gridLayout->addWidget(list_talks, 1, 0, 3, 1);

        pages->addWidget(page_talk);
        edit_input->raise();
        list_talks->raise();
        but_send->raise();
        stack_talks->raise();
        page_friends = new QWidget();
        page_friends->setObjectName("page_friends");
        gridLayout_4 = new QGridLayout(page_friends);
        gridLayout_4->setObjectName("gridLayout_4");
        edit_show2 = new CustomTextEdit(page_friends);
        edit_show2->setObjectName("edit_show2");
        edit_show2->setEnabled(true);
        sizePolicy.setHeightForWidth(edit_show2->sizePolicy().hasHeightForWidth());
        edit_show2->setSizePolicy(sizePolicy);
        edit_show2->setMouseTracking(false);
        edit_show2->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
        edit_show2->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    border: 0.5px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 10px;\n"
"    background: white;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"    width: 7px;                /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"    background: #f0f0f0;      /* \346\273\232\345\212\250\346\235\241\350\203\214\346\231\257 */\n"
"    border-radius: 8px;       /* \346\233\264\346\230\216\346\230\276\347\232\204\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #888;          /* \346\273\232\345\212\250\346\235\241\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"    min-height: 40px;          /* \346\273\221\345\235\227\346\234\200\345\260\217\351\253\230\345\272\246 */\n"
"    border-radius: 10px;       /* \345\242\236\345\244\247\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #555;          /* \346\273\221\345\235\227\346"
                        "\202\254\345\201\234\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    background: none;          /* \344\270\212\344\270\213\347\256\255\345\244\264\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"    background: none;          /* \351\241\265\351\235\242\347\232\204\351\242\234\350\211\262 */\n"
"}"));

        gridLayout_4->addWidget(edit_show2, 1, 1, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        but_fangdajing2 = new QPushButton(page_friends);
        but_fangdajing2->setObjectName("but_fangdajing2");
        sizePolicy5.setHeightForWidth(but_fangdajing2->sizePolicy().hasHeightForWidth());
        but_fangdajing2->setSizePolicy(sizePolicy5);
        but_fangdajing2->setMinimumSize(QSize(0, 0));
        but_fangdajing2->setStyleSheet(QString::fromUtf8("background:transparent;\n"
" border: none;\n"
""));
        but_fangdajing2->setIcon(icon8);

        horizontalLayout_13->addWidget(but_fangdajing2, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignVCenter);

        line_serach2 = new LineSerach(page_friends);
        line_serach2->setObjectName("line_serach2");
        sizePolicy5.setHeightForWidth(line_serach2->sizePolicy().hasHeightForWidth());
        line_serach2->setSizePolicy(sizePolicy5);
        line_serach2->setMinimumSize(QSize(140, 26));
        line_serach2->setMaximumSize(QSize(140, 26));
        line_serach2->setMouseTracking(false);
        line_serach2->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_serach2->setStyleSheet(QString::fromUtf8("font: 10pt \"Microsoft YaHei UI\";\n"
"border-radius: 3px;\n"
"background:rgb(245, 245, 245);\n"
"color:grey;\n"
"padding: 5px;\n"
""));
        line_serach2->setMaxLength(20);

        horizontalLayout_13->addWidget(line_serach2, 0, Qt::AlignmentFlag::AlignLeft);

        but_addfriends = new QPushButton(page_friends);
        but_addfriends->setObjectName("but_addfriends");
        sizePolicy5.setHeightForWidth(but_addfriends->sizePolicy().hasHeightForWidth());
        but_addfriends->setSizePolicy(sizePolicy5);
        but_addfriends->setMinimumSize(QSize(0, 40));
        but_addfriends->setCursor(QCursor(Qt::PointingHandCursor));
        but_addfriends->setStyleSheet(QString::fromUtf8("background:transparent;\n"
" border: none;\n"
""));
        but_addfriends->setIcon(icon9);

        horizontalLayout_13->addWidget(but_addfriends, 0, Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);


        gridLayout_4->addLayout(horizontalLayout_13, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer_3 = new QSpacerItem(600, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        but_minwindow2 = new QPushButton(page_friends);
        but_minwindow2->setObjectName("but_minwindow2");
        QSizePolicy sizePolicy9(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(15);
        sizePolicy9.setHeightForWidth(but_minwindow2->sizePolicy().hasHeightForWidth());
        but_minwindow2->setSizePolicy(sizePolicy9);
        but_minwindow2->setMinimumSize(QSize(20, 20));
        but_minwindow2->setMaximumSize(QSize(20, 20));
        but_minwindow2->setCursor(QCursor(Qt::PointingHandCursor));
        but_minwindow2->setMouseTracking(true);
        but_minwindow2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    background: grey;\n"
"    border-radius: 3px;\n"
"}"));
        but_minwindow2->setIcon(icon3);
        but_minwindow2->setIconSize(QSize(12, 10));

        horizontalLayout_6->addWidget(but_minwindow2, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        but_maxwindow2 = new QPushButton(page_friends);
        but_maxwindow2->setObjectName("but_maxwindow2");
        sizePolicy9.setHeightForWidth(but_maxwindow2->sizePolicy().hasHeightForWidth());
        but_maxwindow2->setSizePolicy(sizePolicy9);
        but_maxwindow2->setMinimumSize(QSize(20, 20));
        but_maxwindow2->setMaximumSize(QSize(20, 20));
        but_maxwindow2->setCursor(QCursor(Qt::PointingHandCursor));
        but_maxwindow2->setMouseTracking(true);
        but_maxwindow2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    background: grey;\n"
"    border-radius: 3px;\n"
"}"));
        but_maxwindow2->setIcon(icon4);
        but_maxwindow2->setIconSize(QSize(12, 12));

        horizontalLayout_6->addWidget(but_maxwindow2, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        but_deletewindow2 = new QPushButton(page_friends);
        but_deletewindow2->setObjectName("but_deletewindow2");
        sizePolicy9.setHeightForWidth(but_deletewindow2->sizePolicy().hasHeightForWidth());
        but_deletewindow2->setSizePolicy(sizePolicy9);
        but_deletewindow2->setMinimumSize(QSize(20, 20));
        but_deletewindow2->setMaximumSize(QSize(20, 20));
        but_deletewindow2->setCursor(QCursor(Qt::PointingHandCursor));
        but_deletewindow2->setMouseTracking(true);
        but_deletewindow2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: rgb(78, 78, 78);\n"
"    font:  16pt 'Microsoft New Tai Lue';\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: white;\n"
"    font:  16pt 'Microsoft New Tai Lue';\n"
"    background: red;\n"
"    border-radius: 3px;\n"
"}"));
        but_deletewindow2->setIconSize(QSize(15, 15));

        horizontalLayout_6->addWidget(but_deletewindow2, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);


        verticalLayout_4->addLayout(horizontalLayout_6);


        horizontalLayout_5->addLayout(verticalLayout_4);


        gridLayout_4->addLayout(horizontalLayout_5, 0, 1, 1, 1);

        list_friends = new FriendList(page_friends);
        list_friends->setObjectName("list_friends");
        sizePolicy8.setHeightForWidth(list_friends->sizePolicy().hasHeightForWidth());
        list_friends->setSizePolicy(sizePolicy8);
        list_friends->setMinimumSize(QSize(185, 0));
        list_friends->setMaximumSize(QSize(150, 16777215));
        list_friends->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        list_friends->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        list_friends->setStyleSheet(QString::fromUtf8("QListView {\n"
"font: 420 11pt \"Microsoft YaHei UI Light\";\n"
"border: 0.5px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 10px;\n"
"background: rgb(255, 248, 253);\n"
"padding: 5px;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"width: 7px; /* \346\273\232\345\212\250\346\235\241\345\256\275\345\272\246 */\n"
"background: #f0f0f0; /* \346\273\232\345\212\250\346\235\241\350\203\214\346\231\257 */\n"
"border-radius: 8px; /* \346\233\264\346\230\216\346\230\276\347\232\204\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"background: #888; /* \346\273\232\345\212\250\346\235\241\346\273\221\345\235\227\351\242\234\350\211\262 */\n"
"min-height: 40px; /* \346\273\221\345\235\227\346\234\200\345\260\217\351\253\230\345\272\246 */\n"
"border-radius: 10px; /* \345\242\236\345\244\247\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"background: #555; /* \346\273\221\345\235\227\346\202\254\345\201\234\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-line"
                        ":vertical, QScrollBar::sub-line:vertical {\n"
"background: none; /* \344\270\212\344\270\213\347\256\255\345\244\264\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"background: none; /* \351\241\265\351\235\242\347\232\204\351\242\234\350\211\262 */\n"
"}"));
        list_friends->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        list_friends->setDragEnabled(false);
        list_friends->setDragDropMode(QAbstractItemView::DragDropMode::DropOnly);
        list_friends->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        list_friends->setIconSize(QSize(100, 100));
        list_friends->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
        list_friends->setMovement(QListView::Movement::Static);
        list_friends->setResizeMode(QListView::ResizeMode::Adjust);
        list_friends->setViewMode(QListView::ViewMode::ListMode);
        list_friends->setUniformItemSizes(false);
        list_friends->setBatchSize(1);
        list_friends->setSelectionRectVisible(false);

        gridLayout_4->addWidget(list_friends, 1, 0, 1, 1);

        pages->addWidget(page_friends);

        gridLayout_2->addWidget(pages, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        pages->setCurrentIndex(0);
        stack_talks->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\351\200\237\350\201\212-\345\260\221\346\225\260\344\272\272\347\232\204\350\201\212\345\244\251\350\275\257\344\273\266", nullptr));
        lab_avator->setText(QString());
        but_chat->setText(QString());
        but_friends->setText(QString());
        but_set->setText(QString());
        but_send->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        lab_friendname->setText(QCoreApplication::translate("MainWindow", "  \350\201\224\347\263\273\344\272\272", nullptr));
        but_minwindow->setText(QString());
        but_maxwindow->setText(QString());
        but_deletewindow->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        but_tool1->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\345\233\276\347\211\207", nullptr));
        but_tool2->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        but_tool3->setText(QCoreApplication::translate("MainWindow", "\350\257\255\351\237\263\350\201\212\345\244\251", nullptr));
        but_fangdajing->setText(QString());
        line_serach->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242", nullptr));
        but_add0->setText(QString());
        but_fangdajing2->setText(QString());
        line_serach2->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242", nullptr));
        but_addfriends->setText(QString());
        but_minwindow2->setText(QString());
        but_maxwindow2->setText(QString());
        but_deletewindow2->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
