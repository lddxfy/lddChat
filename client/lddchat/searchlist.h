#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QWidget>
#include <QListWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>
#include "global.h"
#include "findsuccessdlg.h"
#include "userdata.h"
#include "loadingdlg.h"
class SearchList : public QListWidget
{
    Q_OBJECT
public:
    explicit SearchList(QWidget *parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);
    void addTipItem();
    void waitPending(bool b_pending);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
public slots:

private:
    QWidget* search_edit_;
    LoadingDlg* loadingDialog_;
    std::shared_ptr<QDialog> find_dlg_;
    bool send_pending_;

private slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
};

#endif // SEARCHLIST_H
