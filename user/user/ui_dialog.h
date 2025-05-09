/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <autocleartextedit.h>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *but_delete;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    AutoClearTextEdit *textEdit_notice;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->setWindowModality(Qt::WindowModality::WindowModal);
        Dialog->resize(250, 140);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_3 = new QSpacerItem(50, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        but_delete = new QPushButton(Dialog);
        but_delete->setObjectName("but_delete");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(but_delete->sizePolicy().hasHeightForWidth());
        but_delete->setSizePolicy(sizePolicy);
        but_delete->setMinimumSize(QSize(0, 30));
        but_delete->setCursor(QCursor(Qt::PointingHandCursor));
        but_delete->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        but_delete->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 13pt 'Microsoft YaHei UI';\n"
"    background-color: rgb(238, 238, 238);\n"
"    color: black;\n"
"    border:none;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgb(225, 225, 225);\n"
"    color: black;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(206, 206, 206);\n"
"    color: black;\n"
"    border-radius: 5px;\n"
"}"));

        horizontalLayout->addWidget(but_delete);

        horizontalSpacer_4 = new QSpacerItem(50, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(13, 87, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        textEdit_notice = new AutoClearTextEdit(Dialog);
        textEdit_notice->setObjectName("textEdit_notice");
        textEdit_notice->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_notice->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 12pt \"Microsoft YaHei UI\";\n"
"padding:5px;\n"
"background:transparent;"));

        horizontalLayout_2->addWidget(textEdit_notice);

        horizontalSpacer_2 = new QSpacerItem(13, 87, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        but_delete->setText(QCoreApplication::translate("Dialog", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
