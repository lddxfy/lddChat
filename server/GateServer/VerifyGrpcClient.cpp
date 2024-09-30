#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"
RPConPool::~RPConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
	}
}
std::unique_ptr<VerifyService::Stub> RPConPool::getConnection()
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

	auto Rpccon = std::move(connections_.front());
	connections_.pop();
	return Rpccon;
}

void RPConPool::returnConnection(std::unique_ptr<VerifyService::Stub> context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	connections_.push(std::move(context));
	//有连接则唤醒
	cond_.notify_one();
}

void RPConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}

RPConPool::RPConPool(size_t poolsize,std::string host,std::string port) : poolSize_(poolsize),b_stop_(false),host_(host),port_(port)
{
	for (int i = 0; i < poolsize; i++) {
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port, grpc::InsecureChannelCredentials());
		connections_.push(VerifyService::NewStub(channel));
	}
}

GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email)
{
	ClientContext context;
	GetVerifyReq req;
	GetVerifyRsp rsp;

	req.set_email(email);

	auto stub = pool_->getConnection();
	Status status = stub->GetVerifyCode(&context, req, &rsp);

	if (status.ok()) {
		pool_->returnConnection(std::move(stub));
		return rsp;
	}
	else {
		pool_->returnConnection(std::move(stub));
		rsp.set_error(ErrorCodes::RPCFailed);
	}
	return rsp;
}

VerifyGrpcClient::VerifyGrpcClient()
{
	auto& cfg = ConfigMgr::GetInstance();
	std::string host = cfg["VerifyServer"]["Host"];
	std::string port = cfg["VerifyServer"]["Port"];
	pool_.reset(new RPConPool(5, host, port));
}
