#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singleton.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QObject>
#include <QNetworkReply>


class HttpMgr : public QObject,public Singleton<HttpMgr>
        ,public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
public:
    ~HttpMgr();

    void PostHttpReq(QUrl url,QJsonObject json,ReqId req_id,Modules mod);
private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;
private slots:
    void slot_http_finish(ReqId id, QString res, ResCodes err, Modules mod);

signals:
    void sig_http_finish(ReqId id, QString res, ResCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ResCodes err);
    void sig_reset_mod_finish(ReqId id, QString res, ResCodes err);
    void sig_login_mod_finish(ReqId id, QString res, ResCodes err);

};

#endif // HTTPMGR_H
