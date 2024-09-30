#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>
#include "global.h"
#include "applyfrienditem.h"
#include "applyfriendlist.h"
#include <unordered_map>
#include "usermgr.h"
#include "authenfriend.h"

namespace Ui {
class ApplyFriendPage;
}

class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();
    void AddNewApply(std::shared_ptr<AddFriendApply> apply);
signals:
    void sig_show_search(bool bsearch);
private:
    void loadApplyList();
    Ui::ApplyFriendPage *ui;
    //存放已经添加的条目
    std::unordered_map<int, ApplyFriendItem*> _unauth_items;
    AuthenFriend* authfriend_;
private slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
};

#endif // APPLYFRIENDPAGE_H
