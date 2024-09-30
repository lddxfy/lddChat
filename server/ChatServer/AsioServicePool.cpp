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
//��Ϊ����ִ��work.reset��������iocontext��run��״̬���˳�
//��iocontext�Ѿ����˶���д�ļ����¼��󣬻���Ҫ�ֶ�stop�÷���
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
		//Ϊÿһ��ioc�����İ�һ��work����ֹ��ֱ���˳�
		works_[i] = std::unique_ptr<Work>(new Work(IOServices_[i]));
	}

	for (size_t i = 0; i < poolsize; i++) {
		threads_.emplace_back([this, i]() {
			IOServices_[i].run();
			});
	}
}
