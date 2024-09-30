#ifndef APPLYFRIENDLIST_H
#define APPLYFRIENDLIST_H

#include <QWidget>
#include <QListWidget>

class ApplyFriendList : public QListWidget
{
    Q_OBJECT
public:
    explicit ApplyFriendList(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void sig_show_search(bool bsearch);
public slots:
};

#endif // APPLYFRIENDLIST_H
