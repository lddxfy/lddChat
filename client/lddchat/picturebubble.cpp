#include "picturebubble.h"
#include <QLabel>
#define PIC_MAX_WIDTH 160
#define PIC_MAX_HEIGHT 90

PictureBubble::PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent) : BubbleFrame (role,parent)
{
    QLabel *lb = new QLabel();
    //图像将被缩放以填充整个标签，同时保持其宽高比
    lb->setScaledContents(true);
    //创建一个缩放后的 QPixmap 对象，同时保持图像的宽高比
    QPixmap pix = picture.scaled(QSize(PIC_MAX_WIDTH, PIC_MAX_HEIGHT), Qt::KeepAspectRatio);
    lb->setPixmap(pix);
    this->setWidget(lb);
    //获取当前布局的左边距和右边距：
    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int v_margin = this->layout()->contentsMargins().bottom();
    //设置固定大小
    setFixedSize(pix.width()+left_margin + right_margin, pix.height() + v_margin *2);
}
