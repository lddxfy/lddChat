#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "global.h"
#include "userdata.h"

class ChatUserList : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatUserList(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool _load_pending;
signals:
    void sig_loading_chat_user();
    void sig_switch_user_chat_page(std::shared_ptr<UserInfo> userinfo);
public slots:
    void slot_item_clicked(QListWidgetItem *item);
    
};

#endif // CHATUSERLIST_H
