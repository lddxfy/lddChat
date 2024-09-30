#ifndef FRIENDLABEL_H
#define FRIENDLABEL_H

#include <QWidget>

namespace Ui {
class FriendLabel;
}

class FriendLabel : public QWidget
{
    Q_OBJECT

public:
    explicit FriendLabel(QWidget *parent = nullptr);
    ~FriendLabel();
    void SetText(QString);

    QString Text();

public slots:
    void slot_close();

signals:
    void sig_close(QString);

private:
    Ui::FriendLabel *ui;
    QString _text;
    int _width;
    int _height;
};


#endif // FRIENDLABEL_H
