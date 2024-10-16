#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QListWidget>
#include <QMainWindow>
#include <QLabel>
#include <QTcpSocket>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QPainterPath>
#include <QStandardItemModel>
#include <QBrush>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QMouseEvent>
#include <QTimer>
#include <QMutex>
#include <QJsonArray>
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QMenu>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtConcurrent/QtConcurrent>
#include <QListView>
#include <QPushButton>
#include <QSqlError>
#include "addfriends.h"
#include "changeinformation.h"
#include "changepassword.h"
#include "logout.h"
#include "mainwindow-else.h"


class FilterProxyModel;

class TalkFilterProxyModel;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow//主窗口类
{
    Q_OBJECT

public:
    explicit MainWindow(QString accountNumber0,QWidget *parent = nullptr);
    ~MainWindow();

public:
    struct AccountInfo {//存储好友信息
        QString account;//账号
        QString name;//名称
        QString avator_base64;//头像的 Base64 编码字符串
        QString gender;//性别
        QString signature;//签名
    };

private:
    void initAvatar();//初始化头像
    void setupFriendList();//设置好友列表模型
    void setupTalkList();//设置聊天列表模型
    void setupMenu();//设置左下角按钮菜单
    QString pixmapToBase64(const QPixmap &pixmap);//图片转换成base64格式
    QPixmap base64ToPixmap(const QString &base64Str);//将base64转换为图片
    void positionSendButton();//设置按钮位置
    void paintEvent(QPaintEvent *event);//窗口背景
    void createUserDatabase(const QString& account);//在本地建立或初始化该用户的数据库
    void setupMessageList(QListWidget *list);//设置消息页面模型
    bool tcpConnect();//连接服务器
    void havelogin(const QString& account);//发送初始化请求
    QPixmap getRoundedPixmap(const QPixmap srcPixmap, const int radius);//获得圆角图片
    bool checkSocket();//检查是不是连接上服务器了
    bool deleteSomeoneInFriendList(const QString &account);//清理好友列表中的某个好友
    bool deleteSomeoneInTalkList(const QString &account);//删除聊天列表中的某个聊天
    bool ifTalkHaveOpened(const QString &account);//判断某个人的聊天是否已经在消息列表中了
    int addSomeoneToTalkList(const AccountInfo &friendMessage, QString message, QString date, QString unread);//向聊天列表加入某个好友
    int selectSomeoneInTalkList(const QString &account);//打开某个人的聊天框
    void sendMessageToFriend(const QString &account);//给某个好友发送消息
    bool switchPageTo(const QString &friendId);//将聊天页面切换到账号为account的页面
    void addMessageTo(const QWidget *page, const QString &sender, const QString &messageType,
                      const QString &message, const QString &timestamp);//把聊天记录添加到某个聊天页面(时间传入utc时间)
    void addMessageToDatabase(const QString &sender,const QString &receiver, const QString &messageType,//把聊天记录添加到本地数据库
                              const QString &message, const QString &timestamp);
    QPixmap loadAvator(const QString& friendId);//获得某人头像
    bool printTimeOrNot(const QString& messageTime, const QString& preMessageTime, QString& result);//判断聊天时是否需要加载新的时间
    void checkFriendInTalk(const QString& friendId);//检查这个好友是不是已经有打开的聊天窗口了 如果没有 则打开一个
    void updateTalkList(const QString& friendId);//更新某人的消息列表信息
    void updateTalkListFree(const QString& friendId, const QString& latestMessage, const QString& timestamp);//更新某人的消息列表信息 指定更新
    int updateUnread(const QString& friendId);//使某人的未读消息条数加一 如果正在聊天 则返回
    void clearUnread(const QString& friendId);//清空某人的未读消息
    void liftSomebody(const QString& friendId);//把某人从消息列表提升到最上面

private slots:
    void on_but_maxwindow_clicked();//最大化按钮
    void on_but_minwindow_clicked();//最小化按钮
    void on_but_deletewindow_clicked();//关闭按钮
    void on_but_minwindow2_clicked();//页面2最小化按钮
    void on_but_maxwindow2_clicked();//页面2的最大化按钮
    void on_but_deletewindow2_clicked();//页面2关闭按钮
    void on_but_chat_clicked();//跳转到聊天窗口页面
    void on_but_friends_clicked();//跳转到联系人窗口
    void on_edit_input_textChanged();//输入框改变内容判断能否发送消息
    void on_pages_currentChanged(int arg1);//页面发生改变 修改左边菜单栏图标的背景颜色
    void on_but_addfriends_clicked();//添加好友按钮
    void on_but_add0_clicked();//添加好友按钮
    void goSerachFriends(const QString &account);//向服务器发送搜索用户信息申请
    void clearEditShow2();//清理右边好友信息视图
    void updateEditShow2New();//更新右边好友信息视图(好友申请)
    void updateEditShow2Normal(const QModelIndex &index);//更新右边好友信息视图
    void on_line_serach2_textChanged(const QString &arg1);//搜索输入 更新好友列表视图
    void on_line_serach_textChanged(const QString &arg1);//搜索输入 更新聊天列表视图
    void deleteFriend();//删除好友
    void goAddFriends(const QString &friendAccount);//添加好友
    void changeInfo();//弹出修改个人资料窗口
    void listtalkChoice(const QJsonObject& json);//聊天列表的菜单选择完毕
    void sendMessage(const MyAccountInfo& info);//收到修改资料窗口的信号 发送信息
    void changePassword1(const QJsonObject &json);//收到修改密码窗口的信号 发送信息
    void changePassword2(const QJsonObject &json);//收到修改密码窗口的信号 发送最终的信息
    void goLogout(const QJsonObject &json);//收到注销窗口发送的信号 发送注销申请
    void rejectAddFriends(const QString &account, const QString &sender);//拒绝好友申请
    void acceptAddFriends(const QString &account, const QString &sender);//接受好友申请
    void on_but_send_clicked();//发送信息
    void on_but_tool1_clicked();//发送图片
    void on_but_tool2_clicked();//发送文件
    void sendDoc(const QByteArray &jsonData, const QString &filename, const QString &timestamp, const QString &receiver);//发送文件
    void onTalkItemCurrentChanged();//聊天页面项切换项更新消息框视图
    void onFriendItemCurrentChanged();//好友列表页面项切换项更新右边视图
    void mousePressEvent(QMouseEvent *event);//鼠标点击事件
    void mouseMoveEvent(QMouseEvent *event);//实现拖拽移动效果与调整大小效果与移动到边框显示不同的图标效果
    void mouseReleaseEvent(QMouseEvent *event);//重置移动状态
    void onReadyRead();//读取服务器回复数据

public://读取服务器回发数据之后执行的函数
    void uploadAll(const QJsonObject& json);//加载好友数据(包含聊天记录 好友申请等)
    void uploadFriedList(const QJsonArray &friendsArray);//加载好友列表
    void uploadFriedRequest(const QJsonArray &friendsRequestArray);//加载好友申请
    void uploadListMessages(const QString& account);//加载消息列表
    void uploadMessages(const QString& account);//加载消息记录
    void uploadUnreadMessages(const QJsonArray& messagesArray);//加载未读聊天记录
    void deleteSucceed(const QJsonObject& json);//删除好友成功
    void deleteFail(const QJsonObject& json);//删除失败
    void sendSerach(const QJsonObject& json);//搜索好友的结果
    void changeMyInfo(const QJsonObject& json);//修改用户信息的结果
    void changePasswordAns1(const QJsonObject& json);//修改账号密码第一个申请的结果
    void changePasswordAns2(const QJsonObject& json);//修改账号密码第二个申请的结果
    void logoutAns(const QJsonObject& json);//注销的结果
    void dealFriendsRequest(const QJsonObject& json);//处理好友申请的结果
    void dealNewAddRequest(const QJsonObject& json);//接收到新的好友申请 将其加入好友申请列表
    void dealAddRequestPass(const QJsonObject& json);//发送的好友申请通过 将其加入好友列表
    void dealYouAreDeleted(const QJsonObject& json);//处理被删除好友
    void dealYouAreKickedOffline(const QJsonObject& json);//处理被挤下线
    void dealMessages(const QJsonObject& json);//处理接收到的聊天消息
    void dealDocument(const QJsonObject& json);//处理接收到的文件

signals:
    void deleteDone();//删除好友操作完毕
    void serachResult(const QJsonObject &json);//查找账号的结果
    void changeResult(const QJsonObject &json);//修改用户信息的结果
    void changePasswordAnswer1(const QJsonObject& json);//修改账号密码第一个申请的结果
    void changePasswordAnswer2(const QJsonObject& json);//修改账号密码第一个申请的结果
    void logoutAnswer(const QJsonObject& json);//注销的结果

private:
    Ui::MainWindow *ui;
    AccountInfo myInfo;//用户自己的信息
    QVector<AccountInfo> newFriendArray;//好友申请列表
    QHash<QString, AccountInfo> friendHash;//以账号为索引存储好友列表
    QHash<QString, AccountInfo> messageListHash;//以账号为索引存储消息列表
    QMap<QString, QPixmap> avatorMap;//存储头像
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray jsonData;
    QMutex mutex;
    QString accountNumber;//账号
    QSqlDatabase db;//用户的数据库
    QTcpSocket *socket;
    QRegularExpressionValidator *validator;
    QPoint dragPosition;
    QStandardItem *parentItem;
    QString deleteFriendNum;
    int maxFlag = 0 ;//是否窗口最大化了;
    int moveFlag = 0;//移动状态
    int addFriendsFlag = 0;//是否已经打开添加好友窗体了
    int changeInfoFlag = 0;//是否已经打开修改个人信息窗体了
    int changePassFlag = 0;//是否已经打开修改密码窗体了
    int logoutFlag = 0;//是否已经打开注销窗体了
    int resizeFlag = 0;//改变窗口大小状态
    int frame = 5;//模拟边框大小
    AddFriends* dialogAdd;//添加好友窗口
    ChangeInformation* dialogChangeInfo;//修改个人信息窗口
    ChangePassword *dialogChangePass;//修改密码窗口
    Logout *logout;//注销窗口
    QPushButton *sendButton;//发送信息按钮
    QPushButton *deleteButton;//删除好友按钮
    QVBoxLayout *newLayout;//好友列表主布局
    QStandardItemModel *friendModel;//好友列表项模型
    FilterProxyModel *filterProxyModel;//好友列表筛选模型
    QStandardItemModel *talkModel;//聊天列表项模型
    TalkFilterProxyModel *talkFilterProxyModel;//聊天列表筛选模型
    QStandardItemModel *messageModel;//消息页面模型
    QMenu *toolMenu;//左上角菜单栏
};

#endif // MAINWINDOW_H
