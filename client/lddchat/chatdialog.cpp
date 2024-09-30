#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "chatuserwid.h"
#include <QAction>
#include <QRandomGenerator>
#include "loadingdlg.h"
#include <QDebug>
#include <searchlist.h>
#include <QMouseEvent>
#include "friendinfopage.h"
//测试数据
std::vector<QString>  strs1 ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                            "You have to love yourself",
                            "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads1 = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg"
};
std::vector<QString> names1 = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};

void ChatDialog::addChatUserList()
{
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if(!friend_list.empty()){
        for(auto &friend_ele : friend_list){
            auto find_iter = _chat_item_added.find(friend_ele->_uid);
            if(find_iter != _chat_item_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item,chat_user_wid);
            _chat_item_added.insert(friend_ele->_uid,item);
        }
    }

    // 创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs1.size();
        int head_i = randomValue%heads1.size();
        int name_i = randomValue%names1.size();
        auto *chat_user_wid = new ChatUserWid();
        auto userinfo = std::make_shared<UserInfo>(0,names1[name_i],"",heads1[head_i],0,strs1[str_i]);
        chat_user_wid->SetInfo(userinfo);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

void ChatDialog::AddLBGroup(StateWidget* stw)
{
    list_stw_.push_back(stw);
}

void ChatDialog::ClearLabelState(StateWidget *stw)
{
    for(auto & ele:list_stw_){
        if(ele == stw){
            continue;
        }
        ele->ClearState();
    }
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *ev)
{
    if(mode_ != ChatUIMode::SearchMode){
        return;
    }
    // 将鼠标点击位置转换为搜索列表坐标系中的位置
    QPoint posInSearchList = ui->search_list->mapFromGlobal(ev->globalPos());
    // 判断点击位置是否在聊天列表的范围内
    if(!ui->search_list->rect().contains(posInSearchList)){
        ui->search_edit->clear();
        state_ = ChatUIMode::ChatMode;
        ShowSearch(false);
    }
}



bool ChatDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        auto *mouseevent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseevent);
    }
    return QDialog::eventFilter(obj,event);
}

void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (!friend_list.empty()) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_item_added.find(friend_ele->_uid);
            if(find_iter != _chat_item_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_item_added.insert(friend_ele->_uid, item);
        }
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::GetInstance()->GetConListPerPage();
    if (friend_list.empty()) {
       for(auto & friend_ele : friend_list){
           auto *chat_user_wid = new ConUserItem();
           chat_user_wid->SetInfo(friend_ele->_uid,friend_ele->_name,
                                  friend_ele->_icon);
           QListWidgetItem *item = new QListWidgetItem;
           //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
           item->setSizeHint(chat_user_wid->sizeHint());
           ui->con_user_list->addItem(item);
           ui->con_user_list->setItemWidget(item, chat_user_wid);
       }
   }
}

void ChatDialog::slot_loading_chat_user()
{
    if(b_loading_){
        return;
    }
    b_loading_ = true;
    LoadingDlg *loadingdlg = new LoadingDlg(this);
    loadingdlg->setModal(true);//设置为模态，不阻塞当前对话框
    loadingdlg->show();
    qDebug() << "more user loading...";
    loadMoreChatUser();
    // 加载完成后关闭对话框
    loadingdlg->deleteLater();

    b_loading_ = false;

}

