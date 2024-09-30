#ifndef CLICKEDONCELABEL_H
#define CLICKEDONCELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>
#include "global.h"
class ClickedOnceLabel : public QLabel
{
    Q_OBJECT;
public:
    ClickedOnceLabel(QWidget* parent = Q_NULLPTR);

    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void clicked(QString);


};

#endif // CLICKEDONCELABEL_H
