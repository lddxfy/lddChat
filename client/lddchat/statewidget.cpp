#include "statewidget.h"
#include <QVariant>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
StateWidget::StateWidget(QWidget *parent) : QWidget(parent),curstate_(Normal)
{
    setCursor(Qt::PointingHandCursor); // 设置鼠标光标变为手形
    AddRedPoint();
}

void StateWidget::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    normal_ = normal;
    normal_hover_ = hover;
    normal_press_ = press;

    selected_ = select;
    selected_hover_ = select_hover;
    selected_press_ = selected_press_;

    setProperty("state",normal);
    repolish(this);
}

void StateWidget::AddRedPoint()
{
    //添加红点示意图
    red_point_ = new QLabel();
    red_point_->setObjectName("red_point");
    QVBoxLayout* layout2 = new QVBoxLayout;
    red_point_->setAlignment(Qt::AlignCenter);
    layout2->addWidget(red_point_);
    layout2->setMargin(0);
    this->setLayout(layout2);
    red_point_->setVisible(true);
}

void StateWidget::ShowRedPoint(bool flag)
{
    red_point_->setVisible(flag);
}

void StateWidget::ClearState()
{
    curstate_ = ClickLbState::Normal;
    this->setProperty("state",normal_);
    repolish(this);
    update();
}

void StateWidget::SetSelected(bool flag)
{
    if(flag){
        curstate_ = ClickLbState::Selected;
        this->setProperty("state",selected_);
        repolish(this);
        update();
    }else{
        curstate_ = ClickLbState::Normal;
        this->setProperty("state",normal_);
        repolish(this);
        update();
    }
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    return;
}

void StateWidget::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        if(curstate_ == ClickLbState::Selected){
            qDebug()<<"PressEvent , already to selected press: "<< selected_press_;
            QWidget::mousePressEvent(ev);
            return;
        }

        if(curstate_ == ClickLbState::Normal){
            qDebug()<<"PressEvent , change to selected press: "<< selected_press_;
            curstate_ = ClickLbState::Selected;
            setProperty("state",selected_press_);
            repolish(this);
            update();
        }

        return;
    }
    QWidget::mousePressEvent(ev);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        if(curstate_ == ClickLbState::Selected){
            setProperty("state",selected_hover_);
            repolish(this);
            update();
        }else{
            setProperty("state",normal_hover_);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }
    QWidget::mouseReleaseEvent(ev);
}

void StateWidget::enterEvent(QEvent *event)
{
    if(curstate_ == ClickLbState::Normal){
        setProperty("state",normal_hover_);
        repolish(this);
        update();
    }else{
        setProperty("state",selected_hover_);
        repolish(this);
        update();
    }
    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    if(curstate_ == ClickLbState::Normal){
        setProperty("state",normal_);
        repolish(this);
        update();
    }else{
        setProperty("state",selected_);
        repolish(this);
        update();
    }
    QWidget::leaveEvent(event);
}



