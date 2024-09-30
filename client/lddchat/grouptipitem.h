#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H

#include <QWidget>
#include <listitembase.h>
namespace Ui {
class GroupTipItem;
}

class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);
    void SetGroupTip(QString);
    QSize sizeHint() const override;
    ~GroupTipItem();

private:
    Ui::GroupTipItem *ui;
};

#endif // GROUPTIPITEM_H
