#pragma once
#include "const.h"
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(boost::asio::io_context& ioc);

	tcp::socket& GetSocket() { return this->socket_; }

	void Start();
private:
	void PreParseGetParam();
	void CheckDeadline();
	void WriteResponse();
	void HandleReq();

	tcp::socket socket_;
	//�����ݻ�����
	beast::flat_buffer buffer_{ 8192 };
	//httpreq���� http::dynamic_body���Զ�̬�Ľ��ո�����������
	http::request<http::dynamic_body> request_;
	//httpresp��Ӧ
	http::response<http::dynamic_body> response_;
	//http���Ӷ�ʱ��
	net::steady_timer deadline_{
		socket_.get_executor(),std::chrono::seconds(60)
	};
	//������url������  ����localhost:8888/gettest?key1=value1&key2=value2   �е�gettest
	std::string _get_url;
	//�����Ĳ���k-v
	std::unordered_map<std::string, std::string> _get_params;

};