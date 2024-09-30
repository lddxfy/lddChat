#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include "global.h"
#include <QTimer>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    void showTip(QString str,bool b_ok);
    void initHttpHandlers();

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPwdValid();
    bool checkCpwdValid();
    bool checkVerifycodeValid();
    void Addtip(ErrTips er,QString et);
    void DelTip(ErrTips er);


private slots:
    void on_huoqubtn_clicked();

    void slot_req_mod_finish(ReqId id, QString res, ResCodes err);
    void on_okbtn_clicked();

    void on_cancelbtn_clicked();

signals:
    void sig_switch_log();

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QTimer *conuter_timer;
    int counter;
    QMap<ErrTips,QString> tip_err_;
};

#endif // REGISTERDIALOG_H
