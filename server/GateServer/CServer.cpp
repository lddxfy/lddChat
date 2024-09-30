#include "CServer.h"
#include "AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc),
																	_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
																	,_socket(ioc)
{

}

void CServer::Start() {
	auto self = shared_from_this();
	//std::cout << "server start" << std::endl;
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	//这个连接绑定给其他线程上的ioc处理，当前线程只负责接收连接
	auto NewCon = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(NewCon->GetSocket(), [self,NewCon](boost::system::error_code ec) {
		try
		{
			if (ec) {
				self->Start();
				return;
			}
			
			//处理新链接，创建HpptConnection类管理新连接
			//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			//该函数内部，对于套接字的读写事件都是注册在其他线程的ioc上
			NewCon->Start();
			//继续监听
			self->Start();
			
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception is :" << e.what() << std::endl;
			self->Start();
		}
	});
}