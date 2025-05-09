/********************************************************************************
** Form generated from reading UI file 'changepassword.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPASSWORD_H
#define UI_CHANGEPASSWORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <changepassword.h>

QT_BEGIN_NAMESPACE

class Ui_ChangePassword
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_account;
    LineChangePass *line_account;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_password;
    LineChangePass *line_password;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *but_yes;
    QSpacerItem *horizontalSpacer;
    QWidget *page_2;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *lab_newpassword;
    LineChangePass *line_newpassword;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lab_newpassword2;
    LineChangePass *line_newpassword2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *but_yes_2;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *but_deletewindow;

    void setupUi(QDialog *ChangePassword)
    {
        if (ChangePassword->objectName().isEmpty())
            ChangePassword->setObjectName("ChangePassword");
        ChangePassword->resize(322, 217);
        gridLayout = new QGridLayout(ChangePassword);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(ChangePassword);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        stackedWidget->setStyleSheet(QString::fromUtf8("border:none;"));
        page = new QWidget();
        page->setObjectName("page");
        gridLayout_2 = new QGridLayout(page);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lab_account = new QLabel(page);
        lab_account->setObjectName("lab_account");
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_account->sizePolicy().hasHeightForWidth());
        lab_account->setSizePolicy(sizePolicy);
        lab_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_4->addWidget(lab_account);

        line_account = new LineChangePass(page);
        line_account->setObjectName("line_account");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(line_account->sizePolicy().hasHeightForWidth());
        line_account->setSizePolicy(sizePolicy1);
        line_account->setMinimumSize(QSize(224, 38));
        line_account->setMaximumSize(QSize(224, 38));
        line_account->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_account->setMaxLength(12);

        horizontalLayout_4->addWidget(line_account);


        gridLayout_2->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        lab_password = new QLabel(page);
        lab_password->setObjectName("lab_password");
        sizePolicy.setHeightForWidth(lab_password->sizePolicy().hasHeightForWidth());
        lab_password->setSizePolicy(sizePolicy);
        lab_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_3->addWidget(lab_password);

        line_password = new LineChangePass(page);
        line_password->setObjectName("line_password");
        sizePolicy1.setHeightForWidth(line_password->sizePolicy().hasHeightForWidth());
        line_password->setSizePolicy(sizePolicy1);
        line_password->setMinimumSize(QSize(224, 38));
        line_password->setMaximumSize(QSize(224, 38));
        line_password->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_password->setMaxLength(12);

        horizontalLayout_3->addWidget(line_password);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        but_yes = new QPushButton(page);
        but_yes->setObjectName("but_yes");
        but_yes->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(but_yes->sizePolicy().hasHeightForWidth());
        but_yes->setSizePolicy(sizePolicy2);
        but_yes->setMinimumSize(QSize(175, 35));
        but_yes->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
""));

        horizontalLayout_2->addWidget(but_yes);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        gridLayout_3 = new QGridLayout(page_2);
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        lab_newpassword = new QLabel(page_2);
        lab_newpassword->setObjectName("lab_newpassword");
        sizePolicy.setHeightForWidth(lab_newpassword->sizePolicy().hasHeightForWidth());
        lab_newpassword->setSizePolicy(sizePolicy);
        lab_newpassword->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_7->addWidget(lab_newpassword);

        line_newpassword = new LineChangePass(page_2);
        line_newpassword->setObjectName("line_newpassword");
        sizePolicy1.setHeightForWidth(line_newpassword->sizePolicy().hasHeightForWidth());
        line_newpassword->setSizePolicy(sizePolicy1);
        line_newpassword->setMinimumSize(QSize(224, 38));
        line_newpassword->setMaximumSize(QSize(224, 38));
        line_newpassword->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_newpassword->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_newpassword->setMaxLength(12);

        horizontalLayout_7->addWidget(line_newpassword);


        gridLayout_3->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        lab_newpassword2 = new QLabel(page_2);
        lab_newpassword2->setObjectName("lab_newpassword2");
        sizePolicy.setHeightForWidth(lab_newpassword2->sizePolicy().hasHeightForWidth());
        lab_newpassword2->setSizePolicy(sizePolicy);
        lab_newpassword2->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_6->addWidget(lab_newpassword2);

        line_newpassword2 = new LineChangePass(page_2);
        line_newpassword2->setObjectName("line_newpassword2");
        sizePolicy1.setHeightForWidth(line_newpassword2->sizePolicy().hasHeightForWidth());
        line_newpassword2->setSizePolicy(sizePolicy1);
        line_newpassword2->setMinimumSize(QSize(224, 38));
        line_newpassword2->setMaximumSize(QSize(224, 38));
        line_newpassword2->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_newpassword2->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_newpassword2->setMaxLength(12);

        horizontalLayout_6->addWidget(line_newpassword2);


        gridLayout_3->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        but_yes_2 = new QPushButton(page_2);
        but_yes_2->setObjectName("but_yes_2");
        but_yes_2->setEnabled(false);
        sizePolicy2.setHeightForWidth(but_yes_2->sizePolicy().hasHeightForWidth());
        but_yes_2->setSizePolicy(sizePolicy2);
        but_yes_2->setMinimumSize(QSize(175, 35));
        but_yes_2->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
""));

        horizontalLayout_5->addWidget(but_yes_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        gridLayout_3->addLayout(horizontalLayout_5, 2, 0, 1, 1);

        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ChangePassword);
        label->setObjectName("label");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Microsoft YaHei UI\";\n"
"border:none;"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        but_deletewindow = new QPushButton(ChangePassword);
        but_deletewindow->setObjectName("but_deletewindow");
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(15);
        sizePolicy4.setHeightForWidth(but_deletewindow->sizePolicy().hasHeightForWidth());
        but_deletewindow->setSizePolicy(sizePolicy4);
        but_deletewindow->setMinimumSize(QSize(20, 20));
        but_deletewindow->setMaximumSize(QSize(20, 20));
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
        but_deletewindow->setAutoDefault(false);

        horizontalLayout->addWidget(but_deletewindow);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(ChangePassword);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChangePassword);
    } // setupUi

    void retranslateUi(QDialog *ChangePassword)
    {
        ChangePassword->setWindowTitle(QCoreApplication::translate("ChangePassword", "Dialog", nullptr));
        lab_account->setText(QCoreApplication::translate("ChangePassword", "\350\276\223\345\205\245\350\264\246\345\217\267:", nullptr));
        line_account->setText(QCoreApplication::translate("ChangePassword", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\350\264\246\345\217\267", nullptr));
        lab_password->setText(QCoreApplication::translate("ChangePassword", "\350\276\223\345\205\245\345\257\206\347\240\201:", nullptr));
        line_password->setText(QCoreApplication::translate("ChangePassword", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\257\206\347\240\201", nullptr));
        but_yes->setText(QCoreApplication::translate("ChangePassword", "\347\241\256\350\256\244", nullptr));
        lab_newpassword->setText(QCoreApplication::translate("ChangePassword", "\346\226\260\347\232\204\345\257\206\347\240\201:", nullptr));
        line_newpassword->setText(QCoreApplication::translate("ChangePassword", "\350\257\267\350\276\223\345\205\245\346\226\260\347\232\204\345\257\206\347\240\201", nullptr));
        lab_newpassword2->setText(QCoreApplication::translate("ChangePassword", "\347\241\256\350\256\244\345\257\206\347\240\201:", nullptr));
        line_newpassword2->setText(QCoreApplication::translate("ChangePassword", "\350\257\267\347\241\256\350\256\244\346\202\250\347\232\204\345\257\206\347\240\201", nullptr));
        but_yes_2->setText(QCoreApplication::translate("ChangePassword", "\347\241\256\350\256\244", nullptr));
        label->setText(QCoreApplication::translate("ChangePassword", "       \344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        but_deletewindow->setText(QCoreApplication::translate("ChangePassword", "x", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangePassword: public Ui_ChangePassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPASSWORD_H
