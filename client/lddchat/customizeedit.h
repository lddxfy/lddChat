#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H

#include <QWidget>
#include <QLineEdit>

class CustomizeEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomizeEdit(QWidget *parent = nullptr);
    void SetMaxLength(int maxLen);
signals:
    void sig_foucus_out();
public slots:

protected:
    void focusOutEvent(QFocusEvent *event) override{
        // 执行失去焦点时的处理逻辑
        //qDebug() << "CustomizeEdit focusout";
        // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
        QLineEdit::focusOutEvent(event);
        //发送失去焦点得信号
        emit sig_foucus_out();
    }
private:
    void limitTextLength(QString text);

    int max_len_;
};

#endif // CUSTOMIZEEDIT_H
