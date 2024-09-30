/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <applyfriendpage.h>
#include <chatpage.h>
#include <chatuserlist.h>
#include <clickedbtn.h>
#include <contactuserlist.h>
#include <friendinfopage.h>
#include <statewidget.h>
#include "customizeedit.h"
#include "searchlist.h"

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *status_bar;
    QVBoxLayout *verticalLayout;
    QWidget *state_wid;
    QVBoxLayout *verticalLayout_3;
    QLabel *side_head_lb;
    StateWidget *side_chat_lb;
    StateWidget *side_contact_lb;
    QSpacerItem *verticalSpacer;
    QWidget *chat_user_wid;
    QVBoxLayout *verticalLayout_2;
    QWidget *search_widget;
    QHBoxLayout *horizontalLayout_2;
    CustomizeEdit *search_edit;
    QSpacerItem *horizontalSpacer;
    ClickedBtn *add_btn;
    ChatUserList *chat_user_list;
    SearchList *search_list;
    ContactUserList *con_user_list;
    QStackedWidget *stackedWidget;
    ChatPage *chat_page;
    FriendInfoPage *friend_info_page;
    ApplyFriendPage *friend_apply_page;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName(QString::fromUtf8("ChatDialog"));
        ChatDialog->resize(1050, 900);
        horizontalLayout = new QHBoxLayout(ChatDialog);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        status_bar = new QWidget(ChatDialog);
        status_bar->setObjectName(QString::fromUtf8("status_bar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(status_bar->sizePolicy().hasHeightForWidth());
        status_bar->setSizePolicy(sizePolicy);
        status_bar->setMinimumSize(QSize(56, 0));
        status_bar->setMaximumSize(QSize(56, 16777215));
        verticalLayout = new QVBoxLayout(status_bar);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 30, 0, 0);
        state_wid = new QWidget(status_bar);
        state_wid->setObjectName(QString::fromUtf8("state_wid"));
        verticalLayout_3 = new QVBoxLayout(state_wid);
        verticalLayout_3->setSpacing(30);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        side_head_lb = new QLabel(state_wid);
        side_head_lb->setObjectName(QString::fromUtf8("side_head_lb"));
        side_head_lb->setMinimumSize(QSize(35, 35));
        side_head_lb->setMaximumSize(QSize(35, 35));

        verticalLayout_3->addWidget(side_head_lb);

        side_chat_lb = new StateWidget(state_wid);
        side_chat_lb->setObjectName(QString::fromUtf8("side_chat_lb"));
        side_chat_lb->setMinimumSize(QSize(30, 30));
        side_chat_lb->setMaximumSize(QSize(30, 30));

        verticalLayout_3->addWidget(side_chat_lb);

        side_contact_lb = new StateWidget(state_wid);
        side_contact_lb->setObjectName(QString::fromUtf8("side_contact_lb"));
        side_contact_lb->setMinimumSize(QSize(30, 30));
        side_contact_lb->setMaximumSize(QSize(30, 30));

        verticalLayout_3->addWidget(side_contact_lb);


        verticalLayout->addWidget(state_wid);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(status_bar);

        chat_user_wid = new QWidget(ChatDialog);
        chat_user_wid->setObjectName(QString::fromUtf8("chat_user_wid"));
        chat_user_wid->setMaximumSize(QSize(250, 16777215));
        verticalLayout_2 = new QVBoxLayout(chat_user_wid);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        search_widget = new QWidget(chat_user_wid);
        search_widget->setObjectName(QString::fromUtf8("search_widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(search_widget->sizePolicy().hasHeightForWidth());
        search_widget->setSizePolicy(sizePolicy1);
        search_widget->setMinimumSize(QSize(0, 60));
        search_widget->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_2 = new QHBoxLayout(search_widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(9, 9, 9, 9);
        search_edit = new CustomizeEdit(search_widget);
        search_edit->setObjectName(QString::fromUtf8("search_edit"));
        search_edit->setMaximumSize(QSize(180, 25));

        horizontalLayout_2->addWidget(search_edit);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        add_btn = new ClickedBtn(search_widget);
        add_btn->setObjectName(QString::fromUtf8("add_btn"));
        add_btn->setMaximumSize(QSize(30, 30));

        horizontalLayout_2->addWidget(add_btn);


        verticalLayout_2->addWidget(search_widget);

        chat_user_list = new ChatUserList(chat_user_wid);
        chat_user_list->setObjectName(QString::fromUtf8("chat_user_list"));

        verticalLayout_2->addWidget(chat_user_list);

        search_list = new SearchList(chat_user_wid);
        search_list->setObjectName(QString::fromUtf8("search_list"));

        verticalLayout_2->addWidget(search_list);

        con_user_list = new ContactUserList(chat_user_wid);
        con_user_list->setObjectName(QString::fromUtf8("con_user_list"));

        verticalLayout_2->addWidget(con_user_list);


        horizontalLayout->addWidget(chat_user_wid);

        stackedWidget = new QStackedWidget(ChatDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        chat_page = new ChatPage();
        chat_page->setObjectName(QString::fromUtf8("chat_page"));
        stackedWidget->addWidget(chat_page);
        friend_info_page = new FriendInfoPage();
        friend_info_page->setObjectName(QString::fromUtf8("friend_info_page"));
        stackedWidget->addWidget(friend_info_page);
        friend_apply_page = new ApplyFriendPage();
        friend_apply_page->setObjectName(QString::fromUtf8("friend_apply_page"));
        stackedWidget->addWidget(friend_apply_page);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QApplication::translate("ChatDialog", "Dialog", nullptr));
        side_head_lb->setText(QString());
        add_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
