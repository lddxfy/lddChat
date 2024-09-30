#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "MysqlMgr.h"
#include "RedisMgr.h"
#include "UserMgr.h"
#include "ChatGrpcClient.h"


LogicSystem::~LogicSystem()
{
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg_node)
{
	std::unique_lock<std::mutex> lock(mutex_);
	msg_que_.push(msg_node);
	if (msg_que_.size() == 1) {

		cond_.notify_one();
	}
}

LogicSystem::LogicSystem() : b_stop_(false)
{
	RegisterCallBacks();
	worker_thread_ = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg()
{
	while (true) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (!b_stop_ && msg_que_.empty()) {
			cond_.wait(lock);
		}
		if (b_stop_) {
			while (!msg_que_.empty()) {
				auto msg_node = msg_que_.front();
				std::cout << "recv_msg id  is : " << msg_node->recvnode_->msgid_ << std::endl;
				auto it = fun_callbacks_.find(msg_node->recvnode_->msgid_);
				if (it == fun_callbacks_.end()) {
					msg_que_.pop();
					continue;
				}
				it->second(msg_node->session_, msg_node->recvnode_->msgid_, std::string(msg_node->recvnode_->data_, msg_node->recvnode_->cur_len_));
				msg_que_.pop();
			}
			break;
		}

		auto msg_node = msg_que_.front();
		std::cout << "receive msgnode body is : " << msg_node->recvnode_->data_ << std::endl;
		auto it = fun_callbacks_.find(msg_node->recvnode_->msgid_);
		if (it == fun_callbacks_.end()) {
			msg_que_.pop();
			std::cout << "msg id [" << msg_node->recvnode_->msgid_ << "] handler not found" << std::endl;
			continue;
		}
		it->second(msg_node->session_, msg_node->recvnode_->msgid_, std::string(msg_node->recvnode_->data_, msg_node->recvnode_->cur_len_));
		msg_que_.pop();

	}
}

