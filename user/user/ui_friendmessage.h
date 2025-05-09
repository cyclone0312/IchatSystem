/********************************************************************************
** Form generated from reading UI file 'friendmessage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDMESSAGE_H
#define UI_FRIENDMESSAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <autocleartextedit.h>

QT_BEGIN_NAMESPACE

class Ui_FriendMessage
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *but_cancelwindow;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLabel *lab_avator;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_2;
    AutoClearTextEdit *textEdit_account;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lab;
    AutoClearTextEdit *textEdit_nickname;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lab_3;
    AutoClearTextEdit *textEdit_sig;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_4;
    QLabel *lab_4;
    AutoClearTextEdit *textEdit_gender;

    void setupUi(QDialog *FriendMessage)
    {
        if (FriendMessage->objectName().isEmpty())
            FriendMessage->setObjectName("FriendMessage");
        FriendMessage->resize(306, 355);
        gridLayout = new QGridLayout(FriendMessage);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(FriendMessage);
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

        but_cancelwindow = new QPushButton(FriendMessage);
        but_cancelwindow->setObjectName("but_cancelwindow");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(15);
        sizePolicy1.setHeightForWidth(but_cancelwindow->sizePolicy().hasHeightForWidth());
        but_cancelwindow->setSizePolicy(sizePolicy1);
        but_cancelwindow->setMinimumSize(QSize(20, 20));
        but_cancelwindow->setMaximumSize(QSize(20, 20));
        but_cancelwindow->setCursor(QCursor(Qt::PointingHandCursor));
        but_cancelwindow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        but_cancelwindow->setAutoDefault(false);

        horizontalLayout->addWidget(but_cancelwindow, 0, Qt::AlignmentFlag::AlignTop);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(FriendMessage);
        label_4->setObjectName("label_4");
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(80, 80));
        label_4->setMaximumSize(QSize(47, 100));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: none;;\n"
"color:black;\n"
""));

        horizontalLayout_2->addWidget(label_4);

        lab_avator = new QLabel(FriendMessage);
        lab_avator->setObjectName("lab_avator");
        sizePolicy2.setHeightForWidth(lab_avator->sizePolicy().hasHeightForWidth());
        lab_avator->setSizePolicy(sizePolicy2);
        lab_avator->setMinimumSize(QSize(100, 100));
        lab_avator->setMaximumSize(QSize(100, 100));
        lab_avator->setCursor(QCursor(Qt::ArrowCursor));
        lab_avator->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        lab_avator->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
" border-radius: 8px;"));

        horizontalLayout_2->addWidget(lab_avator);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        lab_2 = new QLabel(FriendMessage);
        lab_2->setObjectName("lab_2");
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lab_2->sizePolicy().hasHeightForWidth());
        lab_2->setSizePolicy(sizePolicy3);
        lab_2->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_6->addWidget(lab_2);

        textEdit_account = new AutoClearTextEdit(FriendMessage);
        textEdit_account->setObjectName("textEdit_account");
        sizePolicy2.setHeightForWidth(textEdit_account->sizePolicy().hasHeightForWidth());
        textEdit_account->setSizePolicy(sizePolicy2);
        textEdit_account->setMinimumSize(QSize(224, 38));
        textEdit_account->setMaximumSize(QSize(224, 38));
        textEdit_account->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background: rgba(255, 255, 255, 0); \n"
"border: none;\n"
"border-radius: 8px;\n"
"padding:5px;"));
        textEdit_account->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_account->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_account->setReadOnly(true);

        horizontalLayout_6->addWidget(textEdit_account);


        gridLayout->addLayout(horizontalLayout_6, 2, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        lab = new QLabel(FriendMessage);
        lab->setObjectName("lab");
        sizePolicy3.setHeightForWidth(lab->sizePolicy().hasHeightForWidth());
        lab->setSizePolicy(sizePolicy3);
        lab->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_7->addWidget(lab);

        textEdit_nickname = new AutoClearTextEdit(FriendMessage);
        textEdit_nickname->setObjectName("textEdit_nickname");
        sizePolicy2.setHeightForWidth(textEdit_nickname->sizePolicy().hasHeightForWidth());
        textEdit_nickname->setSizePolicy(sizePolicy2);
        textEdit_nickname->setMinimumSize(QSize(224, 38));
        textEdit_nickname->setMaximumSize(QSize(224, 38));
        textEdit_nickname->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_nickname->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background: rgba(255, 255, 255, 0); \n"
"border: none;\n"
"border-radius: 8px;\n"
"padding:5px;\n"
""));
        textEdit_nickname->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_nickname->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_nickname->setReadOnly(true);

        horizontalLayout_7->addWidget(textEdit_nickname);


        gridLayout->addLayout(horizontalLayout_7, 3, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        lab_3 = new QLabel(FriendMessage);
        lab_3->setObjectName("lab_3");
        sizePolicy3.setHeightForWidth(lab_3->sizePolicy().hasHeightForWidth());
        lab_3->setSizePolicy(sizePolicy3);
        lab_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_8->addWidget(lab_3);

        textEdit_sig = new AutoClearTextEdit(FriendMessage);
        textEdit_sig->setObjectName("textEdit_sig");
        sizePolicy2.setHeightForWidth(textEdit_sig->sizePolicy().hasHeightForWidth());
        textEdit_sig->setSizePolicy(sizePolicy2);
        textEdit_sig->setMinimumSize(QSize(224, 38));
        textEdit_sig->setMaximumSize(QSize(224, 38));
        textEdit_sig->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_sig->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background: rgba(255, 255, 255, 0); \n"
"border: none;\n"
"border-radius: 8px;\n"
"padding:5px;\n"
""));
        textEdit_sig->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_sig->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_sig->setReadOnly(true);

        horizontalLayout_8->addWidget(textEdit_sig);


        gridLayout->addLayout(horizontalLayout_8, 4, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);

        lab_4 = new QLabel(FriendMessage);
        lab_4->setObjectName("lab_4");
        sizePolicy3.setHeightForWidth(lab_4->sizePolicy().hasHeightForWidth());
        lab_4->setSizePolicy(sizePolicy3);
        lab_4->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_9->addWidget(lab_4);

        textEdit_gender = new AutoClearTextEdit(FriendMessage);
        textEdit_gender->setObjectName("textEdit_gender");
        sizePolicy2.setHeightForWidth(textEdit_gender->sizePolicy().hasHeightForWidth());
        textEdit_gender->setSizePolicy(sizePolicy2);
        textEdit_gender->setMinimumSize(QSize(224, 38));
        textEdit_gender->setMaximumSize(QSize(224, 38));
        textEdit_gender->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_gender->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background: rgba(255, 255, 255, 0); \n"
"border: none;\n"
"border-radius: 8px;\n"
"padding:5px;\n"
""));
        textEdit_gender->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_gender->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_gender->setReadOnly(true);

        horizontalLayout_9->addWidget(textEdit_gender);


        gridLayout->addLayout(horizontalLayout_9, 5, 0, 1, 1);


        retranslateUi(FriendMessage);

        QMetaObject::connectSlotsByName(FriendMessage);
    } // setupUi

    void retranslateUi(QDialog *FriendMessage)
    {
        FriendMessage->setWindowTitle(QCoreApplication::translate("FriendMessage", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FriendMessage", "     \347\224\250\346\210\267\350\265\204\346\226\231", nullptr));
        but_cancelwindow->setText(QCoreApplication::translate("FriendMessage", "x", nullptr));
        label_4->setText(QCoreApplication::translate("FriendMessage", "\345\244\264\345\203\217:", nullptr));
        lab_avator->setText(QString());
        lab_2->setText(QCoreApplication::translate("FriendMessage", "\350\264\246\345\217\267:", nullptr));
        textEdit_account->setHtml(QCoreApplication::translate("FriendMessage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\245\275\345\217\213\350\264\246\345\217\267</p></body></html>", nullptr));
        lab->setText(QCoreApplication::translate("FriendMessage", "\346\230\265\347\247\260:", nullptr));
        textEdit_nickname->setHtml(QCoreApplication::translate("FriendMessage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\245\275\345\217\213\346\230\265\347\247\260</p></body></html>", nullptr));
        lab_3->setText(QCoreApplication::translate("FriendMessage", "\347\255\276\345\220\215:", nullptr));
        textEdit_sig->setHtml(QCoreApplication::translate("FriendMessage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\245\275\345\217\213\347\255\276\345\220\215</p></body></html>", nullptr));
        lab_4->setText(QCoreApplication::translate("FriendMessage", "\346\200\247\345\210\253:", nullptr));
        textEdit_gender->setHtml(QCoreApplication::translate("FriendMessage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\245\275\345\217\213\346\200\247\345\210\253</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendMessage: public Ui_FriendMessage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDMESSAGE_H
