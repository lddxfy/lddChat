#include "tcpmgr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "usermgr.h"
#include <QJsonArray>
TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr() : host_(""),port_(0),b_recv_pending_(false),messageid_(0),messagelen_(0)
{

    connect(&socket_,&QTcpSocket::connected,[this](){
        qDebug() << "Connect Server Success!";
        emit this->sig_connect_finish(true);
    });

    connect(&socket_,&QTcpSocket::readyRead,[this](){
        while(socket_.bytesAvailable() > 0){
            //将tcp缓冲区数据全部读出到buffer
            buffer_.append(socket_.readAll());
            QDataStream stream(&buffer_, QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_5_0);
            if(!b_recv_pending_){
                //读到的数据小于头部长度
                if(buffer_.size() < static_cast<int>(sizeof(quint16) * 2)){
                    //数据不足，等待更多数据可读
                    return;
                }

                stream >> messageid_ >> messagelen_;
                //将buffer移动到数据部分首字节
                buffer_ = buffer_.mid(sizeof(quint16)*2);

                // 输出读取的数据
                qDebug() << "Message ID:" << messageid_ << ", Length:" << messagelen_;
            }

            if(buffer_.size() < messagelen_){
                b_recv_pending_ = true;
                return;
            }

            // 读取消息体
            b_recv_pending_ = false;
            QByteArray messageBody = buffer_.mid(0, messagelen_);
            qDebug() << "receive body msg is " << messageBody ;

            //处理服务器回包
            this->handleMsg(static_cast<ReqId>(messageid_),messagelen_,messageBody);

            buffer_ = buffer_.mid(messagelen_);

        }
    });

    // 处理错误（适用于Qt 5.15之前的版本）
    QObject::connect(&socket_, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                        [&](QTcpSocket::SocketError socketError) {
           qDebug() << "Error:" << socket_.errorString() ;
           switch (socketError) {
               case QTcpSocket::ConnectionRefusedError:
                   qDebug() << "Connection Refused!";
                   emit sig_connect_finish(false);
                   break;
               case QTcpSocket::RemoteHostClosedError:
                   qDebug() << "Remote Host Closed Connection!";
                   break;
               case QTcpSocket::HostNotFoundError:
                   qDebug() << "Host Not Found!";
                   emit sig_connect_finish(false);
                   break;
               case QTcpSocket::SocketTimeoutError:
                   qDebug() << "Connection Timeout!";
                   emit sig_connect_finish(false);
                   break;
               case QTcpSocket::NetworkError:
                   qDebug() << "Network Error!";
                   break;
               default:
                   qDebug() << "Other Error!";
                   break;
           }
     });

    // 处理连接断开
    QObject::connect(&socket_, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });
    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    initHandlers();
}

