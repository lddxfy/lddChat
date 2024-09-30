#include "HttpConnection.h"
#include "LogicSystem.h"
#include "util.h"
HttpConnection::HttpConnection(boost::asio::io_context& ioc) : socket_(ioc){

}
void HttpConnection::Start() {
	auto self = shared_from_this();
	http::async_read(socket_, buffer_, request_, [self](beast::error_code ec,
		std::size_t bytes_transferred) {
			try
			{
				if (ec) {
					std::cout << "http read err is" << ec.what() << std::endl;
					return;
				}
				//�������������
				boost::ignore_unused(bytes_transferred);
				self->HandleReq();
				self->CheckDeadline();
			}
			catch (const std::exception& exp)
			{
				std::cout << "exception is " << exp.what() << std::endl;
			}
			

		});
}

void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();
	deadline_.async_wait([self](boost::system::error_code ec) {
		/*if (ec) {
			std::cout << "time wait err is" << ec.what() << std::endl;
			return;
		}*/
		self->socket_.close(ec);
	});
}

void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t) {
		if (ec) {
			std::cout << "http write err is" << ec.what() << std::endl;
			return;
		}
		self->socket_.shutdown(tcp::socket::shutdown_send, ec);
		self->deadline_.cancel();
	});
}

void HttpConnection::PreParseGetParam() {
	// ��ȡ URI  
	auto uri = request_.target();
	// ���Ҳ�ѯ�ַ����Ŀ�ʼλ�ã��� '?' ��λ�ã�  
	auto query_pos = uri.find('?');
	if (query_pos == std::string::npos) {
		_get_url = uri;
		return;
	}
	_get_url = uri.substr(0, query_pos);
	std::string query_string = uri.substr(query_pos + 1);
	std::string key;
	std::string value;
	size_t pos = 0;
	while ((pos = query_string.find('&')) != std::string::npos) {
		auto pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(pair.substr(0, eq_pos)); // ������ url_decode ����������URL����  
			value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);
	}
	// �������һ�������ԣ����û�� & �ָ�����  
	if (!query_string.empty()) {
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}

void HttpConnection::HandleReq() {
	//���ð汾
	response_.version(request_.version());
	//����Ϊ������
	response_.keep_alive(false);
	bool success = false;
	switch (request_.method())
	{
	case http::verb::get:
		PreParseGetParam();
		success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
		if (!success) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		response_.result(http::status::ok);
		response_.set(http::field::server, "GateServer");
		WriteResponse();
		break;

	case http::verb::post:
		success = LogicSystem::GetInstance()->HandlePost(request_.target(), shared_from_this());
		if (!success) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		response_.result(http::status::ok);
		response_.set(http::field::server, "GateServer");
		WriteResponse();
		break;
	default:
		break;
	}


}