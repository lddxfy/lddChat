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
	//������Ӱ󶨸������߳��ϵ�ioc������ǰ�߳�ֻ�����������
	auto NewCon = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(NewCon->GetSocket(), [self,NewCon](boost::system::error_code ec) {
		try
		{
			if (ec) {
				self->Start();
				return;
			}
			
			//���������ӣ�����HpptConnection�����������
			//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			//�ú����ڲ��������׽��ֵĶ�д�¼�����ע���������̵߳�ioc��
			NewCon->Start();
			//��������
			self->Start();
			
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception is :" << e.what() << std::endl;
			self->Start();
		}
	});
}