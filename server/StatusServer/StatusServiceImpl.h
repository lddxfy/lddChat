#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"  
#include "boost/uuid/uuid_io.hpp" //to_string
#include "RedisMgr.h"
#include "const.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginReq;
using message::LoginRsp;


struct chatServer {
	std::string host_;
	std::string port_;
	std::string name_;
	int con_count;
	chatServer(){}
	chatServer(const std::string& host,const std::string& port) : host_(host),port_(port){}
};

class StatusServiceImpl :public StatusService::Service 
{
public:
	StatusServiceImpl();
	~StatusServiceImpl();
	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response) override;
	void insertToken(int uid,std::string token);
	chatServer getChatServer();
	Status Login(ServerContext* context, const LoginReq* request, LoginRsp* response) override;
	std::unordered_map<std::string, chatServer> _servers;
	int _server_index;
private:
	std::map<int, std::string> tokens_;
	std::mutex server_mtx_;
};

