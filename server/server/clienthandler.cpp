#include "clienthandler.h"

ClientHandler::ClientHandler(QTcpSocket *socket, ConnectionPool& pool, Server *srv)
    : pool(pool) , socket(socket) , srv(srv)
{
    //连接数据库
    databasesConnect();
}
QMutex ClientHandler::dbMutex;
QReadWriteLock ClientHandler::lock;


ClientHandler::~ClientHandler()
{
    if (db.isOpen()) {//检查数据库是否连接
        pool.releaseConnection(db);
    }
    this->disconnect();
    qDebug()<<"某个handler被释放";
}


void ClientHandler::run()//启用事件循环
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
    qDebug()<<"当前所在线程"<<QThread::currentThread()->currentThreadId();
}


bool ClientHandler::databasesConnect()//连接数据库
{
    //检查当前数据库连接是否有效
    if (db.isOpen()) {
        return true;//如果已连接，直接返回 true
    }
    db = pool.getConnection();//从连接池中获取数据库连接
    if (!db.isValid()) {//检查连接是否有效
        qDebug() << " clienthandler获取数据库连接失败";
        return false;
    }
    return true;
}


void ClientHandler::addClient(const QString &account, ClientHandler *client)//将登录成功的用户增加到哈希存储
{
    QWriteLocker locker(&lock);//获取写锁
    if (!srv->clientsMap.contains(account)) {
        srv->clientsMap.insert(account, client);
    } else {
        //处理用户名已存在的情况
    }
}


void ClientHandler::removeClient(const QString &account)//用户断开连接 删除对应的哈希存储
{
    QWriteLocker locker(&lock);//获取写锁
    if (srv->clientsMap.contains(account)) {
        srv->clientsMap.remove(account);//从哈希表中移除账号映射
    } else {
        //处理用户不存在的情况
    }
}


ClientHandler* ClientHandler::getClient(const QString &account)//获得某个账号对应的ClientHandler
{
    QReadLocker locker(&lock);//获取读锁
    if (srv->clientsMap.contains(account)) {
        return srv->clientsMap.value(account);//返回对应的 ClientHandler 指针
    }
    return nullptr;//如果用户不存在，返回 nullptr
}


void ClientHandler::onReadyRead()//读取用户发送的数据
{
    if(flag==0){
        flag=1;
        jsonData.clear();
    }
    //从套接字读取数据
    jsonData += socket->readAll();//使用 += 追加接收的数据
    //检查数据是否以"END"结尾
    while (jsonData.endsWith("END")) {
        flag=0;
        //删除"END"结束符
        jsonData.chop(3);//移除结尾的"END"
        //确保 jsondata 完整，尝试将其解析为 JSON 文档
        jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull()) {
            qDebug() << "未能解析 JSON 数据，当前数据是:" ;
            return;//有可能数据未完整，提前返回
        }
        //处理解析成功的 JSON 对象
        jsonObj = jsonDoc.object();
        //连接数据库
        if (!databasesConnect()) return;
        //根据 tag 处理不同的请求
        if (jsonObj["tag"] == "login") {
            dealLogin(jsonObj);
        }
        else if (jsonObj["tag"] == "register") {
            dealRegister(jsonObj);
        }
        else if (jsonObj["tag"] == "askforavator") {
            dealAskforavator(jsonObj["qq_number"].toString());
        }
        else if (jsonObj["tag"] == "myavator") {
            dealAvator(jsonObj["avator"].toString());
        }
        else if (jsonObj["tag"] == "findpassword1") {
            dealFindpassword1(jsonObj["qq_number"].toString());
        }
        else if (jsonObj["tag"] == "findpassword2") {
            dealFindpassword2(jsonObj);
        }
        else if (jsonObj["tag"] == "findpassword3") {
            dealFindpassword3(jsonObj);
        }
        else if (jsonObj["tag"] == "loginfirst") {
            dealLoginFirst(jsonObj);
        }
        else if (jsonObj["tag"] == "deletefriend") {
            dealDeleteFriend(jsonObj);
        }
        else if (jsonObj["tag"] == "serachfriend") {
            dealSerachAccount(jsonObj);
        }
        else if (jsonObj["tag"] == "changeinformation") {
            dealChangeInformation(jsonObj);
        }
        else if (jsonObj["tag"] == "changepassword1") {
            dealChangePassword(jsonObj);
        }
        else if (jsonObj["tag"] == "changepassword2") {
            dealChangePassword2(jsonObj);
        }
        else if (jsonObj["tag"] == "logout") {
            dealLogout(jsonObj);
        }
        else if (jsonObj["tag"] == "addfriend") {
            dealAddFriends(jsonObj);//处理用户发送添加好友申请的功能
        }
        else if (jsonObj["tag"] == "newfriends") {
            dealAddNewFriends(jsonObj);//处理用户回应是否添加好友的功能
        }
        else if (jsonObj["tag"] == "messages") {
            dealMessages(jsonObj);//处理用户发送的消息
        }
        else if (jsonObj["tag"] == "askfordocument") {
            dealAskDocument(jsonObj);//处理用户要下载文件的
        }
    }
}


void ClientHandler::onDisconnected()//有用户断联了
{
    qDebug() << "断联一位";
    if(account != "0"){
        removeClient(account);
    }
    this->deleteLater();
}


void ClientHandler::receiveMessage(const QJsonObject &json)//收到别的客户端发送的消息 然后转发
{
    if(json["tag"] == "addfriend"){//向在线用户转发用户好友申请
        forwordAddFriendRequest(json);
    }
    else if(json["tag"] == "requestpass"){//向在线用户更新他的好友列表(他的好友申请被通过)
        forwordRequestPass(json);
    }
    else if(json["tag"] == "youaredeleted"){//向在线用户更新他的好友列表(他被删除了)
        forwordYouAreDeleted(json);
    }
    else if(json["tag"] == "kickedoffline"){//把在线用户挤下线
        forwordKickedOffline(json);
    }
    else if(json["tag"] == "yourmessages"){//聊天消息
        forwordMessages(json);
    }
}


QString  ClientHandler::pixmapToBase64(const QPixmap &pixmap)//图片转换成base64格式
{
    QImage image = pixmap.toImage();
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray byteArray = buffer.data();
    return QString(byteArray.toBase64());
}


QPixmap ClientHandler::base64ToPixmap(const QString &base64Str)//将base64转换为图片
{
    QByteArray byteArray = QByteArray::fromBase64(base64Str.toUtf8());
    QImage image;
    //尝试加载PNG格式
    if (!image.loadFromData(byteArray, "PNG")) {
        //如果PNG加载失败，尝试JPG格式
        image.loadFromData(byteArray, "JPG");
    }
    return QPixmap::fromImage(image);
}

