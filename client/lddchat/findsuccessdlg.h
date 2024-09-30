#ifndef FINDSUCCESSDLG_H
#define FINDSUCCESSDLG_H

#include <QDialog>
#include "global.h"
#include "userdata.h"
namespace Ui {
class FindSuccessDlg;
}

class FindSuccessDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDlg(QWidget *parent = nullptr);
    ~FindSuccessDlg();
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);
private slots:


    void on_add_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::FindSuccessDlg *ui;
    std::shared_ptr<SearchInfo> si_;
    QWidget* parent_;
};

#endif // FINDSUCCESSDLG_H
