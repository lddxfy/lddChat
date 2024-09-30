#include "chatpage.h"
#include "ui_chatpage.h"
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include "chatitembase.h"
#include "textbubble.h"
#include "picturebubble.h"
#include "messagetextedit.h"
#include <QDebug>
#include "usermgr.h"
#include <QUuid>
#include <QJsonDocument>
#include "tcpmgr.h"

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);

    ui->emo_lb->SetState("normal","normal_hover","normal_press","selected","selected_hover","selected_press");
    ui->file_lb->SetState("normal","normal_hover","normal_press","selected","selected_hover","selected_press");

    ui->send_btn->SetState("normal","hover","press");
    ui->receive_btn->SetState("normal","hover","press");
}

ChatPage::~ChatPage()
{
    delete ui;
}
//保持与应用程序其他部分的风格一致
void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatPage::SetUserInfo(std::shared_ptr<UserInfo> userinfo)
{
    userinfo_ = userinfo;
    //设置ui界面
    ui->title_label->setText(userinfo_->_name);
    ui->chat_data_wid->removeAllItem();
    for(auto& msg : userinfo_->_chat_msgs){
        AppendChatMsg(msg);
    }
}

void ChatPage::AppendChatMsg(std::shared_ptr<TextChatData> msg)
{
    auto self_info = UserMgr::GetInstance()->GetUserInfo();
    ChatRole role;
    //todo... 添加聊天显示
    if (msg->_from_uid == self_info->_uid) {
        role = ChatRole::Self;
        ChatItemBase* pChatItem = new ChatItemBase(role);

        pChatItem->setUserName(self_info->_name);
        pChatItem->setUserIcon(QPixmap(self_info->_icon));
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_wid->appendChatItem(pChatItem);
    }
    else {
        role = ChatRole::Other;
        ChatItemBase* pChatItem = new ChatItemBase(role);
        auto friend_info = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
        if (friend_info == nullptr) {
            return;
        }
        pChatItem->setUserName(friend_info->_name);
        pChatItem->setUserIcon(QPixmap(friend_info->_icon));
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_wid->appendChatItem(pChatItem);
    }
}

std::shared_ptr<UserInfo> ChatPage::GetUserInfo()
{
    return userinfo_;
}

void ChatPage::on_send_btn_clicked()
{
    if (userinfo_ == nullptr) {
        qDebug() << "friend_info is empty";
        return;
    }
    auto userinfo = UserMgr::GetInstance()->GetUserInfo();
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::Self;
    QString username = userinfo->_name;
    QString usericon = userinfo->_icon;

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    QJsonObject textObj;
    QJsonArray textArray;
    int txt_size = 0;

    for (int i = 0; i < msgList.size(); ++i) {
        if (msgList[i].content.length() > 1024) {
            continue;
        }

        QString type = msgList[i].msgFlag;
        ChatItemBase* pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(username);
        pChatItem->setUserIcon(QPixmap(usericon));
        QWidget* pBubble = nullptr;

        if (type == "text") {
            //生成唯一id
            QUuid uuid = QUuid::createUuid();
            //转为字符串
            QString uuidString = uuid.toString();

            pBubble = new TextBubble(role, msgList[i].content);

            if (txt_size + msgList[i].content.length() > 1024) {
                textObj["fromuid"] = userinfo->_uid;
                textObj["touid"] = userinfo_->_uid;
                textObj["text_array"] = textArray;
                QJsonDocument doc(textObj);
                QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
                //发送并清空之前累计的文本列表
                txt_size = 0;
                textArray = QJsonArray();
                textObj = QJsonObject();
                //发送tcp请求给chat server
                emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
            }

            //将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
            //_bubble_map[uuidString] = pBubble;
            txt_size += msgList[i].content.length();
            QJsonObject obj;
            QByteArray utf8Message = msgList[i].content.toUtf8();
            obj["content"] = QString::fromUtf8(utf8Message);
            obj["msgid"] = uuidString;
            textArray.append(obj);
            auto txt_msg = std::make_shared<TextChatData>(uuidString, obj["content"].toString(),
                userinfo->_uid, userinfo_->_uid);
            emit sig_append_send_chat_msg(txt_msg);

        }
        else if (type == "image") {
            pBubble = new PictureBubble(QPixmap(msgList[i].content), role);
        }
        else if (type == "file") {

        }

        if (pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_data_wid->appendChatItem(pChatItem);
        }
    }


    qDebug() << "textArray is " << textArray;
    //发送给服务器
    textObj["text_array"] = textArray;
    textObj["fromuid"] = userinfo->_uid;
    textObj["touid"] = userinfo_->_uid;
    QJsonDocument doc(textObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    //发送并清空之前累计的文本列表
    txt_size = 0;
    textArray = QJsonArray();
    textObj = QJsonObject();
    //发送tcp请求给chat server
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
}
