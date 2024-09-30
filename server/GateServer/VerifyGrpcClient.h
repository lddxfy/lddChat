#pragma once
#include "const.h"
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool{
	
public:
	RPConPool(size_t poolsize, std::string host, std::string port);
	RPConPool& operator=(const RPConPool&) = default;
	RPConPool(const RPConPool&) = default;
	~RPConPool();
	std::unique_ptr<VerifyService::Stub> getConnection();
	void returnConnection(std::unique_ptr<VerifyService::Stub> context);
	void Close();
private:
	
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;

};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;

public:

	GetVerifyRsp GetVerifyCode(std::string email);
		
private:
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
	//rpcµ÷ÓÃ·½
	//std::unique_ptr<VerifyService::Stub> stub_;
};

