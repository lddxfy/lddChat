#pragma once

#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include <grpcpp/grpcpp.h> 
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <queue>
#include "const.h"
#include "data.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginRsp;
using message::LoginReq;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

using grpc::ServerContext;

class ChatServiceImpl final : public ChatService::Service
{
public:
    ChatServiceImpl();
    Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request,
        AddFriendRsp* reply) override;
    Status NotifyAuthFriend(ServerContext* context,
        const AuthFriendReq* request, AuthFriendRsp* response) override;
    Status NotifyTextChatMsg(::grpc::ServerContext* context,
        const TextChatMsgReq* request, TextChatMsgRsp* response) override;
    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<Userinfo>& userinfo);
private:

};

