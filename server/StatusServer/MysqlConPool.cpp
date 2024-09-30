#include "MysqlConPool.h"

MysqlConPool::MysqlConPool(size_t poolsize, std::string ip, std::string port, std::string user, std::string pass, std::string schema) : host_(ip + ":"+port),user_(user),pass_(pass),schema_(schema)
{
	try
	{
		for (int i = 0; i < poolsize; i++) {
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
			//连接数据库
			std::unique_ptr<sql::Connection> con(driver->connect(host_, user_, pass_));
			//设置使用哪一个数据库
			con->setSchema(schema_);
			//获取当前时间戳
			//auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			////将时间戳转换为秒
			//long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
			pool_.push(std::move(con));

		}
	}
	catch (const std::exception&)
	{
		// 处理异常
		std::cout << "mysql pool init failed" << std::endl;
	}
}

MysqlConPool::~MysqlConPool()
{
	std::unique_lock<std::mutex> lock(mutex_);
	while (!pool_.empty()) {
		pool_.pop();
	}
}

std::unique_ptr<sql::Connection> MysqlConPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		return !pool_.empty();
	});
	if (b_stop_) {
		return nullptr;
	}

	std::unique_ptr<sql::Connection> con(std::move(pool_.front()));
	pool_.pop();
	return con;
}

void MysqlConPool::returnConnection(std::unique_ptr<sql::Connection> con)
{
	
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_)	return;
	if (pool_.size() == poolsize_)	return;
	pool_.push(std::move(con));
	cond_.notify_one();
}

void MysqlConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}
