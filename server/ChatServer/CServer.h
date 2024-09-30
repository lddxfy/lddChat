#pragma once
#include "boost/asio.hpp"
#include "CSession.h"
#include "const.h"
using namespace boost::asio::ip;

class CServer
{
public:
	CServer(boost::asio::io_context& ioc, short port);
	~CServer();
	void ClearSession(std::string);
private:
	void StartAccept();
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);
	boost::asio::io_context& ioc_;
	short port_;
	tcp::acceptor acceptor_;
	std::map<std::string, std::shared_ptr<CSession>> sessions_;
	std::mutex mutex_;
};

