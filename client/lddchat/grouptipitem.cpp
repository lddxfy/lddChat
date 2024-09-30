#include "grouptipitem.h"
#include "ui_grouptipitem.h"

GroupTipItem::GroupTipItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::GroupTipItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::GROUP_TIP_ITEM);
}

void GroupTipItem::SetGroupTip(QString text)
{
    ui->label->setText(text);
    ui->label->adjustSize();
}

QSize GroupTipItem::sizeHint() const
{
    return QSize(250,25);
}

GroupTipItem::~GroupTipItem()
{
    delete ui;
}
