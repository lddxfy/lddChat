/********************************************************************************
** Form generated from reading UI file 'findsuccessdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDSUCCESSDLG_H
#define UI_FINDSUCCESSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedbtn.h>

QT_BEGIN_NAMESPACE

class Ui_FindSuccessDlg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *head_lb;
    QLabel *name_lb;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    ClickedBtn *add_friend_btn;
    ClickedBtn *cancel_friend_btn;

    void setupUi(QDialog *FindSuccessDlg)
    {
        if (FindSuccessDlg->objectName().isEmpty())
            FindSuccessDlg->setObjectName(QString::fromUtf8("FindSuccessDlg"));
        FindSuccessDlg->resize(280, 190);
        FindSuccessDlg->setMinimumSize(QSize(280, 190));
        FindSuccessDlg->setMaximumSize(QSize(280, 190));
        verticalLayout = new QVBoxLayout(FindSuccessDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_2 = new QWidget(FindSuccessDlg);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        head_lb = new QLabel(widget_2);
        head_lb->setObjectName(QString::fromUtf8("head_lb"));
        head_lb->setMinimumSize(QSize(50, 50));
        head_lb->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(head_lb);

        name_lb = new QLabel(widget_2);
        name_lb->setObjectName(QString::fromUtf8("name_lb"));

        horizontalLayout->addWidget(name_lb);

        horizontalSpacer = new QSpacerItem(169, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(FindSuccessDlg);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        add_friend_btn = new ClickedBtn(widget);
        add_friend_btn->setObjectName(QString::fromUtf8("add_friend_btn"));
        add_friend_btn->setMinimumSize(QSize(100, 35));
        add_friend_btn->setMaximumSize(QSize(100, 35));

        horizontalLayout_2->addWidget(add_friend_btn);

        cancel_friend_btn = new ClickedBtn(widget);
        cancel_friend_btn->setObjectName(QString::fromUtf8("cancel_friend_btn"));
        cancel_friend_btn->setMinimumSize(QSize(100, 35));
        cancel_friend_btn->setMaximumSize(QSize(100, 35));

        horizontalLayout_2->addWidget(cancel_friend_btn);


        verticalLayout->addWidget(widget);


        retranslateUi(FindSuccessDlg);

        QMetaObject::connectSlotsByName(FindSuccessDlg);
    } // setupUi

    void retranslateUi(QDialog *FindSuccessDlg)
    {
        FindSuccessDlg->setWindowTitle(QApplication::translate("FindSuccessDlg", "Dialog", nullptr));
        head_lb->setText(QString());
        name_lb->setText(QString());
        add_friend_btn->setText(QApplication::translate("FindSuccessDlg", "\346\267\273\345\212\240\345\210\260\351\200\232\350\256\257\345\275\225", nullptr));
        cancel_friend_btn->setText(QApplication::translate("FindSuccessDlg", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindSuccessDlg: public Ui_FindSuccessDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDSUCCESSDLG_H
