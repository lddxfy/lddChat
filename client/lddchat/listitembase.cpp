#include "listitembase.h"
#include <QStyleOption>
#include <QPainter>

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent)
{

}

void ListItemBase::SetItemType(ListItemType itemType)
{
    itemType_ = itemType;
}

ListItemType ListItemBase::GetItemType()
{
    return itemType_;
}

void ListItemBase::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
