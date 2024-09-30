#include "StatusGrpcClient.h"
#include "ConfigMgr.h"

StatusConPool::StatusConPool(size_t poolsize, const std::string& host, const std::string port) : poolSize_(poolsize),host_(host),port_(port)
{
    for (int i = 0; i < poolsize; i++) {
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
        connections_.push(StatusService::NewStub(channel));
    }
}

StatusConPool::~StatusConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
	}
}

std::unique_ptr<StatusService::Stub> StatusConPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
		});
	if (b_stop_) {
		return nullptr;
	}

	auto StatusConcon = std::move(connections_.front());
	connections_.pop();
	return StatusConcon;
}

void StatusConPool::ReturnConnection(std::unique_ptr<StatusService::Stub> context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	connections_.push(std::move(context));
	cond_.notify_one();
}

void StatusConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}

StatusGrpcClient::~StatusGrpcClient()
{

}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
	//获取调用上下文
	ClientContext context;
	GetChatServerReq req;
	GetChatServerRsp rsp;
	req.set_id(uid);
	//用于调用rpc请求的客户端连接
	auto stub = pool_->GetConnection();
	//调用Rpc请求
	Status status = stub->GetChatServer(&context, req,&rsp);
	if (status.ok()) {
		pool_->ReturnConnection(std::move(stub));
		return rsp;
	}
	else {
		rsp.set_error(ErrorCodes::RPCFailed); 
		pool_->ReturnConnection(std::move(stub));
		return rsp;
	}
}

StatusGrpcClient::StatusGrpcClient()
{
	auto cfg = ConfigMgr::GetInstance();
	auto host = cfg["StatusServer"]["Host"];
	auto port = cfg["StatusServer"]["Port"];
	pool_.reset(new StatusConPool(5, host, port));
}
