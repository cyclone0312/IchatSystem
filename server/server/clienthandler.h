#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

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
#include <QRandomGenerator>
#include <QMutex>
#include <QBuffer>
#include <QJsonArray>
#include <QReadWriteLock>
#include <QTimer>
#include "server.h"
#include "connectionpool.h"

class Server;

class ClientHandler : public QObject//用来处理消息的类
{
    Q_OBJECT
public:
    ClientHandler(QTcpSocket *socket,ConnectionPool& pool,Server *srv);
    ~ClientHandler();
    bool databasesConnect();//连接数据库
    void addClient(const QString &account, ClientHandler *client);//将登录成功的用户增加到哈希存储
    void removeClient(const QString &account);//用户断开连接 删除对应的哈希存储
    ClientHandler* getClient(const QString &account);//获得某个账号对应的ClientHandler

public slots:
    void onReadyRead();//读取用户发送的数据
    void onDisconnected();//有用户断联了
    void receiveMessage(const QJsonObject &json);//收到别的客户端发送的消息 然后转发

public://接收到客户端发送的消息 然后回发
    QString pixmapToBase64(const QPixmap &pixmap);//图片转换成base64格式
    QPixmap base64ToPixmap(const QString &base64Str);//将base64转换为图片
    void dealLogin(const QJsonObject &json);//处理登录请求
    void dealRegister(const QJsonObject &json);//处理注册请求
    void dealAvator(const QString &avator);//上传头像
    void dealAskforavator(const QString &qqnum);//处理索要头像的请求
    void dealFindpassword1(const QString &qqnum);//看看找回的密码是不是有这个账号
    void dealFindpassword2(const QJsonObject &json);//查看密保问题回答对了没有 json和全局变量jsonobj一样
    void dealFindpassword3(const QJsonObject &json);//修改密码 json和全局变量jsonobj一样
    void dealLoginFirst(const QJsonObject &json);//处理用户登录成功后初始化
    void dealDeleteFriend(const QJsonObject &json);//处理删除好友的操作
    void dealSerachAccount(const QJsonObject &json);//处理用户搜索账户信息的功能
    void dealChangeInformation(const QJsonObject &json);//处理用户更新个人信息的功能
    void dealChangePassword(const QJsonObject &json);//处理用户更新密码的功能
    void dealChangePassword2(const QJsonObject &json);//处理用户更新密码的功能
    void dealLogout(const QJsonObject &json);//处理用户注销的功能
    void dealAddFriends(const QJsonObject &json);//处理用发送户添加好友申请的功能
    void dealAddNewFriends(const QJsonObject &json);//处理用户回应是否添加好友的功能
    void dealMessages(const QJsonObject json);//处理用户发送的消息
    void sendNextMessage();//从队列发送下一条消息(处理文件)
    void dealAskDocument(const QJsonObject &json);//处理用户要下载文件的要求

public://接收到别的服务器发送的消息 然后转发
    void forwordAddFriendRequest(const QJsonObject &json);//向在线用户转发用户好友申请
    void forwordRequestPass(const QJsonObject &json);//向在线用户更新他的好友列表(他的好友申请被通过)
    void forwordYouAreDeleted(const QJsonObject &json);//向在线用户更新他的好友列表(他被删除了)
    void forwordKickedOffline(const QJsonObject &json);//把在线用户挤下线
    void forwordMessages(const QJsonObject &json);//向在线用户发送聊天消息

signals:
    void sendMessage(const QJsonObject &json);//给别的客户端发送消息

private:
    QMutex dbMutex;
    QReadWriteLock lock;//读写锁
    QMutex mutex;
    ConnectionPool& pool;
    QSqlDatabase db;
    QTcpSocket *socket;
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QByteArray jsonData;
    QByteArray bufferedData;
    Server *srv;//指向服务器的指针
    QString randomNumber;//注册随机生成账号
    QString account = "0";//登录成功存储账号
    QQueue<QJsonObject> messageQueue; //用于存储待发送的消息
    bool isSending = false; //用于标记消息是否正在发送
    int flag = 0;
};


class DocumentWorker : public QObject//负责查询的类
{
    Q_OBJECT
public:
    DocumentWorker(const QString &filename, const QString &timestamp, QSqlDatabase db)
        : filename(filename), timestamp(timestamp), db(db) {}

signals:
    void resultReady(const QJsonObject &result);

public slots:
    void process();//处理查询文件

private:
    QString filename;
    QString timestamp;
    QSqlDatabase db;
};



#endif // CLIENTHANDLER_H
