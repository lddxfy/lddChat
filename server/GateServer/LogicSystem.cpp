#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "StatusGrpcClient.h"

LogicSystem::LogicSystem() {
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->response_.body()) << "recevice get_test req" <<std::endl;
		int i = 0;
		if (connection->_get_params.empty()) {
			beast::ostream(connection->response_.body()) << "NO param" << std::endl;
		}
		else {
			for (auto& p : connection->_get_params) {
				i++;
				beast::ostream(connection->response_.body()) << "param" << i << " key is " << p.first;
				beast::ostream(connection->response_.body()) << "value is " << p.second;
			}
		}
		
	});

	RegPost("/get_verifycode", [](std::shared_ptr<HttpConnection> connection) {
		//��ȡ������
		auto body_str = boost::beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		//����content_type�ֶ�
		connection->response_.set(http::field::content_type, "text/json");
		//�յ���json
		Json::Value src_root;
		//json������
		Json::Reader reader;
		//Ҫ���͵�json
		Json::Value root;
		//���յ����ֽ���ת��Ϊjson
		bool res = reader.parse(body_str, src_root);
		if (!res) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}

		if (!src_root.isMember("email")) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}


		auto email = src_root["email"].asString();
		root["email"] = src_root["email"];
		//����Rpc�������֤�������,��֤������������ʼ������䣬������֤�����redis�����ù���ʱ��Ϊ10min
		GetVerifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
		root["error"] = rsp.error();
		auto send_str = root.toStyledString();
		beast::ostream(connection->response_.body()) << send_str;
		return true;
	});

	RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->response_.set(http::field::content_type, "text/json");
		Json::Value src_root;
		Json::Reader reader;
		Json::Value root;
		auto res = reader.parse(body_str,src_root);
		if (!res) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}
		//�Ȳ���redis��email��Ӧ����֤���Ƿ����
		auto verifycode = src_root["verifycode"].asString();
		auto user = src_root["user"].asString();
		auto passwd = src_root["passwd"].asString();
		auto email = src_root["email"].asString();

		auto email_code = "code_" + src_root["email"].asString();
		std::string code;
		res = RedisMgr::GetInstance()->Get(email_code,code);
		if (!res) {
			std::cout << " get varify code expired" << std::endl;
			root["error"] = ErrorCodes::VerifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}

		if (verifycode != code) {
			std::cout << " verify code error" << std::endl;
			root["error"] = ErrorCodes::VerifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}

		//�������ݿ��ж��û��Ƿ����
		int uid = MysqlMgr::GetInstance()->RegUser(user, email, passwd);
		if (uid == -1 || uid == 0) {
			std::cout << " RegUser error" << std::endl;
			root["error"] = ErrorCodes::RegUserErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}

		root["error"] = 0;
		root["uid"] = uid;
		root["email"] = email;
		root["user"] = user;
		root["passwd"] = passwd;
		root["varifycode"] = src_root["varifycode"].asString();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->response_.body()) << jsonstr;
		return true;
	});


	RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection> connection) {
		//����������յ�����ת�����ַ���
		auto body_str = boost::beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->response_.set(http::field::content_type, "text/json");
		//�����л���json
		Json::Value src_json;
		Json::Reader reader;
		Json::Value root;

		auto res = reader.parse(body_str, src_json);
		if (!res) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}

		auto user = src_json["user"].asString();
		auto email = src_json["email"].asString();
		auto pwd = src_json["passwd"].asString();
		auto verifycode = src_json["verifycode"].asString();

		auto email_code = "code_" + email;
		std::string code;
		res = RedisMgr::GetInstance()->Get(email_code, code);
		if (!res || verifycode != code) {
			std::cout << "get varify code err" << std::endl;
			root["error"] = ErrorCodes::VerifyCodeErr;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}
		//��ѯ���ݿ��ж��û����������Ƿ�ƥ��
		res = MysqlMgr::GetInstance()->CheckEmail(email);
		if (!res) {
			std::cout << "user email not match err" << std::endl;
			root["error"] = ErrorCodes::EmailNotMatch;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}

		res = MysqlMgr::GetInstance()->ResetPwd(user,pwd);
		if (!res) {
			std::cout << "passwd reset error" << std::endl;
			root["error"] = ErrorCodes::PasswdResetErr;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}
		std::cout << "succeed to update password" << pwd << std::endl;
		root["error"] = Success;
		root["user"] = user;
		root["email"] = email;
		root["passwd"] = pwd;
		auto send_str = root.toStyledString();
		beast::ostream(connection->response_.body()) << send_str;
		return true;
		
	});

	RegPost("/user_login", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->response_.set(http::field::content_type, "text/json");

		Json::Value src_json;
		Json::Reader reader;
		Json::Value root;
		auto res = reader.parse(body_str, src_json);
		if (!res) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}

		auto email = src_json["email"].asString();
		auto pwd = src_json["passwd"].asString();

		Userinfo userinfo;
		res = MysqlMgr::GetInstance()->CheckLogin(email,pwd,userinfo);
		if (!res) {
			std::cout << "user email not match err" << std::endl;
			root["error"] = ErrorCodes::EmailNotMatch;
			//��Ҫ���͵�jsonת��Ϊ�ֽ���
			auto send_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << send_str;
			return true;
		}
		
		GetChatServerRsp rsp = StatusGrpcClient::GetInstance()->GetChatServer(userinfo.id);
		userinfo.token = rsp.token();
		std::cout << "user id is :" << userinfo.id << "user name is " << userinfo.name << "user token is " << userinfo.token <<std::endl;
		std::cout << "succeed to login" << pwd << std::endl;

		root["id"] = userinfo.id;
		root["name"] = userinfo.name;
		root["error"] = Success;
		root["email"] = email;
		root["passwd"] = pwd;
		root["token"] = rsp.token();
		root["host"] = rsp.host();
		root["port"] = rsp.port();
		
		auto send_str = root.toStyledString();
		beast::ostream(connection->response_.body()) << send_str;
		return true;

	});
}

bool LogicSystem::HandleGet(std::string url, std::shared_ptr<HttpConnection> con) {
	if (_get_handlers.find(url) == _get_handlers.end()) {
		return false;
	}

	_get_handlers[url](con);
	return true;
}
void LogicSystem::RegGet(std::string url, HttpHandler handler) {
	_get_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandlePost(std::string url, std::shared_ptr<HttpConnection> con) {
	if (_post_handlers.find(url) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[url](con);
	return true;
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
	_post_handlers.insert(std::make_pair(url, handler));
}