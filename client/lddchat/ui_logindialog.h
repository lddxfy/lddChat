/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <clickedlabel.h>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QWidget *widget_2;
    QLabel *label_2;
    QWidget *widget_3;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *loginbtn;
    QPushButton *registerbtn;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *emaillineEdit;
    QLabel *label;
    QLabel *label_3;
    QLineEdit *pwdlineedit;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    ClickedLabel *forgetpwd;
    QLabel *error_tip;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QString::fromUtf8("LoginDialog"));
        LoginDialog->resize(300, 500);
        widget_2 = new QWidget(LoginDialog);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(9, 29, 282, 211));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 10, 241, 191));
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/res/YANG.png")));
        label_2->setScaledContents(true);
        widget_3 = new QWidget(LoginDialog);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(10, 410, 281, 71));
        gridLayout_2 = new QGridLayout(widget_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        loginbtn = new QPushButton(widget_3);
        loginbtn->setObjectName(QString::fromUtf8("loginbtn"));

        gridLayout_2->addWidget(loginbtn, 0, 1, 1, 1);

        registerbtn = new QPushButton(widget_3);
        registerbtn->setObjectName(QString::fromUtf8("registerbtn"));

        gridLayout_2->addWidget(registerbtn, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        widget = new QWidget(LoginDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 250, 282, 156));
        gridLayoutWidget = new QWidget(widget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 10, 281, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        emaillineEdit = new QLineEdit(gridLayoutWidget);
        emaillineEdit->setObjectName(QString::fromUtf8("emaillineEdit"));

        gridLayout->addWidget(emaillineEdit, 0, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        pwdlineedit = new QLineEdit(gridLayoutWidget);
        pwdlineedit->setObjectName(QString::fromUtf8("pwdlineedit"));

        gridLayout->addWidget(pwdlineedit, 1, 1, 1, 1);

        horizontalLayoutWidget = new QWidget(widget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 90, 281, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        forgetpwd = new ClickedLabel(horizontalLayoutWidget);
        forgetpwd->setObjectName(QString::fromUtf8("forgetpwd"));

        horizontalLayout->addWidget(forgetpwd);

        error_tip = new QLabel(LoginDialog);
        error_tip->setObjectName(QString::fromUtf8("error_tip"));
        error_tip->setGeometry(QRect(13, 10, 271, 20));
        error_tip->setAlignment(Qt::AlignCenter);

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Dialog", nullptr));
        label_2->setText(QString());
        loginbtn->setText(QApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        registerbtn->setText(QApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        label->setText(QApplication::translate("LoginDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        label_3->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        forgetpwd->setText(QApplication::translate("LoginDialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        error_tip->setText(QApplication::translate("LoginDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
