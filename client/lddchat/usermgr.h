#ifndef USERMGR_H
#define USERMGR_H
#include "global.h"
#include "singleton.h"
#include "applyfriendlist.h"
#include <vector>
#include <QJsonArray>
#include "userdata.h"
#include <QMap>
class UserMgr : public QObject,public Singleton<UserMgr>
{
    friend class Singleton<UserMgr>;
public:
    ~UserMgr();
    void SetName(QString name);
    void SetUid(int uid);
    void SetToken(QString token);
    int GetUid(){return userinfo_->_uid;}
    QString GetName(){return userinfo_->_name;}
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
    bool AlreadyApply(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo>);
    void AppendApplyList(QJsonArray array);
    void AppendFriendList(QJsonArray array);
    void SetUserInfo(std::shared_ptr<UserInfo> userinfo);
    std::shared_ptr<UserInfo> GetUserInfo();
    std::vector<std::shared_ptr<FriendInfo>> GetChatListPerPage();
    std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
    bool CheckFriendById(int uid);
    std::shared_ptr<FriendInfo> GetFriendById(int uid);
    void AddFriend(std::shared_ptr<FriendInfo> friendinfo);
    void AppendFriendChatMsg(int uid,std::vector<std::shared_ptr<TextChatData>> chatdatas);
    void UpdateChatLoadedCount(bool flag);
    bool IsLoadChatFin();
    bool IsLoadContactUserFin();
private:
    UserMgr();
    //QString name_;
    QString token_;
    //int uid_;
    std::shared_ptr<UserInfo> userinfo_;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    std::vector<std::shared_ptr<FriendInfo>> _friend_list;
    QMap<int,std::shared_ptr<FriendInfo>> _friend_map;
    int chat_loaded_;
    int contact_user_load_;
};

#endif // USERMGR_H
