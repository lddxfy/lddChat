#include "chatuserwid.h"
#include "ui_chatuserwid.h"

ChatUserWid::ChatUserWid(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

void ChatUserWid::SetInfo(std::shared_ptr<UserInfo> userinfo)
{
    userinfo_ = userinfo;

    //加载头像图片
    QPixmap pixmap(userinfo_->_icon);
    //设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->name_lb->setText(userinfo_->_name);
    ui->msg_lb->setText(userinfo_->_last_msg);
}

void ChatUserWid::SetInfo(std::shared_ptr<FriendInfo> friend_info)
{
    userinfo_ = std::make_shared<UserInfo>(friend_info);
    // 加载图片
    QPixmap pixmap(userinfo_->_icon);

    // 设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->name_lb->setText(userinfo_->_name);
    ui->msg_lb->setText(userinfo_->_last_msg);
}


std::shared_ptr<UserInfo> ChatUserWid::GetUserInfo()
{
    return userinfo_;
}

void ChatUserWid::updateLastMsg(std::vector<std::shared_ptr<TextChatData>> chatdatas)
{
    QString last_msg = "";
    for (auto& msg : chatdatas) {
        last_msg = msg->_msg_content;
        userinfo_->_chat_msgs.push_back(msg);
    }

    userinfo_->_last_msg = last_msg;
    ui->msg_lb->setText(userinfo_->_last_msg);
}
