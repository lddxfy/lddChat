#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include "global.h"

class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    explicit BubbleFrame(ChatRole role,QWidget *parent = nullptr);
    void setMargin(int margin);

    void setWidget(QWidget *w);

protected:
    void paintEvent(QPaintEvent* e);
signals:

public slots:

private:
    ChatRole m_role;
    QHBoxLayout *m_Hlayout;
    int m_margin;
};

#endif // BUBBLEFRAME_H
