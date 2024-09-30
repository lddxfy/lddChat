#include "timerbtn.h"
#include <QMouseEvent>
#include <QDebug>

Timerbtn::Timerbtn(QWidget *parent) : QPushButton(parent),counter_(10)
{
       timer_ = new QTimer(this);

       connect(timer_,&QTimer::timeout,[this](){
           counter_--;
           if(counter_ <= 0){
               timer_->stop();
               counter_ = 10;
               this->setText("获取");
               this->setEnabled(true);
           }else{
               this->setText(QString::number(counter_));
           }

       });
}

Timerbtn::~Timerbtn()
{
    timer_->stop();
}

void Timerbtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        qDebug() << "MyButton was released!";
        this->setText(QString::number(counter_));
        timer_->start(1000);
        this->setEnabled(false);
        emit clicked();
    }

    QPushButton::mouseReleaseEvent(e);
}
