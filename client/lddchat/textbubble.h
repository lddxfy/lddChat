#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H
#include "bubbleframe.h"
#include "global.h"
#include <QTextEdit>

class TextBubble : public BubbleFrame
{
public:
    TextBubble(ChatRole role,const QString& text,QWidget *parent = Q_NULLPTR);
    void setPlainText(const QString &text);
    void initStyleSheet();
    bool eventFilter(QObject *o, QEvent *e);
    void adjustTextHeight();

private:
    QTextEdit * m_TextEdit;
    ChatRole m_role;
};

#endif // TEXTBUBBLE_H
