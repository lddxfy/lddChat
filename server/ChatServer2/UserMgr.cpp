#include "UserMgr.h"

UserMgr::~UserMgr()
{
    uid_to_session_.clear();
}

std::shared_ptr<CSession> UserMgr::GetSession(int uid)
{
    std::lock_guard<std::mutex> lock(session_mtx_);
    auto it = uid_to_session_.find(uid);
    if (it == uid_to_session_.end()) {
        return nullptr;
    }
    
    return it->second;
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<CSession> session)
{
    std::lock_guard<std::mutex> lock(session_mtx_);
    uid_to_session_[uid] = session;
}

void UserMgr::RmvUserSession(int uid)
{
    std::lock_guard<std::mutex> lock(session_mtx_);
    uid_to_session_.erase(uid);
}

UserMgr::UserMgr()
{
}
