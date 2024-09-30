#include "clickedbtn.h"
#include <QVariant>
#include <QMouseEvent>
ClickedBtn::ClickedBtn(QWidget *parent) : QPushButton(parent)
{
    //设置光标为小手
    setCursor(Qt::PointingHandCursor);
    //防止出现无法回车bug
    setFocusPolicy(Qt::NoFocus);
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::enterEvent(QEvent *event)
{
    setProperty("state",hover_);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    setProperty("state",normal_);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
    setProperty("state",press_);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    setProperty("state",hover_);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    normal_ = normal;
    hover_ = hover;
    press_ = press;
    setProperty("state",normal);
    repolish(this);
    update();
}
