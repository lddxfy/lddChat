#pragma once
#include "const.h"
#include "MysqlConPool.h"
struct Userinfo {
    int id;
    std::string name;
    std::string email;
    std::string pwd;
    std::string token;
};

using Userinfo = struct Userinfo;

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int CheckEmail(const std::string& email);
    int ResetPwd(const std::string& name, const std::string& pwd);
    int CheckLogin(const std::string& email, const std::string& pwd,Userinfo& userinfo);
    int Query();
    int insert();
private:
    std::unique_ptr<MysqlConPool> pool_;
};



