#include "clickedlabel.h"
#include <QVariant>
#include <QMouseEvent>
#include <QDebug>

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent),_curstate(Normal)
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        if(this->_curstate == Normal){
             _curstate = ClickLbState::Selected;
            setProperty("state",this->_selected_press);
            repolish(this);
            update();
        }else{
            _curstate = ClickLbState::Normal;
            setProperty("state",this->_normal_press);
            repolish(this);
            update();
        }
        //emit clicked(this->text(),_curstate);
        //return;
    }
    QLabel::mousePressEvent(ev);


}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(this->_curstate == Normal){
            setProperty("state",this->_normal_hover);
            repolish(this);
            update();
        }else{
            setProperty("state",this->_selected_hover);
            repolish(this);
            update();
        }
        emit clicked(this->text(),_curstate);
        //return;
    }
    QLabel::mouseReleaseEvent(event);
}

void ClickedLabel::enterEvent(QEvent *event)
{
    if(this->_curstate == Normal){
        qDebug() << "normal enter";
        setProperty("state",this->_normal_hover);
        repolish(this);
        update();
    }else{
        qDebug() << "selected enter";
        setProperty("state",this->_selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    if(this->_curstate == Normal){
        setProperty("state",this->_normal);
        repolish(this);
        update();
    }else{
        setProperty("state",this->_selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal,QString normal_hover,QString normal_press,
                            QString selected,QString selected_hover,QString selected_press)
{
    _normal = normal;
    _normal_hover = normal_hover;
    _normal_press = normal_press;


    _selected = selected;
    _selected_hover = selected_hover;
    _selected_press = selected_press;

    setProperty("state",normal);
    repolish(this);
}

void ClickedLabel::SetCurState(ClickLbState state)
{
    if(state == Normal){
        _curstate = Normal;
        setProperty("state",_normal);
        repolish(this);
        update();
    }
    if(state == Selected){
        _curstate = Selected;
        setProperty("state",_selected);
        repolish(this);
        update();
    }
}

void ClickedLabel::ResetNormalState()
{
    _curstate = Normal;
    setProperty("state",_normal);
    repolish(this);
    update();
}


