// StatusServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "const.h"
#include "ConfigMgr.h"
#include "hiredis.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "AsioIOServicePool.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "StatusServiceImpl.h"
#include <grpcpp/grpcpp.h>


void RunServer() {
    auto& cfg = ConfigMgr::GetInstance();

    std::string server_addr(cfg["StatusServer"]["Host"] + ":" + cfg["StatusServer"]["Port"]);
    //服务的实现类，继承了grpc中的StatusService
    StatusServiceImpl service;
    //服务的建立者，将服务注册给它
    grpc::ServerBuilder builder;
    //Rpc服务监听端口
    builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
    //注册RPC服务
    builder.RegisterService(&service);

    //构建并启动grpc服务
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on" << server_addr << std::endl;

    boost::asio::io_context ioc;
    //向ioc上注册信号（输入crtl + c时服务器优雅退出）
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

    //设置异步等待SIGINT信号
    signals.async_wait([&server](const boost::system::error_code& error, int signal_num) {
        if (!error) {
            std::cout << "Shutting down server ..." << std::endl;
            server->Shutdown();
        }
        });

    //在单独线程中运行io_context,因ioc.run会阻塞线程，它的底层不断轮询检查是否有就绪事件
    std::thread([&ioc]() {
        ioc.run();
        }).detach();

    //等待服务器关闭
    server->Wait();
    ioc.stop();
    
}


int main()
{
    try
    {
        RunServer();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}

