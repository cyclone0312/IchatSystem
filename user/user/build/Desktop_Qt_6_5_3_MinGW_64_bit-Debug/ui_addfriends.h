/********************************************************************************
** Form generated from reading UI file 'addfriends.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIENDS_H
#define UI_ADDFRIENDS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <addfriends.h>

QT_BEGIN_NAMESPACE

class Ui_AddFriends
{
public:
    QGridLayout *gridLayout;
    QPushButton *but_deletewindow;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    LineSerach_2 *line_serach;
    QPushButton *but_fangdajing;
    QFrame *frame_message;
    QGridLayout *gridLayout_2;
    QVBoxLayout *layout_frame;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *AddFriends)
    {
        if (AddFriends->objectName().isEmpty())
            AddFriends->setObjectName("AddFriends");
        AddFriends->resize(530, 530);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AddFriends->sizePolicy().hasHeightForWidth());
        AddFriends->setSizePolicy(sizePolicy);
        AddFriends->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        AddFriends->setStyleSheet(QString::fromUtf8("border:none"));
        AddFriends->setModal(false);
        gridLayout = new QGridLayout(AddFriends);
        gridLayout->setObjectName("gridLayout");
        but_deletewindow = new QPushButton(AddFriends);
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

        gridLayout->addWidget(but_deletewindow, 0, 1, 1, 1, Qt::AlignmentFlag::AlignRight);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        line_serach = new LineSerach_2(AddFriends);
        line_serach->setObjectName("line_serach");
        sizePolicy.setHeightForWidth(line_serach->sizePolicy().hasHeightForWidth());
        line_serach->setSizePolicy(sizePolicy);
        line_serach->setMinimumSize(QSize(480, 30));
        line_serach->setMaximumSize(QSize(460, 30));
        line_serach->setMouseTracking(false);
        line_serach->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        line_serach->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
"border: none;\n"
"padding:5px;\n"
"font: 400 11pt \"Microsoft YaHei UI Light\";\n"
"color:grey;"));
        line_serach->setMaxLength(12);

        horizontalLayout->addWidget(line_serach);

        but_fangdajing = new QPushButton(AddFriends);
        but_fangdajing->setObjectName("but_fangdajing");
        sizePolicy.setHeightForWidth(but_fangdajing->sizePolicy().hasHeightForWidth());
        but_fangdajing->setSizePolicy(sizePolicy);
        but_fangdajing->setMinimumSize(QSize(30, 30));
        but_fangdajing->setCursor(QCursor(Qt::PointingHandCursor));
        but_fangdajing->setStyleSheet(QString::fromUtf8("background:transparent;\n"
" border: none;\n"
""));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::EditFind")));
        but_fangdajing->setIcon(icon);
        but_fangdajing->setAutoDefault(false);

        horizontalLayout->addWidget(but_fangdajing, 0, Qt::AlignmentFlag::AlignLeft);


        horizontalLayout_3->addLayout(horizontalLayout);


        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 3);

        frame_message = new QFrame(AddFriends);
        frame_message->setObjectName("frame_message");
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(210);
        sizePolicy2.setHeightForWidth(frame_message->sizePolicy().hasHeightForWidth());
        frame_message->setSizePolicy(sizePolicy2);
        frame_message->setMinimumSize(QSize(0, 210));
        frame_message->setStyleSheet(QString::fromUtf8("border-radius: 10px;\n"
"border: 0.5px solid rgb(180, 180, 180);\n"
""));
        frame_message->setFrameShape(QFrame::Shape::StyledPanel);
        frame_message->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame_message);
        gridLayout_2->setObjectName("gridLayout_2");
        layout_frame = new QVBoxLayout();
        layout_frame->setObjectName("layout_frame");

        gridLayout_2->addLayout(layout_frame, 0, 0, 1, 1);


        gridLayout->addWidget(frame_message, 2, 0, 1, 3);

        label = new QLabel(AddFriends);
        label->setObjectName("label");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        label->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        label->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Microsoft YaHei UI\";"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(1, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 2, 1, 1);


        retranslateUi(AddFriends);

        QMetaObject::connectSlotsByName(AddFriends);
    } // setupUi

    void retranslateUi(QDialog *AddFriends)
    {
        AddFriends->setWindowTitle(QCoreApplication::translate("AddFriends", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
#if QT_CONFIG(tooltip)
        AddFriends->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        but_deletewindow->setText(QCoreApplication::translate("AddFriends", "x", nullptr));
        line_serach->setText(QCoreApplication::translate("AddFriends", "\350\276\223\345\205\245\345\257\271\346\226\271\350\264\246\345\217\267\346\220\234\347\264\242", nullptr));
        but_fangdajing->setText(QString());
        label->setText(QCoreApplication::translate("AddFriends", "     \346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddFriends: public Ui_AddFriends {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIENDS_H
