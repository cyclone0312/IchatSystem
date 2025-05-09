/********************************************************************************
** Form generated from reading UI file 'registerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWINDOW_H
#define UI_REGISTERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <registerwindow.h>

QT_BEGIN_NAMESPACE

class Ui_RegisterWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    LabelRegis *lab_avator;
    QGridLayout *gridLayout;
    QComboBox *cbbox_sex;
    QLabel *lab_question;
    LineRegis *line_question;
    QLabel *lab_sex;
    QLabel *label_password2;
    LineRegis *line_password;
    LineRegis *line_password2;
    LineRegis *line_answer;
    QLabel *lab_password;
    LineRegis *line_nickname;
    QLabel *lab_nickname;
    QLabel *label_question;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *submit_but;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RegisterWindow)
    {
        if (RegisterWindow->objectName().isEmpty())
            RegisterWindow->setObjectName("RegisterWindow");
        RegisterWindow->setWindowModality(Qt::WindowModality::WindowModal);
        RegisterWindow->resize(350, 510);
        RegisterWindow->setMinimumSize(QSize(350, 510));
        RegisterWindow->setMaximumSize(QSize(350, 510));
        RegisterWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(RegisterWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(80, 80));
        label_4->setMaximumSize(QSize(47, 100));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        horizontalLayout_2->addWidget(label_4);

        lab_avator = new LabelRegis(centralwidget);
        lab_avator->setObjectName("lab_avator");
        sizePolicy.setHeightForWidth(lab_avator->sizePolicy().hasHeightForWidth());
        lab_avator->setSizePolicy(sizePolicy);
        lab_avator->setMinimumSize(QSize(100, 100));
        lab_avator->setMaximumSize(QSize(100, 100));
        lab_avator->setCursor(QCursor(Qt::PointingHandCursor));
        lab_avator->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(lab_avator);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        cbbox_sex = new QComboBox(centralwidget);
        cbbox_sex->addItem(QString());
        cbbox_sex->addItem(QString());
        cbbox_sex->addItem(QString());
        cbbox_sex->setObjectName("cbbox_sex");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbbox_sex->sizePolicy().hasHeightForWidth());
        cbbox_sex->setSizePolicy(sizePolicy1);
        cbbox_sex->setMinimumSize(QSize(224, 38));
        cbbox_sex->setMaximumSize(QSize(224, 38));
        cbbox_sex->setCursor(QCursor(Qt::PointingHandCursor));
        cbbox_sex->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        cbbox_sex->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    border: 1px solid rgba(0, 0, 0, 0.3);\n"
"    border-radius: 8px;\n"
"    color: black;\n"
"    padding-right: 10px;\n"
"    padding-left: 3px;  \n"
"    text-align: left;  \n"
"    border-radius: 10px;\n"
"    background-color: white;\n"
"}\n"
"QComboBox::drop-down {\n"
"  border: none; \n"
"}"));

        gridLayout->addWidget(cbbox_sex, 1, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        lab_question = new QLabel(centralwidget);
        lab_question->setObjectName("lab_question");
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lab_question->sizePolicy().hasHeightForWidth());
        lab_question->setSizePolicy(sizePolicy2);
        lab_question->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(lab_question, 5, 0, 1, 1);

        line_question = new LineRegis(centralwidget);
        line_question->setObjectName("line_question");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(line_question->sizePolicy().hasHeightForWidth());
        line_question->setSizePolicy(sizePolicy3);
        line_question->setMinimumSize(QSize(224, 38));
        line_question->setMaximumSize(QSize(224, 38));
        line_question->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_question->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_question->setMaxLength(12);

        gridLayout->addWidget(line_question, 4, 1, 1, 1);

        lab_sex = new QLabel(centralwidget);
        lab_sex->setObjectName("lab_sex");
        sizePolicy2.setHeightForWidth(lab_sex->sizePolicy().hasHeightForWidth());
        lab_sex->setSizePolicy(sizePolicy2);
        lab_sex->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(lab_sex, 1, 0, 1, 1);

        label_password2 = new QLabel(centralwidget);
        label_password2->setObjectName("label_password2");
        sizePolicy2.setHeightForWidth(label_password2->sizePolicy().hasHeightForWidth());
        label_password2->setSizePolicy(sizePolicy2);
        label_password2->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(label_password2, 3, 0, 1, 1);

        line_password = new LineRegis(centralwidget);
        line_password->setObjectName("line_password");
        sizePolicy3.setHeightForWidth(line_password->sizePolicy().hasHeightForWidth());
        line_password->setSizePolicy(sizePolicy3);
        line_password->setMinimumSize(QSize(224, 38));
        line_password->setMaximumSize(QSize(224, 38));
        line_password->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_password->setMaxLength(12);

        gridLayout->addWidget(line_password, 2, 1, 1, 1);

        line_password2 = new LineRegis(centralwidget);
        line_password2->setObjectName("line_password2");
        sizePolicy3.setHeightForWidth(line_password2->sizePolicy().hasHeightForWidth());
        line_password2->setSizePolicy(sizePolicy3);
        line_password2->setMinimumSize(QSize(224, 38));
        line_password2->setMaximumSize(QSize(224, 38));
        line_password2->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_password2->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_password2->setMaxLength(12);

        gridLayout->addWidget(line_password2, 3, 1, 1, 1);

        line_answer = new LineRegis(centralwidget);
        line_answer->setObjectName("line_answer");
        sizePolicy3.setHeightForWidth(line_answer->sizePolicy().hasHeightForWidth());
        line_answer->setSizePolicy(sizePolicy3);
        line_answer->setMinimumSize(QSize(224, 38));
        line_answer->setMaximumSize(QSize(224, 38));
        line_answer->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_answer->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_answer->setMaxLength(12);

        gridLayout->addWidget(line_answer, 5, 1, 1, 1);

        lab_password = new QLabel(centralwidget);
        lab_password->setObjectName("lab_password");
        sizePolicy2.setHeightForWidth(lab_password->sizePolicy().hasHeightForWidth());
        lab_password->setSizePolicy(sizePolicy2);
        lab_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(lab_password, 2, 0, 1, 1);

        line_nickname = new LineRegis(centralwidget);
        line_nickname->setObjectName("line_nickname");
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(line_nickname->sizePolicy().hasHeightForWidth());
        line_nickname->setSizePolicy(sizePolicy4);
        line_nickname->setMinimumSize(QSize(224, 38));
        line_nickname->setMaximumSize(QSize(224, 38));
        line_nickname->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_nickname->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3);\n"
"border-radius: 8px;\n"
"color:grey;\n"
""));
        line_nickname->setMaxLength(5);

        gridLayout->addWidget(line_nickname, 0, 1, 1, 1);

        lab_nickname = new QLabel(centralwidget);
        lab_nickname->setObjectName("lab_nickname");
        sizePolicy2.setHeightForWidth(lab_nickname->sizePolicy().hasHeightForWidth());
        lab_nickname->setSizePolicy(sizePolicy2);
        lab_nickname->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(lab_nickname, 0, 0, 1, 1);

        label_question = new QLabel(centralwidget);
        label_question->setObjectName("label_question");
        sizePolicy2.setHeightForWidth(label_question->sizePolicy().hasHeightForWidth());
        label_question->setSizePolicy(sizePolicy2);
        label_question->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"\n"
"color:black;\n"
""));

        gridLayout->addWidget(label_question, 4, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        submit_but = new QPushButton(centralwidget);
        submit_but->setObjectName("submit_but");
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(15);
        sizePolicy5.setVerticalStretch(15);
        sizePolicy5.setHeightForWidth(submit_but->sizePolicy().hasHeightForWidth());
        submit_but->setSizePolicy(sizePolicy5);
        submit_but->setMinimumSize(QSize(170, 60));
        submit_but->setMaximumSize(QSize(5, 16777215));
        submit_but->setCursor(QCursor(Qt::ArrowCursor));
        submit_but->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        submit_but->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 14pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;; \n"
"   border-radius: 15px;\n"
"}\n"
""));

        horizontalLayout_3->addWidget(submit_but);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        gridLayout_2->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        RegisterWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RegisterWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 350, 18));
        RegisterWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RegisterWindow);
        statusbar->setObjectName("statusbar");
        RegisterWindow->setStatusBar(statusbar);

        retranslateUi(RegisterWindow);

        QMetaObject::connectSlotsByName(RegisterWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RegisterWindow)
    {
        RegisterWindow->setWindowTitle(QCoreApplication::translate("RegisterWindow", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        label_4->setText(QCoreApplication::translate("RegisterWindow", "\345\244\264\345\203\217:", nullptr));
        lab_avator->setText(QString());
        cbbox_sex->setItemText(0, QCoreApplication::translate("RegisterWindow", "\345\205\266\344\273\226", nullptr));
        cbbox_sex->setItemText(1, QCoreApplication::translate("RegisterWindow", "\347\224\267", nullptr));
        cbbox_sex->setItemText(2, QCoreApplication::translate("RegisterWindow", "\345\245\263", nullptr));

#if QT_CONFIG(whatsthis)
        cbbox_sex->setWhatsThis(QCoreApplication::translate("RegisterWindow", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        lab_question->setText(QCoreApplication::translate("RegisterWindow", "\351\227\256\351\242\230\347\255\224\346\241\210:", nullptr));
        line_question->setText(QCoreApplication::translate("RegisterWindow", "\350\257\267\350\276\223\345\205\245\351\227\256\351\242\230", nullptr));
        lab_sex->setText(QCoreApplication::translate("RegisterWindow", "\346\200\247      \345\210\253:", nullptr));
        label_password2->setText(QCoreApplication::translate("RegisterWindow", "\347\241\256\350\256\244\345\257\206\347\240\201:", nullptr));
        line_password->setText(QCoreApplication::translate("RegisterWindow", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        line_password2->setText(QCoreApplication::translate("RegisterWindow", "\350\257\267\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        line_answer->setText(QCoreApplication::translate("RegisterWindow", "\350\257\267\350\276\223\345\205\245\347\255\224\346\241\210", nullptr));
        lab_password->setText(QCoreApplication::translate("RegisterWindow", "\344\275\240\347\232\204\345\257\206\347\240\201:", nullptr));
        line_nickname->setText(QCoreApplication::translate("RegisterWindow", "\350\257\267\350\276\223\345\205\245\346\230\265\347\247\260", nullptr));
        lab_nickname->setText(QCoreApplication::translate("RegisterWindow", "\344\275\240\347\232\204\346\230\265\347\247\260:", nullptr));
        label_question->setText(QCoreApplication::translate("RegisterWindow", "\345\257\206\344\277\235\351\227\256\351\242\230:", nullptr));
        submit_but->setText(QCoreApplication::translate("RegisterWindow", "\346\217\220\344\272\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterWindow: public Ui_RegisterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOW_H
