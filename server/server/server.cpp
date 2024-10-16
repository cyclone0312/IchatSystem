#include "server.h"
#include "ui_server.h"
#include "clienthandler.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    //使用全局线程池
    threadPool = QThreadPool::globalInstance();
    QThreadPool::globalInstance()->setMaxThreadCount(300);
    //连接数据库
    databasesConnect();
    //显示表格
    showTable("Users");
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


Server::~Server(){
    delete ui;
}


bool Server::databasesConnect()//连接数据库
{
    ConnectionPool& pool = ConnectionPool::getInstance();//获取连接池实例
    Server::db = pool.getConnection();//从连接池获取一个数据库连接
    if (!db.isValid()) {//检查连接是否有效
        qDebug() << "打开失败: 获取数据库连接失败";
        return false;//连接获取失败
    } else {
        qDebug() << "数据库连接成功";
        qry = QSqlQuery(db);//使用有效连接创建查询对象
        return true;
    }
}


void Server::showTable(const QString &tablename)//显示表格
{
    QString queryStr;
    if (tablename == "Users") {
        queryStr = "SELECT qq_number,password,nickname,signature,gender,question,answer FROM Users";
    } else if (tablename == "Messages") {
        queryStr = "SELECT sender_id,receiver_id,timestamp,message_type,filename,status FROM Messages";
    } else if (tablename == "Friends") {
        queryStr = "SELECT * FROM Friends";
    } else if (tablename == "FriendRequests") {
        queryStr = "SELECT * FROM FriendRequests";
    }
    //创建模型并设置查询
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(queryStr, Server::db);
    if (model->lastError().isValid()) {
        qDebug() << "查询失败:" << model->lastError().text();
        return;
    }
    qDebug() << "加载行数:" << model->rowCount();
    ui->table->setModel(model);
}


bool Server::tcpListen()//建立连接，监听对象
{
    TCP = new QTcpServer(this);
    connect(TCP, &QTcpServer::newConnection, this, &Server::onNewConnection);
    if (!TCP->listen(QHostAddress::Any, 1999)) {
        qDebug() << "监听失败:" << TCP->errorString();
        return false;
    } else {
        qDebug() << "监听成功";
        return true;
    }
}


void Server::on_whichtable_currentIndexChanged(int index)//切换表格
{
    if(index == 0)
        showTable("Users");
    if(index == 1)
        showTable("Messages");
    if(index == 2)
        showTable("Friends");
    if(index == 3)
        showTable("FriendRequests");
}


void Server::on_sqlsubmit_clicked()//提交执行SQL语句
{
    if (qry.prepare(ui->sql->text()) && qry.exec()) {
        showTable(ui->whichtable->currentText());
    } else {
        qDebug() << "SQL 执行错误:" << qry.lastError().text();
    }
}


void Server::onNewConnection()//有新连接到来新建clienthandler
{
    QTcpSocket *socket = TCP->nextPendingConnection();
    ConnectionPool& pool = ConnectionPool::getInstance();//获取连接池实例
    auto handler = std::make_shared<ClientHandler>(socket, pool, this);//使用 shared_ptr 管理 handler
    QThreadPool::globalInstance()->start([handler, socket]() {
        connect(socket, &QTcpSocket::readyRead, handler.get(), &ClientHandler::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, handler.get(), &ClientHandler::onDisconnected);
        //当 socket 断开连接时，handler 会自动释放
        connect(socket, &QTcpSocket::disconnected, [handler, socket]() {
            socket->deleteLater();
        });
    });
}


void Server::on_listen_clicked()//点击开启/关闭服务器 启动/关闭监听
{
    if(!listenFlag) {
        ui->listen->setText("关闭服务器");
        listenFlag = true;
        tcpListen();//直接调用当前对象的 tcpListen 方法
    } else {
        ui->listen->setText("开启服务器");
        listenFlag = false;
        qDebug()<<"服务器关闭";
        TCP->close();//确保关闭 TCP 服务器
    }
}


void Server::on_pushButton_clicked()//刷新表格
{
    qDebug()<<"当前连接的账号有"<<clientsMap;
    on_whichtable_currentIndexChanged(ui->whichtable->currentIndex());
}


