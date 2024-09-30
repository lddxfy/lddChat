#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H
#include "bubbleframe.h"


class PictureBubble : public BubbleFrame
{
public:
    PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent = Q_NULLPTR);

private:
    ChatRole m_role;
};

#endif // PICTUREBUBBLE_H
