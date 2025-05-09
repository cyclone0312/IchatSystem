#include "aichat.h"
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>

AIChat::AIChat(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , model("deepseek-ai/DeepSeek-V3") // 默认模型
    , apiUrl("https://api.siliconflow.com/v1/chat/completions") // 示例URL，需要根据实际API调整
{
    connect(networkManager, &QNetworkAccessManager::finished, this, &AIChat::handleNetworkReply);
}

AIChat::~AIChat()
{
    // QNetworkAccessManager会自动释放
}

void AIChat::setApiKey(const QString &key)
{
    apiKey = key;
}

void AIChat::setModel(const QString &modelName)
{
    model = modelName;
}
void AIChat::sendMessageToAI(const QString &message)
{
    if (apiKey.isEmpty()) {
        emit errorOccurred("API密钥未设置");
        return;
    }

    qDebug() << "准备发送消息到AI:" << message;
    qDebug() << "使用的API URL:" << apiUrl;
    qDebug() << "使用的模型:" << model;
    QNetworkRequest request = QNetworkRequest(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());
    request.setRawHeader("Accept", "application/json");
    QJsonObject jsonObject;
    jsonObject["model"] = model;
    jsonObject["temperature"] = 0.7;  // 控制生成随机性
    jsonObject["max_tokens"] = 1024;  // 限制响应长度
    QJsonArray messagesArray;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = message;
    messagesArray.append(messageObject);
    jsonObject["messages"] = messagesArray;
    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData = jsonDoc.toJson();
     qDebug() << "发送的JSON数据:" << jsonData;
    networkManager->post(request, jsonData);
}
void AIChat::handleNetworkReply(QNetworkReply *reply)
{
    qDebug() << "收到网络响应";

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "收到AI响应：" << responseData;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        if (jsonResponse.isNull()) {
            qDebug() << "JSON解析失败，响应不是有效的JSON";
            emit errorOccurred("收到无效的JSON响应");
            reply->deleteLater();
            return;
        }

        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "JSON对象包含的键:" << jsonObject.keys();

        // 根据硅基API响应结构调整解析逻辑
        if (jsonObject.contains("choices") && jsonObject["choices"].isArray()) {
            QJsonArray choices = jsonObject["choices"].toArray();
            qDebug() << "choices数组长度:" << choices.size();

            if (!choices.isEmpty() && choices[0].isObject()) {
                QJsonObject choice = choices[0].toObject();
                qDebug() << "choice对象包含的键:" << choice.keys();

                if (choice.contains("message") && choice["message"].isObject()) {
                    QJsonObject message = choice["message"].toObject();
                    qDebug() << "message对象包含的键:" << message.keys();

                    if (message.contains("content")) {
                        QString content = message["content"].toString();
                        qDebug() << "提取的内容:" << content;
                        emit receivedAIResponse(content);
                    } else {
                        qDebug() << "message对象中没有content键";
                        emit errorOccurred("AI响应格式错误：缺少content字段");
                    }
                } else {
                    qDebug() << "choice对象中没有message键或message不是对象";
                    emit errorOccurred("AI响应格式错误：缺少message字段");
                }
            } else {
                qDebug() << "choices数组为空或第一个元素不是对象";
                emit errorOccurred("AI响应格式错误：choices数组问题");
            }
        } else {
            qDebug() << "JSON对象中没有choices键或choices不是数组";

            // 检查是否有错误信息
            if (jsonObject.contains("error") && jsonObject["error"].isObject()) {
                QJsonObject error = jsonObject["error"].toObject();
                if (error.contains("message")) {
                    QString errorMessage = error["message"].toString();
                    qDebug() << "API错误信息:" << errorMessage;
                    emit errorOccurred("API错误: " + errorMessage);
                } else {
                    emit errorOccurred("API返回了错误，但没有错误消息");
                }
            } else {
                emit errorOccurred("AI响应格式错误：缺少choices字段");
            }
        }
    } else {
        // 错误处理保持不变
        qDebug() << "AI请求错误：" << reply->errorString();
        qDebug() << "错误代码：" << reply->error();
        qDebug() << "HTTP状态码：" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "响应内容：" << reply->readAll();

        emit errorOccurred(reply->errorString());
    }
    reply->deleteLater();
}
