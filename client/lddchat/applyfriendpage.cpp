#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"
#include "QRandomGenerator"
#include "authenfriend.h"

std::vector<QString>  strs2 ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                            "You have to love yourself",
                            "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads2 = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg"
};
std::vector<QString> names2 = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};
ApplyFriendPage::ApplyFriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    connect(ui->apply_friend_list,&ApplyFriendList::sig_show_search,this,&ApplyFriendPage::sig_show_search);
    loadApplyList();
    //接受tcp传递的authrsp信号处理
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ApplyFriendPage::slot_auth_rsp);
    //更改添加按钮，防止一直发送请求
    //connect(this->authfriend_,&AuthenFriend::sig_friend_added,this,&ApplyFriendPage::slot_friend_added);
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::AddNewApply(std::shared_ptr<AddFriendApply> apply)
{
    //先模拟头像随机，以后头像资源增加资源服务器后再显示
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int head_i = randomValue % heads2.size();
    auto* apply_item = new ApplyFriendItem();
    auto apply_info = std::make_shared<ApplyInfo>(apply->_from_uid,
             apply->_name, apply->_desc,heads2[head_i], apply->_name, 0, 0);
    apply_item->SetInfo( apply_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(apply_item->sizeHint());
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
    ui->apply_friend_list->insertItem(0,item);
    ui->apply_friend_list->setItemWidget(item, apply_item);
    apply_item->ShowAddBtn(true);
    //收到审核好友信号
    connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info) {
        authfriend_ = new AuthenFriend(this);
        authfriend_->setModal(true);
        authfriend_->SetApplyInfo(apply_info);
        authfriend_->show();
        });
}

void ApplyFriendPage::loadApplyList()
{
    //添加好友申请
    auto apply_list = UserMgr::GetInstance()->GetApplyList();
    for(auto &apply: apply_list){
        qDebug() << "friend apply name is: " << apply->_name;
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int head_i = randomValue % heads2.size();
        auto* apply_item = new ApplyFriendItem();
        apply->SetIcon(heads2[head_i]);
        apply_item->SetInfo(apply);
        QListWidgetItem* item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->apply_friend_list->insertItem(0,item);
        ui->apply_friend_list->setItemWidget(item, apply_item);

        if(apply->_status){
            apply_item->ShowAddBtn(false);
        }else{
             apply_item->ShowAddBtn(true);
             auto uid = apply_item->GetUid();
             _unauth_items[uid] = apply_item;
        }
        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info) {
            authfriend_ = new AuthenFriend(this);
            authfriend_->setModal(true);
            authfriend_->SetApplyInfo(apply_info);
            authfriend_->show();
            });
    }
    // 模拟假数据，创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs2.size();
        int head_i = randomValue%heads2.size();
        int name_i = randomValue%names2.size();
        auto *apply_item = new ApplyFriendItem();
        auto apply = std::make_shared<ApplyInfo>(0, names2[name_i], strs2[str_i],
                                    heads2[head_i], names2[name_i], 0, 1);
        apply_item->SetInfo(apply);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->apply_friend_list->addItem(item);
        ui->apply_friend_list->setItemWidget(item, apply_item);
        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info){
            authfriend_ = new AuthenFriend(this);
            authfriend_->setModal(true);
            authfriend_->SetApplyInfo(apply_info);
            authfriend_->show();
        });
    }


}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    auto uid = auth_rsp->_uid;
    auto find_item = _unauth_items.find(uid);
    if(find_item == _unauth_items.end()){
        return;
    }
    find_item->second->ShowAddBtn(false);
}
