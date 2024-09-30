#include "searchlist.h"
#include "tcpmgr.h"
#include "adduseritem.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "customizeedit.h"
#include "findfaildlg.h"
#include "usermgr.h"
SearchList::SearchList(QWidget *parent) : QListWidget(parent),find_dlg_(nullptr),search_edit_(nullptr),send_pending_(false)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    //连接点击的信号和槽
    connect(this,&QListWidget::itemClicked,this,&SearchList::slot_item_clicked);

    //添加条目
    addTipItem();
    //连接搜索条目
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_user_search,this,&SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    find_dlg_->hide();
    find_dlg_->deleteLater();
}

void SearchList::SetSearchEdit(QWidget *edit)
{
    search_edit_ = edit;
}

void SearchList::addTipItem()
{
    auto *invalid_item = new QWidget();
    QListWidgetItem *item_tmp = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    //将 invalid_item 设置为 item_tmp 的子控件
    this->setItemWidget(item_tmp, invalid_item);
    //使其不可被选中
    item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);

    auto *add_user_item = new AddUserItem();
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,add_user_item);
}

void SearchList::waitPending(bool b_pending)
{
    if(!b_pending){
        loadingDialog_->hide();
        loadingDialog_->deleteLater();
        send_pending_ = b_pending;
    }else{
        loadingDialog_ = new LoadingDlg(this);
        loadingDialog_->setModal(true);
        loadingDialog_->show();
        send_pending_ = b_pending;
    }

}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数
        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);
        return true; // 停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* widget = this->itemWidget(item);
    if(!widget){
        qDebug() << "slot item clicked widget is nullptr";
        return;
    }
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::ADD_USER_TIP_ITEM){
//        find_dlg_ = std::make_shared<FindSuccessDlg>(this);
//        auto si = std::make_shared<SearchInfo>(0,"lddxfy","lddxfy","hello,my friend!",0);
//        std::dynamic_pointer_cast<FindSuccessDlg>(find_dlg_)->SetSearchInfo(si);
//        find_dlg_->show();
        if(send_pending_){
            return;
        }

        if(!search_edit_){
            return;
        }
        waitPending(true);

        auto edit = dynamic_cast<CustomizeEdit*>(search_edit_);
        auto uid_str = edit->text();
        QJsonObject jsonobj;
        jsonobj["id"] = uid_str;

        QJsonDocument doc(jsonobj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ,jsonData);
        return;
    }

    //清除弹出框
    CloseFindDlg();

}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot_user_search";
    waitPending(false);

    if(si == nullptr){
        find_dlg_ = std::make_shared<FindFailDlg>(this);
    }else{
        //如果是自己，先直接返回
        auto self_id = UserMgr::GetInstance()->GetUid();
        if(si->_uid == self_id){
            return;
        }

        auto isfriend = UserMgr::GetInstance()->CheckFriendById(si->_uid);
        if(isfriend){
            emit sig_jump_chat_item(si);
            return;
        }
        //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
        //查找是否已经是好友 todo...
        find_dlg_ = std::make_shared<FindSuccessDlg>(this);
        std::dynamic_pointer_cast<FindSuccessDlg>(find_dlg_)->SetSearchInfo(si);
    }
    find_dlg_->show();
}


