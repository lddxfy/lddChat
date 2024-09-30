#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include "statewidget.h"
#include <QList>
#include "userdata.h"
#include "tcpmgr.h"
#include <QListWidgetItem>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void ShowSearch(bool bsearch);
    void addChatUserList();
    void AddLBGroup(StateWidget* stw);
    void ClearLabelState(StateWidget* stw);
    void handleGlobalMousePress(QMouseEvent*);
    void SetSelectChatItem(int uid = 0);
    void SetSelectChatPage(int uid = 0);
    void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>> chatdatas);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:

    Ui::ChatDialog *ui;
    ChatUIMode mode_;
    ChatUIMode state_;
    bool b_loading_;
    QList<StateWidget*> list_stw_;
    QMap<int,QListWidgetItem*> _chat_item_added;
    void loadMoreChatUser();
    void loadMoreConUser();
    QWidget* _last_widget;
    int _cur_chat_uid;
private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_loading_contact_user();
    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_switch_friend_info_page(std::shared_ptr<UserInfo> user_info);
    void slot_switch_apply_friend_page();
    void slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info);
    void slot_item_clicked(std::shared_ptr<UserInfo> userinfo);
    void slot_append_send_chat_msg(std::shared_ptr<TextChatData> msg);
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};

#endif // CHATDIALOG_H
