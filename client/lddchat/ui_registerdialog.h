/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clickedlabel.h"
#include "timerbtn.h"

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QLabel *error_tip;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *userlineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *emaillineEdit;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QLineEdit *passwordlineEdit;
    ClickedLabel *pass_visible;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label;
    QLineEdit *confirepasslineEdit;
    ClickedLabel *confirm_visible;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_5;
    QLineEdit *yanzhenmalineEdit;
    Timerbtn *huoqubtn;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *okbtn;
    QPushButton *cancelbtn;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *tip_backlogin;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *backbtn;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName(QString::fromUtf8("RegisterDialog"));
        RegisterDialog->resize(300, 500);
        horizontalLayout = new QHBoxLayout(RegisterDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        stackedWidget = new QStackedWidget(RegisterDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_2 = new QWidget(page);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        error_tip = new QLabel(widget_2);
        error_tip->setObjectName(QString::fromUtf8("error_tip"));
        error_tip->setScaledContents(false);
        error_tip->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(error_tip);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(page);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(6, 6, 6, 6);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        userlineEdit = new QLineEdit(widget);
        userlineEdit->setObjectName(QString::fromUtf8("userlineEdit"));

        horizontalLayout_3->addWidget(userlineEdit);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(6, 6, 6, 6);
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        emaillineEdit = new QLineEdit(widget);
        emaillineEdit->setObjectName(QString::fromUtf8("emaillineEdit"));

        horizontalLayout_4->addWidget(emaillineEdit);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(6, 6, 6, 6);
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        passwordlineEdit = new QLineEdit(widget);
        passwordlineEdit->setObjectName(QString::fromUtf8("passwordlineEdit"));

        horizontalLayout_7->addWidget(passwordlineEdit);

        pass_visible = new ClickedLabel(widget);
        pass_visible->setObjectName(QString::fromUtf8("pass_visible"));
        pass_visible->setMinimumSize(QSize(30, 30));
        pass_visible->setMaximumSize(QSize(30, 30));

        horizontalLayout_7->addWidget(pass_visible);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_8->addWidget(label);

        confirepasslineEdit = new QLineEdit(widget);
        confirepasslineEdit->setObjectName(QString::fromUtf8("confirepasslineEdit"));

        horizontalLayout_8->addWidget(confirepasslineEdit);

        confirm_visible = new ClickedLabel(widget);
        confirm_visible->setObjectName(QString::fromUtf8("confirm_visible"));
        confirm_visible->setMinimumSize(QSize(30, 30));
        confirm_visible->setMaximumSize(QSize(30, 30));

        horizontalLayout_8->addWidget(confirm_visible);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(6, 6, 6, 6);
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_9->addWidget(label_5);

        yanzhenmalineEdit = new QLineEdit(widget);
        yanzhenmalineEdit->setObjectName(QString::fromUtf8("yanzhenmalineEdit"));

        horizontalLayout_9->addWidget(yanzhenmalineEdit);

        huoqubtn = new Timerbtn(widget);
        huoqubtn->setObjectName(QString::fromUtf8("huoqubtn"));
        huoqubtn->setMinimumSize(QSize(60, 0));
        huoqubtn->setIconSize(QSize(20, 20));

        horizontalLayout_9->addWidget(huoqubtn);


        verticalLayout_3->addLayout(horizontalLayout_9);


        verticalLayout->addWidget(widget);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        widget_3 = new QWidget(page);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        okbtn = new QPushButton(widget_3);
        okbtn->setObjectName(QString::fromUtf8("okbtn"));

        horizontalLayout_2->addWidget(okbtn);

        cancelbtn = new QPushButton(widget_3);
        cancelbtn->setObjectName(QString::fromUtf8("cancelbtn"));

        horizontalLayout_2->addWidget(cancelbtn);


        verticalLayout->addWidget(widget_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_4 = new QVBoxLayout(page_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widget_4 = new QWidget(page_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_5 = new QHBoxLayout(widget_4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        tip_backlogin = new QLabel(widget_4);
        tip_backlogin->setObjectName(QString::fromUtf8("tip_backlogin"));
        tip_backlogin->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(tip_backlogin);


        verticalLayout_4->addWidget(widget_4);

        widget_5 = new QWidget(page_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        horizontalLayout_6 = new QHBoxLayout(widget_5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        backbtn = new QPushButton(widget_5);
        backbtn->setObjectName(QString::fromUtf8("backbtn"));

        horizontalLayout_6->addWidget(backbtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_4->addWidget(widget_5);

        stackedWidget->addWidget(page_2);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(RegisterDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QApplication::translate("RegisterDialog", "Dialog", nullptr));
        error_tip->setText(QApplication::translate("RegisterDialog", "<html><head/><body><p>\351\224\231\350\257\257\346\217\220\347\244\272</p></body></html>", nullptr));
        label_2->setText(QApplication::translate("RegisterDialog", "\347\224\250\346\210\267\357\274\232", nullptr));
        label_4->setText(QApplication::translate("RegisterDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        label_3->setText(QApplication::translate("RegisterDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        pass_visible->setText(QString());
        label->setText(QApplication::translate("RegisterDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        confirm_visible->setText(QString());
        label_5->setText(QApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        huoqubtn->setText(QApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        okbtn->setText(QApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        cancelbtn->setText(QApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
        backbtn->setText(QApplication::translate("RegisterDialog", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
