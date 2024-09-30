#include "findsuccessdlg.h"
#include "ui_findsuccessdlg.h"
#include <QDir>
#include "applyfriend.h"
#include <QDebug>

FindSuccessDlg::FindSuccessDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindSuccessDlg),parent_(parent)
{
    ui->setupUi(this);
    //设置对话框标题
    setWindowTitle("添加");
    //隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path +
                             QDir::separator() + "static"+QDir::separator()+"head_1.jpg");
    //添加头像图片
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->head_lb->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal","hover","press");
    ui->cancel_friend_btn->SetState("normal","hover","press");
    this->setModal(true);

    connect(ui->add_friend_btn,&QPushButton::clicked,this,&FindSuccessDlg::on_add_btn_clicked);
    connect(ui->cancel_friend_btn,&QPushButton::clicked,this,&FindSuccessDlg::on_cancel_btn_clicked);
}

FindSuccessDlg::~FindSuccessDlg()
{
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lb->setText(si->_name);
    si_ = si;
}

void FindSuccessDlg::on_add_btn_clicked()
{
    qDebug() << "on_add_btn_clicked";
    this->hide();
    auto applyfriend = new ApplyFriend(parent_);
    applyfriend->SetSearchInfo(si_);
    applyfriend->setModal(true);
    applyfriend->show();
}

void FindSuccessDlg::on_cancel_btn_clicked()
{
    this->hide();
}


