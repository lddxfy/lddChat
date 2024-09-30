#include "StatusServiceImpl.h"
#include "ConfigMgr.h"

std::string generate_unique_string() {
	// 创建UUID对象
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	// 将UUID转换为字符串
	std::string unique_string = to_string(uuid);
	return unique_string;
}

StatusServiceImpl::StatusServiceImpl() : _server_index(0)
{
	auto cfg = ConfigMgr::GetInstance();
	std::vector<std::string> servers_name;
	std::string server_name;
	std::stringstream  servers(cfg["ChatServers"]["Name"]);
	while (std::getline(servers, server_name, ',')) {
		servers_name.push_back(server_name);
	}
	for (auto& word : servers_name) {
		if (cfg[word]["Name"].empty()) {
			continue;
		}
		chatServer server;
		server.host_ = cfg[word]["Host"];
		server.port_ = cfg[word]["Port"];
		server.name_ = cfg[word]["Name"];
		_servers.insert(std::make_pair(word,server));
	}
}

StatusServiceImpl::~StatusServiceImpl()
{
}



Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response)
{
	std::string prefix("lddxfy status server has received : ");
	const auto& server = getChatServer();
	response->set_host(server.host_);
	response->set_port(server.port_);
	response->set_error(Success);
	response->set_token(generate_unique_string());
	insertToken(request->id(), response->token());
	return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(token_key, token);
}

chatServer StatusServiceImpl::getChatServer()
{
	std::lock_guard<std::mutex> lock(server_mtx_);
	auto minServer = _servers.begin()->second;
	auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, minServer.name_);
	if (count_str.empty()) {
		//不存在则默认设置为最大
		minServer.con_count = INT_MAX;
	}
	else {
		minServer.con_count = std::stoi(count_str);
	}

	for (auto& server : _servers) {
		if (server.second.name_ == minServer.name_) {
			continue;
		}

		auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name_);
		if (count_str.empty()) {
			server.second.con_count = INT_MAX;

		}
		else {
			server.second.con_count = std::stoi(count_str);
		}

		if (server.second.con_count < minServer.con_count) {
			minServer = server.second;
		}
	}
	return minServer;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* response)
{
	auto uid = request->uid();
	auto token = request->token();
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (!success) {
		response->set_error(ErrorCodes::UidInvalid);
		return Status::OK;
	}

	if (token_value != token) {
		response->set_error(ErrorCodes::TokenInvalid);
		return Status::OK;
	}

	response->set_uid(uid);
	response->set_token(token);
	response->set_error(Success);
	return Status::OK;
}
