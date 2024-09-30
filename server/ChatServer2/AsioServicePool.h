#pragma once
#include "Singleton.h"
#include "const.h"
class AsioServicePool : public Singleton<AsioServicePool>
{
	friend class Singleton<AsioServicePool>;
public:
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioServicePool();
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	//自动获取cpu核数
	AsioServicePool(size_t poolsize = std::thread::hardware_concurrency());
	size_t poolsize_;
	std::vector<boost::asio::io_context> IOServices_;
	std::vector<WorkPtr> works_;
	std::vector<std::thread> threads_;
	size_t nextService_;
};

