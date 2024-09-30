#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "global.h"
#include "userdata.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    void paintEvent(QPaintEvent *event) override;
    void SetUserInfo(std::shared_ptr<UserInfo> userinfo);
    void AppendChatMsg(std::shared_ptr<TextChatData> msg);
    std::shared_ptr<UserInfo> GetUserInfo();
signals:
    void sig_append_send_chat_msg(std::shared_ptr<TextChatData> msg);
private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> userinfo_;
};

#endif // CHATPAGE_H
