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
    Error_Json = 1001,  //Json��������
    RPCFailed = 1002,  //RPC�������
    VerifyExpired = 1003, //��֤�볬ʱ
    VerifyCodeErr = 1004,//��֤�벻��ȷ
    RegUserErr = 1005, //ע���û�ʧ��
    EmailNotMatch = 1006,//�û����䲻ƥ��
    PasswdResetErr = 1007,//�޸�����ʧ��
    UidInvalid = 1008,//�û�id������
    TokenInvalid = 1009,//token��ƥ��
};

enum ReqId {
    MSG_CHAT_LOGIN = 1005, //��¼��Ϣ
    MSG_CHAT_LOGIN_RSP = 1006, //��¼�ذ���Ϣ
    ID_SEARCH_USER_REQ = 1007, //�û���������
    ID_SEARCH_USER_RSP = 1008, //�����û��ذ�
    ID_ADD_FRIEND_REQ = 1009, //�ͻ����û�������Ӻ�������
    ID_ADD_FRIEND_RSP = 1010, //�ͻ����û�������Ӻ��ѻظ�
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //֪ͨ�ͻ����û�����Ӻ�������
    ID_AUTH_FRIEND_REQ = 1013,  //��֤��������
    ID_AUTH_FRIEND_RSP = 1014,  //��֤���ѻظ�
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //֪ͨ�û���֤��������
    ID_TEXT_CHAT_MSG_REQ = 1017, //�ı�������Ϣ����
    ID_TEXT_CHAT_MSG_RSP = 1018, //�ı�������Ϣ�ظ�
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //֪ͨ�û��ı�������Ϣ
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