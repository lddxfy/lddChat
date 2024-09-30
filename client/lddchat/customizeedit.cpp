#include "customizeedit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent) : QLineEdit(parent),max_len_(0)
{
    connect(this,&QLineEdit::textChanged,this,&CustomizeEdit::limitTextLength);
}

void CustomizeEdit::SetMaxLength(int maxLen)
{
    max_len_ = maxLen;
}

void CustomizeEdit::limitTextLength(QString text)
{
    if(max_len_ <= 0){
        return;
    }
    QByteArray textarr = text.toUtf8();
    if(textarr.size() > max_len_){
        textarr = textarr.left(max_len_);
        this->setText(QString::fromUtf8(textarr));
    }
}
