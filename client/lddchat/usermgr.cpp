#include "usermgr.h"
#define CHAT_COUNT_PER_PAGE 20;
UserMgr::~UserMgr()
{

}

//void UserMgr::SetName(QString name)
//{
//    this->name_ = name;
//}

//void UserMgr::SetUid(int uid)
//{
//    this->uid_ = uid;
//}

void UserMgr::SetToken(QString token)
{
    this->token_ = token;
}

std::vector<std::shared_ptr<ApplyInfo>> UserMgr::GetApplyList()
{
    return _apply_list;
}

bool UserMgr::AlreadyApply(int uid)
{
    for(auto& info : _apply_list){
        if(info->_uid == uid){
            return true;
        }
    }
    return false;
}

void UserMgr::AddApplyList(std::shared_ptr<ApplyInfo> apply)
{
    _apply_list.push_back(apply);
}

void UserMgr::AppendApplyList(QJsonArray array)
{
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue value : array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["id"].toInt();
        auto status = value["status"].toInt();
        auto info = std::make_shared<ApplyInfo>(uid, name,
                           desc, icon, nick, sex, status);
        _apply_list.push_back(info);
    }
}

void UserMgr::AppendFriendList(QJsonArray array)
{
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue value : array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["id"].toInt();
        auto back = value["back"].toString();
        //auto status = value["status"].toInt();
        auto info = std::make_shared<FriendInfo>(uid,name,nick,icon,sex,"",back);
        AddFriend(info);
    }
}

void UserMgr::SetUserInfo(std::shared_ptr<UserInfo> userinfo)
{
     userinfo_ = userinfo;

}

std::shared_ptr<UserInfo> UserMgr::GetUserInfo()
{
    return userinfo_;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::GetChatListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friendlist;
    int begin = chat_loaded_;
    int end = chat_loaded_ + CHAT_COUNT_PER_PAGE;

    if(begin >= _friend_list.size()){
        return friendlist;
    }

    if(end > _friend_list.size()){
        friendlist = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin,_friend_list.end());
        return friendlist;
    }

    friendlist = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin,_friend_list.begin()+end);
    UpdateChatLoadedCount(true);
    return friendlist;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::GetConListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friendlist;
    int begin = contact_user_load_;
    int end = contact_user_load_ + CHAT_COUNT_PER_PAGE;

    if(begin >= _friend_list.size()){
        return friendlist;
    }

    if(end > _friend_list.size()){
        friendlist = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin,_friend_list.end());
        return friendlist;
    }

    friendlist = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin,_friend_list.begin()+end);
    UpdateChatLoadedCount(false);
    return friendlist;
}

void UserMgr::UpdateChatLoadedCount(bool flag) {
    int begin;
    if(flag){
        begin = chat_loaded_;
    }else{
        begin = contact_user_load_;
    }

    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= _friend_list.size()) {
        return ;
    }

    if (end > _friend_list.size()) {
        if(flag){
            chat_loaded_ = _friend_list.size();
        }else{
            contact_user_load_ = _friend_list.size();
        }
        return ;
    }

    if(flag){
        chat_loaded_ = end;
    }else{
        contact_user_load_ = end;
    }
}

bool UserMgr::IsLoadChatFin()
{
    if(chat_loaded_ == _friend_list.size()){
        return true;
    }
    return false;
}

bool UserMgr::IsLoadContactUserFin()
{
    if(contact_user_load_ == _friend_list.size()){
        return true;
    }
    return false;
}


bool UserMgr::CheckFriendById(int uid)
{
    auto it = _friend_map.find(uid);
    if(it == _friend_map.end()){
        return false;
    }
    return true;
}

std::shared_ptr<FriendInfo> UserMgr::GetFriendById(int uid)
{
    auto it = _friend_map.find(uid);
    if(it == _friend_map.end()){
        return nullptr;
    }
    return it.value();
}

void UserMgr::AddFriend(std::shared_ptr<FriendInfo> friendinfo)
{
    _friend_list.push_back(friendinfo);
    _friend_map[friendinfo->_uid] = friendinfo;
}

void UserMgr::AppendFriendChatMsg(int uid, std::vector<std::shared_ptr<TextChatData> > chatdatas)
{
    auto friend_info = GetFriendById(uid);
    if(friend_info == nullptr){
        return;
    }
    friend_info->AppendChatMsgs(chatdatas);

}

UserMgr::UserMgr() : chat_loaded_(0),contact_user_load_(0)
{

}
