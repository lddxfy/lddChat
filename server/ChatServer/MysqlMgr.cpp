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

int MysqlMgr::CheckLogin(const std::string& email, const std::string& pwd, Userinfo& userinfo)
{
    return dao_.CheckLogin(email,pwd,userinfo);
}

int MysqlMgr::QueryUserFromId(int uid, std::shared_ptr<Userinfo>& userinfo)
{
    return dao_.QueryUserFromId(uid,userinfo);
}

int MysqlMgr::QueryUserFromId(std::string uid, std::shared_ptr<Userinfo>& userinfo)
{
    return dao_.QueryUserFromId(uid, userinfo);
}

int MysqlMgr::GetApplyList(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& applylist, int begin, int limit)
{
    return dao_.GetApplyList(to_uid,applylist,begin, limit);
}

int MysqlMgr::GetFriendList(int uid, std::vector<std::shared_ptr<Userinfo>>& friendlist, int begin, int limit)
{
    return dao_.GetFriendList(uid,friendlist,begin,limit);
}

int MysqlMgr::AuthFriendApply(int uid, int to_uid)
{
    return dao_.AuthFriendApply(uid,to_uid);
}

int MysqlMgr::AddFriend(int uid, int to_uid, std::string bkname)
{
    return dao_.AddFriend(uid,to_uid,bkname);
}

int MysqlMgr::AddFriendApply(int uid, int touid)
{
    return dao_.AddFriendApply(uid,touid);
}



int MysqlMgr::insert()
{
    return dao_.insert();
}

MysqlMgr::MysqlMgr()
{
}
