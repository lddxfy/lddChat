#include "bubbleframe.h"

#include <QPainter>
#include <QDebug>
const int WIDTH_SHANJIAO = 8;//三角宽度

BubbleFrame::BubbleFrame(ChatRole role,QWidget *parent) : QFrame(parent),m_role(role),m_margin(3)
{
    m_Hlayout = new QHBoxLayout();
    if(m_role == Self){
        m_Hlayout->setContentsMargins(m_margin,m_margin,m_margin + WIDTH_SHANJIAO,m_margin);
    }else{
        m_Hlayout->setContentsMargins(m_margin + WIDTH_SHANJIAO,m_margin,m_margin,m_margin);
    }
    this->setLayout(m_Hlayout);
}

void BubbleFrame::setMargin(int margin)
{

}

void BubbleFrame::setWidget(QWidget *w)
{
    if(m_Hlayout->count() > 0){
        return;
    }
    m_Hlayout->addWidget(w);
}

void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if(m_role == Self){
        //设置背景颜色
        QColor bk_color(158,234,106);
        painter.setBrush(QBrush(bk_color));
        //画气泡
        QRect bk_rect = QRect(0,0,this->width()-WIDTH_SHANJIAO,this->height());
        painter.drawRoundedRect(bk_rect,5,5);

        //画三角
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(), 12),
            QPointF(bk_rect.x()+bk_rect.width(), 12+WIDTH_SHANJIAO +2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_SHANJIAO, 10+WIDTH_SHANJIAO-WIDTH_SHANJIAO/2),
        };
        painter.drawPolygon(points, 3);
    }else{
        //设置背景颜色
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        //画气泡
        QRect bk_rect = QRect(WIDTH_SHANJIAO,0,this->width()-WIDTH_SHANJIAO,this->height());
        painter.drawRoundedRect(bk_rect,5,5);

        //画三角
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 12+WIDTH_SHANJIAO +2),
            QPointF(bk_rect.x()-WIDTH_SHANJIAO, 10+WIDTH_SHANJIAO-WIDTH_SHANJIAO/2),
        };
        painter.drawPolygon(points, 3);
    }

    return QFrame::paintEvent(e);
}
