#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(size_t size) : nextIOService_(0),IOServices_(size),Works_(size)
{
	//��io_context��work�󶨣�����ᵼ���߳�ֱ���˳�����Ϊ�ڲ��¼�ѭ��û���¼�ִ��
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
	//��Ϊ����ִ��work.reset��������iocontext��run��״̬���˳�
	//��iocontext�Ѿ����˶���д�ļ����¼��󣬻���Ҫ�ֶ�stop�÷���
	for (auto& work : Works_) {
		work.get()->get_io_context().stop();
		work.reset();

	}

	for (auto& t: threads_) {
		t->join();
	}
}
