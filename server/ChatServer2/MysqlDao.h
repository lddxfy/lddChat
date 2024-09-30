#pragma once
#include "const.h"
#include "MysqlConPool.h"
#include "data.h"

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    int CheckEmail(const std::string& email);
    int ResetPwd(const std::string& name, const std::string& pwd);
    int CheckLogin(const std::string& email, const std::string& pwd,Userinfo& userinfo);
    int QueryUserFromId(int uid, std::shared_ptr<Userinfo>& userinfo);
    int QueryUserFromId(std::string uid, std::shared_ptr<Userinfo>& userinfo);
    int GetApplyList(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& applylist, int begin, int limit);
    int GetFriendList(int uid, std::vector<std::shared_ptr<Userinfo>>& friendlist, int begin, int limit);
    int AuthFriendApply(int uid, int to_uid);
    int AddFriend(int uid, int to_uid, std::string bkname);
    int AddFriendApply(int uid, int touid);
    int insert();
private:
    std::unique_ptr<MysqlConPool> pool_;
};



