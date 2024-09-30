#include "clickedoncelabel.h"

ClickedOnceLabel::ClickedOnceLabel(QWidget* parent) : QLabel(parent)
{
    //设置鼠标悬浮小手
    setCursor(Qt::PointingHandCursor);
}

void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(this->text());
        return;
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(event);
}
