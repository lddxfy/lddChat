#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"

namespace Ui {
class ChatUserWid;
}

class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = nullptr);
    ~ChatUserWid();

    QSize sizeHint() const override {
        return QSize(250, 70); // 返回自定义的尺寸
    }

    //void SetInfo(QString name, QString head, QString msg);
    void SetInfo(std::shared_ptr<UserInfo>);
    void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    std::shared_ptr<UserInfo> GetUserInfo();
    void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> chatdatas);
private:
    Ui::ChatUserWid *ui;
//    QString name_;  //姓名
//    QString head_;  //头像
//    QString msg_;   //聊天记录

    std::shared_ptr<UserInfo> userinfo_;
};

#endif // CHATUSERWID_H
