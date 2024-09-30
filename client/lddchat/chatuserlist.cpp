#include "chatuserlist.h"
#include "chatdialog.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>
#include "usermgr.h"
#include <QTimer>
#include <QApplication>
#include "listitembase.h"
#include "chatuserwid.h"
ChatUserList::ChatUserList(QWidget *parent) : QListWidget(parent),_load_pending(false)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);

    connect(this,&ChatUserList::itemClicked,this,&ChatUserList::slot_item_clicked);

}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
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
        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量
        if (maxScrollValue - currentValue <= 0) {
            // 滚动到底部，加载新的联系人
            bool b_loaded = UserMgr::GetInstance()->IsLoadChatFin();
            if(b_loaded)    return true;
            if(_load_pending)   return true;

            qDebug()<<"load more chat user";
            _load_pending = true;
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_chat_user();
         }
        return true; // 停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}

void ChatUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* widget = this->itemWidget(item);
    if(widget == nullptr){
        return;
    }
    auto itembase = qobject_cast<ListItemBase*>(widget);
    if(itembase == nullptr){
        return;
    }
    auto itemtype = itembase->GetItemType();
    if(itemtype == ListItemType::CHAT_USER_ITEM){
        // 创建对话框，提示用户
       qDebug()<< "chat user item clicked ";
       auto user_item = qobject_cast<ChatUserWid*>(itembase);
       auto user_info = user_item->GetUserInfo();
       //跳转到好友信息界面
       emit sig_switch_user_chat_page(user_info);
    }
}
