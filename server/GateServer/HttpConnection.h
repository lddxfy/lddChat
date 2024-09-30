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
	//读数据缓冲区
	beast::flat_buffer buffer_{ 8192 };
	//httpreq请求 http::dynamic_body可以动态的接收各种数据类型
	http::request<http::dynamic_body> request_;
	//httpresp响应
	http::response<http::dynamic_body> response_;
	//http连接定时器
	net::steady_timer deadline_{
		socket_.get_executor(),std::chrono::seconds(60)
	};
	//解析的url不带参  例如localhost:8888/gettest?key1=value1&key2=value2   中的gettest
	std::string _get_url;
	//解析的参数k-v
	std::unordered_map<std::string, std::string> _get_params;

};