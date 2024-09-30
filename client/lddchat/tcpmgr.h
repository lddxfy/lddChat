#ifndef TCPMGR_H
#define TCPMGR_H

#include <QObject>
#include "singleton.h"
#include "global.h"
#include <QTcpSocket>
#include "userdata.h"

class TcpMgr : public QObject,public Singleton<TcpMgr>,std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    friend class Singleton<TcpMgr>;
    ~TcpMgr();

private:
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    QTcpSocket socket_;
    QString host_;
    uint16_t port_;
    QByteArray buffer_;
    bool b_recv_pending_;
    quint16 messageid_;
    quint16 messagelen_;

    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> handlers_;
signals:
    void sig_connect_finish(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_login_failed(int err);
    void sig_switch_chatdlg();
    void sig_user_search(std::shared_ptr<SearchInfo> si);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);           //向服务器发送通过好友验证后的回包发送的信号
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);  //收到对方通过好友验证发送信号
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg>);
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);
};

#endif // TCPMGR_H
