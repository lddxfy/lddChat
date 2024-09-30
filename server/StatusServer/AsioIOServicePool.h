#pragma once
#include "const.h"
#include "Singleton.h"
class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	IOService& GetIOService();
	void Stop();
	
private:
	AsioIOServicePool(size_t size = 2);
	std::vector<IOService> IOServices_;
	std::vector<std::unique_ptr<Work>> Works_;
	std::vector<std::thread*> threads_;
	std::size_t nextIOService_;

};

