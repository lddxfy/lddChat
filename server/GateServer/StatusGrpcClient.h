#pragma once
#include "const.h"
#include "Singleton.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;


class StatusConPool
{
public:
	StatusConPool(size_t poolsize,const std::string& host,const std::string port);
	~StatusConPool();
	std::unique_ptr<StatusService::Stub> GetConnection();
	void ReturnConnection(std::unique_ptr<StatusService::Stub> context);
	void Close();
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<StatusService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


class StatusGrpcClient : public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient();

	GetChatServerRsp GetChatServer(int uid);



private:
	StatusGrpcClient();
	std::unique_ptr<StatusConPool> pool_;
};

