#pragma once
#include "const.h"
#include "Singleton.h"
#include "MysqlDao.h"
class MysqlMgr : public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	int CheckEmail(const std::string& email);
	int ResetPwd(const std::string& name, const std::string& pwd);
	int CheckLogin(const std::string& email, const std::string& pwd, Userinfo& userinfo);
	int query();
	int insert();
private:
	MysqlMgr();
	MysqlDao dao_;
};

