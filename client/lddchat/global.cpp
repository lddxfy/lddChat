#include "global.h"
/*
  用来刷新Qss
*/
std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};

QString gate_url_prefix = "";
//对密码进行加密处理（和长度进行异或操作）
std::function<QString(QString)> xorString = [](QString str)->QString{
    int length = str.size();
    QString res = str;
    ushort xor_code = length % 255;
    for(int i = 0; i < length;i++){
        res[i] = QChar(static_cast<ushort>(str[i].unicode() ^ xor_code));
    }
    return res;
};
