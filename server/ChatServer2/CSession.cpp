#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"

CSession::CSession(boost::asio::io_context& ioc, CServer* server) : recv_head_node_(0),recv_msg_node_(0),socket_(ioc),b_stop_(false),b_head_parse_(false),server_(server)
{
	recv_head_node_.reset(new MsgNode(HEAD_TOTAL_LEN));
}

CSession::~CSession()
{
}

void CSession::Start()
{
	AsyncReadHead(HEAD_TOTAL_LEN);
}

void CSession::Close()
{
	socket_.close();
	b_stop_ = true;
}

void CSession::Send(std::string msg_data,short msgid)
{
	std::lock_guard<std::mutex> lock(send_lock_);
	if (send_que_.size() > MAX_QUEUE) {
		std::cout << " send que fulled, size is " << MAX_QUEUE << std::endl;
		return;
	}

	send_que_.push(std::make_shared<SendNode>(msg_data.c_str(),msg_data.length(), msgid));
	if (send_que_.size() > 1) {
		return;
	}
	auto msgnode = send_que_.front();
	boost::asio::async_write(socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_), 
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1,shared_from_this()));
	

}

void CSession::Send(char* msg, short max_length, short msgid)
{
	std::lock_guard<std::mutex> lock(send_lock_);
	if (send_que_.size() > MAX_QUEUE) {
		std::cout << " send que fulled, size is " << MAX_QUEUE << std::endl;
		return;
	}

	send_que_.push(std::make_shared<SendNode>(msg, max_length, msgid));
	if (send_que_.size() > 1) {
		return;
	}
	auto& msgnode = send_que_.front();
	boost::asio::async_write(socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}


void CSession::AsyncReadHead(int total_len)
{
	auto self = shared_from_this();
	asyncReadFull(total_len, [self,this](const boost::system::error_code& ec, size_t byte_tansfered) {
		if (ec) {
			std::cout << "handle read failed, error is " << ec.what() << std::endl;
			Close();
			server_->ClearSession(session_id_);
			return;
		}

		recv_head_node_->Clear();
		memcpy(recv_head_node_->data_, buffer_, byte_tansfered);

		short msgid;
		memcpy(&msgid, recv_head_node_->data_, HEAD_ID_LEN);
		msgid = boost::asio::detail::socket_ops::network_to_host_short(msgid);
		std::cout << "msgid is " << msgid << std::endl;

		short msglen;
		memcpy(&msglen, recv_head_node_->data_ + HEAD_ID_LEN, HEAD_DATA_LEN);
		msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
		std::cout << "msglen is " << msglen << std::endl;
		if (msglen > MAX_LENGTH) {
			std::cout << "invaild msglen is " << msglen << std::endl;
			Close();
			server_->ClearSession(session_id_);
			return;
		}

		recv_msg_node_.reset(new RecvNode(msglen, msgid));
		AsyncReadBody(msglen);

	});
}

void CSession::AsyncReadBody(int total_len)
{
	auto self = shared_from_this();
	asyncReadFull(total_len, [self, this](const boost::system::error_code& ec, size_t byte_tansfered) {
		if (ec) {
			std::cout << "handle read failed, error is " << ec.what() << std::endl;
			Close();
			server_->ClearSession(session_id_);
			return;
		}

		memcpy(recv_msg_node_->data_, buffer_, byte_tansfered);
		recv_msg_node_->cur_len_ += byte_tansfered;
		recv_msg_node_->data_[recv_msg_node_->total_len_] = '\0';
		std::cout << "recv msg is" << recv_msg_node_->data_;

		//此处将消息投递到逻辑队列中
		LogicSystem::GetInstance()->PostMsgToQue(std::make_shared<LogicNode>(shared_from_this(), recv_msg_node_));

		AsyncReadHead(HEAD_TOTAL_LEN);

	});
}

void CSession::asyncReadFull(size_t max_len, std::function<void(const boost::system::error_code&, size_t)> handler)
{
	memset(buffer_, 0, MAX_LENGTH);
	asyncReadLen(0, max_len, handler);
}

void CSession::asyncReadLen(size_t read_len, size_t total_len, std::function<void(const boost::system::error_code&, size_t)> handler)
{
	auto self = shared_from_this();
	socket_.async_read_some(boost::asio::buffer(buffer_ + read_len, total_len - read_len), [self,read_len,total_len,handler](boost::system::error_code err, std::size_t byte_tansfered) {
		if (err) {
			handler(err, read_len+byte_tansfered);
			return;
		}
		if (read_len+byte_tansfered >= HEAD_TOTAL_LEN) {
			handler(err, read_len + byte_tansfered);
			return;
		}
		//没有错误，但是长度不够头部长度则继续读
		self->asyncReadLen(read_len + byte_tansfered, total_len, handler);
	});
}

void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self)
{
	if (!error) {
		send_que_.pop();
		if (!send_que_.empty()) {
			std::lock_guard<std::mutex> lock(send_lock_);
			auto& msgnode = send_que_.front();
			boost::asio::async_write(socket_, boost::asio::buffer(msgnode->data_, msgnode->total_len_),
				std::bind(&CSession::HandleWrite, this, std::placeholders::_1,shared_self));
		}
	}
	else {
		std::cout << "handle write failed, error is " << error.what() << std::endl;
		Close();
		server_->ClearSession(session_id_);
	}
}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvnode) : session_(session),recvnode_(recvnode)
{
}
