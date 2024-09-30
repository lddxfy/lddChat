#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QWidget>
#include <QLabel>
#include "global.h"

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickedLabel(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void SetState(QString normal,QString normal_hover,QString normal_press,
                  QString selected,QString selected_hover,QString selected_press);
    ClickLbState GetState(){return _curstate;}
    void SetCurState(ClickLbState state);
    void ResetNormalState();
private:

    QString _normal;
    QString _normal_hover;
    QString _normal_press;


    QString _selected;
    QString _selected_press;
    QString _selected_hover;

    ClickLbState _curstate;


signals:
    void clicked(QString,ClickLbState);
public slots:
};

#endif // CLICKEDLABEL_H
