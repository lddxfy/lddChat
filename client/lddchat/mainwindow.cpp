#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmgr.h"

/******************************************************************************
 *
 * @file       mainwindow.cpp
 * @brief      XXXX Function
 *
 * @author     懒惰的小肥羊
 * @date       2024/09/02
 * @history
 *****************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/res/talk.png"));

    _login_dlg = new LoginDialog(this);
    //隐藏边框和关闭缩放按钮，使其完全嵌入主界面
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);

    connect(this->_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::slot_switch_register);
    connect(this->_login_dlg,&LoginDialog::switchResetPwd,this,&MainWindow::slot_switch_reset);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_switch_chatdlg,this,&MainWindow::slot_switch_chatdlg);

    //emit TcpMgr::GetInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_switch_login()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    _login_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::slot_switch_register);
    //连接登录界面忘记密码信号
    connect(_login_dlg, &LoginDialog::switchResetPwd, this, &MainWindow::slot_switch_reset);


}

void MainWindow::slot_switch_register()
{
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reg_dlg);
     //连接注册界面返回登录信号
    connect(_reg_dlg, &RegisterDialog::sig_switch_log, this, &MainWindow::slot_switch_login);
    _reg_dlg->show();
}

void MainWindow::slot_switch_reset()
{
    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::slot_switch_login);
    _login_dlg->hide();
    _reset_dlg->show();
}

void MainWindow::slot_switch_chatdlg()
{
    _chat_dlg = new ChatDialog(this);
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_chat_dlg);
    _chat_dlg->show();
    _login_dlg->hide();
    this->setMinimumSize(QSize(1050,900));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}


