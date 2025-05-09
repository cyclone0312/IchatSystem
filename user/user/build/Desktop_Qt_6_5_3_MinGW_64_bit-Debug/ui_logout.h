/********************************************************************************
** Form generated from reading UI file 'logout.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGOUT_H
#define UI_LOGOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <changepassword.h>

QT_BEGIN_NAMESPACE

class Ui_Logout
{
public:
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *but_deletewindow;
    QHBoxLayout *horizontalLayout_10;
    QLabel *lab_account;
    LineChangePass *line_account;
    QHBoxLayout *horizontalLayout_9;
    QLabel *lab_password;
    LineChangePass *line_password;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *but_yes;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QDialog *Logout)
    {
        if (Logout->objectName().isEmpty())
            Logout->setObjectName("Logout");
        Logout->resize(302, 187);
        Logout->setStyleSheet(QString::fromUtf8("border:none;"));
        gridLayout_4 = new QGridLayout(Logout);
        gridLayout_4->setObjectName("gridLayout_4");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(Logout);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Microsoft YaHei UI\";\n"
"border:none;"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label, 0, Qt::AlignmentFlag::AlignTop);

        but_deletewindow = new QPushButton(Logout);
        but_deletewindow->setObjectName("but_deletewindow");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(15);
        sizePolicy1.setHeightForWidth(but_deletewindow->sizePolicy().hasHeightForWidth());
        but_deletewindow->setSizePolicy(sizePolicy1);
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

        horizontalLayout->addWidget(but_deletewindow, 0, Qt::AlignmentFlag::AlignTop);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        lab_account = new QLabel(Logout);
        lab_account->setObjectName("lab_account");
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lab_account->sizePolicy().hasHeightForWidth());
        lab_account->setSizePolicy(sizePolicy2);
        lab_account->setMinimumSize(QSize(0, 50));
        lab_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_10->addWidget(lab_account);

        line_account = new LineChangePass(Logout);
        line_account->setObjectName("line_account");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(line_account->sizePolicy().hasHeightForWidth());
        line_account->setSizePolicy(sizePolicy3);
        line_account->setMinimumSize(QSize(224, 38));
        line_account->setMaximumSize(QSize(224, 38));
        line_account->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_account->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_account->setMaxLength(12);

        horizontalLayout_10->addWidget(line_account);


        gridLayout_4->addLayout(horizontalLayout_10, 1, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        lab_password = new QLabel(Logout);
        lab_password->setObjectName("lab_password");
        sizePolicy2.setHeightForWidth(lab_password->sizePolicy().hasHeightForWidth());
        lab_password->setSizePolicy(sizePolicy2);
        lab_password->setMinimumSize(QSize(0, 50));
        lab_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_9->addWidget(lab_password);

        line_password = new LineChangePass(Logout);
        line_password->setObjectName("line_password");
        sizePolicy3.setHeightForWidth(line_password->sizePolicy().hasHeightForWidth());
        line_password->setSizePolicy(sizePolicy3);
        line_password->setMinimumSize(QSize(224, 38));
        line_password->setMaximumSize(QSize(224, 38));
        line_password->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_password->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        line_password->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"color:grey;\n"
"\n"
""));
        line_password->setMaxLength(12);

        horizontalLayout_9->addWidget(line_password);


        gridLayout_4->addLayout(horizontalLayout_9, 2, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        but_yes = new QPushButton(Logout);
        but_yes->setObjectName("but_yes");
        but_yes->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(but_yes->sizePolicy().hasHeightForWidth());
        but_yes->setSizePolicy(sizePolicy4);
        but_yes->setMinimumSize(QSize(175, 35));
        but_yes->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
""));

        horizontalLayout_8->addWidget(but_yes);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        gridLayout_4->addLayout(horizontalLayout_8, 3, 0, 1, 1);


        retranslateUi(Logout);

        QMetaObject::connectSlotsByName(Logout);
    } // setupUi

    void retranslateUi(QDialog *Logout)
    {
        Logout->setWindowTitle(QCoreApplication::translate("Logout", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Logout", "         \346\263\250\351\224\200\350\264\246\345\217\267", nullptr));
        but_deletewindow->setText(QCoreApplication::translate("Logout", "x", nullptr));
        lab_account->setText(QCoreApplication::translate("Logout", "\350\276\223\345\205\245\350\264\246\345\217\267:", nullptr));
        line_account->setText(QCoreApplication::translate("Logout", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\350\264\246\345\217\267", nullptr));
        lab_password->setText(QCoreApplication::translate("Logout", "\350\276\223\345\205\245\345\257\206\347\240\201:", nullptr));
        line_password->setText(QCoreApplication::translate("Logout", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\257\206\347\240\201", nullptr));
        but_yes->setText(QCoreApplication::translate("Logout", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Logout: public Ui_Logout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGOUT_H
