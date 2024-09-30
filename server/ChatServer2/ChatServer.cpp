// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "const.h"
#include "CServer.h"
#include "AsioServicePool.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"



int main()
{
	auto& cfg = ConfigMgr::GetInstance();
	auto server_name = cfg["SelfServer"]["Name"];
	try
	{
		auto port = cfg["SelfServer"]["Port"];
		auto pool = AsioServicePool::GetInstance();
		
		RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, "0");
		std::string server_address(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);
		ChatServiceImpl service;
		grpc::ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&service);
		//构建并启动grpc服务器
		std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
		std::cout<< "RPC Server listening on " << server_address << std::endl;

		//单独启动一个线程处理grpc服务
		std::thread grpc_server_thread([&]() {
			server->Wait();
			});

		boost::asio::io_context ioc;
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc, pool,&server](auto, auto) {
			ioc.stop();
			pool->Stop();
			server->Shutdown();
			});
		CServer s(ioc, atoi(port.c_str()));
		ioc.run();

		RedisMgr::GetInstance()->HDel(LOGIN_COUNT,server_name);
		RedisMgr::GetInstance()->Close();
		grpc_server_thread.join();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception is : " << e.what() << std::endl;
	}
    return 0;
}


