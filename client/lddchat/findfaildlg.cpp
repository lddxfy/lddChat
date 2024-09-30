#include "findfaildlg.h"
#include "ui_findfaildlg.h"

FindFailDlg::FindFailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("FindFailDlg");
    ui->pushButton->SetState("normal","hover","press");
    this->setModal(true);
    connect(ui->pushButton,&QPushButton::clicked,this,&FindFailDlg::slot_clicked_sure_btn);
}

FindFailDlg::~FindFailDlg()
{
    delete ui;
}

void FindFailDlg::slot_clicked_sure_btn()
{
    this->hide();
    this->deleteLater();
}
