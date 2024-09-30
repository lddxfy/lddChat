#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H

#include <QWidget>
#include <QPushButton>
#include "global.h"

class ClickedBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ClickedBtn(QWidget *parent = nullptr);
    ~ClickedBtn();
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void SetState(QString normal,QString hover,QString press);
signals:

public slots:

private:
    QString normal_;
    QString press_;
    QString hover_;
};

#endif // CLICKEDBTN_H