void LogicSystem::RegisterCallBacks()
{
	fun_callbacks_[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	fun_callbacks_[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	fun_callbacks_[ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	fun_callbacks_[ID_AUTH_FRIEND_REQ] = std::bind(&LogicSystem::AuthFriendApply, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	fun_callbacks_[ID_TEXT_CHAT_MSG_REQ] = std::bind(&LogicSystem::DealChatTextMsg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Value src_root;
	Json::Value root;
	Json::Reader reader;

	bool res = reader.parse(msg_data, src_root);
	
	if (!res) {

	}
	auto uid = src_root["id"].asInt();
	auto token = src_root["token"].asString();
	std::cout << "user login uid is  " << uid << " user token is "
		<< token << std::endl;
	
	Defer defer([this,&root,session]() {
		std::string return_str = root.toStyledString();
		std::cout << "return_str is: " << return_str << std::endl;
		session->Send(return_str.c_str(), MSG_CHAT_LOGIN_RSP);
	});

	//判断token是否合法，不合法直接返回，合法则去数据库中查找
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	res = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (!res) {
		root["error"] = ErrorCodes::UidInvalid;
		return;
	}

	if (token != token_value) {
		root["error"] = ErrorCodes::TokenInvalid;
		return;
	}
	root["error"] = ErrorCodes::Success;
	//获取用户信息
	std::string base_key = USER_BASE_INFO + uid_str;
	auto user_info = std::make_shared<Userinfo>();
	bool b_base = GetBaseInfo(base_key, uid, user_info);
	if (!b_base) {
		root["error"] = ErrorCodes::UidInvalid;
		std::cout << "uid 不存在";
		return;
	}

	root["id"] = uid;
	root["pwd"] = user_info->pwd;
	root["name"] = user_info->name;
	root["email"] = user_info->email;
	root["nick"] = user_info->nick;
	root["desc"] = user_info->desc;
	root["sex"] = user_info->sex;
	root["icon"] = user_info->icon;

	//TODO    从数据库获取申请列表
	std::vector<std::shared_ptr<ApplyInfo>> apply_list;
	auto b_apply = GetFriendApplyInfo(uid, apply_list);
	if (b_apply) {
		for (auto& apply : apply_list) {
			Json::Value obj;
			obj["name"] = apply->_name;
			obj["id"] = apply->_uid;
			obj["icon"] = apply->_icon;
			obj["nick"] = apply->_nick;
			obj["sex"] = apply->_sex;
			obj["desc"] = apply->_desc;
			obj["status"] = apply->_status;
			root["apply_list"].append(obj);
		}
	}
	//TODO     获取好友列表
	std::vector<std::shared_ptr<Userinfo>> friend_list;
	auto b_friend = GetFriendList(uid, friend_list);
	if (b_friend) {
		for (auto& userinfo : friend_list) {
			Json::Value obj;
			obj["name"] = userinfo->name;
			obj["id"] = userinfo->id;
			obj["icon"] = userinfo->icon;
			obj["nick"] = userinfo->nick;
			obj["sex"] = userinfo->sex;
			obj["desc"] = userinfo->desc;
			obj["back"] = userinfo->back;
			root["friend_list"].append(obj);
		}
	}

	auto cfg = ConfigMgr::GetInstance();
	auto server_name = cfg["SelfServer"]["Name"];
	//将登录数量增加
	auto rd_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
	int count = 0;
	if (!rd_res.empty()) {
		count = std::stoi(rd_res);
	}
	count++;
	auto count_str = std::to_string(count);
	RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);
	//session绑定用户uid
	session->SetUserId(uid_str);
	//为用户设置登录ip server的名字,用于向不在当前服务器的用户发请求
	std::string ipkey = USERIPPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(ipkey, server_name);

	//uid和session绑定管理，方便以后踢人操作
	UserMgr::GetInstance()->SetUserSession(uid, session);

	return;
}

bool LogicSystem::GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& applylist)
{
	return MysqlMgr::GetInstance()->GetApplyList(to_uid, applylist, 0, 100);
}

bool LogicSystem::GetFriendList(int uid, std::vector<std::shared_ptr<Userinfo>>& friendlist)
{
	return MysqlMgr::GetInstance()->GetFriendList(uid, friendlist, 0, 100);
}

bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<Userinfo>& userinfo)
{
	std::string base_val = "";
	auto res = RedisMgr::GetInstance()->Get(base_key, base_val);
	std::cout <<"RedisMgr::GetInstance()->Get: " << base_val << std::endl;
	if (res) {
		Json::Value root;
		Json::Reader reader;
		reader.parse(base_val, root);
		userinfo->id = root["id"].asInt();
		userinfo->name = root["name"].asString();
		userinfo->pwd = root["pwd"].asString();
		userinfo->email = root["email"].asString();
		userinfo->nick = root["nick"].asString();
		userinfo->desc = root["desc"].asString();
		userinfo->sex = root["sex"].asInt();
		userinfo->icon = root["icon"].asString();
		std::cout << "user login uid is  " << userinfo->id << " name  is "
			<< userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << std::endl;
	}
	else {
		bool flag = MysqlMgr::GetInstance()->QueryUserFromId(uid, userinfo);
		if (!flag) {
			return false;
		}
		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["id"] = uid;
		redis_root["pwd"] = userinfo->pwd;
		redis_root["name"] = userinfo->name;
		redis_root["email"] = userinfo->email;
		redis_root["nick"] = userinfo->nick;
		redis_root["desc"] = userinfo->desc;
		redis_root["sex"] = userinfo->sex;
		redis_root["icon"] = userinfo->icon;
		RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
	}


	return true;
}

void LogicSystem::SearchInfo(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Value src_root;
	Json::Value root;
	Json::Reader reader;

	reader.parse(msg_data, src_root);
	auto uid_str = src_root["id"].asString();
	std::cout << "user SearchInfo uid is  " << uid_str << std::endl;

	Defer defer([this, &uid_str, session,&root]() {
		std::string return_str = root.toStyledString();
		session->Send(return_str.c_str(),ID_SEARCH_USER_RSP);
	});

	bool b_digit = isPureDigit(uid_str);
	if (b_digit) {
		GetUserById(uid_str,root);
	}
	else {
		GetUserByName(uid_str, root);
	}

}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Value src_root;
	Json::Value root;
	Json::Reader reader;

	reader.parse(msg_data, src_root);
	auto uid = src_root["id"].asInt();
	auto applyname = src_root["applyname"].asString();
	auto bakname = src_root["bakname"].asString();
	auto touid = src_root["touid"].asInt();
	std::cout << "user login uid is  " << uid << " applyname  is "
		<< applyname << " bakname is " << bakname << " touid is " << touid << std::endl;

	root["error"] = Success;
	Defer defer([this, &uid, session, &root]() {
		std::string return_str = root.toStyledString();
		session->Send(return_str.c_str(), ID_ADD_FRIEND_RSP);
		});

	//先更新数据库
	MysqlMgr::GetInstance()->AddFriendApply(uid, touid);
	std::string to_uid_str = std::to_string(touid);
	//查询redis 查找touid对应的server ip
	std::string to_uid_servername = "";
	bool b_ip = RedisMgr::GetInstance()->Get(USERIPPREFIX + to_uid_str, to_uid_servername);
	if (!b_ip)	return;

	std::cout << "to_uid_servername is : " << to_uid_servername;

	auto cfg = ConfigMgr::GetInstance();
	auto self_name = cfg["SelfServer"]["Name"];
	//在当前服务器中，发送请求给对方
	if (to_uid_servername == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session) {
			Json::Value notify;
			notify["error"] = Success;
			notify["applyuid"] = uid;
			notify["name"] = applyname;
			notify["desc"] = "";

			auto send_str = notify.toStyledString();
			session->Send(send_str, ID_NOTIFY_ADD_FRIEND_REQ);
		}
		return;
	}
	auto from_uid_str = std::to_string(uid);
	//找到当前发起好友申请用户的基本信息
	std::string base_key = USER_BASE_INFO + from_uid_str;
	auto user_info = std::make_shared<Userinfo>();
	GetBaseInfo(base_key,uid,user_info);

	AddFriendReq req;
	req.set_applyuid(user_info->id);
	req.set_desc(user_info->desc);
	req.set_icon(user_info->icon);
	req.set_nick(user_info->nick);
	req.set_name(user_info->name);
	req.set_sex(user_info->sex);
	req.set_touid(touid);
	//向在其他服务器的用户发起好友申请
	ChatGrpcClient::GetInstance()->NotifyAddFriend(to_uid_servername, req);	
}

