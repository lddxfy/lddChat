#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);

    void SetItemType(ListItemType itemType);
    ListItemType GetItemType();
    void paintEvent(QPaintEvent *event) override;
signals:

public slots:
private:
     ListItemType itemType_;
};

#endif // LISTITEMBASE_H
