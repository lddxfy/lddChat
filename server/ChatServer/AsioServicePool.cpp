#include "AsioServicePool.h"

AsioServicePool::~AsioServicePool()
{
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioServicePool::GetIOService()
{
	auto& service = IOServices_[nextService_++];
	if (nextService_ == IOServices_.size()) {
		nextService_ = 0;
	}
	return service;
}
//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
void AsioServicePool::Stop()
{
	for (auto& work : works_) {
		work->get_io_context().stop();
		work.reset();
	}

	for (auto& t : threads_) {
		t.join();
	}
}

AsioServicePool::AsioServicePool(size_t poolsize) : IOServices_(poolsize),works_(poolsize),poolsize_(poolsize),nextService_(0)
{
	for (size_t i = 0; i < poolsize; i++) {
		//为每一个ioc上下文绑定一个work，防止它直接退出
		works_[i] = std::unique_ptr<Work>(new Work(IOServices_[i]));
	}

	for (size_t i = 0; i < poolsize; i++) {
		threads_.emplace_back([this, i]() {
			IOServices_[i].run();
			});
	}
}
