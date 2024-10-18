#include "clienthandler.h"
ClientHandler::ClientHandler(QTcpSocket *socket, ConnectionPool& pool, Server *srv)
    : pool(pool) , socket(socket) , srv(srv)
{
    //连接数据库
    databasesConnect();
}


ClientHandler::~ClientHandler()
{
    this->disconnect();
    if (db.isOpen()) {//检查数据库是否连接
        db = QSqlDatabase();
    }
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


ClientHandler* ClientHandler::getClient(const QString &account) {
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
        pool.releaseConnection(db);//释放连接
    }
}


void ClientHandler::onDisconnected()//有用户断联了
{
    qDebug() << "断联一位";
    if(account != "0"){
        removeClient(account);
    }
    if (db.isOpen()) {//检查数据库是否连接
        db = QSqlDatabase();
    }
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
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    qry.prepare("SELECT COUNT(*) FROM Users WHERE qq_number = :qq_number AND password = :password");
    qry.bindValue(":qq_number", json["qq_number"].toString());
    qry.bindValue(":password", json["password"].toString());
    QJsonObject qjsonObj;
    if(qry.exec()){
        qry.next();
        int count = qry.value(0).toInt();
        if(count==1){//登录成功
            auto it = getClient(json["qq_number"].toString());
            if(it){//不是空指针 说明有人已经登录这个账号了 给那个客户端发送被挤下去的通知
                qDebug()<<json["qq_number"]<<"已经在登录啦";
                QJsonObject qjson;
                qjson["tag"] = "kickedoffline";
                connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
                emit sendMessage(qjson);
                QTimer::singleShot(0, this, [this, it]() {
                    disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
                });
            }
            //是空指针则不用管 正常登录
            qjsonObj["tag"] = "login";
            qjsonObj["answer"] = "dengluchenggong";
            jsonDoc=QJsonDocument(qjsonObj);
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
        }
        else{//登录失败
            qjsonObj["tag"] = "login";
            qjsonObj["answer"] = "denglushibai";
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
        }
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
    qDebug() << "让我看看问题回答对了没有";
    //创建 SQL 查询
    qry.prepare("SELECT COUNT(*) FROM Users WHERE qq_number = :qqnum AND answer = :answer");
    qry.bindValue(":qqnum", json["qq_number"]);
    qry.bindValue(":answer", json["theanswer"]);
    QJsonObject qjsonObj;
    //执行查询
    if (!qry.exec()) {
        qDebug() << "查找密保答案失败" << qry.lastError().text();
        //处理查询错误
        qjsonObj["tag"] = "findpassword2_answer";
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
            qjsonObj["tag"] = "findpassword2_answer";
            qjsonObj["answer"] = "yes";//账号存在
        } else {
            qjsonObj["tag"] = "findpassword2_answer";
            qjsonObj["answer"] = "no";//账号不存在
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    qDebug() << "你好" << messageWithSeparator;
    socket->write(messageWithSeparator);
    socket->flush();
    qDebug()<<"发送了你的密保答案结果"<<messageWithSeparator;
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
    QJsonArray unreadMessageArray;//维度消息
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
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    bool deleteSucceed = false;
    //开始事务
    if (!db.transaction()) {
        qDebug() << "开始事务失败:" << db.lastError().text();
        return;
    }
    qry.prepare("DELETE FROM Friends WHERE (user_id = :account AND friend_id = :friend) OR (user_id = :friend AND friend_id = :account)");
    qry.bindValue(":account", json["account"].toString());
    qry.bindValue(":friend", json["friend"].toString());
    if (!qry.exec()) {
        qDebug() << "删除好友失败" << qry.lastError().text();
        db.rollback();//回滚事务
        qjsonObj["tag"] = "deletefriendfail";
    } else {
        db.commit();//提交事务
        qjsonObj["tag"] = "deletefriendsucceed";
        qjsonObj["account"] = json["friend"];//被用户删除的人
        qDebug() << "删除好友成功";
        deleteSucceed = true;
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
    //如果删除好友成功 则查看被删除的人是否在线 在线则更新它的好友列表
    if (deleteSucceed){//如果删除成功
        auto it = getClient(json["friend"].toString());//it代表被删除人的ClientHandler
        if(it == nullptr){
            qDebug()<<"被删除的人不在线";
            return;
        }
        QJsonObject qjson;
        qjson["tag"] = "youaredeleted";
        qjson["account"] = json["account"];//删除好友的发起人
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(qjson);
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
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    //开始事务
    if (!db.transaction()) {
        qDebug() << "开始事务失败:" << db.lastError().text();
        return;
    }
    // 启用外键支持
    if (!qry.exec("PRAGMA foreign_keys = ON;")) {
        qjsonObj["tag"] = "logout";
        qjsonObj["answer"] = "fail";
        //发送消息
        QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
        socket->write(messageWithSeparator);
        socket->flush();
        return;
    }
    qry.prepare("SELECT * FROM Users WHERE qq_number = :account AND password = :password");
    qry.bindValue(":account", json["account"].toString());
    qry.bindValue(":password", json["password"].toString());
    // 执行查询并检查结果
    if (!qry.exec()) {
        qDebug() << "查找失败" << qry.lastError().text();
        qjsonObj["tag"] = "logout";
        qjsonObj["answer"] = "fail";
    } else {
        if (qry.next()) {
            // 找到记录
            qry.prepare("DELETE FROM Users Where qq_number = :account");
            qry.bindValue(":account", json["account"].toString());
            if (!qry.exec()) {//注销账号失败
                qjsonObj["tag"] = "logout";
                qjsonObj["answer"] = "fail";
                db.rollback();
            }
            else{//注销成功
                qjsonObj["tag"] = "logout";
                qjsonObj["answer"] = "success";
                db.commit();
            }
        } else {
            // 没有找到记录
            qjsonObj["tag"] = "logout";
            qjsonObj["answer"] = "user_not_found";
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END";//添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
}


void ClientHandler::dealAddFriends(const QJsonObject &json)//处理用户发送添加好友申请的功能
{
    auto it = getClient(json["friend"].toString());//it代表接收人的ClientHandler
    if (it == nullptr) {
        qDebug() << "当前用户不在线:" << json["friend"].toString();
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
    else{
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
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(json);
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
    }
}


void ClientHandler::dealAddNewFriends(const QJsonObject &json)//处理用户回应是否添加好友的功能
{
    QMutexLocker locker(&dbMutex);//锁住互斥量以确保线程安全
    QSqlQuery qry(db);
    QJsonObject qjsonObj;
    qjsonObj["sender"] = json["sender"];
    if (json["answer"] == "reject") {//拒绝好友申请
        qjsonObj["type"] = "reject";
        qry.prepare("UPDATE FriendRequests SET status = 'rejected' WHERE sender_id = :sender_id AND receiver_id = :receiver_id");
        qry.bindValue(":sender_id", json["sender"].toString());
        qry.bindValue(":receiver_id", json["account"].toString());
        if (!qry.exec()) {
            qDebug() << "更新好友申请失败" << qry.lastError().text();
            qjsonObj["tag"] = "updatefriendship";
            qjsonObj["answer"] = "fail";
        } else {
            qjsonObj["tag"] = "updatefriendship";
            qjsonObj["answer"] = "true";
        }
    }
    else if (json["answer"] == "accept") {//接受好友申请
        qjsonObj["type"] = "accept";
        db.transaction(); //开始事务
        qry.prepare("UPDATE FriendRequests SET status = 'accepted' WHERE sender_id = :sender_id AND receiver_id = :receiver_id");
        qry.bindValue(":sender_id", json["sender"].toString());
        qry.bindValue(":receiver_id", json["account"].toString());
        if (!qry.exec()) {
            qDebug() << "更新好友申请失败" << qry.lastError().text();
            qjsonObj["tag"] = "updatefriendship";
            qjsonObj["answer"] = "fail";
            db.rollback(); //回滚事务
        } else {
            //检查好友关系是否已存在
            qry.prepare("SELECT COUNT(*) FROM Friends WHERE (user_id = :user_id AND friend_id = :friend_id) OR (user_id = :friend_id AND friend_id = :user_id)");
            qry.bindValue(":user_id", json["account"].toString());
            qry.bindValue(":friend_id", json["sender"].toString());
            qry.exec();
            qry.next();
            if (qry.value(0).toInt() > 0) {
                qjsonObj["tag"] = "updatefriendship";
                qjsonObj["answer"] = "friendship_exists";
                db.rollback(); //回滚事务
            } else {
                //插入好友关系
                qry.prepare("INSERT INTO Friends(user_id, friend_id) VALUES(:user_id, :friend_id)");
                qry.bindValue(":user_id", json["account"].toString());
                qry.bindValue(":friend_id", json["sender"].toString());
                if (!qry.exec()) {
                    qDebug() << "更新好友关系失败" << qry.lastError().text();
                    qjsonObj["tag"] = "updatefriendship";
                    qjsonObj["answer"] = "fail";
                    db.rollback(); //回滚事务
                } else {
                    //插入反向好友关系
                    qry.prepare("INSERT INTO Friends(user_id, friend_id) VALUES(:friend_id, :user_id)");
                    qry.bindValue(":friend_id", json["sender"].toString());
                    qry.bindValue(":user_id", json["account"].toString());
                    if (!qry.exec()) {
                        qDebug() << "更新反向好友关系失败" << qry.lastError().text();
                        qjsonObj["tag"] = "updatefriendship";
                        qjsonObj["answer"] = "fail";
                        db.rollback(); //回滚事务
                    } else {
                        db.commit(); //提交事务
                        qjsonObj["tag"] = "updatefriendship";
                        qjsonObj["answer"] = "succeed";
                    }
                }
            }
        }
    }
    //发送消息
    QByteArray messageWithSeparator = QJsonDocument(qjsonObj).toJson() + "END"; // 添加标识符
    socket->write(messageWithSeparator);
    socket->flush();
    //如果添加好友成功 则查看申请人是否在线 在线则更新它的好友列表
    if (json["answer"] == "accept"){//如果是接受了
        auto it = getClient(json["sender"].toString());//it代表发送申请人的ClientHandler
        if(it == nullptr){
            qDebug()<<"发送好友申请的人不在线";
            return;
        }
        qDebug()<<"发送好友申请的人在线";
        QJsonObject qjson;
        qjson["tag"] = "requestpass";
        qjson["account"] = json["account"];//jsonaccount通过了申请人的好友申请
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(qjson);
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
        if (!isSending) {
            //如果当前没有正在发送的消息，则立即开始发送
            sendNextMessage();
            return;
        }
    }
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
    auto it = getClient(json["receiver"].toString());
    if (it == nullptr) {
        qry.bindValue(":status", "unread");
    } else {//接收人在线
        QJsonObject responseJson;
        responseJson["tag"] = "yourmessages";
        responseJson["sender"] = json["sender"];
        responseJson["messagetype"] = json["messagetype"];
        responseJson["receiver"] = json["receiver"];
        responseJson["messages"] = (json["messagetype"].toString() != "document") ? json["messages"] : json["filename"];
        responseJson["timestamp"] = json["timestamp"];
        connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        emit sendMessage(responseJson);
        //断开连接的定时器
        QTimer::singleShot(0, this, [this, it]() {
            disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
        });
        qry.bindValue(":status", "haveread");
    }
    //执行 SQL 查询
    if (!qry.exec()) {
        qDebug() << "消息插入失败: " << qry.lastError().text();
    }
    pool.releaseConnection(db);
    //发送下一条消息
    sendNextMessage();
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
        qDebug()<<"启动线程再处理一个文件";
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
        auto it = getClient(json["receiver"].toString());
        if (it == nullptr) {
            qry.bindValue(":status", "unread");
        } else {//接收人在线
            QJsonObject responseJson;
            responseJson["tag"] = "yourmessages";
            responseJson["sender"] = json["sender"];
            responseJson["messagetype"] = json["messagetype"];
            responseJson["receiver"] = json["receiver"];
            responseJson["messages"] = (json["messagetype"].toString() != "document") ? json["messages"] : json["filename"];
            responseJson["timestamp"] = json["timestamp"];
            connect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
            emit sendMessage(responseJson);
            //断开连接的定时器
            QTimer::singleShot(0, this, [this, it]() {
                disconnect(this, &ClientHandler::sendMessage, it, &ClientHandler::receiveMessage);
            });
            qry.bindValue(":status", "haveread");
        }
        //执行SQL查询
        if (!qry.exec()) {
            qDebug() << "消息插入失败: " << qry.lastError().text();
        }
        pool.releaseConnection(db);
        //发送下一条消息
        qDebug()<<"线程处理文件结束";
        sendNextMessage();
    });
}


void ClientHandler::dealAskDocument(const QJsonObject &json)//处理用户要下载文件的要求
{
    qDebug()<<"有人想要文件";
    QString filename = json["filename"].toString();
    QString timestamp = json["timestamp"].toString();
    QThread *thread = QThread::create([=]() {
        DocumentWorker worker(filename, timestamp, db);
        connect(&worker, &DocumentWorker::resultReady, this, [this](const QJsonObject &result) {
            QByteArray messageWithSeparator = QJsonDocument(result).toJson() + "END";
            socket->write(messageWithSeparator);
            socket->flush();
        });
        worker.process();//开始处理
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
    if (qry.exec() && qry.next()) {
        qjsonObj["tag"] = "document";
        qjsonObj["filename"] = filename;
        qjsonObj["data"] = qry.value("content").toString();
    } else {
        qjsonObj["tag"] = "error";
        qjsonObj["message"] = "File not found";
    }
    emit resultReady(qjsonObj); //发射信号，将结果发送回主线程
}
