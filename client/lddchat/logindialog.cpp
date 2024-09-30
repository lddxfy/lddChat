#include "logindialog.h"
#include "ui_logindialog.h"
#include "resetdialog.h"
#include "httpmgr.h"
#include "tcpmgr.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->registerbtn,&QPushButton::clicked,[=](){
        emit LoginDialog::switchRegister();
    });

    ui->forgetpwd->SetState("normal","hover","","selected","selected_hover","");
    ui->forgetpwd->setCursor(Qt::PointingHandCursor);

    ui->pwdlineedit->setEchoMode(QLineEdit::Password);

    connect(ui->forgetpwd,&ClickedLabel::clicked,[this](){
        emit switchResetPwd();
    });

    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_login_mod_finish,this,&LoginDialog::slot_login_mod_finish);
    initHttpHandlers();
    ui->error_tip->clear();

    connect(this,&LoginDialog::sig_tcp_connect,TcpMgr::GetInstance().get(),&TcpMgr::slot_tcp_connect);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_connect_finish,this,&LoginDialog::slot_tcp_con_finish);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initHttpHandlers()
{
    _handlers.insert(USER_LOGIN_ID,[this](const QJsonObject& rsp){
        if(rsp["error"] == ERR_USERLOGIN){
            this->showTip(tr("邮箱或密码不正确"),false);
            return;
        }
        this->showTip(tr("登录成功,正在连接到聊天服务器..."),true);
        auto email = rsp["email"].toString();
        auto user = rsp["name"].toString();
        ServerInfo si;
        si.uid = rsp["id"].toInt();
        si.host = rsp["host"].toString();
        si.port = rsp["port"].toString();
        si.token = rsp["token"].toString();

        uid_ = si.uid;
        token_ = si.token;

        qDebug() << "user name is " << user;
        emit sig_tcp_connect(si);
    });
}

bool LoginDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->emaillineEdit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        return false;
    }
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pass = ui->pwdlineedit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        return false;;
    }
    return true;
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->error_tip->setProperty("state","normal");
    }else{
        ui->error_tip->setProperty("state","err");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

void LoginDialog::enableBtn(bool flag)
{
    ui->loginbtn->setEnabled(flag);
}

void LoginDialog::on_loginbtn_clicked()
{
    qDebug() << "loginbtn";

    if(!checkEmailValid()){
        showTip(tr("邮箱格式不正确"),false);
        return;
    }

    if(!checkPwdValid()){
        showTip(tr("密码格式不正确"),false);
        return;
    }
    ui->error_tip->clear();
    auto email = ui->emaillineEdit->text();
    auto pwd = ui->pwdlineedit->text();
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),json_obj,USER_LOGIN_ID,LOGINMOD);

}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ResCodes err)
{
    if(err != SUCEESS){
        showTip(tr("网络请求错误"),false);
        return;
    }
    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        return;
    }

    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    QJsonObject jsonobj = jsonDoc.object();
    //调用对应的逻辑
    auto fun = _handlers[id];
    if(fun == nullptr)  qDebug() << "test";
    fun(jsonDoc.object());
    return;
}

void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{
    if(bsuccess){
        showTip(tr("聊天服务连接成功，正在校验身份..."),true);
        QJsonObject json_obj;
        json_obj["id"] = uid_;
        json_obj["token"] = token_;
        QJsonDocument doc(json_obj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        //发送tcp请求给chat server
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::CHAT_LOGIN_ID, jsonData);
    }
}

void LoginDialog::slot_login_faild(int err)
{
    QString result = QString("登录失败, err is %1")
                                 .arg(err);
    showTip(result,false);
    enableBtn(true);
}
