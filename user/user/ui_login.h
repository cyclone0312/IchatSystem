/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <login.h>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_tubiao;
    QLabel *label;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *but_deletewindow;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lab_avator;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *rgs_pbt;
    QVBoxLayout *verticalLayout;
    LineInput *line_qqnum;
    LineInput *line_password;
    QHBoxLayout *horizontalLayout;
    QCheckBox *ckbox_remeber;
    QCheckBox *auto_ckbox;
    QPushButton *fgt_pbt;
    QPushButton *gologin;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menubar;
    QMenu *menuQQ;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->setWindowModality(Qt::WindowModality::NonModal);
        Login->resize(500, 320);
        Login->setMinimumSize(QSize(500, 320));
        Login->setMaximumSize(QSize(500, 320));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::CameraWeb")));
        Login->setWindowIcon(icon);
        Login->setWindowOpacity(0.995000000000000);
        Login->setStyleSheet(QString::fromUtf8(" border: none;\n"
"\n"
""));
        centralwidget = new QWidget(Login);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        lab_tubiao = new QLabel(centralwidget);
        lab_tubiao->setObjectName("lab_tubiao");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_tubiao->sizePolicy().hasHeightForWidth());
        lab_tubiao->setSizePolicy(sizePolicy);
        lab_tubiao->setMinimumSize(QSize(30, 30));
        lab_tubiao->setMaximumSize(QSize(30, 30));

        horizontalLayout_3->addWidget(lab_tubiao, 0, Qt::AlignmentFlag::AlignLeft);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("qproperty-alignment: 'AlignLeft';\n"
"font: 10pt ;\n"
""));

        horizontalLayout_3->addWidget(label, 0, Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalSpacer_5 = new QSpacerItem(330, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 2);

        but_deletewindow = new QPushButton(centralwidget);
        but_deletewindow->setObjectName("but_deletewindow");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(15);
        sizePolicy1.setHeightForWidth(but_deletewindow->sizePolicy().hasHeightForWidth());
        but_deletewindow->setSizePolicy(sizePolicy1);
        but_deletewindow->setMinimumSize(QSize(20, 20));
        but_deletewindow->setMaximumSize(QSize(20, 20));
        but_deletewindow->setCursor(QCursor(Qt::PointingHandCursor));
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

        gridLayout->addWidget(but_deletewindow, 0, 2, 1, 1, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lab_avator = new QLabel(centralwidget);
        lab_avator->setObjectName("lab_avator");
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lab_avator->sizePolicy().hasHeightForWidth());
        lab_avator->setSizePolicy(sizePolicy2);
        lab_avator->setMinimumSize(QSize(100, 100));
        lab_avator->setMaximumSize(QSize(100, 100));
        lab_avator->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
"border-radius: 8px;"));
        lab_avator->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(lab_avator, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalSpacer = new QSpacerItem(92, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);

        rgs_pbt = new QPushButton(centralwidget);
        rgs_pbt->setObjectName("rgs_pbt");
        rgs_pbt->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"       font: 10pt \"Microsoft YaHei UI\";\n"
"       border: none;\n"
"       text-decoration: none; \n"
"       color: gray;\n"
"   }\n"
"\n"
"   QPushButton:hover {\n"
"       text-decoration: underline; \n"
"       color: gray;\n"
"   }"));

        verticalLayout_2->addWidget(rgs_pbt, 0, Qt::AlignmentFlag::AlignBottom);


        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        line_qqnum = new LineInput(centralwidget);
        line_qqnum->setObjectName("line_qqnum");
        sizePolicy.setHeightForWidth(line_qqnum->sizePolicy().hasHeightForWidth());
        line_qqnum->setSizePolicy(sizePolicy);
        line_qqnum->setMinimumSize(QSize(289, 37));
        line_qqnum->setMaximumSize(QSize(289, 37));
        line_qqnum->setCursor(QCursor(Qt::IBeamCursor));
        line_qqnum->setTabletTracking(false);
        line_qqnum->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_qqnum->setAcceptDrops(true);
        line_qqnum->setAutoFillBackground(false);
        line_qqnum->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 10px;\n"
"color:grey;\n"
""));
        line_qqnum->setInputMethodHints(Qt::InputMethodHint::ImhNone);
        line_qqnum->setMaxLength(12);
        line_qqnum->setFrame(true);
        line_qqnum->setEchoMode(QLineEdit::EchoMode::Normal);
        line_qqnum->setCursorPosition(7);
        line_qqnum->setAlignment(Qt::AlignmentFlag::AlignCenter);
        line_qqnum->setCursorMoveStyle(Qt::CursorMoveStyle::VisualMoveStyle);
        line_qqnum->setClearButtonEnabled(false);

        verticalLayout->addWidget(line_qqnum);

        line_password = new LineInput(centralwidget);
        line_password->setObjectName("line_password");
        sizePolicy.setHeightForWidth(line_password->sizePolicy().hasHeightForWidth());
        line_password->setSizePolicy(sizePolicy);
        line_password->setMinimumSize(QSize(289, 37));
        line_password->setMaximumSize(QSize(289, 37));
        line_password->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_password->setAcceptDrops(true);
        line_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 10px;\n"
