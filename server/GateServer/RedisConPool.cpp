#include "RedisConPool.h"

RedisConPool::RedisConPool(size_t poolsize, const char * host, int port, const char* pwd) : poolSize_(poolsize),host_(host),port_(port),b_stop_(false)
{
	for (int i = 0; i < poolsize; i++) {
		
		redisContext* context = redisConnect(host,port);
		if (context == nullptr || context->err) {
			if (context != nullptr) {
				redisFree(context);
			}
			continue;
		}
		redisReply* reply = (redisReply*)redisCommand(context, "AUTH %s", pwd);
		if (reply->type == REDIS_REPLY_ERROR) {
			std::cout << "��֤ʧ��" << std::endl;
			//ִ�гɹ� �ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�
			freeReplyObject(reply);
			continue;
		}
		//ִ�гɹ� �ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�
		freeReplyObject(reply);
		std::cout << "��֤�ɹ�" << std::endl;
		RedisCons_.push(context);
	}
}

RedisConPool::~RedisConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	while (!RedisCons_.empty()) {
		auto* con = RedisCons_.front();
		redisFree(con);
		RedisCons_.pop();
	}
}


redisContext* RedisConPool::GetRedisCon()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		return !RedisCons_.empty();
	});
	if (b_stop_) {
		return nullptr;
	}
	auto* con = RedisCons_.front();
	RedisCons_.pop();
	return con;

}

void RedisConPool::returnConnection(redisContext* con)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	RedisCons_.push(con);
	cond_.notify_one();

	
}

void RedisConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}