void ClientHandler::dealLogin(const QJsonObject &json)//处理登录请求
{
    // 声明一个布尔标志，用于指示是否需要发送“被挤下线”通知
    bool flag = false;
    // 声明一个 ClientHandler 指针，用于存储找到的、已经登录的同一账号的客户端处理器
    ClientHandler *it = nullptr;

    // 使用 QMutexLocker 锁定 dbMutex 互斥量
    // 确保在进行数据库操作时，其他线程无法同时访问受此互斥量保护的共享数据库资源（例如连接池或共享连接）
    { // 这是一个作用域块，当退出此块时，QMutexLocker 会自动解锁互斥量
        QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全

        QSqlQuery qry(db);

        // 准备 SQL 查询语句：查询 Users 表中 qq_number 和 password 都匹配的记录数量。
        // 使用参数绑定 (:qq_number, :password) 是为了安全（防止SQL注入）和正确处理数据类型。
        qry.prepare("SELECT COUNT(*) FROM Users WHERE qq_number = :qq_number AND password = :password");

        // 绑定 QQ 号码参数：从输入的 json 对象中获取 "qq_number" 字段的值，并将其绑定到 SQL 语句中的 :qq_number 占位符。
        // toString() 将 QJsonValue 转换为 QString，toVariant() 将其封装为 QVariant 以便绑定。
        qry.bindValue(":qq_number", json["qq_number"].toString());

        // 绑定密码参数：从输入的 json 对象中获取 "password" 字段的值，并将其绑定到 SQL 语句中的 :password 占位符。
        qry.bindValue(":password", json["password"].toString());

        // 声明一个 QJsonObject 用于构建发送给客户端的响应消息
        QJsonObject qjsonObj;

        // 执行准备好的 SQL 查询
        if(qry.exec()){
            // 如果查询执行成功（不代表找到记录，只代表 SQL 语句本身执行无误）
            // 移动结果集指针到第一条记录。COUNT(*) 查询总会返回一条记录。
            qry.next();
            // 获取结果集中第一列（索引为 0，即 COUNT(*) 的结果）的值，并转换为整数
            int count = qry.value(0).toInt();

            // 检查计数结果，如果等于 1，表示用户名和密码都正确，登录成功
            if(count==1){//登录成功
                // 尝试查找是否有其他 ClientHandler 实例已经使用同一个 QQ 号登录
                it = getClient(json["qq_number"].toString());
                // 如果 it 不为 nullptr，说明找到了一个正在使用此账号的客户端处理器，即该账号已在别处登录
                if(it){//不是空指针 说明有人已经登录这个账号了 给那个客户端发送被挤下去的通知
                    // 打印调试信息，表明该账号已经在登录中
                    qDebug()<<json["qq_number"]<<"已经在登录啦";
                    // 设置 flag 为 true，标记需要向那个已经登录的客户端发送“被挤下线”通知
                    flag = true;
                }
                // 如果 it 是空指针，则表示该账号当前没有其他在线连接，可以正常登录

                // 构建登录成功响应 JSON 对象
                qjsonObj["tag"] = "login"; // 消息标签为 "login"
                qjsonObj["answer"] = "dengluchenggong"; // 回复为 "登录成功"

                // 将响应 JSON 对象转换为 QJsonDocument
                jsonDoc=QJsonDocument(qjsonObj);
                // 将 QJsonDocument 转换为 JSON 格式的 QByteArray
                jsonData = jsonDoc.toJson();
                // 添加消息结束标识符 "END"，用于在流式传输中区分消息边界
                QByteArray messageWithSeparator = jsonData + "END";
                // 将带分隔符的 JSON 数据写入到当前客户端的 socket 中
                socket->write(messageWithSeparator);
                // 强制发送缓冲区中的数据
                socket->flush();
                // 清空发送缓冲区
                jsonData.clear();
            }
            else{//登录失败
                // 如果计数结果不等于 1，表示用户名或密码不正确，登录失败
                // 构建登录失败响应 JSON 对象
                qjsonObj["tag"] = "login"; // 消息标签为 "login"
                qjsonObj["answer"] = "denglushibai"; // 回复为 "登录失败"

                // 将响应 JSON 对象转换为 QJsonDocument 并转为 QByteArray
                jsonDoc = QJsonDocument(qjsonObj);
                jsonData = jsonDoc.toJson();

                // 发送登录失败响应给当前客户端
                // (这段 JSON 转换和发送代码也重复了一次，可以优化)
                jsonDoc = QJsonDocument(qjsonObj);
                jsonData = jsonDoc.toJson();
                // 添加消息结束标识符 "END"
                QByteArray messageWithSeparator = jsonData + "END";
                // 将数据写入 socket 并发送
                socket->write(messageWithSeparator);
                socket->flush();
                // 清空缓冲区
                jsonData.clear();
            }
        }
        // 如果 qry.exec() 执行失败（例如 SQL 语句有语法错误，或数据库连接有问题），则会跳过上面的 if(qry.exec()){...} 块
        // 注意: 在这里，如果使用了连接池，应该调用 pool.releaseConnection(db) 来归还连接。但代码中没有显示。
    } // QMutexLocker 在这里超出作用域，dbMutex 自动解锁

    // 在数据库操作（锁定区域）之外，如果 flag 为 true（表示在登录成功的数据库查询中发现账号已在别处登录）
    if(flag){
        // 构建一个“被挤下线”的通知消息 JSON 对象
        QJsonObject qjson;
        qjson["tag"] = "kickedoffline"; // 消息标签为 "kickedoffline"

        // 建立一个临时的信号与槽连接：当当前的 ClientHandler 发射 sendMessage 信号时，调用之前找到的那个 ClientHandler (it) 的 receiveMessage 槽
        // 这里的 this 是当前正在处理登录请求的 ClientHandler 实例，it 是之前已经登录的那个 ClientHandler 实例
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        // 发射 sendMessage 信号，将“被挤下线”消息发送给那个已经登录的客户端处理器
        emit sendMessage(qjson);
        // 使用一个延迟为 0 的单次定时器来安排在当前事件循环迭代结束后断开信号与槽的连接
        // 这是为了确保 sendMessage 信号发射后，it->receiveMessage 槽有机会执行完毕后再断开连接
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}

void ClientHandler::dealRegister(const QJsonObject &json)//处理注册请求
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    //生成1到9的随机数字
    int firstDigit = 1;//第一位固定为1
    //生成第二到十位的随机数字
    randomNumber = QString::number(firstDigit);
    for (int i = 1; i < 10; ++i) {
        int randomDigit = QRandomGenerator::global()->bounded(0, 10);//0到9
        randomNumber.append(QString::number(randomDigit));
    }
    qry.prepare("SELECT COUNT(*) FROM Users WHERE qq_number = :qq_number");
    qry.bindValue(":qq_number", randomNumber);
    if(qry.exec()){
        qry.next();
        int count = qry.value(0).toInt();
        if(count == 1){//如果有这个账号 则再次执行处理注册函数
            dealRegister(json);
            return;
        }
        if (count == 0) {//如果没有这个账号，则插入所有数据
            QString nickname = json["nickname"].toString();
            QString gender = json["gender"].toString();
            QString password = json["password"].toString();
            QString signature = "快快和我聊天吧。";//默认个性签名
            //准备 SQL 插入语句
            qry.prepare("INSERT INTO Users (qq_number, nickname, signature, gender, password, question,answer) "
                        "VALUES (:qq_number, :nickname, :signature, :gender, :password, :question,:answer)");
            //绑定参数
            qry.bindValue(":qq_number", randomNumber);
            qry.bindValue(":nickname", nickname);
            qry.bindValue(":signature", signature);
            qry.bindValue(":gender", gender);
            qry.bindValue(":password", password);
            qry.bindValue(":question", jsonObj["question"].toString());
            qry.bindValue(":answer", jsonObj["answer"].toString());
            //执行查询
            if (!qry.exec()) {
                qDebug() << "用户注册失败" << qry.lastError().text();
                db.rollback();
                return;
            } else {
                db.commit();
                qDebug() << "用户注册成功(没头像)";
            }
        }
    }
    //插入后向客户端发送消息申请获得头像
    qDebug()<<"申请我要一个头像";
    QJsonObject qjsonObj;
    qjsonObj["tag"] = "register_askforavator";
    qjsonObj["answer"]="askforavator";
    jsonDoc = QJsonDocument(qjsonObj);
    jsonData = jsonDoc.toJson();
    //发送消息
    jsonDoc = QJsonDocument(qjsonObj);
    jsonData = jsonDoc.toJson();
    //添加标识符
    QByteArray messageWithSeparator = jsonData + "END";
    //发送JSON 数据
    socket->write(messageWithSeparator);
    socket->flush();
    jsonData.clear();
    qDebug() << "发送索要头像请求";
}


void ClientHandler::dealAvator(const QString &avator)//上传头像
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    qDebug()<<"准备插入头像";
    //准备插入头像
    qry.prepare("UPDATE Users SET avator = :avator WHERE qq_number = :qq_number");
    //绑定参数
    qry.bindValue(":avator", avator);
    qry.bindValue(":qq_number", randomNumber);
    //执行查询
    if(!qry.exec()){//如果执行失败
        db.rollback();
        QJsonObject qjsonObj;
        qjsonObj["tag"] = "register";
        qjsonObj["answer"]="zhuceshibai";
        jsonDoc = QJsonDocument(qjsonObj);
        jsonData = jsonDoc.toJson();
        //添加标识符
        QByteArray messageWithSeparator = jsonData + "END";
        //发送JSON 数据
        socket->write(messageWithSeparator);
        socket->flush();
        jsonData.clear();
    }
    else{//执行成功
        db.commit();
        QJsonObject qjsonObj;
        qjsonObj["tag"] = "register";
        qjsonObj["answer"] = "zhucechenggong";
        qjsonObj["qq_number"] = randomNumber;
        jsonDoc = QJsonDocument(qjsonObj);
        jsonData = jsonDoc.toJson();
        //添加标识符
        QByteArray messageWithSeparator = jsonData + "END";
        //发送JSON 数据
        socket->write(messageWithSeparator);
        socket->flush();
        jsonData.clear();
    }
}


