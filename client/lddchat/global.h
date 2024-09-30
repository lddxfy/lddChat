#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include "QStyle"
#include <QString>
#include <memory>
#include <iostream>
#include <mutex>
#include <QMap>
#include <QByteArray>
//其他文件只要引入了这个头文件就能使用这些全局变量
extern QString gate_url_prefix;
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;

enum ReqId{
    GET_VARIFT_CODE_ID = 1001, //获取验证码
    USER_REGISTER_ID = 1002,   //用户注册
    ID_RESET_PWD = 1003,       //重置密码
    USER_LOGIN_ID = 1004,       //用户登录
    CHAT_LOGIN_ID = 1005,       //登录到聊天服务器
    CHAT_LOGIN_RSP_ID = 1006,   //聊天登录回包
    ID_SEARCH_USER_REQ = 1007,  //搜索用户
    ID_SEARCH_USER_RSP = 1008,  //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,  //添加好友申请
    ID_ADD_FRIEND_RSP = 1010, //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,  //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,  //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,  //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
};

enum ResCodes{
    SUCEESS = 0,
    ERR_JSON = 1,
    ERR_NETWORK = 2,
    ERR_VERIFYCODE = 3,
    ERR_REGUSER = 4,
    ERR_USERLOGIN = 5,

};

enum ErrTips{
    Tip_Success = 0,
    Tip_User,
    Tip_Pwd,
    Tip_Email,
    Tip_VerifyCode,
    Tip_ConfirePwd,
};

enum ClickLbState{
    Normal = 0,
    Selected = 1
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

struct ServerInfo{
    int uid;
    QString host;
    QString port;
    QString token;
};

enum ChatUIMode{
    SearchMode = 0,
    ChatMode = 1,
    ContactMode = 2,
};

enum ListItemType{
    CHAT_USER_ITEM, //聊天用户
    CONTACT_USER_ITEM, //联系人用户
    SEARCH_USER_ITEM, //搜索到的用户
    ADD_USER_TIP_ITEM, //提示添加用户
    INVALID_ITEM,  //不可点击条目
    GROUP_TIP_ITEM, //分组提示条目
    LINE_ITEM,  //分割线
    APPLY_FRIEND_ITEM, //好友申请
};

enum ChatRole{
    Self,
    Other,

};

struct MsgInfo{
    QString msgFlag;//"text,image,file"
    QString content;//表示文件和图像的url,文本信息
    QPixmap pixmap;//文件和图片的缩略图
};

const int tip_offset = 5;

const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "添加标签 ";




#endif // GLOBAL_H
