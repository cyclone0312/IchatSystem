/********************************************************************************
** Form generated from reading UI file 'cutavator.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUTAVATOR_H
#define UI_CUTAVATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <cutavator.h>

QT_BEGIN_NAMESPACE

class Ui_CutAvator
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *but_cut;
    QPushButton *but_yes;
    QDockWidget *dock_avator;
    QWidget *dock;
    DraggableResizableLabel *lab_cut;

    void setupUi(QDialog *CutAvator)
    {
        if (CutAvator->objectName().isEmpty())
            CutAvator->setObjectName("CutAvator");
        CutAvator->resize(394, 376);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CutAvator->sizePolicy().hasHeightForWidth());
        CutAvator->setSizePolicy(sizePolicy);
        CutAvator->setMinimumSize(QSize(311, 311));
        CutAvator->setMaximumSize(QSize(900, 900));
        CutAvator->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        gridLayout = new QGridLayout(CutAvator);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        but_cut = new QPushButton(CutAvator);
        but_cut->setObjectName("but_cut");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(but_cut->sizePolicy().hasHeightForWidth());
        but_cut->setSizePolicy(sizePolicy1);
        but_cut->setMinimumSize(QSize(80, 30));
        but_cut->setCursor(QCursor(Qt::PointingHandCursor));
        but_cut->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        but_cut->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt 'Microsoft YaHei UI';\n"
"    background-color: rgb(5, 186, 251);\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(5, 186, 251, 0.7);\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 123, 255, 0.8);\n"
"    color: rgba(255, 255, 255, 0.9);\n"
"    border-radius: 10px;\n"
"}"));

        horizontalLayout->addWidget(but_cut);

        but_yes = new QPushButton(CutAvator);
        but_yes->setObjectName("but_yes");
        sizePolicy1.setHeightForWidth(but_yes->sizePolicy().hasHeightForWidth());
        but_yes->setSizePolicy(sizePolicy1);
        but_yes->setMinimumSize(QSize(80, 30));
        but_yes->setCursor(QCursor(Qt::PointingHandCursor));
        but_yes->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        but_yes->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font: 12pt 'Microsoft YaHei UI';\n"
"    background-color: rgb(5, 186, 251);\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(5, 186, 251, 0.7);\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(0, 123, 255, 0.8);\n"
"    color: rgba(255, 255, 255, 0.9);\n"
"    border-radius: 10px;\n"
"}"));

        horizontalLayout->addWidget(but_yes);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        dock_avator = new QDockWidget(CutAvator);
        dock_avator->setObjectName("dock_avator");
        sizePolicy.setHeightForWidth(dock_avator->sizePolicy().hasHeightForWidth());
        dock_avator->setSizePolicy(sizePolicy);
        dock_avator->setMinimumSize(QSize(277, 300));
        dock_avator->setMaximumSize(QSize(277, 300));
        dock_avator->setStyleSheet(QString::fromUtf8("  QDockWidget::title {\n"
"    background: rgb(241, 231, 234)\n"
"        }"));
        dock = new QWidget();
        dock->setObjectName("dock");
        lab_cut = new DraggableResizableLabel(dock);
        lab_cut->setObjectName("lab_cut");
        lab_cut->setGeometry(QRect(0, 0, 277, 283));
        lab_cut->setStyleSheet(QString::fromUtf8("border: 2px dashed rgb(0, 0, 0);\n"
"color:rgb(0, 0, 127);\n"
""));
        lab_cut->setAlignment(Qt::AlignmentFlag::AlignCenter);
        dock_avator->setWidget(dock);

        gridLayout->addWidget(dock_avator, 0, 0, 1, 1, Qt::AlignmentFlag::AlignHCenter);


        retranslateUi(CutAvator);

        QMetaObject::connectSlotsByName(CutAvator);
    } // setupUi

    void retranslateUi(QDialog *CutAvator)
    {
        CutAvator->setWindowTitle(QCoreApplication::translate("CutAvator", "\347\274\226\350\276\221\345\244\264\345\203\217", nullptr));
        but_cut->setText(QCoreApplication::translate("CutAvator", "\350\243\201\345\211\252", nullptr));
        but_yes->setText(QCoreApplication::translate("CutAvator", "\347\241\256\350\256\244", nullptr));
        lab_cut->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CutAvator: public Ui_CutAvator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUTAVATOR_H