void TcpMgr::initHandlers()
{
    //连接聊天服务器后通过token验证之后的回包
    handlers_.insert(CHAT_LOGIN_RSP_ID,[this](ReqId id, int len, QByteArray data){
        qDebug()<< "handle id is "<< id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull()){
           qDebug() << "Failed to create QJsonDocument.";
           return;
        }

        QJsonObject json_obj = jsonDoc.object();
        if(!json_obj.contains("error")){
            int err = ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err ;
            emit sig_login_failed(err);
            return;
        }
        int err = json_obj["error"].toInt();
        if(err != SUCEESS){
            qDebug() << "Login Failed, err is " << err ;
            emit sig_login_failed(err);
            return;
        }

        auto uid = json_obj["id"].toInt();
        auto name = json_obj["name"].toString();
        auto nick = json_obj["nick"].toString();
        auto icon =json_obj["icon"].toString();
        auto sex = json_obj["sex"].toInt();
        auto user_info = std::make_shared<UserInfo>(uid, name, nick, icon, sex);

        UserMgr::GetInstance()->SetUserInfo(user_info);
        UserMgr::GetInstance()->SetToken(json_obj["token"].toString());
        //添加好友申请列表
        if(json_obj.contains("apply_list")){
            UserMgr::GetInstance()->AppendApplyList(json_obj["apply_list"].toArray());
        }

        //添加好友列表
        if (json_obj.contains("friend_list")) {
            UserMgr::GetInstance()->AppendFriendList(json_obj["friend_list"].toArray());
        }

        emit sig_switch_chatdlg();


    });

    handlers_.insert(ID_SEARCH_USER_RSP,[this](ReqId id, int len, QByteArray data){
        qDebug()<< "handle id is "<< id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull()){
           qDebug() << "Failed to create QJsonDocument.";
           return;
        }

        QJsonObject json_obj = jsonDoc.object();
        if(!json_obj.contains("error")){
            int err = ERR_JSON;
            qDebug() << "err is Json Parse Err" << err ;
            return;
        }
        int err = json_obj["error"].toInt();
        if(err != SUCEESS){
            qDebug() << "NOT Find User,err is " << err ;
            return;
        }

        auto search_info = std::make_shared<SearchInfo>(json_obj["id"].toInt(),json_obj["name"].toString(),
                json_obj["nick"].toString(),json_obj["desc"].toString(),
                json_obj["sex"].toInt(),json_obj["icon"].toString());

        emit sig_user_search(search_info);
    });

    handlers_.insert(ID_ADD_FRIEND_RSP,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
    });

    handlers_.insert(ID_NOTIFY_ADD_FRIEND_REQ,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            int err = ERR_JSON;
            qDebug() << "NOTIFY_ADD_FRIEND Failed, err is Json Parse Err" << err;
            emit sig_user_search(nullptr);
            return;
        }
        int err = jsonObj["error"].toInt();
        if (err != SUCEESS) {
            qDebug() << "NOTIFY_ADD_FRIEND Failed, err is " << err;
            emit sig_user_search(nullptr);
            return;
        }

        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();
        auto apply_info = std::make_shared<AddFriendApply>(
                    from_uid, name, desc,
                      icon, nick, sex);
        emit sig_friend_apply(apply_info);

    });

    handlers_.insert(ID_AUTH_FRIEND_RSP,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            int err = ERR_JSON;
            qDebug() << "Auth Friend Failed, err is Json Parse Err" << err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if (err != SUCEESS) {
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto uid = jsonObj["id"].toInt();
        auto rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(rsp);
        qDebug() << "Auth Friend Success " ;
    });

    handlers_.insert(ID_NOTIFY_AUTH_FRIEND_REQ,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
       qDebug() << "handle id is " << id << " data is " << data;
       // 将QByteArray转换为QJsonDocument
       QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
       // 检查转换是否成功
       if (jsonDoc.isNull()) {
           qDebug() << "Failed to create QJsonDocument.";
           return;
       }
       QJsonObject jsonObj = jsonDoc.object();
       if (!jsonObj.contains("error")) {
           int err = ERR_JSON;
           qDebug() << "Auth Friend Failed, err is " << err;
           return;
       }
       int err = jsonObj["error"].toInt();
       if (err != SUCEESS) {
           qDebug() << "Auth Friend Failed, err is " << err;
           return;
       }
       int from_uid = jsonObj["fromuid"].toInt();
       QString name = jsonObj["name"].toString();
       QString nick = jsonObj["nick"].toString();
       QString icon = jsonObj["icon"].toString();
       int sex = jsonObj["sex"].toInt();
       auto auth_info = std::make_shared<AuthInfo>(from_uid,name,
                                                   nick, icon, sex);
       emit sig_add_auth_friend(auth_info);
    });

    handlers_.insert(ID_NOTIFY_TEXT_CHAT_MSG_REQ,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
       qDebug() << "handle id is " << id << " data is " << data;
       // 将QByteArray转换为QJsonDocument
       QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
       // 检查转换是否成功
       if (jsonDoc.isNull()) {
           qDebug() << "Failed to create QJsonDocument.";
           return;
       }
       QJsonObject jsonObj = jsonDoc.object();
       if (!jsonObj.contains("error")) {
           int err = ERR_JSON;
           qDebug() << "NOTIFY_TEXT_CHAT_MSG Failed, err is " << err;
           return;
       }
       int err = jsonObj["error"].toInt();
       if (err != SUCEESS) {
           qDebug() << "NOTIFY_TEXT_CHAT_MSG Failed, err is " << err;
           return;
       }
       qDebug() << "Receive Text Chat Notify Success " ;
       auto msg_ptr = std::make_shared<TextChatMsg>(jsonObj["fromuid"].toInt(),
               jsonObj["touid"].toInt(),jsonObj["text_array"].toArray());
       emit sig_text_chat_msg(msg_ptr);

    });

    handlers_.insert(ID_TEXT_CHAT_MSG_RSP,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
       qDebug() << "handle id is " << id << " data is " << data;
       // 将QByteArray转换为QJsonDocument
       QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
       // 检查转换是否成功
       if (jsonDoc.isNull()) {
           qDebug() << "Failed to create QJsonDocument.";
           return;
       }
       QJsonObject jsonObj = jsonDoc.object();
       if (!jsonObj.contains("error")) {
           int err = ERR_JSON;
           qDebug() << "TEXT_CHAT_MSG Failed, err is " << err;
           return;
       }
       int err = jsonObj["error"].toInt();
       if (err != SUCEESS) {
           qDebug() << "TEXT_CHAT_MSG Failed, err is " << err;
           return;
       }

       qDebug() << "Receive Text Chat Rsp Success " ;
        //ui设置送达等标记 todo...
    });

}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    //qDebug() << "ReqId is: " << id;
    auto it = handlers_.find(id);
    if(it == handlers_.end()){
        qDebug() << "not find handler";
        return;
    }
    it.value()(id,len,data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<< "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    host_ = si.host;
    port_ = static_cast<uint16_t>(si.port.toUInt());
    qDebug() << host_<< ":" <<port_;
    socket_.connectToHost(host_,port_);
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    quint16 id = static_cast<quint16>(reqId);
    //将QString转成QByteArray类型
    //QByteArray databytes = data.toUtf8();
    //计算数据长度
    quint16 len = static_cast<quint16>(data.size());
    // 创建一个QByteArray用于存储要发送的所有数据
    QByteArray senddate;
    QDataStream out(&senddate, QIODevice::WriteOnly);
    // 写入id和数据长度，数据内容
    out << id << len;
    senddate.append(data);
    //发送数据
    socket_.write(senddate);

}
