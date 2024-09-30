#pragma once

#include "const.h"
#include "MsgNode.h"
class CServer;
class CSession : public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& ioc, CServer* server);
	~CSession();
	tcp::socket& GetSocket() { return socket_; }
	void Start();
	void Close();
	void Send(std::string msg_data,short msgid);
	void Send(char* msg, short max_length, short msgid);
	const std::string& GetSessionId() { return session_id_; }
	void SetUserId(std::string user_id) { user_id_ = user_id; }
	const std::string& GetUserId() { return user_id_; }
	void AsyncReadHead(int total_len);
	void AsyncReadBody(int total_len);
private:
	//¶ÁÂúÖ¸¶¨×Ö½Ú
	void asyncReadFull(size_t max_len, std::function<void(const boost::system::error_code&,size_t)> handler);
	void asyncReadLen(size_t read_len, size_t total_len,
		std::function<void(const boost::system::error_code&, size_t)> handler);
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);


	std::shared_ptr<MsgNode> recv_head_node_;

	std::shared_ptr<RecvNode> recv_msg_node_;
	std::shared_ptr<SendNode> send_msg_node_;

	tcp::socket socket_;
	CServer* server_;
	char buffer_[MAX_LENGTH];
	bool b_stop_;
	std::queue<std::shared_ptr<SendNode> > send_que_;
	std::mutex send_lock_;
	bool b_head_parse_;
	std::string session_id_;
	std::string user_id_;
};

class LogicNode {
	friend class LogicSystem;
public:
	LogicNode(std::shared_ptr<CSession>, std::shared_ptr<RecvNode>);
private:
	std::shared_ptr<CSession> session_;
	std::shared_ptr<RecvNode> recvnode_;
};

