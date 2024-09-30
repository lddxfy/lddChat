#pragma once
#include "const.h"
#include "RedisMgr.h"
class RedisConPool
{
public:
	RedisConPool(size_t poolsize, const char* host, int port, const char* pwd);
	~RedisConPool();
	redisContext* GetRedisCon();
	void returnConnection(redisContext* con);
	void Close();
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	const char* host_;
	int port_;
	std::queue<redisContext*> RedisCons_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

