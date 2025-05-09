/********************************************************************************
** Form generated from reading UI file 'choicedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOICEDIALOG_H
#define UI_CHOICEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <autocleartextedit.h>

QT_BEGIN_NAMESPACE

class Ui_ChoiceDialog
{
public:
    QGridLayout *gridLayout;
    AutoClearTextEdit *textEdit_notice;
    QPushButton *but_no;
    QPushButton *but_yes;

    void setupUi(QDialog *ChoiceDialog)
    {
        if (ChoiceDialog->objectName().isEmpty())
            ChoiceDialog->setObjectName("ChoiceDialog");
        ChoiceDialog->resize(247, 142);
        gridLayout = new QGridLayout(ChoiceDialog);
        gridLayout->setObjectName("gridLayout");
        textEdit_notice = new AutoClearTextEdit(ChoiceDialog);
        textEdit_notice->setObjectName("textEdit_notice");
        textEdit_notice->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        textEdit_notice->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 12pt \"Microsoft YaHei UI\";\n"
"padding:5px;\n"
"background:transparent;"));

        gridLayout->addWidget(textEdit_notice, 0, 0, 1, 2);

        but_no = new QPushButton(ChoiceDialog);
        but_no->setObjectName("but_no");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(but_no->sizePolicy().hasHeightForWidth());
        but_no->setSizePolicy(sizePolicy);
        but_no->setMinimumSize(QSize(0, 30));
        but_no->setCursor(QCursor(Qt::PointingHandCursor));
        but_no->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        but_no->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 13pt 'Microsoft YaHei UI';\n"
"    background-color: rgb(238, 238, 238);\n"
"    color: black;\n"
"    border:none;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgb(225, 225, 225);\n"
"    color: red;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(206, 206, 206);\n"
"    color: red;\n"
"    border-radius: 5px;\n"
"}"));

        gridLayout->addWidget(but_no, 1, 0, 1, 1);

        but_yes = new QPushButton(ChoiceDialog);
        but_yes->setObjectName("but_yes");
        sizePolicy.setHeightForWidth(but_yes->sizePolicy().hasHeightForWidth());
        but_yes->setSizePolicy(sizePolicy);
        but_yes->setMinimumSize(QSize(0, 30));
        but_yes->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        but_yes->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        gridLayout->addWidget(but_yes, 1, 1, 1, 1);


        retranslateUi(ChoiceDialog);

        QMetaObject::connectSlotsByName(ChoiceDialog);
    } // setupUi

    void retranslateUi(QDialog *ChoiceDialog)
    {
        ChoiceDialog->setWindowTitle(QCoreApplication::translate("ChoiceDialog", "Dialog", nullptr));
        but_no->setText(QCoreApplication::translate("ChoiceDialog", "\347\241\256\345\256\232", nullptr));
        but_yes->setText(QCoreApplication::translate("ChoiceDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChoiceDialog: public Ui_ChoiceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOICEDIALOG_H