void LogicSystem::AuthFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	Json::Value src_root;
	reader.parse(msg_data, src_root);
	auto uid = src_root["fromuid"].asInt();
	auto touid = src_root["touid"].asInt();
	auto back_name = src_root["back"].asString();
	std::cout << "from " << uid << " auth friend to " << touid << std::endl;
	root["error"] = ErrorCodes::Success;
	auto user_info = std::make_shared<Userinfo>();
	auto to_str = std::to_string(touid);
	std::string base_key = USER_BASE_INFO + to_str;
	auto b_info = GetBaseInfo(base_key, touid, user_info);
	if (b_info) {
		root["name"] = user_info->name;
		root["nick"] = user_info->nick;
		root["icon"] = user_info->icon;
		root["sex"] = user_info->sex;
		root["id"] = touid;
	}
	else {
		root["error"] = ErrorCodes::UidInvalid;
	}

	Defer defer([this, &uid, session, &root]() {
		std::string return_str = root.toStyledString();
		session->Send(return_str.c_str(), ID_AUTH_FRIEND_RSP);
		});


	//先更新数据库
	MysqlMgr::GetInstance()->AuthFriendApply(uid, touid);
	//更新数据库添加好友
	MysqlMgr::GetInstance()->AddFriend(uid, touid, back_name);

	//查询redis 查找touid对应的server ip
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetInstance()->Get(to_ip_key, to_ip_value);
	if (!b_ip) {
		return;
	}
	auto& cfg = ConfigMgr::GetInstance();
	auto self_name = cfg["SelfServer"]["Name"];
	//直接通知对方有认证通过消息
	if (to_ip_value == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session) {
			//在内存中则直接发送通知对方
			Json::Value  notify;
			notify["error"] = ErrorCodes::Success;
			notify["fromuid"] = uid;
			notify["touid"] = touid;
			std::string base_key = USER_BASE_INFO + std::to_string(uid);
			auto user_info = std::make_shared<Userinfo>();
			bool b_info = GetBaseInfo(base_key, uid, user_info);
			if (b_info) {
				notify["name"] = user_info->name;
				notify["nick"] = user_info->nick;
				notify["icon"] = user_info->icon;
				notify["sex"] = user_info->sex;
			}
			else {
				notify["error"] = ErrorCodes::UidInvalid;
			}
			std::string return_str = notify.toStyledString();
			session->Send(return_str, ID_NOTIFY_AUTH_FRIEND_REQ);
		}
		return;
	}
	AuthFriendReq auth_req;
	auth_req.set_fromuid(uid);
	auth_req.set_touid(touid);
	//发送通知
	ChatGrpcClient::GetInstance()->NotifyAuthFriend(to_ip_value, auth_req);



}