void ChatDialog::slot_loading_contact_user()
{
    qDebug() << "slot loading contact user";
    if(b_loading_){
        return;
    }

    b_loading_ = true;
    LoadingDlg *loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreConUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    b_loading_ = false;
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item " << endl;
    auto find_iter = _chat_item_added.find(si->_uid);
    if(find_iter != _chat_item_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget
    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_item_added.insert(si->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();
}

void ChatDialog::slot_switch_apply_friend_page()
{
    qDebug() << "receive switch apply friend page sig";
    _last_widget = ui->friend_apply_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
}

void ChatDialog::slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info)
{
    qDebug() << "slot jump chat item from infopage" << endl;
    auto find_iter = _chat_item_added.find(user_info->_uid);
    if(find_iter != _chat_item_added.end()){
        qDebug() << "jump to chat item from infopage, uid is " << user_info->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(user_info->_uid);
        //更新聊天界面信息
        SetSelectChatPage(user_info->_uid);
        slot_side_chat();
        return;
    }
    //如果没找到，则创建新的插入listwidget
    auto* chat_user_wid = new ChatUserWid();
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_item_added.insert(user_info->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(user_info->_uid);
    //更新聊天界面信息
    SetSelectChatPage(user_info->_uid);
    slot_side_chat();
}

void ChatDialog::slot_item_clicked(std::shared_ptr<UserInfo> userinfo)
{
    SetSelectChatItem(userinfo->_uid);
    //更新聊天界面信息
    SetSelectChatPage(userinfo->_uid);
}

void ChatDialog::slot_append_send_chat_msg(std::shared_ptr<TextChatData> msg)
{
    if (_cur_chat_uid == 0) {
        return;
    }
    auto it = _chat_item_added.find(_cur_chat_uid);
    if (it == _chat_item_added.end()) {
        return;
    }
    QWidget* widget = ui->chat_user_list->itemWidget(it.value());
    if (widget == nullptr) {
        return;
    }

    auto itembase = qobject_cast<ListItemBase*>(widget);
    auto itemtype = itembase->GetItemType();
    if (itemtype == ListItemType::CHAT_USER_ITEM) {
        auto chat_user = qobject_cast<ChatUserWid*>(itembase);
        if (chat_user == nullptr) {
            return;
        }

        auto user_info = chat_user->GetUserInfo();
        user_info->_chat_msgs.push_back(msg);
        return;
    }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    auto find_iter = _chat_item_added.find(msg->_from_uid);
    if(find_iter != _chat_item_added.end()){
        qDebug() << "set chat item msg, uid is " << msg->_from_uid;
        QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatUserWid*>(widget);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录
        UpdateChatMsg(msg->_chat_msgs);
        UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
        return;
    }
    //如果没找到，则创建新的插入listwidget
    auto* chat_user_wid = new ChatUserWid();
    //查询好友信息
    auto fi_ptr = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
    chat_user_wid->SetInfo(fi_ptr);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    chat_user_wid->updateLastMsg(msg->_chat_msgs);
     UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_item_added.insert(msg->_from_uid, item);
}

void ChatDialog::slot_switch_friend_info_page(std::shared_ptr<UserInfo> user_info)
{
    qDebug()<<"receive switch friend info page sig";
    _last_widget = ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_info_page);
    ui->friend_info_page->SetInfo(user_info);
}

void ChatDialog::slot_side_chat()
{
    qDebug()<< "receive side chat clicked";
    //实现侧边工具列表中只有一个是selected状态
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    state_ = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact()
{
    qDebug()<< "receive side contact clicked";
    ClearLabelState(ui->side_contact_lb);
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    state_ = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_changed(const QString& str)
{
    if(!str.isEmpty()){
        ShowSearch(true);
    }
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply)
{
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
            << apply->_name << " desc is " << apply->_desc;

    auto b_already = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
    if(b_already){
        return;
    }
    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->AddNewApply(apply);


}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid
        << " name is " << auth_info->_name << " nick is " << auth_info->_nick;
    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(bfriend){
        return;
    }

    auto friendinfo = std::make_shared<FriendInfo>(auth_info);
    UserMgr::GetInstance()->AddFriend(friendinfo);

//    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
//    int str_i = randomValue % strs1.size();
//    int head_i = randomValue % heads1.size();
//    int name_i = randomValue % names1.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);
    _chat_item_added.insert(auth_info->_uid,item);

}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_rsp->_uid
        << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;
    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(bfriend){
        return;
    }

    auto friendinfo = std::make_shared<FriendInfo>(auth_rsp);
    UserMgr::GetInstance()->AddFriend(friendinfo);

//    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
//    int str_i = randomValue % strs1.size();
//    int head_i = randomValue % heads1.size();
//    int name_i = randomValue % names1.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);
    _chat_item_added.insert(auth_rsp->_uid,item);
}

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),mode_(ChatUIMode::ChatMode),state_(ChatUIMode::ChatMode),b_loading_(false),_last_widget(nullptr),_cur_chat_uid(0)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal","hover","press");
    ui->search_edit->SetMaxLength(15);

    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));//设置图片
    ui->search_edit->addAction(searchAction,QLineEdit::LeadingPosition);//设置位置
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));//设置默认文本

    //创建一个清除动作并设置图标
    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    // 初始时不显示清除图标
    // 将清除动作添加到LineEdit的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);
    // 当需要显示清除图标时，更改为实际的清除图标
    connect(ui->search_edit,&QLineEdit::textChanged,[clearAction](const QString& text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        }else{
            clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 文本为空时，切换回透明图标
        }
    });
    // 连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction,&QAction::triggered,[this, clearAction](){
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 清除文本后，切换回透明图标
        ui->search_edit->clearFocus();
        //清除按钮被按下则不显示搜索框
        ShowSearch(false);
    });
    ShowSearch(false);
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);
    connect(ui->con_user_list,&ContactUserList::sig_loading_contact_user,this,&ChatDialog::slot_loading_contact_user);
    addChatUserList();

    QPixmap pixmap(":/res/head_1.jpg");
    ui->side_head_lb->setPixmap(pixmap);
    QPixmap scaledPixmap = pixmap.scaled( ui->side_head_lb->size(), Qt::KeepAspectRatio); // 将图片缩放到label的大小
    ui->side_head_lb->setPixmap(scaledPixmap); // 将缩放后的图片设置到QLabel上
    ui->side_head_lb->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小


    ui->side_chat_lb->ShowRedPoint(false);
    ui->side_contact_lb->ShowRedPoint(false);
    ui->side_chat_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    ui->side_contact_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    //开始默认选中聊天界面
    ui->side_chat_lb->setProperty("state","selected_pressed");
    SetSelectChatItem();
    SetSelectChatPage();
    //往侧边工具列表里添加组件
    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);
    //点击聊天图片时，自动切换到聊天模式
    connect(ui->side_chat_lb,&StateWidget::clicked,this,&ChatDialog::slot_side_chat);
    //点击联系人图片时，自动切换到联系人模式
    connect(ui->side_contact_lb,&StateWidget::clicked,this,&ChatDialog::slot_side_contact);
    //当往搜索框中输入内容时，自动切换为搜索模式
    connect(ui->search_edit,&QLineEdit::textChanged,this,&ChatDialog::slot_text_changed);

    //检测鼠标点击位置判断是否要清空搜索框
    this->installEventFilter(this); // 安装事件过滤器

    ui->search_list->SetSearchEdit(ui->search_edit);

    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_friend_apply,this,&ChatDialog::slot_apply_friend);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_auth_rsp,this,&ChatDialog::slot_auth_rsp);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_add_auth_friend,this,&ChatDialog::slot_add_auth_friend);

    connect(ui->search_list,&SearchList::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item);
    connect(ui->friend_info_page,&FriendInfoPage::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item_from_infopage);

    connect(ui->con_user_list,&ContactUserList::sig_switch_friend_info_page,this,&ChatDialog::slot_switch_friend_info_page);
    connect(ui->con_user_list,&ContactUserList::sig_switch_apply_friend_page,this,&ChatDialog::slot_switch_apply_friend_page);
    //设置中心部件为chatpage
    ui->stackedWidget->setCurrentWidget(ui->chat_page);



    connect(ui->chat_user_list,&ChatUserList::sig_switch_user_chat_page,this,&ChatDialog::slot_item_clicked);
    connect(ui->chat_page, &ChatPage::sig_append_send_chat_msg, this, &ChatDialog::slot_append_send_chat_msg);

    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_text_chat_msg,this,&ChatDialog::slot_text_chat_msg);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::ShowSearch(bool bsearch)
{
    if(bsearch){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        mode_ = ChatUIMode::SearchMode;
    }else if(state_ == ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        mode_ = ChatUIMode::ChatMode;
    }else if(state_ == ChatUIMode::ContactMode){
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        mode_ = ChatUIMode::ContactMode;
    }
}

void ChatDialog::SetSelectChatItem(int uid)
{
    if(ui->chat_user_list->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chat_user_list->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if(!con_item){
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;

        return;
    }

    auto find_iter = _chat_item_added.find(uid);
    if(find_iter == _chat_item_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }

    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid)
{
    if( ui->chat_user_list->count() <= 0){
        return;
    }

    if (uid == 0) {
       auto item = ui->chat_user_list->item(0);
       //转为widget
       QWidget* widget = ui->chat_user_list->itemWidget(item);
       if (!widget) {
           return;
       }

       auto con_item = qobject_cast<ChatUserWid*>(widget);
       if (!con_item) {
           return;
       }

       //设置信息
       auto user_info = con_item->GetUserInfo();
       ui->chat_page->SetUserInfo(user_info);
       return;
    }

    auto find_iter = _chat_item_added.find(uid);
    if(find_iter == _chat_item_added.end()){
        return;
    }

    //转为widget
    QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
    if(!widget){
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if(!con_item){
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);

        return;
    }

}

void ChatDialog::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData> > chatdatas)
{
    for(auto &text : chatdatas){
        if(text->_from_uid != _cur_chat_uid){
            break;
        }
        ui->chat_page->AppendChatMsg(text);
    }
}
