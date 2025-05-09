/********************************************************************************
** Form generated from reading UI file 'changeinformation.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEINFORMATION_H
#define UI_CHANGEINFORMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <changeinformation.h>

QT_BEGIN_NAMESPACE

class Ui_ChangeInformation
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *but_cancelwindow;
    QFrame *frame_message;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lab_account;
    QTextEdit *textEdit_account;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_sex;
    QComboBox *cbbox_sex;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_nickname;
    LineMessage *line_nickname;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lab_sig;
    LineMessage *line_sig;
    QVBoxLayout *layout_frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    LabelChooseAva *lab_avator;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *but_yes;
    QPushButton *but_no;

    void setupUi(QDialog *ChangeInformation)
    {
        if (ChangeInformation->objectName().isEmpty())
            ChangeInformation->setObjectName("ChangeInformation");
        ChangeInformation->resize(333, 410);
        ChangeInformation->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        ChangeInformation->setStyleSheet(QString::fromUtf8("border:none"));
        gridLayout = new QGridLayout(ChangeInformation);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ChangeInformation);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Microsoft YaHei UI\";"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        but_cancelwindow = new QPushButton(ChangeInformation);
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

        horizontalLayout->addWidget(but_cancelwindow);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        frame_message = new QFrame(ChangeInformation);
        frame_message->setObjectName("frame_message");
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(210);
        sizePolicy2.setHeightForWidth(frame_message->sizePolicy().hasHeightForWidth());
        frame_message->setSizePolicy(sizePolicy2);
        frame_message->setMinimumSize(QSize(0, 210));
        frame_message->setStyleSheet(QString::fromUtf8("border-radius: 10px;\n"
"border: none;\n"
""));
        frame_message->setFrameShape(QFrame::Shape::StyledPanel);
        frame_message->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame_message);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        lab_account = new QLabel(frame_message);
        lab_account->setObjectName("lab_account");
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lab_account->sizePolicy().hasHeightForWidth());
        lab_account->setSizePolicy(sizePolicy3);
        lab_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_6->addWidget(lab_account);

        textEdit_account = new QTextEdit(frame_message);
        textEdit_account->setObjectName("textEdit_account");
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(textEdit_account->sizePolicy().hasHeightForWidth());
        textEdit_account->setSizePolicy(sizePolicy4);
        textEdit_account->setMinimumSize(QSize(224, 38));
        textEdit_account->setMaximumSize(QSize(224, 38));
        textEdit_account->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"background: rgba(255, 255, 255, 0); \n"
"border: none;\n"
"border-radius: 8px;\n"
"padding:5px;\n"
""));
        textEdit_account->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_account->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        textEdit_account->setReadOnly(true);

        horizontalLayout_6->addWidget(textEdit_account);


        gridLayout_2->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lab_sex = new QLabel(frame_message);
        lab_sex->setObjectName("lab_sex");
        sizePolicy3.setHeightForWidth(lab_sex->sizePolicy().hasHeightForWidth());
        lab_sex->setSizePolicy(sizePolicy3);
        lab_sex->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_4->addWidget(lab_sex);

        cbbox_sex = new QComboBox(frame_message);
        cbbox_sex->addItem(QString());
        cbbox_sex->addItem(QString());
        cbbox_sex->addItem(QString());
        cbbox_sex->setObjectName("cbbox_sex");
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(cbbox_sex->sizePolicy().hasHeightForWidth());
        cbbox_sex->setSizePolicy(sizePolicy5);
        cbbox_sex->setMinimumSize(QSize(224, 38));
        cbbox_sex->setMaximumSize(QSize(224, 38));
        cbbox_sex->setCursor(QCursor(Qt::PointingHandCursor));
        cbbox_sex->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    border: 1px solid rgba(0, 0, 0, 0.1);\n"
"    border-radius: 8px;\n"
"    color: black;\n"
"    padding-right: 10px;\n"
"    padding-left: 3px;  \n"
"    text-align: left;  \n"
"    border-radius: 10px;\n"
"    background-color: white;\n"
"    padding:5px;\n"
"}\n"
"QComboBox::drop-down {\n"
"  border: none; \n"
"}"));

        horizontalLayout_4->addWidget(cbbox_sex);


        gridLayout_2->addLayout(horizontalLayout_4, 4, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        lab_nickname = new QLabel(frame_message);
        lab_nickname->setObjectName("lab_nickname");
        sizePolicy3.setHeightForWidth(lab_nickname->sizePolicy().hasHeightForWidth());
        lab_nickname->setSizePolicy(sizePolicy3);
        lab_nickname->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_3->addWidget(lab_nickname);

        line_nickname = new LineMessage(frame_message);
        line_nickname->setObjectName("line_nickname");
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(line_nickname->sizePolicy().hasHeightForWidth());
        line_nickname->setSizePolicy(sizePolicy6);
        line_nickname->setMinimumSize(QSize(224, 38));
        line_nickname->setMaximumSize(QSize(224, 38));
        line_nickname->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_nickname->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;\n"
"\n"
""));
        line_nickname->setMaxLength(5);

        horizontalLayout_3->addWidget(line_nickname);


        gridLayout_2->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        lab_sig = new QLabel(frame_message);
        lab_sig->setObjectName("lab_sig");
        sizePolicy3.setHeightForWidth(lab_sig->sizePolicy().hasHeightForWidth());
        lab_sig->setSizePolicy(sizePolicy3);
        lab_sig->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border:none;\n"
"color:black;\n"
""));

        horizontalLayout_5->addWidget(lab_sig);

        line_sig = new LineMessage(frame_message);
        line_sig->setObjectName("line_sig");
        sizePolicy6.setHeightForWidth(line_sig->sizePolicy().hasHeightForWidth());
        line_sig->setSizePolicy(sizePolicy6);
        line_sig->setMinimumSize(QSize(224, 38));
        line_sig->setMaximumSize(QSize(224, 38));
        line_sig->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_sig->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.1);\n"
"border-radius: 8px;\n"
"padding:5px;"));
        line_sig->setMaxLength(12);

        horizontalLayout_5->addWidget(line_sig);


        gridLayout_2->addLayout(horizontalLayout_5, 3, 0, 1, 1);

        layout_frame = new QVBoxLayout();
        layout_frame->setObjectName("layout_frame");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(frame_message);
        label_4->setObjectName("label_4");
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);
        label_4->setMinimumSize(QSize(80, 80));
        label_4->setMaximumSize(QSize(47, 100));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: none;;\n"
"color:black;\n"
""));

        horizontalLayout_2->addWidget(label_4);

        lab_avator = new LabelChooseAva(frame_message);
        lab_avator->setObjectName("lab_avator");
        sizePolicy4.setHeightForWidth(lab_avator->sizePolicy().hasHeightForWidth());
        lab_avator->setSizePolicy(sizePolicy4);
        lab_avator->setMinimumSize(QSize(100, 100));
        lab_avator->setMaximumSize(QSize(100, 100));
        lab_avator->setCursor(QCursor(Qt::PointingHandCursor));
        lab_avator->setStyleSheet(QString::fromUtf8("border: 3px solid black;\n"
" border-radius: 8px;"));

        horizontalLayout_2->addWidget(lab_avator);


        layout_frame->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        layout_frame->addItem(verticalSpacer);


        gridLayout_2->addLayout(layout_frame, 0, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        but_yes = new QPushButton(frame_message);
        but_yes->setObjectName("but_yes");
        but_yes->setEnabled(false);
        but_yes->setMinimumSize(QSize(0, 35));
        but_yes->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt \"Microsoft YaHei UI\";\n"
"    background-color: rgb(167, 214, 255); \n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
""));

        horizontalLayout_7->addWidget(but_yes);

        but_no = new QPushButton(frame_message);
        but_no->setObjectName("but_no");
        but_no->setMinimumSize(QSize(0, 35));
        but_no->setCursor(QCursor(Qt::PointingHandCursor));
        but_no->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt 'Microsoft YaHei UI';\n"
"    background-color: rgb(240, 240, 240);\n"
"    color: black; \n"
"    border: 0.5px solid rgb(220, 220, 220);\n"
"    border-radius: 10px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(210, 210, 210, 0.7);\n"
"    color: black;\n"
"    border-radius: 10px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(200, 200, 200, 0.8);\n"
"    color: rgba(0, 0, 0, 0.9);\n"
"    border-radius: 10px;\n"
"}"));

        horizontalLayout_7->addWidget(but_no);


        gridLayout_2->addLayout(horizontalLayout_7, 5, 0, 1, 1);


        gridLayout->addWidget(frame_message, 1, 0, 1, 1);


        retranslateUi(ChangeInformation);

        QMetaObject::connectSlotsByName(ChangeInformation);
    } // setupUi

    void retranslateUi(QDialog *ChangeInformation)
    {
        ChangeInformation->setWindowTitle(QCoreApplication::translate("ChangeInformation", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ChangeInformation", "     \347\274\226\350\276\221\350\265\204\346\226\231", nullptr));
        but_cancelwindow->setText(QCoreApplication::translate("ChangeInformation", "x", nullptr));
        lab_account->setText(QCoreApplication::translate("ChangeInformation", "\350\264\246\345\217\267:", nullptr));
        textEdit_account->setHtml(QCoreApplication::translate("ChangeInformation", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\344\275\240\347\232\204\350\264\246\345\217\267</p></body></html>", nullptr));
        lab_sex->setText(QCoreApplication::translate("ChangeInformation", "\346\200\247\345\210\253:", nullptr));
        cbbox_sex->setItemText(0, QCoreApplication::translate("ChangeInformation", "\345\205\266\344\273\226", nullptr));
        cbbox_sex->setItemText(1, QCoreApplication::translate("ChangeInformation", "\347\224\267", nullptr));
        cbbox_sex->setItemText(2, QCoreApplication::translate("ChangeInformation", "\345\245\263", nullptr));

#if QT_CONFIG(whatsthis)
        cbbox_sex->setWhatsThis(QCoreApplication::translate("ChangeInformation", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        lab_nickname->setText(QCoreApplication::translate("ChangeInformation", "\346\230\265\347\247\260:", nullptr));
        line_nickname->setText(QCoreApplication::translate("ChangeInformation", "\350\257\267\350\276\223\345\205\245\346\230\265\347\247\260", nullptr));
        lab_sig->setText(QCoreApplication::translate("ChangeInformation", "\347\255\276\345\220\215:", nullptr));
        line_sig->setText(QCoreApplication::translate("ChangeInformation", "\350\257\267\350\276\223\345\205\245\344\270\252\346\200\247\347\255\276\345\220\215", nullptr));
        label_4->setText(QCoreApplication::translate("ChangeInformation", "\345\244\264\345\203\217:", nullptr));
        lab_avator->setText(QString());
        but_yes->setText(QCoreApplication::translate("ChangeInformation", "\347\241\256\350\256\244", nullptr));
        but_no->setText(QCoreApplication::translate("ChangeInformation", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangeInformation: public Ui_ChangeInformation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEINFORMATION_H