"color:grey;\n"
""));
        line_password->setMaxLength(12);
        line_password->setFrame(true);
        line_password->setEchoMode(QLineEdit::EchoMode::Normal);
        line_password->setAlignment(Qt::AlignmentFlag::AlignCenter);
        line_password->setCursorMoveStyle(Qt::CursorMoveStyle::VisualMoveStyle);
        line_password->setClearButtonEnabled(false);

        verticalLayout->addWidget(line_password);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        ckbox_remeber = new QCheckBox(centralwidget);
        ckbox_remeber->setObjectName("ckbox_remeber");
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(50);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ckbox_remeber->sizePolicy().hasHeightForWidth());
        ckbox_remeber->setSizePolicy(sizePolicy3);
        ckbox_remeber->setStyleSheet(QString::fromUtf8(" color: gray;"));

        horizontalLayout->addWidget(ckbox_remeber);

        auto_ckbox = new QCheckBox(centralwidget);
        auto_ckbox->setObjectName("auto_ckbox");
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(auto_ckbox->sizePolicy().hasHeightForWidth());
        auto_ckbox->setSizePolicy(sizePolicy4);
        auto_ckbox->setStyleSheet(QString::fromUtf8(" color: gray;"));

        horizontalLayout->addWidget(auto_ckbox);

        fgt_pbt = new QPushButton(centralwidget);
        fgt_pbt->setObjectName("fgt_pbt");
        sizePolicy3.setHeightForWidth(fgt_pbt->sizePolicy().hasHeightForWidth());
        fgt_pbt->setSizePolicy(sizePolicy3);
        fgt_pbt->setStyleSheet(QString::fromUtf8(" QPushButton {\n"
"       font: 10pt \"Microsoft YaHei UI\";\n"
"       border: none;\n"
"       text-decoration: none; \n"
"      color: gray;\n"
"   }\n"
"\n"
"   QPushButton:hover {\n"
"       text-decoration: underline; \n"
"       color: gray;\n"
"   }"));

        horizontalLayout->addWidget(fgt_pbt);


        verticalLayout->addLayout(horizontalLayout);

        gologin = new QPushButton(centralwidget);
        gologin->setObjectName("gologin");
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(28);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(gologin->sizePolicy().hasHeightForWidth());
        gologin->setSizePolicy(sizePolicy5);
        gologin->setMinimumSize(QSize(289, 37));
        gologin->setMaximumSize(QSize(289, 37));
        gologin->setCursor(QCursor(Qt::PointingHandCursor));
        gologin->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;; \n"
"    border-radius: 15px;\n"
"}\n"
""));

        verticalLayout->addWidget(gologin);


        gridLayout->addLayout(verticalLayout, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(91, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        Login->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Login);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 500, 18));
        menuQQ = new QMenu(menubar);
        menuQQ->setObjectName("menuQQ");
        Login->setMenuBar(menubar);
        statusbar = new QStatusBar(Login);
        statusbar->setObjectName("statusbar");
        Login->setStatusBar(statusbar);

        menubar->addAction(menuQQ->menuAction());

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QMainWindow *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "\351\200\237\350\201\212-\345\260\221\346\225\260\344\272\272\347\232\204\347\244\276\344\272\244\350\275\257\344\273\266", nullptr));
        lab_tubiao->setText(QString());
        label->setText(QCoreApplication::translate("Login", "\351\200\237\350\201\212", nullptr));
        but_deletewindow->setText(QCoreApplication::translate("Login", "x", nullptr));
        lab_avator->setText(QString());
        rgs_pbt->setText(QCoreApplication::translate("Login", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        line_qqnum->setInputMask(QString());
        line_qqnum->setText(QCoreApplication::translate("Login", "\350\257\267\350\276\223\345\205\245\344\275\240\347\232\204\350\264\246\345\217\267", nullptr));
        line_qqnum->setPlaceholderText(QString());
        line_password->setText(QCoreApplication::translate("Login", "\350\257\267\350\276\223\345\205\245\344\275\240\347\232\204\345\257\206\347\240\201", nullptr));
        line_password->setPlaceholderText(QString());
        ckbox_remeber->setText(QCoreApplication::translate("Login", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        auto_ckbox->setText(QCoreApplication::translate("Login", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        fgt_pbt->setText(QCoreApplication::translate("Login", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        gologin->setText(QCoreApplication::translate("Login", "\345\256\211\345\205\250\347\231\273\345\275\225", nullptr));
        menuQQ->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
