#pragma once
#include "const.h"
#include "Singleton.h"
#include "CSession.h"
#include "MsgNode.h"
#include "data.h"
#include "ConfigMgr.h"
#include "message.pb.h"
#include "message.grpc.pb.h"

using message::AddFriendReq;

using FunCallBack = std::function<void(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data)>;



class LogicSystem : public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(std::shared_ptr<LogicNode>);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();
	void LoginHandler(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data);
	bool GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& applylist);
	bool GetFriendList(int uid, std::vector<std::shared_ptr<Userinfo>>& friendlist);
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<Userinfo>& userinfo);
	void SearchInfo(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data);
	void AddFriendApply(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data);
	void AuthFriendApply(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data);
	void DealChatTextMsg(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data);
	bool isPureDigit(std::string uid);
	void GetUserById(std::string uid, Json::Value& root);
	void GetUserByName(std::string uid, Json::Value& root);
	std::thread worker_thread_;
	std::queue<std::shared_ptr<LogicNode>> msg_que_;
	std::map<int, Userinfo> users_;
	std::mutex mutex_;
	std::condition_variable cond_;
	bool b_stop_;
	std::map<short, FunCallBack> fun_callbacks_;
};

