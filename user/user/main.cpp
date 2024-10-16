#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTcpSocket>
#include <QPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "user_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Login w;
    //使用std::unique_ptr管理 QTcpSocket的实例
    auto socket = std::make_unique<QTcpSocket>();
    //使用QPointer来管理 MainWindow的指针
    QPointer<MainWindow> mainWind = nullptr;
    //连接login窗口的 loginsucceed信号到槽
    QObject::connect(&w, &Login::loginSucceed, [&](const QString &account) {
        if (!mainWind) {
            mainWind = new MainWindow(account);
            mainWind->show();
        }
    });
    w.show();
    //////////////////////////////////////////////////////////////////
    Login w2;
    //使用std::unique_ptr管理 QTcpSocket的实例
    auto socket2 = std::make_unique<QTcpSocket>();
    //使用QPointer来管理 MainWindow的指针
    QPointer<MainWindow> mainWind2 = nullptr;
    //连接login 窗口的 loginsucceed信号到槽
    QObject::connect(&w2, &Login::loginSucceed, [&](const QString &account) {
        if (!mainWind2) {
            mainWind2 = new MainWindow(account);
            mainWind2->show();
        }
    });
    w2.show();
    return a.exec();
}
