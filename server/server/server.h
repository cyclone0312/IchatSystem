#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QTableView>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QByteArray>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>
#include <QAbstractSocket>
#include <QMutex>

class ClientHandler;

QT_BEGIN_NAMESPACE
namespace Ui {
class Server;
}
QT_END_NAMESPACE


class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    bool databasesConnect();//连接数据库
    void showTable(const QString &tablename);//显示表格
    bool tcpListen();//建立连接，监听对象

private slots:
    void on_whichtable_currentIndexChanged(int index);//切换表格
    void on_sqlsubmit_clicked();//使用SQL语句
    void onNewConnection();//有新连接到来新建clienthandler
    void on_listen_clicked();//点击开启/关闭服务器 启动/关闭监听
    void on_pushButton_clicked();//刷新表格

private:
    Ui::Server *ui;
    QThreadPool *threadPool;//线程池
    QMutex mapMutex; // 互斥量，确保线程安全
    QSqlDatabase db;
    QSqlQuery qry;
    QTcpServer *TCP;
    bool listenFlag = false;

public:
    QHash<QString, ClientHandler*> clientsMap;//存储账号与ClientHandler的映射 共享资源
};

#endif // SERVER_H
