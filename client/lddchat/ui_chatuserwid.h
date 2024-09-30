/********************************************************************************
** Form generated from reading UI file 'chatuserwid.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATUSERWID_H
#define UI_CHATUSERWID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatUserWid
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *icon_wid;
    QHBoxLayout *horizontalLayout_2;
    QLabel *icon_lb;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *name_lb;
    QLabel *msg_lb;
    QWidget *time_wid;
    QHBoxLayout *horizontalLayout_3;
    QLabel *time_lb;

    void setupUi(QWidget *ChatUserWid)
    {
        if (ChatUserWid->objectName().isEmpty())
            ChatUserWid->setObjectName(QString::fromUtf8("ChatUserWid"));
        ChatUserWid->resize(272, 70);
        ChatUserWid->setMinimumSize(QSize(0, 70));
        ChatUserWid->setMaximumSize(QSize(16777215, 70));
        horizontalLayout = new QHBoxLayout(ChatUserWid);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        icon_wid = new QWidget(ChatUserWid);
        icon_wid->setObjectName(QString::fromUtf8("icon_wid"));
        icon_wid->setMinimumSize(QSize(50, 50));
        icon_wid->setMaximumSize(QSize(50, 50));
        horizontalLayout_2 = new QHBoxLayout(icon_wid);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        icon_lb = new QLabel(icon_wid);
        icon_lb->setObjectName(QString::fromUtf8("icon_lb"));
        icon_lb->setMinimumSize(QSize(45, 45));
        icon_lb->setMaximumSize(QSize(45, 45));

        horizontalLayout_2->addWidget(icon_lb);


        horizontalLayout->addWidget(icon_wid);

        widget_2 = new QWidget(ChatUserWid);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(120, 0));
        widget_2->setMaximumSize(QSize(120, 16777215));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        name_lb = new QLabel(widget_2);
        name_lb->setObjectName(QString::fromUtf8("name_lb"));

        verticalLayout->addWidget(name_lb);

        msg_lb = new QLabel(widget_2);
        msg_lb->setObjectName(QString::fromUtf8("msg_lb"));

        verticalLayout->addWidget(msg_lb);


        horizontalLayout->addWidget(widget_2);

        time_wid = new QWidget(ChatUserWid);
        time_wid->setObjectName(QString::fromUtf8("time_wid"));
        time_wid->setMinimumSize(QSize(50, 50));
        time_wid->setMaximumSize(QSize(50, 50));
        horizontalLayout_3 = new QHBoxLayout(time_wid);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        time_lb = new QLabel(time_wid);
        time_lb->setObjectName(QString::fromUtf8("time_lb"));
        time_lb->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(time_lb);


        horizontalLayout->addWidget(time_wid);


        retranslateUi(ChatUserWid);

        QMetaObject::connectSlotsByName(ChatUserWid);
    } // setupUi

    void retranslateUi(QWidget *ChatUserWid)
    {
        ChatUserWid->setWindowTitle(QApplication::translate("ChatUserWid", "Form", nullptr));
        icon_lb->setText(QString());
        name_lb->setText(QApplication::translate("ChatUserWid", "lddxfy", nullptr));
        msg_lb->setText(QApplication::translate("ChatUserWid", "hello world!", nullptr));
        time_lb->setText(QApplication::translate("ChatUserWid", "13.54", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatUserWid: public Ui_ChatUserWid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATUSERWID_H