void ClientHandler::dealAskforavator(const QString &qqnum)//处理索要头像的请求
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    //创建 SQL 查询
    qry.prepare("SELECT avator FROM Users WHERE qq_number = :qqnum");
    qry.bindValue(":qqnum", qqnum);
    //执行查询
    if (!qry.exec()) {
        qDebug() << "查找头像失败" << qry.lastError().text();
        return;
    }
    //检查查询结果
    if (qry.next()) {
        qDebug()<<"给你记住头像";
        QString avatorBase64 = qry.value(0).toString();
        QJsonObject qjsonObj;
        qjsonObj["tag"] = "youravator";
        qjsonObj["avator"] = avatorBase64;
        //发送消息
        jsonDoc = QJsonDocument(qjsonObj);
        jsonData = jsonDoc.toJson();
        //添加标识符
        QByteArray messageWithSeparator = jsonData + "END";
        //发送JSON 数据
        socket->write(messageWithSeparator);
        socket->flush();
        jsonData.clear();
        qDebug()<<"头像发送成功";
    } else {
        qDebug() << "没有头像" << qqnum;
    }
}


void ClientHandler::dealFindpassword1(const QString &qqnum)//看看找回的密码是不是有这个账号
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    qDebug() << "让我看看有没有这个账号"<<qqnum;
    //创建 SQL 查询
    qry.prepare("SELECT COUNT(*),question FROM Users WHERE qq_number = :qqnum");
    qry.bindValue(":qqnum", qqnum);
    QJsonObject qjsonObj;
    //执行查询
    if (!qry.exec()) {
        qDebug() << "查找账号失败" << qry.lastError().text();
        //处理查询错误
        qjsonObj["tag"] = "findpassword1_answer";
        qjsonObj["answer"] = "no";//查询失败时返回"no"
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";
        socket->write(messageWithSeparator);
        socket->flush();
        return;
    }
    //检查查询结果
    if (qry.next()) {
        int count = qry.value(0).toInt();//获取用户计数
        if (count == 1) {
            qjsonObj["tag"] = "findpassword1_answer";
            qjsonObj["answer"] = "yes";//账号存在
            qjsonObj["question"] = qry.value(1).toString();
        } else {
            qjsonObj["tag"] = "findpassword1_answer";
            qjsonObj["answer"] = "no";//账号不存在
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    qDebug() << "你好" << messageWithSeparator;
    socket->write(messageWithSeparator);
    socket->flush();
}

void ClientHandler::dealFindpassword2(const QJsonObject &json)//查看密保问题回答对了没有 json和全局变量jsonobj一样
{

    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);

    qDebug() << "让我看看问题回答对了没有"; // 打印调试信息，表明正在进行密保答案验证

    // 创建 SQL 查询语句：查询 Users 表中 qq_number 和 answer 都匹配的记录数量。
    // COUNT(*) 会返回匹配的行数。如果找到一个用户且答案正确，计数将为 1。
    // 使用命名参数绑定 (:qqnum, :answer) 是一种安全的做法，防止 SQL 注入。
    qry.prepare("SELECT COUNT(*) FROM Users WHERE qq_number = :qqnum AND answer = :answer");

    // 绑定参数：将输入的 json 对象中 "qq_number" 字段的值绑定到 SQL 语句的 :qqnum 占位符。
    // json["qq_number"] 返回一个 QJsonValue，Qt 会自动将其转换为 QVariant 再绑定。
    qry.bindValue(":qqnum", json["qq_number"]);

    // 绑定参数：将输入的 json 对象中 "theanswer" 字段的值绑定到 SQL 语句的 :answer 占位符。
    // "theanswer" 是客户端发送的密保答案字段名。
    qry.bindValue(":answer", json["theanswer"]);

    // 声明一个 QJsonObject 用于构建发送给客户端的响应消息
    QJsonObject qjsonObj;

    // 执行准备好的 SQL 查询语句
    if (!qry.exec()) {
        // 如果 qry.exec() 返回 false，说明 SQL 语句本身执行失败（例如语法错误、数据库连接中断等），而不是答案错误。
        qDebug() << "查找密保答案失败" << qry.lastError().text(); // 打印详细的错误信息
        // 处理查询执行错误：构建一个响应 JSON 告知客户端验证失败
        qjsonObj["tag"] = "findpassword2_answer"; // 消息标签
        qjsonObj["answer"] = "no";//查询失败时返回"no"。 // 返回 "no"。注意：这里将执行失败也返回了 "no"，可能与答案错误混淆。更精确的逻辑可能需要区分这两种情况。
        // 将响应 JSON 对象转换为 JSON 格式的 QByteArray 并添加结束标识符
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";
        // 将响应发送回客户端
        socket->write(messageWithSeparator);
        socket->flush();
        // 函数立即返回，不再进行后续的结果处理
        return;
    }

    // 如果 qry.exec() 执行成功，继续处理查询结果
    // 检查结果集是否有下一条记录。对于 COUNT(*) 查询，如果执行成功，总会有一条记录。
    // qry.next() 将内部指针移动到这唯一的记录上。
    if (qry.next()) {
        // 如果成功移动到记录上 (对于 COUNT(*) 查询，这里总是会执行)
        // 获取当前记录（第一条记录）中第一列（索引为 0）的值，并转换为整数。这个值就是 COUNT(*) 的结果。
        int count = qry.value(0).toInt();//获取用户计数

        // 检查计数结果
        if (count == 1) {
            // 如果计数为 1，表示找到一个匹配的用户且答案正确
            qjsonObj["tag"] = "findpassword2_answer";
            qjsonObj["answer"] = "yes";//账号存在且答案正确
        } else {
            // 如果计数不为 1 (只能是 0，因为 WHERE 条件限制)，表示没有找到匹配的用户或答案不正确
            qjsonObj["tag"] = "findpassword2_answer";
            qjsonObj["answer"] = "no";//账号不存在或答案错误
        }
    }
    // else 如果 qry.next() 返回 false，这对于 SELECT COUNT(*) 在 exec 成功后几乎不可能发生，除非结果集为空，但 COUNT(*) 对空表返回 0。
    // 因此这里的 if (qry.next()) 块是主要的成功处理路径。

    // 发送最终的验证结果消息（无论是 "yes" 还是 "no"）给客户端
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    qDebug() << "你好" << messageWithSeparator; // 打印发送的完整消息内容（包含分隔符）
    socket->write(messageWithSeparator); // 将消息写入 socket
    socket->flush(); // 强制发送缓冲区数据
    qDebug()<<"发送了你的密保答案结果"<<messageWithSeparator; // 再次打印确认发送

    // 函数在此结束，互斥锁 (dbMutex) 在 QMutexLocker 超出作用域时自动解锁。
    // !!! 注意: 同样缺少 pool.releaseConnection(db); 如果使用了连接池的话。
}

void ClientHandler::dealFindpassword3(const QJsonObject &json)//修改密码 json和全局变量jsonobj一样
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    qDebug() << "让我为你修改密码";
    qry.prepare("UPDATE Users SET password = :password WHERE qq_number = :qqnum");
    qry.bindValue(":password", json["password"]);
    qry.bindValue(":qqnum", json["qq_number"]);
    QJsonObject qjsonObj;
    //执行修改密码语句
    if (!qry.exec()) {
        qDebug() << "修改密码失败" << qry.lastError().text();
        db.rollback();
        //处理查询错误
        qjsonObj["tag"] = "findpassword3_answer";
        qjsonObj["answer"] = "no";//插入失败时返回"no"
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";
        socket->write(messageWithSeparator);
        socket->flush();
        return;
    }
    qjsonObj["tag"] = "findpassword3_answer";
    qjsonObj["answer"] = "yes";//修改成功
    db.commit();
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
    qDebug()<<"发送了你的修改密码结果是"<<messageWithSeparator;
}