void LogicSystem::DealChatTextMsg(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	Json::Value src_root;
	reader.parse(msg_data, src_root);

	const Json::Value  arrays = src_root["text_array"];
	auto fromuid = src_root["fromuid"].asInt();
	auto touid = src_root["touid"].asInt();

	root["error"] = Success;
	root["text_array"] = arrays;
	root["fromuid"] = fromuid;
	root["touid"] = touid;

	Defer defer([this, &root, session]() {
		std::string return_str = root.toStyledString();
		session->Send(return_str, ID_TEXT_CHAT_MSG_RSP);
		});

	std::string to_str = std::to_string(touid);
	//查询redis 查找touid对应的server ip
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetInstance()->Get(to_ip_key, to_ip_value);
	if (!b_ip) {
		return;
	}

	auto& cfg = ConfigMgr::GetInstance();
	auto self_name = cfg["SelfServer"]["Name"];
	if (to_ip_value == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session) {
			//在内存中则直接发送通知对方
			std::string return_str = root.toStyledString();
			session->Send(return_str, ID_NOTIFY_TEXT_CHAT_MSG_REQ);
		}
		return;
	}

	TextChatMsgReq text_msg_req;
	text_msg_req.set_fromuid(fromuid);
	text_msg_req.set_touid(touid);
	for (const auto& txt_obj : arrays) {
		auto content = txt_obj["content"].asString();
		auto msgid = txt_obj["msgid"].asString();
		std::cout << "content is " << content << std::endl;
		std::cout << "msgid is " << msgid << std::endl;
		auto* text_msg = text_msg_req.add_textmsgs();
		text_msg->set_msgid(msgid);
		text_msg->set_msgcontent(content);
	}
	//发送通知 
	ChatGrpcClient::GetInstance()->NotifyTextChatMsg(to_ip_value, text_msg_req, root);
}

bool LogicSystem::isPureDigit(std::string uid)
{
	for (char& c : uid) {
		if (!std::isdigit(c)) {
			return false;
		}
	}
	return true;
}

