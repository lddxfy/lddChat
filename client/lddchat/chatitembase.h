#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QWidget>
#include <QLabel>
#include "global.h"

class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role,QWidget *parent = nullptr);
    void setUserName(const QString &name);
    void setUserIcon(const QPixmap &icon);
    void setWidget(QWidget *w);
signals:

public slots:

private:
    QLabel *m_pNameLabel;
    QLabel *m_pIconLabel;
    QWidget* m_pBubble;
    ChatRole m_role;
};

#endif // CHATITEMBASE_H
