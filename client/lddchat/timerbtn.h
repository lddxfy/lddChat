#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>

class Timerbtn : public QPushButton
{
    Q_OBJECT
public:
    explicit Timerbtn(QWidget *parent = nullptr);
    ~Timerbtn();

    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    //void clicked();

private:
    QTimer *timer_;
    int counter_;
};

#endif // TIMERBTN_H
