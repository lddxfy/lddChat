#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>
class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);

    void SetState(QString normal="", QString hover="", QString press="",
                      QString select="", QString select_hover="", QString select_press="");

    void AddRedPoint();
    void ShowRedPoint(bool flag);
    void ClearState();
    ClickLbState GetCurState();
    void SetSelected(bool flag);

protected:
    void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

signals:
    void clicked();
public slots:

private:
    QString normal_;
    QString normal_hover_;
    QString normal_press_;

    QString selected_;
    QString selected_hover_;
    QString selected_press_;

    ClickLbState curstate_;
    QLabel *red_point_;
};

#endif // STATEWIDGET_H
