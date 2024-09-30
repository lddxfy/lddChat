#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "conuseritem.h"
#include "grouptipitem.h"
#include "tcpmgr.h"
#include "global.h"
#include "usermgr.h"
class ContactUserList : public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget* parent = Q_NULLPTR);
    void ShowRedPoint(bool bshow = false);
    void addContactUserList();

signals:
    void sig_loading_contact_user();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo> user_info);
    void sig_switch_apply_friend_page();
private:
    bool eventFilter(QObject *watched, QEvent *event);
    ConUserItem* _add_friend_item;
    QListWidgetItem* _groupitem;
    bool _load_pending;
private slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);

};

#endif // CONTACTUSERLIST_H
