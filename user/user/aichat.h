#ifndef AICHAT_H
#define AICHAT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>

class AIChat : public QObject
{
    Q_OBJECT

public:
    explicit AIChat(QObject *parent = nullptr);
    ~AIChat();

    // 发送消息到AI服务
    void sendMessageToAI(const QString &message);

    // 设置API密钥
    void setApiKey(const QString &apiKey);

    // 设置AI模型
    void setModel(const QString &model);

signals:
    // 当收到AI回复时发出信号
    void receivedAIResponse(const QString &response);
    // 发生错误时发出信号
    void errorOccurred(const QString &errorMessage);

private slots:
    // 处理网络请求完成
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
    QString model;

    // API端点URL
    QString apiUrl;
};

#endif // AICHAT_H
