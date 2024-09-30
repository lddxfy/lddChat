#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(size_t size) : nextIOService_(0),IOServices_(size),Works_(size)
{
	//将io_context与work绑定，否则会导致线程直接退出，因为内部事件循环没有事件执行
	for (int i = 0; i < size; i++) {
		//Works_[i] = std::unique_ptr<Work>(new Work(IOServices_[i]));
		Works_[i] = std::make_unique<Work>(IOServices_[i]);
	}

	for (int i = 0; i < IOServices_.size(); i++) {
		threads_.emplace_back(new std::thread([this,i]() {
			IOServices_[i].run();
		}));
	}
}

AsioIOServicePool::~AsioIOServicePool()
{
	Stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

AsioIOServicePool::IOService& AsioIOServicePool::GetIOService()
{
	auto& service = IOServices_[nextIOService_++];
	if (nextIOService_ == IOServices_.size()) {
		nextIOService_ = 0;
	}
	return service;
}


void AsioIOServicePool::Stop()
{
	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
	for (auto& work : Works_) {
		work.get()->get_io_context().stop();
		work.reset();

	}

	for (auto& t: threads_) {
		t->join();
	}
}