void LogicSystem::GetUserById(std::string uid, Json::Value& root)
{
	root["error"] = Success;
	auto key = USER_BASE_INFO + uid;
	std::string value = "";
	bool flag = RedisMgr::GetInstance()->Get(key, value);
	if (flag) {
		Json::Value valuejson;
		Json::Reader reader;
		reader.parse(value, valuejson);
		auto uid = valuejson["id"].asInt();
		auto name = valuejson["name"].asString();
		auto pwd = valuejson["pwd"].asString();
		auto email = valuejson["email"].asString();
		auto nick = valuejson["nick"].asString();
		auto desc = valuejson["desc"].asString();
		auto sex = valuejson["sex"].asInt();
		auto icon = valuejson["icon"].asString();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon << std::endl;

		root["id"] = uid;
		root["pwd"] = pwd;
		root["name"] = name;
		root["email"] = email;
		root["nick"] = nick;
		root["desc"] = desc;
		root["sex"] = sex;
		root["icon"] = icon;
		return;
	}
	else {
		auto user_info = std::make_shared<Userinfo>();
		int uidint = atoi(uid.c_str());
		bool flag = MysqlMgr::GetInstance()->QueryUserFromId(uidint, user_info);
		if (!flag) {
			root["error"] = ErrorCodes::UidInvalid;
			return;
		}
		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["id"] = user_info->id;
		redis_root["pwd"] = user_info->pwd;
		redis_root["name"] = user_info->name;
		redis_root["email"] = user_info->email;
		redis_root["nick"] = user_info->nick;
		redis_root["desc"] = user_info->desc;
		redis_root["sex"] = user_info->sex;
		redis_root["icon"] = user_info->icon;

		RedisMgr::GetInstance()->Set(key, redis_root.toStyledString());

		//返回数据
		root["id"] = user_info->id;
		root["pwd"] = user_info->pwd;
		root["name"] = user_info->name;
		root["email"] = user_info->email;
		root["nick"] = user_info->nick;
		root["desc"] = user_info->desc;
		root["sex"] = user_info->sex;
		root["icon"] = user_info->icon;
		return;
	}

}

void LogicSystem::GetUserByName(std::string uid, Json::Value& root)
{
	root["error"] = Success;
	auto key = NAME_INFO + uid;
	std::string value = "";
	bool flag = RedisMgr::GetInstance()->Get(key, value);
	if (flag) {
		Json::Value valuejson;
		Json::Reader reader;
		reader.parse(value, valuejson);
		auto uid = valuejson["id"].asInt();
		auto name = valuejson["name"].asString();
		auto pwd = valuejson["pwd"].asString();
		auto email = valuejson["email"].asString();
		auto nick = valuejson["nick"].asString();
		auto desc = valuejson["desc"].asString();
		auto sex = valuejson["sex"].asInt();
		auto icon = valuejson["icon"].asString();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon << std::endl;

		root["id"] = uid;
		root["pwd"] = pwd;
		root["name"] = name;
		root["email"] = email;
		root["nick"] = nick;
		root["desc"] = desc;
		root["sex"] = sex;
		root["icon"] = icon;
		return;
	}
	else {
		auto user_info = std::make_shared<Userinfo>();
		bool flag = MysqlMgr::GetInstance()->QueryUserFromId(uid, user_info);
		if (!flag) {
			root["error"] = ErrorCodes::UidInvalid;
			return;
		}
		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["id"] = user_info->id;
		redis_root["pwd"] = user_info->pwd;
		redis_root["name"] = user_info->name;
		redis_root["email"] = user_info->email;
		redis_root["nick"] = user_info->nick;
		redis_root["desc"] = user_info->desc;
		redis_root["sex"] = user_info->sex;
		redis_root["icon"] = user_info->icon;

		RedisMgr::GetInstance()->Set(key, redis_root.toStyledString());

		//返回数据
		root["id"] = user_info->id;
		root["pwd"] = user_info->pwd;
		root["name"] = user_info->name;
		root["email"] = user_info->email;
		root["nick"] = user_info->nick;
		root["desc"] = user_info->desc;
		root["sex"] = user_info->sex;
		root["icon"] = user_info->icon;
		return;
	}
}
