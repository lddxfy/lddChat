#include "MysqlMgr.h"

MysqlMgr::~MysqlMgr()
{
}

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    return dao_.RegUser(name, email, pwd);
}

int MysqlMgr::CheckEmail(const std::string& email)
{

    return dao_.CheckEmail(email);
}

int MysqlMgr::ResetPwd(const std::string& name, const std::string& pwd)
{
    return dao_.ResetPwd(name,pwd);
}

int MysqlMgr::CheckLogin(const std::string& email, const std::string& pwd)
{
    return dao_.CheckLogin(email,pwd);
}

int MysqlMgr::query()
{
    return dao_.Query();
}

int MysqlMgr::insert()
{
    return dao_.insert();
}

MysqlMgr::MysqlMgr()
{
}
