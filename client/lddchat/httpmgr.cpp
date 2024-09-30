#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    qDebug() << 2;
    //创建一个HTTP POST请求，并设置请求头和请求体
    QByteArray data = QJsonDocument(json).toJson();
    //通过url构造请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));
    //发送请求，并处理响应, 获取自己的智能指针，构造伪闭包并增加智能指针引用计数
    auto self = shared_from_this();
    QNetworkReply *reply = _manager.post(request,data);
    //设置信号和槽等待发送完成
    QObject::connect(reply,&QNetworkReply::finished,[reply,self,req_id,mod](){
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            //发送信号通知完成
            emit self->sig_http_finish(req_id,"",ERR_NETWORK,mod);
            reply->deleteLater();
            return;
        }
        //无错误则读回请求
        QString res = reply->readAll();
        //发送信号通知完成
        qDebug() << 3;
        emit self->sig_http_finish(req_id,res,SUCEESS,mod);
        reply->deleteLater();
        return;
    });

}

HttpMgr::HttpMgr()
{
    connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ResCodes err, Modules mod)
{
    qDebug() <<"mod is:" <<mod;
    if(mod == REGISTERMOD){
        emit sig_reg_mod_finish(id,res,err);
    }

    if(mod == RESETMOD){
        emit sig_reset_mod_finish(id,res,err);
    }

    if(mod == LOGINMOD){
        emit sig_login_mod_finish(id,res,err);
    }
}