void ClientHandler::dealLoginFirst(const QJsonObject &json)//处理用户登录成功后初始化
{
    databasesConnect();
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    //添加账号和ClientHandler映射
    account = json["account"].toString();
    addClient(account, this);
    QSqlQuery qry(db);
    qDebug() << "有人第一次登录" << json << json["account"];
    //查询好友的 ID 列表
    qry.prepare("SELECT * FROM Friends WHERE user_id = :user_id");
    qry.bindValue(":user_id", json["account"]);
    if (!qry.exec()) {
        qDebug() << "用户第一次登录查找好友信息失败" << qry.lastError().text();
        return;//失败时，返回
    }
    QJsonArray friendsArray;//好友信息
    QJsonArray friendsRequestsArray;//未读好友申请
    QJsonArray unreadMessageArray;//未读消息
    QSqlQuery userQuery(db);
    //遍历所有好友
    while (qry.next()) {
        QString friendId = qry.value("friend_id").toString();
        //为每个好友查询用户信息
        userQuery.prepare("SELECT * FROM Users WHERE qq_number = :friend");
        userQuery.bindValue(":friend", friendId);
        if (!userQuery.exec()) {
            qDebug() << "查找好友信息失败" << userQuery.lastError().text();
            continue;//跳过此好友
        }
        //获取好友的信息
        while (userQuery.next()) {
            QJsonObject friendJson;
            friendJson["account_num"] = userQuery.value("qq_number").toString();
            friendJson["nickname"] = userQuery.value("nickname").toString();
            friendJson["gender"] = userQuery.value("gender").toString();
            friendJson["signature"] = userQuery.value("signature").toString();
            friendJson["avator"] = userQuery.value("avator").toString();
            friendsArray.append(friendJson);//添加好友信息到数组
        }
    }
    //查询用户本人的信息
    userQuery.prepare("SELECT * FROM Users WHERE qq_number = :account");
    userQuery.bindValue(":account", json["account"]);
    if (!userQuery.exec()) {
        qDebug() << "查找用户信息失败" << userQuery.lastError().text();
        return;//失败时，返回
    }
    if (userQuery.next()) {
        QJsonObject accountJson;
        accountJson["accountflag"] = true;
        accountJson["account_num"] = userQuery.value("qq_number").toString();
        accountJson["nickname"] = userQuery.value("nickname").toString();
        accountJson["gender"] = userQuery.value("gender").toString();
        accountJson["signature"] = userQuery.value("signature").toString();
        accountJson["avator"] = userQuery.value("avator").toString();
        //将用户本人的信息添加到数组中
        friendsArray.append(accountJson);
    }
    //查询好友请求
    qry.prepare("SELECT sender_id FROM FriendRequests WHERE receiver_id = :receiver_id AND status = :status");
    qry.bindValue(":receiver_id", json["account"]);
    qry.bindValue(":status", "pending");
    if (!qry.exec()) {
        qDebug() << "查询好友请求失败" << qry.lastError().text();
    } else {
        //遍历所有好友请求
        while (qry.next()) {
            QString senderId = qry.value("sender_id").toString();
            userQuery.prepare("SELECT * FROM Users WHERE qq_number = :sender");
            userQuery.bindValue(":sender", senderId);
            if (!userQuery.exec()) {
                qDebug() << "查找申请用户信息失败" << userQuery.lastError().text();
                continue;//跳过此用户
            }
            //获取发送申请用户的信息
            while (userQuery.next()) {
                QJsonObject friendJson;
                friendJson["account_num"] = userQuery.value("qq_number").toString();
                friendJson["nickname"] = userQuery.value("nickname").toString();
                friendJson["gender"] = userQuery.value("gender").toString();
                friendJson["signature"] = userQuery.value("signature").toString();
                friendJson["avator"] = userQuery.value("avator").toString();
                friendsRequestsArray.append(friendJson);//添加发送好友的信息到数组
            }
        }
    }
    //查询未读消息
    qry.prepare("SELECT * FROM Messages WHERE receiver_id = :receiver_id AND status = :status");
    qry.bindValue(":receiver_id", json["account"]);
    qry.bindValue(":status", "unread");
    if (!qry.exec()) {
        qDebug() << "查询好友请求失败" << qry.lastError().text();
    } else {
        //遍历所有未读消息
        while (qry.next()) {
            //获取发送申请用户的信息
            QJsonObject messageJson;
            messageJson["sender"] = qry.value("sender_id").toString();
            messageJson["receiver"] = qry.value("receiver_id").toString();
            messageJson["message_type"] = qry.value("message_type").toString();
            if(messageJson["message_type"].toString() != "document"){
                messageJson["content"] = qry.value("content").toString();}
            else{
                messageJson["content"] = qry.value("filename").toString();}
            messageJson["timestamp"] = qry.value("timestamp").toString();
            unreadMessageArray.append(messageJson);//添加未读消息到数组
        }
    }
    //将所有未读消息的状态改为已读
    qry.clear();
    qry.prepare("UPDATE Messages SET status = :new_status WHERE receiver_id = :receiver_id AND status = :status");
    qry.bindValue(":new_status", "haveread");
    qry.bindValue(":receiver_id", json["account"]);
    qry.bindValue(":status", "unread");
    if (!qry.exec()) {
        qDebug() << "更新消息状态失败" << qry.lastError().text();
    }
    //创建一个最终的JSON 对象
    QJsonObject finalJson;
    finalJson["tag"] = "friendmessage";
    finalJson["friends"] = friendsArray;//添加全部好友信息到数组
    finalJson["newfriends"] = friendsRequestsArray;//添加全部未读的申请的用户信息到数组
    finalJson["unreadmessages"] = unreadMessageArray;//添加全部未读的申请的用户信息到数组
    //完整的JSON 文档加上"END" 字符
    QByteArray messageWithSeparator = QJsonDocument(finalJson).toJson() + "END";
    socket->write(messageWithSeparator);
    socket->flush();
}

