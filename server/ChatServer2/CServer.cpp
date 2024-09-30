#include "CServer.h"
#include "AsioServicePool.h"
#include "UserMgr.h"

CServer::CServer(boost::asio::io_context& ioc, int port) : ioc_(ioc), port_(port), acceptor_(ioc, tcp::endpoint(tcp::v4(), port))
{
	std::cout << "Server start success, listen on port : " << port_ << std::endl;
	StartAccept();
}

CServer::~CServer()
{
	sessions_.clear();
}

void CServer::ClearSession(std::string session_id)
{
	
	if (sessions_.find(session_id) != sessions_.end()) {
		UserMgr::GetInstance()->RmvUserSession(atoi(sessions_[session_id]->GetUserId().c_str()));
	}

	{
		std::lock_guard<std::mutex> lock(mutex_);
		sessions_.erase(session_id);
	}

}

void CServer::StartAccept()
{
	auto& ioc = AsioServicePool::GetInstance()->GetIOService();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(ioc, this);
	acceptor_.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept,this,new_session,std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error)
{
	if (!error){
		new_session->Start();
		std::lock_guard<std::mutex> lock(mutex_);
		sessions_.insert(std::make_pair(new_session->GetSessionId(), new_session));
	}
	else {
		std::cout << "session accept failed, error is " << error.what() << std::endl;
	}
	StartAccept();
}
