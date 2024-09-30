#pragma once
#include "const.h"
#include "MysqlConPool.h"
class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int CheckEmail(const std::string& email);
    int ResetPwd(const std::string& name, const std::string& pwd);
    int CheckLogin(const std::string& email, const std::string& pwd);
    int Query();
    int insert();
private:
    std::unique_ptr<MysqlConPool> pool_;
};



