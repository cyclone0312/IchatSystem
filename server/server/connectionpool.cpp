#include "connectionpool.h"

ConnectionPool& ConnectionPool::getInstance()//获取单例实例
{
    static ConnectionPool instance;//确保是同一个实例
    return instance;
}


QSqlDatabase ConnectionPool::getConnection() // 获取数据库连接
{
    QMutexLocker locker(&mutex);
    if (!pool.isEmpty()) {
        QSqlDatabase db = pool.dequeue();
        if (db.isOpen()) {
            qDebug() << "有现成的连接";
            return db;
        } else {
            db.close();
            qDebug() << "连接已关闭，丢弃该连接，尝试创建新连接...";
            return getConnection();
        }
    }
    //如果没有可用的连接且池未满，则新建连接
    if (pool.size() < maxConnections) {
        QString connectionName = QString("Connection_%1").arg(connectionCounter++);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);//connectionName用来区分不同的连接  返回一个 QSqlDatabase 对象。这个对象代表了你刚刚注册的、指定类型的数据库连接。
        db.setDatabaseName(dbName);
        if (!db.open()) {
            qDebug() << "数据库打开失败:" << db.lastError().text();
            return QSqlDatabase();
        }
        return db; // 返回新连接
    } else {
        qDebug() << "数据库最大连接数已达到!";
        return QSqlDatabase();
    }
}


void ConnectionPool::releaseConnection(QSqlDatabase db)//释放连接
{
    QMutexLocker locker(&mutex);
    if (pool.size() < maxConnections) {
        pool.enqueue(db);
    } else {
        db.close();
    }
}


void ConnectionPool::setMaxConnections(int max)//设置最大连接数
{
    QMutexLocker locker(&mutex);
    maxConnections = max;
}


int ConnectionPool::getMaxConnections() const//获取当前最大连接数
{
    return maxConnections;
}


ConnectionPool::ConnectionPool() : maxConnections(301)//私有构造函数  默认最大连接数
{
    //建立数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qDebug() << "打开数据库失败" << db.lastError().text();
    }
    else{
        //检查和创建表的 SQL 语句
        QSqlQuery query(db);
        //创建用户表
        query.exec("CREATE TABLE IF NOT EXISTS Users ("
                   "qq_number VARCHAR(20) PRIMARY KEY NOT NULL, "
                   "password VARCHAR(255) NOT NULL, "
                   "avator LONGTEXT, "
                   "nickname VARCHAR(50), "
                   "signature TEXT, "
                   "gender TEXT CHECK (gender IN ('男', '女', '其他')), "
                   "question VARCHAR(255), "
                   "answer VARCHAR(255));");
        if (query.lastError().isValid()) {
            qDebug() << "创建用户表失败:" << query.lastError().text();
        }
        //创建聊天消息表
        query.exec("CREATE TABLE IF NOT EXISTS Messages ("
                   "message_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "sender_id VARCHAR(20) NOT NULL, "
                   "receiver_id VARCHAR(20) NOT NULL, "
                   "content LONGTEXT, "
                   "filename VARCHAT(20),"
                   "status VARCHAT(20),"
                   "timestamp TIMESTAMP, "
                   "message_type VARCHAR(20) NOT NULL, "
                   "FOREIGN KEY (sender_id) REFERENCES Users(qq_number) ON DELETE CASCADE);");
        if (query.lastError().isValid()) {
            qDebug() << "创建聊天消息表失败:" << query.lastError().text();
        }
        //创建好友关系表
        query.exec("CREATE TABLE IF NOT EXISTS Friends ("
                   "friendship_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "user_id VARCHAR(20) NOT NULL, "
                   "friend_id VARCHAR(20) NOT NULL, "
                   "FOREIGN KEY (user_id) REFERENCES Users(qq_number) ON DELETE CASCADE, "
                   "FOREIGN KEY (friend_id) REFERENCES Users(qq_number) ON DELETE CASCADE);");
        if (query.lastError().isValid()) {
            qDebug() << "创建好友关系表失败:" << query.lastError().text();
        }
        //创建申请表
        query.exec("CREATE TABLE IF NOT EXISTS FriendRequests ("
                   "request_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "sender_id VARCHAR(20) NOT NULL, "
                   "receiver_id INT NOT NULL, "
                   "request_type TEXT CHECK (request_type IN ('friend', 'group')) NOT NULL, "
                   "status TEXT CHECK (status IN ('pending', 'accepted', 'rejected')) NOT NULL DEFAULT 'pending', "
                   "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                   "FOREIGN KEY (sender_id) REFERENCES Users(qq_number) ON DELETE CASCADE);");
        if (query.lastError().isValid()) {
            qDebug() << "创建申请表失败:" << query.lastError().text();
    }
        db.close();
  }
}


ConnectionPool::~ConnectionPool()//私有析构函数
{
    while (!pool.isEmpty()) {
        QSqlDatabase db = pool.dequeue();
        db.close();//关闭所有连接
    }
}
