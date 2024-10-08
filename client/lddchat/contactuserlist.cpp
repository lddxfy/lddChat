#include "contactuserlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QScrollBar>
#include <QRandomGenerator>

//测试数据
std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                            "You have to love yourself",
                            "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg"
};
std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};
ContactUserList::ContactUserList(QWidget* parent) : QListWidget (parent),_load_pending(false)
{
    //Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);


    //模拟从数据库或者后端传输过来的数据,进行列表加载
    addContactUserList();

    connect(this,&QListWidget::itemClicked,this,&ContactUserList::slot_item_clicked);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_auth_rsp,this,&ContactUserList::slot_auth_rsp);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_add_auth_friend,this,&ContactUserList::slot_add_auth_friend);
}

void ContactUserList::ShowRedPoint(bool bshow)
{
    _add_friend_item->ShowRedPoint(bshow);
}

void ContactUserList::addContactUserList()
{
    auto grouptip = new GroupTipItem();
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(grouptip->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,grouptip);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    _add_friend_item = new ConUserItem();
    _add_friend_item->setObjectName("new_friend_item");
    _add_friend_item->SetInfo(0,tr("新的朋友"),":/res/add_friend.png");
    _add_friend_item->SetItemType(ListItemType::APPLY_FRIEND_ITEM);

     QListWidgetItem* add_item = new QListWidgetItem();
     add_item->setSizeHint(_add_friend_item->sizeHint());
     this->addItem(add_item);
     this->setItemWidget(add_item,_add_friend_item);

     //默认设置新的朋友申请条目被选中
     this->setCurrentItem(add_item);

     auto groupcon = new GroupTipItem();
     groupcon->SetGroupTip(tr("联系人"));
     _groupitem = new QListWidgetItem();
     _groupitem->setSizeHint(groupcon->sizeHint());
     this->addItem(_groupitem);
     this->setItemWidget(_groupitem,groupcon);
     _groupitem->setFlags(_groupitem->flags() & ~Qt::ItemIsSelectable);

     //加载后端发送过来的好友列表
     auto con_list = UserMgr::GetInstance()->GetConListPerPage();
     for(auto & con_ele : con_list){
         auto *con_user_wid = new ConUserItem();
         con_user_wid->SetInfo(con_ele->_uid,con_ele->_name, con_ele->_icon);
         QListWidgetItem *item = new QListWidgetItem;
         //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
         item->setSizeHint(con_user_wid->sizeHint());
         this->addItem(item);
         this->setItemWidget(item, con_user_wid);
     }

     for(int i = 0; i < 13;i++){
         int randomValue = QRandomGenerator::global()->bounded(100);
         int str_i = randomValue%strs.size();
         int head_i = randomValue%heads.size();
         int name_i = randomValue%names.size();

         auto con_user = new ConUserItem();
         con_user->SetItemType(ListItemType::CONTACT_USER_ITEM);
         con_user->SetInfo(0,names[name_i],heads[head_i]);
         QListWidgetItem* useritem = new QListWidgetItem();
         useritem->setSizeHint(con_user->sizeHint());
         this->addItem(useritem);
         this->setItemWidget(useritem,con_user);
     }

}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this->viewport()){
        if(event->type() == QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else if(event->type() == QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

    }
    // 检查事件是否是鼠标滚轮事件
    if(watched == this->viewport() && event->type() == QEvent::Wheel){
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数
        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);
        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量
        if (maxScrollValue - currentValue <= 0) {
            bool b_loaded = UserMgr::GetInstance()->IsLoadContactUserFin();
            if(b_loaded)    return true;
            if(_load_pending)   return true;

            qDebug()<<"load more chat user";
            _load_pending = true;
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_contact_user();
         }
        return true; // 停止事件传递
    }
    return QListWidget::eventFilter(watched, event);

}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* widget = this->itemWidget(item);
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }
    ListItemBase* customitem = qobject_cast<ListItemBase*>(widget);
    if(!customitem){
        qDebug()<< "slot item clicked widget is not customitem";
        return;
    }

    auto itemtype = customitem->GetItemType();
    if(itemtype == ListItemType::INVALID_ITEM || itemtype == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }
    if(itemtype == ListItemType::APPLY_FRIEND_ITEM){
       // 创建对话框，提示用户
       qDebug()<< "apply friend item clicked ";
       //跳转到好友申请界面
       emit sig_switch_apply_friend_page();
       return;
    }

    if(itemtype == ListItemType::CONTACT_USER_ITEM){
        // 创建对话框，提示用户
       qDebug()<< "contact user item clicked ";
       auto con_item = qobject_cast<ConUserItem*>(customitem);
       auto user_info = con_item->GetInfo();
       //跳转到好友信息界面
       emit sig_switch_friend_info_page(user_info);
       return;
    }

}

void ContactUserList::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "slot auth rsp called";
    bool isfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(isfriend){
        return;
    }

    auto* con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_rsp);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(con_user_wid->sizeHint());
    // 获取 groupitem 的索引
    int index = this->row(_groupitem);

    this->insertItem(index+1,item);
    this->setItemWidget(item,con_user_wid);
}

void ContactUserList::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "slot auth rsp called";
    bool isfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(isfriend){
        return;
    }

    auto* con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_info);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(con_user_wid->sizeHint());
    // 获取 groupitem 的索引
    int index = this->row(_groupitem);

    this->insertItem(index+1,item);
    this->setItemWidget(item,con_user_wid);
}


