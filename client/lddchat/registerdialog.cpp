#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "httpmgr.h"
#include "clickedlabel.h"
#include <QLabel>


RegisterDialog::RegisterDialog(QWidget *parent) : counter(5),
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);
    ui->confirepasslineEdit->setEchoMode(QLineEdit::Password);

    ui->error_tip->setProperty("state","normal");
    repolish(ui->error_tip);
    ui->error_tip->clear();


    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,this,&RegisterDialog::slot_req_mod_finish);

    connect(ui->userlineEdit,&QLineEdit::editingFinished,[this](){
        checkUserValid();
    });

    connect(ui->emaillineEdit,&QLineEdit::editingFinished,[this](){
        checkEmailValid();
    });

    connect(ui->passwordlineEdit,&QLineEdit::editingFinished,[this](){
        checkPwdValid();
    });

    connect(ui->confirepasslineEdit,&QLineEdit::editingFinished,[this](){
        checkCpwdValid();
    });

    connect(ui->yanzhenmalineEdit,&QLineEdit::editingFinished,[this](){
        checkVerifycodeValid();
    });



    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                                "visible_hover","");

    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");


    //设置浮动显示手形状
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    connect(ui->pass_visible,&ClickedLabel::clicked,[this](){
        if(ui->pass_visible->GetState() == Normal){
            ui->passwordlineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->passwordlineEdit->setEchoMode(QLineEdit::Normal);
        }
    });

    connect(ui->confirm_visible,&ClickedLabel::clicked,[this](){
        if(ui->confirm_visible->GetState() == Normal){
            ui->confirepasslineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirepasslineEdit->setEchoMode(QLineEdit::Normal);
        }
    });

    conuter_timer = new QTimer(this);
    connect(conuter_timer,&QTimer::timeout,[this](){
        if(counter == 0){
            conuter_timer->stop();
            counter = 5;
            emit sig_switch_log();
            return;
        }
        counter--;
        auto str = QString("注册成功，%1 s后返回登录").arg(counter);
        ui->tip_backlogin->setText(str);
    });

    connect(ui->backbtn,&QPushButton::clicked,[this](){
        conuter_timer->stop();
        emit sig_switch_log();
    });

    initHttpHandlers();



}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_huoqubtn_clicked()
{
    //验证邮箱的地址正则表达式
    auto email = ui->emaillineEdit->text();

    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(match){
        //发送http请求获取验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_verifycode"),json_obj,GET_VARIFT_CODE_ID,REGISTERMOD);
    }else{
        //提示邮箱不正确
        showTip(tr("邮箱地址不正确"),false);
    }

}

void RegisterDialog::slot_req_mod_finish(ReqId id, QString res, ResCodes err)
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

void RegisterDialog::showTip(QString str,bool b_ok){
    if(b_ok){
        ui->error_tip->setProperty("state","normal");
    }else{
        ui->error_tip->setProperty("state","err");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

void RegisterDialog::initHttpHandlers()
{
    _handlers.insert(ReqId::GET_VARIFT_CODE_ID,[this](QJsonObject jsonobj){
        int error = jsonobj["error"].toInt();
        if(error != SUCEESS){
            this->showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonobj["email"].toString();
        this->showTip(tr("验证码已发送到邮箱，注意查收"),true);
        qDebug()<< "email is " << email;
    });

    _handlers.insert(ReqId::USER_REGISTER_ID,[this](QJsonObject jsonobj){
        int error = jsonobj["error"].toInt();
        if(error == ERR_VERIFYCODE){
            this->showTip(tr("验证码错误"),false);
            return;
        }
        if(error == ERR_REGUSER){
            this->showTip(tr("用户名或邮箱已存在"),false);
            return;
        }
        this->showTip(tr("注册成功"),true);
        qDebug()<< "userid is " << jsonobj["uid"].toInt();
        //切换界面
        conuter_timer->stop();
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        // 启动定时器，设置间隔为1000毫秒（1秒）
        conuter_timer->start(1000);
    });
}

bool RegisterDialog::checkUserValid()
{
    if(ui->userlineEdit->text() == ""){
        Addtip(Tip_User,"用户名不能为空");
        return false;
    }
    DelTip(Tip_User);
    return true;

}

bool RegisterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->emaillineEdit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        Addtip(Tip_Email, tr("邮箱格式不正确"));
        return false;
    }
    DelTip(Tip_Email);
    return true;
}

bool RegisterDialog::checkPwdValid()
{
    auto pass = ui->passwordlineEdit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        Addtip(Tip_Pwd, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        Addtip(Tip_Pwd, tr("不能包含非法字符"));
        return false;;
    }
    DelTip(Tip_Pwd);
    return true;
}

bool RegisterDialog::checkCpwdValid()
{
    auto pass = ui->confirepasslineEdit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        Addtip(Tip_ConfirePwd, tr("确认密码与密码不符"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        Addtip(Tip_ConfirePwd, tr("确认密码与密码不符"));
        return false;
    }


    if(ui->passwordlineEdit->text() != ui->confirepasslineEdit->text()){
        Addtip(Tip_ConfirePwd, tr("确认密码与密码不符"));
        return false;
    }
    DelTip(Tip_ConfirePwd);
    return true;
}

bool RegisterDialog::checkVerifycodeValid()
{
    auto pass = ui->yanzhenmalineEdit->text();
    if(pass.isEmpty()){
        Addtip(Tip_VerifyCode, tr("验证码不能为空"));
        return false;
    }
    DelTip(Tip_VerifyCode);
    return true;
}

void RegisterDialog::Addtip(ErrTips er, QString et)
{
    tip_err_[er] = et;
    showTip(et,false);
}

void RegisterDialog::DelTip(ErrTips er)
{
    tip_err_.remove(er);
    if(tip_err_.empty()){
        ui->error_tip->clear();
        return;
    }
    showTip(tip_err_.first(),false);
}

void RegisterDialog::on_okbtn_clicked()
{
    if(!tip_err_.empty()){
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->userlineEdit->text();
    json_obj["email"] = ui->emaillineEdit->text();
    json_obj["passwd"] = xorString(ui->passwordlineEdit->text());
    json_obj["verifycode"] = ui->yanzhenmalineEdit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),json_obj,ReqId::USER_REGISTER_ID,Modules::REGISTERMOD);
}

void RegisterDialog::on_cancelbtn_clicked()
{
    emit sig_switch_log();
}
