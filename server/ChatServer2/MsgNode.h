#pragma once
#include "const.h"


struct MsgNode {
	MsgNode(short total_len) :total_len_(total_len), cur_len_(0) {
		data_ = new char[total_len + 1]();
		data_[total_len_] = '\0';
	}

	~MsgNode() {
		std::cout << "destruct Msgnode " << std::endl;
		delete[] data_;
	}

	void Clear() {
		memset(data_, 0, total_len_);
		cur_len_ = 0;
	}

	short total_len_;
	short cur_len_;
	char* data_;

};

class RecvNode : public MsgNode {
	friend class LogicSystem;
public:
	RecvNode(short max_len,short msgid);
private:
	short msgid_;
};

class SendNode : public MsgNode {
public:
	SendNode(const char* data,short max_len, short msgid);
private:
	short msgid_;
};

