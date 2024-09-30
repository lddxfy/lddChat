#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "hiredis.h"

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

#define HEAD_TOTAL_LEN 4
#define HEAD_DATA_LEN 2
#define HEAD_ID_LEN 2
#define MAX_LENGTH 1024 * 2
#define MAX_QUEUE 64

enum ErrorCodes {
    Success = 0,
    Error_Json = 1001,  //Json解析错误
    RPCFailed = 1002,  //RPC请求错误
    VerifyExpired = 1003, //验证码超时
    VerifyCodeErr = 1004,//验证码不正确
    RegUserErr = 1005, //注册用户失败
    EmailNotMatch = 1006,//用户邮箱不匹配
    PasswdResetErr = 1007,//修改密码失败
    UidInvalid = 1008,//用户id不存在
    TokenInvalid = 1009,//token不匹配
};

enum ReqId {
    MSG_CHAT_LOGIN = 1005, //登录消息
    MSG_CHAT_LOGIN_RSP = 1006, //登录回包消息
    ID_SEARCH_USER_REQ = 1007, //用户搜索请求
    ID_SEARCH_USER_RSP = 1008, //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009, //客户端用户申请添加好友请求
    ID_ADD_FRIEND_RSP = 1010, //客户端用户申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //通知客户端用户有添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,  //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,  //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ = 1017, //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP = 1018, //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
};

class Defer {
public:
    Defer(std::function<void()> func) : func_(func) {};
    ~Defer() {
        func_();
    }
private:
    std::function<void()> func_;
};




namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#define USERIPPREFIX  "uip_"
#define USERTOKENPREFIX  "utoken_"
#define IPCOUNTPREFIX  "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT  "logincount"
#define NAME_INFO  "nameinfo_"