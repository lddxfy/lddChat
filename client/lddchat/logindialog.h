#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QJsonObject>
#include "global.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void initHttpHandlers();

    bool checkEmailValid();
    bool checkPwdValid();
    void showTip(QString str,bool b_ok);
    void enableBtn(bool flag);
signals:
    void switchRegister();
    void switchResetPwd();
    void sig_tcp_connect(ServerInfo);



private slots:
    void on_loginbtn_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ResCodes err);
    void slot_tcp_con_finish(bool bsuccess);
    void slot_login_faild(int err);

private:
    Ui::LoginDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

    int uid_;
    QString token_;
};

#endif // LOGINDIALOG_H
