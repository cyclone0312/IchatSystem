/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Server
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *whichtable;
    QPushButton *listen;
    QPushButton *pushButton;
    QTableView *table;
    QHBoxLayout *horizontalLayout;
    QLineEdit *sql;
    QPushButton *sqlsubmit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Server)
    {
        if (Server->objectName().isEmpty())
            Server->setObjectName("Server");
        Server->resize(1100, 600);
        Server->setMinimumSize(QSize(1100, 600));
        Server->setMaximumSize(QSize(1100, 600));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::CallStart")));
        Server->setWindowIcon(icon);
        centralwidget = new QWidget(Server);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(20, 10, 1061, 561));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        whichtable = new QComboBox(layoutWidget);
        whichtable->addItem(QString());
        whichtable->addItem(QString());
        whichtable->addItem(QString());
        whichtable->addItem(QString());
        whichtable->setObjectName("whichtable");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(50);
        sizePolicy.setVerticalStretch(50);
        sizePolicy.setHeightForWidth(whichtable->sizePolicy().hasHeightForWidth());
        whichtable->setSizePolicy(sizePolicy);
        whichtable->setMinimumSize(QSize(150, 32));
        whichtable->setStyleSheet(QString::fromUtf8("font: 10pt \"Microsoft YaHei UI\";\n"
"\n"
""));

        horizontalLayout_2->addWidget(whichtable);

        listen = new QPushButton(layoutWidget);
        listen->setObjectName("listen");
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(9);
        sizePolicy1.setHeightForWidth(listen->sizePolicy().hasHeightForWidth());
        listen->setSizePolicy(sizePolicy1);
        listen->setMinimumSize(QSize(80, 27));
        listen->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font: 10pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"}\n"
"  QPushButton:pressed {\n"
"font: 10pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"background-color: rgb(255, 251, 255); \n"
"   }"));
        listen->setAutoDefault(false);
        listen->setFlat(false);

        horizontalLayout_2->addWidget(listen);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName("pushButton");
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(10);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(80, 27));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font: 10pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"}\n"
"  QPushButton:pressed {\n"
"font: 10pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"background-color: rgb(255, 251, 255); \n"
"   }"));

        horizontalLayout_2->addWidget(pushButton, 0, Qt::AlignmentFlag::AlignLeft);


        verticalLayout->addLayout(horizontalLayout_2);

        table = new QTableView(layoutWidget);
        table->setObjectName("table");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(50);
        sizePolicy3.setVerticalStretch(50);
        sizePolicy3.setHeightForWidth(table->sizePolicy().hasHeightForWidth());
        table->setSizePolicy(sizePolicy3);
        table->setStyleSheet(QString::fromUtf8("font: 10pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
""));
        table->setAlternatingRowColors(false);
        table->horizontalHeader()->setCascadingSectionResizes(false);
        table->verticalHeader()->setCascadingSectionResizes(false);
        table->verticalHeader()->setProperty("showSortIndicator", QVariant(false));

        verticalLayout->addWidget(table);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        sql = new QLineEdit(layoutWidget);
        sql->setObjectName("sql");
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(50);
        sizePolicy4.setVerticalStretch(50);
        sizePolicy4.setHeightForWidth(sql->sizePolicy().hasHeightForWidth());
        sql->setSizePolicy(sizePolicy4);
        sql->setStyleSheet(QString::fromUtf8("font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
""));

        horizontalLayout->addWidget(sql);

        sqlsubmit = new QPushButton(layoutWidget);
        sqlsubmit->setObjectName("sqlsubmit");
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(5);
        sizePolicy5.setVerticalStretch(50);
        sizePolicy5.setHeightForWidth(sqlsubmit->sizePolicy().hasHeightForWidth());
        sqlsubmit->setSizePolicy(sizePolicy5);
        sqlsubmit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"}\n"
"  QPushButton:pressed {\n"
"font: 12pt \"Microsoft YaHei UI\";\n"
"border: 1px solid rgba(0, 0, 0, 0.3); \n"
"background-color: rgb(255, 251, 255); \n"
"   }"));

        horizontalLayout->addWidget(sqlsubmit);


        verticalLayout->addLayout(horizontalLayout);

        Server->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Server);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 18));
        Server->setMenuBar(menubar);
        statusbar = new QStatusBar(Server);
        statusbar->setObjectName("statusbar");
        Server->setStatusBar(statusbar);

        retranslateUi(Server);

        listen->setDefault(false);


        QMetaObject::connectSlotsByName(Server);
    } // setupUi

    void retranslateUi(QMainWindow *Server)
    {
        Server->setWindowTitle(QCoreApplication::translate("Server", "server", nullptr));
        whichtable->setItemText(0, QCoreApplication::translate("Server", "Users", nullptr));
        whichtable->setItemText(1, QCoreApplication::translate("Server", "Messages", nullptr));
        whichtable->setItemText(2, QCoreApplication::translate("Server", "Friends", nullptr));
        whichtable->setItemText(3, QCoreApplication::translate("Server", "FriendRequests ", nullptr));

        listen->setText(QCoreApplication::translate("Server", "\345\274\200\345\220\257\346\234\215\345\212\241\345\231\250", nullptr));
        pushButton->setText(QCoreApplication::translate("Server", "\345\210\267\346\226\260\350\241\250\346\240\274", nullptr));
        sql->setText(QCoreApplication::translate("Server", "\350\257\267\350\276\223\345\205\245SQL\350\257\255\345\217\245", nullptr));
        sqlsubmit->setText(QCoreApplication::translate("Server", "\346\217\220\344\272\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Server: public Ui_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