void ClientHandler::dealDeleteFriend(const QJsonObject &json)//处理删除好友的操作
{
    // 声明一个布尔标志，用于记录数据库删除操作是否成功并提交事务
    bool deleteSucceed = false;

    // 使用 QMutexLocker 锁定 dbMutex 互斥量。
    // 确保在进行数据库操作的整个过程中，其他线程无法同时访问受此互斥量保护的共享数据库资源。
    // 当退出这个作用域块时，互斥锁会自动解锁。
    { // 数据库操作的临界区开始
        QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全

        QSqlQuery qry(db); // 创建一个 QSqlQuery 对象，关联到获取的数据库连接

        QJsonObject qjsonObj; // 声明一个 QJsonObject 用于构建发送给发起者的响应消息

        // 开始数据库事务。
        // 事务是一个原子操作单元：其中的所有数据库修改要么全部成功并提交，要么全部失败并回滚，数据库回到事务开始前的状态。
        // 在这里，删除好友关系可能需要确保删除操作作为一个整体要么完成，要么不发生。
        if (!db.transaction()) { // 尝试开始一个新事务
            qDebug() << "开始事务失败:" << db.lastError().text(); // 如果开始事务失败，打印错误信息
            return; // 函数立即返回
        }

        // 准备 SQL DELETE 语句：从 Friends 表中删除好友关系记录。
        // WHERE 子句使用了 OR 条件：删除 user_id 是发起者且 friend_id 是被删除者 的记录，或者 user_id 是被删除者且 friend_id 是发起者 的记录。
        // 这段 SQL 处理了好友关系在表中可能以两种顺序存储的情况，或者确保对称地删除关系。
        // 使用参数绑定 (:account, :friend) 防止 SQL 注入。
        qry.prepare("DELETE FROM Friends WHERE (user_id = :account AND friend_id = :friend) OR (user_id = :friend AND friend_id = :account)");

        // 绑定参数：将输入的 json 对象中 "account" 字段（发起删除者的账号）的值绑定到 :account 占位符。
        qry.bindValue(":account", json["account"].toString());
        // 绑定参数：将输入的 json 对象中 "friend" 字段（被删除好友的账号）的值绑定到 :friend 占位符。
        qry.bindValue(":friend", json["friend"].toString());

        // 执行 SQL DELETE 语句
        if (!qry.exec()) { // 如果执行失败
            qDebug() << "删除好友失败" << qry.lastError().text(); // 打印详细错误信息
            // 回滚事务。由于执行失败，撤销在此事务中已经进行的所有数据库修改（如果有的话），数据库恢复到事务开始前的状态。
            db.rollback();//回滚事务
            qjsonObj["tag"] = "deletefriendfail"; // 构建删除失败的响应 JSON
        } else { // 如果执行成功
            // 提交事务。将在此事务中进行的所有数据库修改永久地保存到数据库中。
            db.commit();//提交事务
            qjsonObj["tag"] = "deletefriendsucceed"; // 构建删除成功的响应 JSON
            qjsonObj["account"] = json["friend"];//被用户删除的人 // 在成功消息中包含被删除好友的账号信息
            qDebug() << "删除好友成功"; // 打印调试信息
            deleteSucceed = true; // 设置标志为 true，以便在临界区外进行在线通知
        }

        // 向发起删除操作的客户端发送响应消息（无论是成功还是失败）
        // 将响应 JSON 对象转换为 JSON 格式的 QByteArray 并添加结束标识符 "END"
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
        socket->write(messageWithSeparator); // 写入 socket
        socket->flush(); // 强制发送缓冲区数据
    } // 数据库操作临界区结束，dbMutex 自动解锁

    // 在数据库操作完成后（并且互斥锁已解锁），如果删除操作在数据库中成功提交
    if (deleteSucceed){//如果删除成功
        // 查找被删除好友（由 json["friend"] 指定的账号）对应的 ClientHandler 实例，看他是否在线
        auto it = getClient(json["friend"].toString());//it代表被删除人的ClientHandler

        // 如果 it 为 nullptr，表示被删除的好友不在线
        if(it == nullptr){
            qDebug()<<"被删除的人不在线"; // 打印调试信息
            return; // 函数返回，不进行在线通知
        }

        // 如果被删除的好友在线 (it 不为 nullptr)
        QJsonObject qjson; // 构建一个通知消息 JSON 对象
        qjson["tag"] = "youaredeleted"; // 消息标签为 "youaredeleted"，表示“你被删除了”
        qjson["account"] = json["account"];//删除好友的发起人 // 包含是谁删除了他（发起者的账号）

        // 建立一个临时的信号与槽连接：当当前的 ClientHandler 发射 sendMessage 信号时，调用被删除好友的 ClientHandler (it) 的 receiveMessage 槽
        // 这是线程安全的跨线程通信（如果 ClientHandler 实例在不同线程，Qt 默认使用排队连接）
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        // 发射 sendMessage 信号，将被删除通知发送给在线的好友
        emit sendMessage(qjson);

        // 使用一个延迟为 0 的单次定时器，安排在当前事件循环迭代结束后断开信号与槽的连接
        // 确保 sendMessage 信号发射后，it->receiveMessage 槽有机会执行完毕后再断开连接
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}

void ClientHandler::dealSerachAccount(const QJsonObject &json)//处理用户搜索账户信息的功能
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account");
    qry.bindValue(":account", json["account"].toString());
    if (!qry.exec()) {
        qDebug() << "查找用户信息失败" << qry.lastError().text();
        qjsonObj["tag"] = "serchaccount";
        qjsonObj["answer"] = "fail";
    } else {
        qDebug() << "查找用户信息成功";
        if(qry.next()){//如果有一个用户信息的话
            qjsonObj["tag"] = "serchaccount";
            qjsonObj["answer"] = "succeed";
            qjsonObj["account_num"] = qry.value("qq_number").toString();
            qjsonObj["nickname"] = qry.value("nickname").toString();
            qjsonObj["gender"] = qry.value("gender").toString();
            qjsonObj["signature"] = qry.value("signature").toString();
            qjsonObj["avator"] = qry.value("avator").toString();
        }
        else{//没有用户信息的话
            qjsonObj["tag"] = "serchaccount";
            qjsonObj["answer"] = "fail";
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::dealChangeInformation(const QJsonObject &json)//处理用户更新个人信息的功能
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    //开始事务
    if (!db.transaction()) {
        qDebug() << "开始事务失败:" << db.lastError().text();
        return;
    }
    QJsonObject qjsonObj;
    qry.prepare("UPDATE Users SET nickname = :nickname, gender = :gender, signature = :signature, avator = :avator WHERE qq_number = :account;");
    qry.bindValue(":nickname", json["nickname"].toString());
    qry.bindValue(":gender", json["gender"].toString());
    qry.bindValue(":signature", json["signature"].toString());
    qry.bindValue(":avator", json["avator"].toString());
    qry.bindValue(":account", json["account"].toString());
    // 执行查询并检查结果
    if (!qry.exec()) {
        qDebug() << "更新用户信息失败" << qry.lastError().text();
        qjsonObj["tag"] = "changeinformation";
        qjsonObj["answer"] = "fail";
        db.rollback();
    } else {
        qDebug() << "更新用户信息成功";
        qjsonObj["tag"] = "changeinformation";
        qjsonObj["answer"] = "succeed";
        qjsonObj["nickname"] = json["nickname"];
        qjsonObj["gender"] = json["gender"];
        qjsonObj["signature"] = json["signature"];
        qjsonObj["avator"] = json["avator"];
        qjsonObj["account"] = json["account"];
        db.commit();
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::dealChangePassword(const QJsonObject &json)//处理用户更新密码的功能
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account AND password = :password");
    qry.bindValue(":account", json["account"].toString());
    qry.bindValue(":password", json["password"].toString());
    // 执行查询并检查结果
    if (!qry.exec()) {
        qDebug() << "查找失败" << qry.lastError().text();
        qjsonObj["tag"] = "changepassword1";
        qjsonObj["answer"] = "fail";
    } else {
        if (qry.next()) {
            // 找到记录
            qjsonObj["tag"] = "changepassword1";
            qjsonObj["answer"] = "succeed";
        } else {
            // 没有找到记录
            qjsonObj["tag"] = "changepassword1";
            qjsonObj["answer"] = "user_not_found";
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::dealChangePassword2(const QJsonObject &json)//处理用户更新密码的功能
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    //开始事务
    if (!db.transaction()) {
        qDebug() << "开始事务失败:" << db.lastError().text();
        return;
    }
    qry.prepare("UPDATE Users SET password = :password WHERE qq_number = :account");
    qry.bindValue(":account", json["account"].toString());
    qry.bindValue(":password", json["password"].toString());
    // 执行查询并检查结果
    if (!qry.exec()) {
        qDebug() << "修改失败" << qry.lastError().text();
        qjsonObj["tag"] = "changepassword2";
        qjsonObj["answer"] = "fail";
        db.rollback();
    } else {
        qjsonObj["tag"] = "changepassword2";
        qjsonObj["answer"] = "succeed";
        db.commit();
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}

void ClientHandler::dealLogout(const QJsonObject &json)//处理用户注销的功能
{

    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db); // 创建一个 QSqlQuery 对象，关联到获取的数据库连接
    QJsonObject qjsonObj; // 声明一个 QJsonObject 用于构建发送给客户端的响应消息

    // 开始数据库事务。
    // 事务是一个原子操作单元：其中的所有数据库修改要么全部成功并提交，要么全部失败并回滚，数据库回到事务开始前的状态。
    // 这里使用事务是因为删除用户账号是一个重要的操作，并且会涉及到外键的级联删除，应该保证整个过程的完整性。
    if (!db.transaction()) { // 尝试开始一个新事务
        qDebug() << "开始事务失败:" << db.lastError().text(); // 如果开始事务失败，打印错误信息
        // 注意：如果在这里返回，dbMutex 会解锁，但如果 db 是从池中获取的，它将不会被释放回池。这是一个潜在问题。
        return; // 函数立即返回
    }

    // 启用 SQLite 的外键支持。
    // PRAGMA foreign_keys 是 SQLite 数据库特有的命令。
    // 在 SQLite 中，外键约束默认是关闭的，需要通过这个 PRAGMA 命令为每个连接单独启用。
    // ON DELETE CASCADE 等级联操作只有在外键支持启用时才会生效。
    // 这个命令必须在数据库连接打开后执行，且在依赖外键的操作（如这里的 DELETE）之前执行。
    if (!qry.exec("PRAGMA foreign_keys = ON;")) { // 执行 PRAGMA 命令
        // 如果 PRAGMA 命令执行失败（极少见，除非驱动或 SQLite 版本有问题）
        qjsonObj["tag"] = "logout";
        qjsonObj["answer"] = "fail"; // 构建一个失败响应
        // 发送失败消息给客户端
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
        socket->write(messageWithSeparator);
        socket->flush();
        // 注意：这里没有回滚事务。如果事务已经开始，它可能会隐式回滚或保持打开状态直到连接关闭。通常在任何失败路径上都应该显式调用 rollback()。
        return; // 函数立即返回
    }

    // 准备 SQL SELECT 语句：查询 Users 表中 qq_number 和 password 都匹配的记录。
    // 这里的目的是验证用户提供的账号和密码是否正确，以确认是合法用户在执行注销操作。
    // 使用 SELECT * 是获取所有列，其实对于验证目的，使用 SELECT COUNT(*) 会更高效，只需要检查计数是否为 1。
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account AND password = :password");
    // 绑定参数：将输入的 json 中 "account" 和 "password" 的值绑定到占位符。
    qry.bindValue(":account", json["account"].toString());
    qry.bindValue(":password", json["password"].toString());

    // 执行验证查询并检查执行是否成功
    if (!qry.exec()) { // 如果执行失败（如连接问题、SQL 错误等）
        qDebug() << "查找失败" << qry.lastError().text(); // 打印错误信息
        qjsonObj["tag"] = "logout";
        qjsonObj["answer"] = "fail"; // 构建失败响应
        // 注意：这里也没有回滚事务。
    } else { // 如果执行成功（不代表找到记录，只代表 SQL 语句执行无误）
        // 检查查询结果集是否有下一条记录。对于 SELECT *，如果有匹配的记录，qry.next() 将返回 true。
        if (qry.next()) { // 如果找到了匹配账号和密码的记录（用户验证成功）
            // 找到记录，可以执行删除操作了
            // 准备 SQL DELETE 语句：删除 Users 表中 qq_number 匹配指定账号的记录。
            qry.prepare("DELETE FROM Users Where qq_number = :account");
            // 绑定参数：将要删除的账号绑定到占位符。
            qry.bindValue(":account", json["account"].toString());

            // 执行 DELETE 语句
            if (!qry.exec()) {//注销账号失败
                // 如果 DELETE 执行失败（例如外键约束问题，尽管启用了 ON DELETE CASCADE 通常不会因这个失败）
                qjsonObj["tag"] = "logout";
                qjsonObj["answer"] = "fail"; // 构建失败响应
                // 回滚事务。撤销包括 DELETE 在内的所有事务内修改（如果启用了外键且级联删除也失败，也会回滚）。
                db.rollback();
            }
            else{//注销成功
                // 如果 DELETE 执行成功。因为启用了 PRAGMA foreign_keys = ON 且 Users 表在其他表中有外键引用并设置了 ON DELETE CASCADE，
                // 数据库会自动删除该用户在 Messages, Friends, FriendRequests 表中的相关记录。
                qjsonObj["tag"] = "logout";
                qjsonObj["answer"] = "success"; // 构建成功响应
                // 提交事务。将删除 Users 记录以及所有级联删除的记录永久保存到数据库。
                db.commit();
            }
        } else { // 如果 qry.next() 返回 false，表示没有找到匹配账号和密码的记录
            // 没有找到记录，用户验证失败
            qjsonObj["tag"] = "logout";
            qjsonObj["answer"] = "user_not_found"; // 构建“用户不存在或密码错误”的响应
            // 注意：这里也没有回滚事务。由于 SELECT 没有修改数据库，不回滚也不会造成数据问题，但显式回滚是良好的事务处理习惯。
        }
    }
    // 执行失败或成功后，都需要发送响应消息给客户端

    // 将最终的响应 JSON 对象转换为 JSON 格式的 QByteArray 并添加结束标识符
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator); // 写入 socket
    socket->flush(); // 强制发送缓冲区数据

    // 函数在此结束，互斥锁 (dbMutex) 在 QMutexLocker 超出作用域时自动解锁。
    // !!! 注意: 同样缺少 pool.releaseConnection(db); 如果使用了连接池的话。
}


void ClientHandler::dealAddFriends(const QJsonObject &json)//处理用户发送添加好友申请的功能
{
    auto it = getClient(json["friend"].toString());//it代表接收人的ClientHandler
    if (it == nullptr) {
        qDebug() << "当前用户不在线:" << json["friend"].toString();
        {
            QMutexLocker locker(&dbMutex); // 锁住互斥量以确保线程安全
            QSqlQuery qry(db);
            //检查是否已存在记录
            qry.prepare("SELECT COUNT(*) FROM FriendRequests WHERE sender_id = :sender AND receiver_id = :receiver AND status = :status");
            qry.bindValue(":sender", json["account"].toString());
            qry.bindValue(":receiver", json["friend"].toString());
            qry.bindValue(":status", "pending");
            if (!qry.exec()) {
                qDebug() << "查询失败" << qry.lastError().text();
                return;
            }
            qry.next();
            if (qry.value(0).toInt() > 0) {
                qDebug() << "好友申请已存在，未重复添加";
                return; //如果记录已存在，则返回
            }
            //记录不存在，执行插入
            //开始事务
            if (!db.transaction()) {
                qDebug() << "开始事务失败:" << db.lastError().text();
                return;
            }
            qry.prepare("INSERT INTO FriendRequests(sender_id, receiver_id, request_type) "
                        "VALUES(:sender, :receiver, :request_type)");
            qry.bindValue(":sender", json["account"].toString());
            qry.bindValue(":receiver", json["friend"].toString());
            qry.bindValue(":request_type", "friend");
            //执行查询并检查结果
            if (!qry.exec()) {
                qDebug() << "执行失败" << qry.lastError().text();
                db.rollback();
            }
            else{
                db.commit();
            }
        }
    }
    else{
        {
            QMutexLocker locker(&dbMutex); // 锁住互斥量以确保线程安全
            QSqlQuery qry(db);
            //检查是否已存在记录
            qry.prepare("SELECT COUNT(*) FROM FriendRequests WHERE sender_id = :sender AND receiver_id = :receiver AND status = :status");
            qry.bindValue(":sender", json["account"].toString());
            qry.bindValue(":receiver", json["friend"].toString());
            qry.bindValue(":status", "pending");
            if (!qry.exec()) {
                qDebug() << "查询失败" << qry.lastError().text();
                return;
            }
            qry.next();
            if (qry.value(0).toInt() > 0) {
                qDebug() << "好友申请已存在，未重复添加";
                return; //如果记录已存在，则返回
            }
            //记录不存在，执行插入
            //开始事务
            if (!db.transaction()) {
                qDebug() << "开始事务失败:" << db.lastError().text();
                return;
            }
            qry.prepare("INSERT INTO FriendRequests(sender_id, receiver_id, request_type) "
                        "VALUES(:sender, :receiver, :request_type)");
            qry.bindValue(":sender", json["account"].toString());
            qry.bindValue(":receiver", json["friend"].toString());
            qry.bindValue(":request_type", "friend");
            //执行查询并检查结果
            if (!qry.exec()) {
                qDebug() << "执行失败" << qry.lastError().text();
                db.rollback();
            }
            else{
                db.commit();
            }
        }
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(json);
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}

void ClientHandler::dealAddNewFriends(const QJsonObject &json)//处理用户回应是否添加好友的功能
{

    { // 数据库操作的临界区开始
        QMutexLocker locker(&dbMutex);//QMutexLocker 会锁住互斥量dbMutex以确保线程安全

        QSqlQuery qry(db); // 创建一个 QSqlQuery 对象，关联到获取的数据库连接

        QJsonObject qjsonObj; // 声明一个 QJsonObject 用于构建发送给回应好友申请的客户端的响应消息
        // 将原申请发送者的账号添加到响应消息中，方便客户端知道是哪个申请得到了回应。
        qjsonObj["sender"] = json["sender"];

        // 判断用户 B 的回应是“拒绝”还是“接受”
        if (json["answer"] == "reject") {//拒绝好友申请
            qjsonObj["type"] = "reject"; // 在响应中标记回应类型为“拒绝”

            // 准备 SQL UPDATE 语句：更新 FriendRequests 表中对应的申请记录状态为 'rejected' (已拒绝)。
            // WHERE 子句通过申请发送者 ID (sender_id, 也就是原申请人 A 的账号) 和接收者 ID (receiver_id, 也就是当前回应人 B 的账号) 来精确找到是哪一条申请记录。
            qry.prepare("UPDATE FriendRequests SET status = 'rejected' WHERE sender_id = :sender_id AND receiver_id = :receiver_id");
            // 绑定参数：将原申请人 A 的账号 (json["sender"]) 绑定到 :sender_id
            qry.bindValue(":sender_id", json["sender"].toString());
            // 绑定参数：将当前回应人 B 的账号 (json["account"]) 绑定到 :receiver_id
            qry.bindValue(":receiver_id", json["account"].toString());

            // 执行 UPDATE 语句
            if (!qry.exec()) { // 如果执行失败
                qDebug() << "更新好友申请失败" << qry.lastError().text(); // 打印错误信息
                qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                qjsonObj["answer"] = "fail"; // 构建失败响应
            } else { // 如果执行成功
                qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                qjsonObj["answer"] = "true"; // 构建成功响应（表示申请状态更新成功）
            }
            // 注意：拒绝操作只包含一个 UPDATE，所以这里没有使用事务。

        } else if (json["answer"] == "accept") {//接受好友申请
            qjsonObj["type"] = "accept"; // 在响应中标记回应类型为“接受”

            // 开始数据库事务。接受好友申请包含多个数据库操作（更新申请状态、插入好友关系记录），需要确保这些操作的原子性。
            db.transaction(); //开始事务

            // 准备 SQL UPDATE 语句：更新 FriendRequests 表中对应的申请记录状态为 'accepted' (已接受)。
            // WHERE 子句和参数绑定与拒绝操作类似。
            qry.prepare("UPDATE FriendRequests SET status = 'accepted' WHERE sender_id = :sender_id AND receiver_id = :receiver_id");
            qry.bindValue(":sender_id", json["sender"].toString());
            qry.bindValue(":receiver_id", json["account"].toString());

            // 执行更新申请状态的 UPDATE 语句
            if (!qry.exec()) { // 如果执行失败
                qDebug() << "更新好友申请失败" << qry.lastError().text(); // 打印错误信息
                qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                qjsonObj["answer"] = "fail"; // 构建失败响应
                db.rollback(); // 回滚事务。撤销在此事务中已进行的所有修改。
            } else { // 如果更新申请状态成功，继续执行后续步骤
                // 检查好友关系是否已存在：在 Friends 表中查询当前回应人 B (json["account"]) 和原申请人 A (json["sender"]) 之间是否已经有好友记录。
                // 这是一个冗余检查，以防在处理申请期间两人已经通过其他方式成为好友，避免重复插入。
                qry.prepare("SELECT COUNT(*) FROM Friends WHERE (user_id = :user_id AND friend_id = :friend_id) OR (user_id = :friend_id AND friend_id = :user_id)");
                // 绑定参数：用户 B 的账号作为 :user_id，用户 A 的账号作为 :friend_id
                qry.bindValue(":user_id", json["account"].toString());
                qry.bindValue(":friend_id", json["sender"].toString());
                qry.exec(); // 执行检查好友关系的查询
                qry.next(); // 移动结果集指针到第一条记录 (COUNT(*) 总会返回一条记录)

                // 检查计数结果。如果大于 0，表示好友关系已经存在
                if (qry.value(0).toInt() > 0) {
                    qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                    qjsonObj["answer"] = "friendship_exists"; // 构建“好友已存在”响应
                    db.rollback(); // 回滚事务。因为好友已存在，接受申请的整个过程作废。
                } else { // 如果好友关系不存在，可以插入新的好友记录
                    // 插入好友关系 (方向一)：在 Friends 表中插入一条记录，表示 B 是 A 的好友。
                    // 这里假设 Friends 表存储的是有方向的关系，或者需要对称插入。
                    qry.prepare("INSERT INTO Friends(user_id, friend_id) VALUES(:user_id, :friend_id)");
                    // 绑定参数：B 是用户 (user_id)，A 是好友 (friend_id)
                    qry.bindValue(":user_id", json["account"].toString());
                    qry.bindValue(":friend_id", json["sender"].toString());
                    // 执行插入好友关系 (方向一)
                    if (!qry.exec()) { // 如果执行失败
                        qDebug() << "更新好友关系失败" << qry.lastError().text(); // 打印错误信息
                        qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                        qjsonObj["answer"] = "fail"; // 构建失败响应
                        db.rollback(); // 回滚事务。撤销之前成功的 UPDATE 申请状态和任何已进行的修改。
                    } else { // 如果插入好友关系 (方向一) 成功，继续插入方向二
                        // 插入好友关系 (方向二)：在 Friends 表中插入另一条记录，表示 A 是 B 的好友。
                        // 这段代码表明 Friends 表存储的是对称关系，需要插入两条记录来表示双方互为好友。
                        qry.prepare("INSERT INTO Friends(user_id, friend_id) VALUES(:friend_id, :user_id)");
                        // 绑定参数：A 是用户 (user_id)，B 是好友 (friend_id)
                        qry.bindValue(":friend_id", json["sender"].toString());
                        qry.bindValue(":user_id", json["account"].toString());
                        // 执行插入好友关系 (方向二)
                        if (!qry.exec()) { // 如果执行失败
                            qDebug() << "更新反向好友关系失败" << qry.lastError().text(); // 打印错误信息
                            qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                            qjsonObj["answer"] = "fail"; // 构建失败响应
                            db.rollback(); // 回滚事务。撤销之前成功的 UPDATE 申请状态和插入方向一的记录。
                        } else { // 如果所有操作（更新申请状态、插入方向一、插入方向二）都成功
                            db.commit(); // 提交事务。将所有修改永久保存到数据库。
                            qjsonObj["tag"] = "updatefriendship"; // 构建响应标签
                            qjsonObj["answer"] = "succeed"; // 构建成功响应
                        }
                    }
                }
            }
        }
        // 在所有数据库操作和逻辑处理完成后，向当前回应好友申请的客户端（用户 B）发送响应消息。
        // 将最终的响应 JSON 对象转换为 JSON 格式的 QByteArray 并添加结束标识符
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END"; // 添加标识符
        socket->write(messageWithSeparator); // 写入 socket
        socket->flush(); // 强制发送缓冲区数据
    } // 数据库操作临界区结束，dbMutex 自动解锁
    // !!! 注意: 同样缺少 pool.releaseConnection(db); 如果使用了连接池的话。

    // 在数据库操作完成后（并且互斥锁已解锁），如果用户 B 的回应是“接受”
    if (json["answer"] == "accept"){//如果是接受了
        // 查找原申请人 A 的 ClientHandler 实例，看他是否在线
        auto it = getClient(json["sender"].toString());//it代表发送申请人的ClientHandler

        // 如果 it 为 nullptr，表示原申请人 A 不在线
        if(it == nullptr){
            qDebug()<<"发送好友申请的人不在线"; // 打印调试信息
            return; // 函数返回，不进行在线通知
        }

        // 如果原申请人 A 在线 (it 不为 nullptr)
        qDebug()<<"发送好友申请的人在线"; // 打印调试信息
        QJsonObject qjson; // 构建一个通知消息 JSON 对象
        qjson["tag"] = "requestpass"; // 消息标签，表示“好友申请通过了”
        qjson["account"] = json["account"];//jsonaccount通过了申请人的好友申请 // 包含是哪个账号（当前回应人 B 的账号）通过了他的申请。

        // 建立一个临时的信号与槽连接：当当前的 ClientHandler 发射 sendMessage 信号时，调用原申请人 A 的 ClientHandler (it) 的 receiveMessage 槽
        // 这是线程安全的跨对象、跨线程通信方式。
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        // 发射 sendMessage 信号，将申请通过的通知发送给在线的原申请人 A
        emit sendMessage(qjson);

        // 使用一个延迟为 0 的单次定时器，安排在当前事件循环迭代结束后断开信号与槽的连接
        // 确保 sendMessage 信号发射后，it->receiveMessage 槽有机会执行完毕后再安全地断开连接
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}


void ClientHandler::dealMessages(const QJsonObject json)//处理用户发送的消息
{
    if(json["messagetype"] == "document"){
        QMutexLocker locker(&mutex);
        //将消息加入队列
        messageQueue.enqueue(json);
        //处理文件
        if(!isSending){
            sendNextMessage();
        }
        return;
    }
    QJsonObject responseJson;
    bool flag = false;
    ClientHandler* it;
    {
        QMutexLocker locker(&dbMutex);
        QSqlDatabase db = pool.getConnection();
        QSqlQuery qry(db);
        qry.prepare("INSERT INTO Messages (sender_id, receiver_id, content, message_type, status, timestamp, filename) "
                    "VALUES (:sender, :receiver, :content, :messagetype, :status, :timestamp, :filename)");
        qry.bindValue(":sender", json["sender"].toVariant());
        qry.bindValue(":receiver", json["receiver"].toVariant());
        qry.bindValue(":content", json["messages"].toVariant());
        qry.bindValue(":messagetype", json["messagetype"].toVariant());
        qry.bindValue(":status", json["status"].toVariant());
        qry.bindValue(":timestamp", json["timestamp"].toVariant());
        qry.bindValue(":filename", json["filename"].toVariant());
        //检查接收人是否在线
        it = getClient(json["receiver"].toString());
        if (it == nullptr) {
            qry.bindValue(":status", "unread");
        } else {//接收人在线
            responseJson["tag"] = "yourmessages";
            responseJson["sender"] = json["sender"];
            responseJson["messagetype"] = json["messagetype"];
            responseJson["receiver"] = json["receiver"];
            responseJson["messages"] = (json["messagetype"].toString() != "document") ? json["messages"] : json["filename"];
            responseJson["timestamp"] = json["timestamp"];
            qry.bindValue(":status", "haveread");
            flag = true;
        }
        //执行 SQL 查询
        if (!qry.exec()) {
            qDebug() << "消息插入失败: " << qry.lastError().text();
        }
        pool.releaseConnection(db);
    }
    if(flag){
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(responseJson);
        //断开连接的定时器
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}


void ClientHandler::sendNextMessage()//从队列发送下一条消息(处理文件)
{
    //如果消息队列为空，设置isSending为false并返回
    if (messageQueue.isEmpty()) {
        isSending = false;
        return;
    }
    //从队列中获取当前待发送的消息
    QJsonObject json = messageQueue.dequeue();
    isSending = true;
    //启动一个线程来处理发送消息
    QThreadPool::globalInstance()->start([this, json]() mutable {
        ClientHandler* it;
        bool flag = false;
        QJsonObject responseJson;
        qDebug()<<"启动线程再处理一个文件";
        {
            QMutexLocker locker(&dbMutex);
            QSqlDatabase db = pool.getConnection();
            QSqlQuery qry(db);
            qry.prepare("INSERT INTO Messages (sender_id, receiver_id, content, message_type, status, timestamp, filename) "
                        "VALUES (:sender, :receiver, :content, :messagetype, :status, :timestamp, :filename)");
            qry.bindValue(":sender", json["sender"].toVariant());
            qry.bindValue(":receiver", json["receiver"].toVariant());
            qry.bindValue(":content", json["messages"].toVariant());
            qry.bindValue(":messagetype", json["messagetype"].toVariant());
            qry.bindValue(":status", json["status"].toVariant());
            qry.bindValue(":timestamp", json["timestamp"].toVariant());
            qry.bindValue(":filename", json["filename"].toVariant());
            //检查接收人是否在线
            it = getClient(json["receiver"].toString());
            if (it == nullptr) {
                qry.bindValue(":status", "unread");
            } else {//接收人在线
                responseJson["tag"] = "yourmessages";
                responseJson["sender"] = json["sender"];
                responseJson["messagetype"] = json["messagetype"];
                responseJson["receiver"] = json["receiver"];
                responseJson["messages"] = (json["messagetype"].toString() != "document") ? json["messages"] : json["filename"];
                responseJson["timestamp"] = json["timestamp"];
                flag = true;
                qry.bindValue(":status", "haveread");
            }
            //执行SQL查询
            if (!qry.exec()) {
                qDebug() << "消息插入失败: " << qry.lastError().text();
            }
            pool.releaseConnection(db);
        }
        if(flag){
            connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
            emit sendMessage(responseJson);
            //断开连接的定时器
            QTimer::singleShot(0, this, [this, it]() {
                disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
            });
        }
        //发送下一条消息
        qDebug()<<"线程处理消息结束";
        sendNextMessage();
    });
}


void ClientHandler::dealAskDocument(const QJsonObject &json)//处理用户要下载文件的要求（连续多次下载会崩溃 可自行创建数据结构管理下载请求或对客户端下载请求进行管理）
{
    qDebug()<<"有人想要文件";
    QString filename = json["filename"].toString();
    QString timestamp = json["timestamp"].toString();
    QSqlDatabase db = pool.getConnection();
    DocumentWorker* worker = new DocumentWorker(filename, timestamp, db);
    QThread *thread = QThread::create([=]() {
        connect(worker, &DocumentWorker::resultReady, this, [this, &worker, &db](const QJsonObject &result) {
            QByteArray messageWithSeparator = QJsonDocument(result).toJson() + "END";
            socket->write(messageWithSeparator);
            socket->flush();
            pool.releaseConnection(db);
            worker->deleteLater();
        });
        worker->process();//开始处理
    });
    thread->start(); //启动线程
}


void ClientHandler::forwordAddFriendRequest(const QJsonObject &json)//向在线用户转发用户好友申请
{
    qDebug()<<json["friend"]<<"我在线并且收到了添加好友申请";
    //json[friend]是当前ClientHandler的账号 申请发送者账号是json[account]
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qjsonObj["tag"] = "newaddrequest";
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account");
    qry.bindValue(":account", json["account"].toString());
    if (qry.exec()) {
        qDebug() << "查找是谁添加好友";
        if(qry.next()){//如果有一个用户信息的话
            qjsonObj["account_num"] = qry.value("qq_number").toString();
            qjsonObj["nickname"] = qry.value("nickname").toString();
            qjsonObj["gender"] = qry.value("gender").toString();
            qjsonObj["signature"] = qry.value("signature").toString();
            qjsonObj["avator"] = qry.value("avator").toString();
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::forwordRequestPass(const QJsonObject &json)//向在线用户更新他的好友列表(他的好友申请被通过)
{
    //json[account]是通过人
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qjsonObj["tag"] = "addrequestpass";
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account");
    qry.bindValue(":account", json["account"].toString());
    if (qry.exec()) {
        qDebug() << "查找是谁通过了好友申请";
        if(qry.next()){//如果有一个用户信息的话
            qjsonObj["account_num"] = qry.value("qq_number").toString();
            qjsonObj["nickname"] = qry.value("nickname").toString();
            qjsonObj["gender"] = qry.value("gender").toString();
            qjsonObj["signature"] = qry.value("signature").toString();
            qjsonObj["avator"] = qry.value("avator").toString();
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::forwordYouAreDeleted(const QJsonObject &json)//向在线用户更新他的好友列表(他被删除了)
{
    //json[account]是谁删除了你
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QJsonObject qjsonObj;
    qjsonObj["tag"] = "youaredeleted";
    qjsonObj["account"] = json["account"];
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::forwordKickedOffline(const QJsonObject &json)//把在线用户挤下线
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QJsonObject qjsonObj;
    qjsonObj["tag"] = "youarekickedoffline";
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::forwordMessages(const QJsonObject &json)//向在线用户发送聊天消息
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(json).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void DocumentWorker::process()//处理查询文件
{
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qry.prepare("SELECT content FROM Messages WHERE filename = :filename AND timestamp = :timestamp");
    qry.bindValue(":filename", filename);
    qry.bindValue(":timestamp", timestamp);
    qDebug()<<filename<<timestamp;
    if (qry.exec() && qry.next()) {
        qjsonObj["tag"] = "document";
        qjsonObj["filename"] = filename;
        qjsonObj["data"] = qry.value("content").toString();
        qDebug()<<"文件查询完毕";
    } else {
        qjsonObj["tag"] = "error";
        qjsonObj["message"] = "File not found";
        qDebug()<<"文件查询失败";
    }
    emit resultReady(qjsonObj); //发射信号，将结果发送回主线程
}
