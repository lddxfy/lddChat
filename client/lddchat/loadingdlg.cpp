#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    //设置无边框，无工具栏
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明

    setFixedSize(parent->size());

    QMovie * movie = new QMovie(":/res/loading.gif");
    ui->loading_lb->setMovie(movie);
    movie->start();
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
