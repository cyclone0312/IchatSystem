/********************************************************************************
** Form generated from reading UI file 'findpassword.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDPASSWORD_H
#define UI_FINDPASSWORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <findpassword.h>

QT_BEGIN_NAMESPACE

class Ui_FindPassword
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QStackedWidget *page_find;
    QWidget *pages_1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_5;
    QLabel *lab_pages1;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    LineFind *line_pages1;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_pages1;
    QWidget *pages_2;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_21;
    QLabel *lab_pages2;
    QSpacerItem *horizontalSpacer_22;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_23;
    LineFind *line_pages2;
    QSpacerItem *horizontalSpacer_24;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_25;
    QPushButton *but_pages2;
    QWidget *pages_3;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer_26;
    LineFind *line_pages3_2;
    QSpacerItem *horizontalSpacer_27;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer_28;
    LineFind *line_pages3;
    QSpacerItem *horizontalSpacer_29;
    QHBoxLayout *horizontalLayout_18;
    QSpacerItem *horizontalSpacer_30;
    QPushButton *but_pages3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FindPassword)
    {
        if (FindPassword->objectName().isEmpty())
            FindPassword->setObjectName("FindPassword");
        FindPassword->setWindowModality(Qt::WindowModality::WindowModal);
        FindPassword->resize(390, 203);
        FindPassword->setMinimumSize(QSize(390, 203));
        FindPassword->setMaximumSize(QSize(390, 203));
        FindPassword->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        FindPassword->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(FindPassword);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacer = new QSpacerItem(371, 11, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(verticalSpacer, 0, 0, 1, 1);

        page_find = new QStackedWidget(centralwidget);
        page_find->setObjectName("page_find");
        page_find->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        pages_1 = new QWidget();
        pages_1->setObjectName("pages_1");
        verticalLayout = new QVBoxLayout(pages_1);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        lab_pages1 = new QLabel(pages_1);
        lab_pages1->setObjectName("lab_pages1");
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_pages1->sizePolicy().hasHeightForWidth());
        lab_pages1->setSizePolicy(sizePolicy);
        lab_pages1->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        horizontalLayout->addWidget(lab_pages1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        line_pages1 = new LineFind(pages_1);
        line_pages1->setObjectName("line_pages1");
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(line_pages1->sizePolicy().hasHeightForWidth());
        line_pages1->setSizePolicy(sizePolicy1);
        line_pages1->setMinimumSize(QSize(224, 38));
        line_pages1->setMaximumSize(QSize(224, 38));
        line_pages1->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_pages1->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_pages1->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_pages1->setMaxLength(12);
        line_pages1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(line_pages1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer = new QSpacerItem(286, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        but_pages1 = new QPushButton(pages_1);
        but_pages1->setObjectName("but_pages1");
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(20);
        sizePolicy2.setVerticalStretch(20);
        sizePolicy2.setHeightForWidth(but_pages1->sizePolicy().hasHeightForWidth());
        but_pages1->setSizePolicy(sizePolicy2);
        but_pages1->setMinimumSize(QSize(50, 35));
        but_pages1->setCursor(QCursor(Qt::PointingHandCursor));
        but_pages1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 10pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(50, 150, 255);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(50, 150, 255, 0.7);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 100, 255, 0.8);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}"));

        horizontalLayout_3->addWidget(but_pages1);


        verticalLayout->addLayout(horizontalLayout_3);

        page_find->addWidget(pages_1);
        pages_2 = new QWidget();
        pages_2->setObjectName("pages_2");
        verticalLayout_5 = new QVBoxLayout(pages_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_21);

        lab_pages2 = new QLabel(pages_2);
        lab_pages2->setObjectName("lab_pages2");
        sizePolicy.setHeightForWidth(lab_pages2->sizePolicy().hasHeightForWidth());
        lab_pages2->setSizePolicy(sizePolicy);
        lab_pages2->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        horizontalLayout_15->addWidget(lab_pages2);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_22);


        verticalLayout_5->addLayout(horizontalLayout_15);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_23);

        line_pages2 = new LineFind(pages_2);
        line_pages2->setObjectName("line_pages2");
        sizePolicy1.setHeightForWidth(line_pages2->sizePolicy().hasHeightForWidth());
        line_pages2->setSizePolicy(sizePolicy1);
        line_pages2->setMinimumSize(QSize(224, 38));
        line_pages2->setMaximumSize(QSize(224, 38));
        line_pages2->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_pages2->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_pages2->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_pages2->setMaxLength(12);
        line_pages2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_14->addWidget(line_pages2);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_24);


        verticalLayout_5->addLayout(horizontalLayout_14);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_25);

        but_pages2 = new QPushButton(pages_2);
        but_pages2->setObjectName("but_pages2");
        sizePolicy2.setHeightForWidth(but_pages2->sizePolicy().hasHeightForWidth());
        but_pages2->setSizePolicy(sizePolicy2);
        but_pages2->setMinimumSize(QSize(50, 35));
        but_pages2->setCursor(QCursor(Qt::PointingHandCursor));
        but_pages2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 10pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(50, 150, 255);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(50, 150, 255, 0.7);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 100, 255, 0.8);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}"));

        horizontalLayout_13->addWidget(but_pages2);


        verticalLayout_5->addLayout(horizontalLayout_13);

        page_find->addWidget(pages_2);
        pages_3 = new QWidget();
        pages_3->setObjectName("pages_3");
        gridLayout = new QGridLayout(pages_3);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_26);

        line_pages3_2 = new LineFind(pages_3);
        line_pages3_2->setObjectName("line_pages3_2");
        sizePolicy1.setHeightForWidth(line_pages3_2->sizePolicy().hasHeightForWidth());
        line_pages3_2->setSizePolicy(sizePolicy1);
        line_pages3_2->setMinimumSize(QSize(224, 38));
        line_pages3_2->setMaximumSize(QSize(224, 38));
        line_pages3_2->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_pages3_2->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_pages3_2->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_pages3_2->setMaxLength(12);
        line_pages3_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_16->addWidget(line_pages3_2);

        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_27);


        gridLayout->addLayout(horizontalLayout_16, 0, 0, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_28);

        line_pages3 = new LineFind(pages_3);
        line_pages3->setObjectName("line_pages3");
        sizePolicy1.setHeightForWidth(line_pages3->sizePolicy().hasHeightForWidth());
        line_pages3->setSizePolicy(sizePolicy1);
        line_pages3->setMinimumSize(QSize(224, 38));
        line_pages3->setMaximumSize(QSize(224, 38));
        line_pages3->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_pages3->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_pages3->setStyleSheet(QString::fromUtf8("font: 13pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_pages3->setMaxLength(12);
        line_pages3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_17->addWidget(line_pages3);

        horizontalSpacer_29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_29);


        gridLayout->addLayout(horizontalLayout_17, 1, 0, 1, 1);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        horizontalSpacer_30 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_30);

        but_pages3 = new QPushButton(pages_3);
        but_pages3->setObjectName("but_pages3");
        sizePolicy2.setHeightForWidth(but_pages3->sizePolicy().hasHeightForWidth());
        but_pages3->setSizePolicy(sizePolicy2);
        but_pages3->setMinimumSize(QSize(50, 35));
        but_pages3->setCursor(QCursor(Qt::PointingHandCursor));
        but_pages3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 10pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(50, 150, 255);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(50, 150, 255, 0.7);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 100, 255, 0.8);\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: white;\n"
"}"));

        horizontalLayout_18->addWidget(but_pages3);


        gridLayout->addLayout(horizontalLayout_18, 2, 0, 1, 1);

        page_find->addWidget(pages_3);

        gridLayout_2->addWidget(page_find, 1, 0, 1, 2);

        FindPassword->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FindPassword);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 390, 18));
        FindPassword->setMenuBar(menubar);
        statusbar = new QStatusBar(FindPassword);
        statusbar->setObjectName("statusbar");
        FindPassword->setStatusBar(statusbar);

        retranslateUi(FindPassword);

        page_find->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FindPassword);
    } // setupUi

    void retranslateUi(QMainWindow *FindPassword)
    {
        FindPassword->setWindowTitle(QCoreApplication::translate("FindPassword", "\346\211\276\345\233\236\345\257\206\347\240\201", nullptr));
        lab_pages1->setText(QCoreApplication::translate("FindPassword", "\350\257\267\350\276\223\345\205\245\350\246\201\346\211\276\345\233\236\347\232\204\350\264\246\345\217\267", nullptr));
        line_pages1->setText(QCoreApplication::translate("FindPassword", "\350\257\267\350\276\223\345\205\245\344\275\240\347\232\204\350\264\246\345\217\267", nullptr));
        but_pages1->setText(QCoreApplication::translate("FindPassword", "\344\270\213\344\270\200\346\255\245", nullptr));
        lab_pages2->setText(QCoreApplication::translate("FindPassword", "\345\201\207\350\243\205\350\277\231\346\230\257\344\270\200\344\270\252\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
        line_pages2->setText(QCoreApplication::translate("FindPassword", "\350\257\267\350\276\223\345\205\245\345\257\206\344\277\235\347\255\224\346\241\210", nullptr));
        but_pages2->setText(QCoreApplication::translate("FindPassword", "\344\270\213\344\270\200\346\255\245", nullptr));
        line_pages3_2->setText(QCoreApplication::translate("FindPassword", "\350\257\267\350\276\223\345\205\245\346\226\260\347\232\204\345\257\206\347\240\201", nullptr));
        line_pages3->setText(QCoreApplication::translate("FindPassword", "\350\257\267\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        but_pages3->setText(QCoreApplication::translate("FindPassword", "\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindPassword: public Ui_FindPassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDPASSWORD_H
