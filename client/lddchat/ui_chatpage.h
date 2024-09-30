/********************************************************************************
** Form generated from reading UI file 'chatpage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATPAGE_H
#define UI_CHATPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedbtn.h>
#include "chatview.h"
#include "clickedlabel.h"
#include "messagetextedit.h"

QT_BEGIN_NAMESPACE

class Ui_ChatPage
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_4;
    QWidget *title_widget;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_4;
    QLabel *title_label;
    ChatView *chat_data_wid;
    QWidget *tool_wid;
    QHBoxLayout *horizontalLayout_2;
    ClickedLabel *emo_lb;
    ClickedLabel *file_lb;
    QSpacerItem *horizontalSpacer;
    MessageTextEdit *chatEdit;
    QWidget *send_wid;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    ClickedBtn *receive_btn;
    QSpacerItem *horizontalSpacer_3;
    ClickedBtn *send_btn;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *ChatPage)
    {
        if (ChatPage->objectName().isEmpty())
            ChatPage->setObjectName(QString::fromUtf8("ChatPage"));
        ChatPage->resize(715, 664);
        horizontalLayout = new QHBoxLayout(ChatPage);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(ChatPage);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_4 = new QVBoxLayout(widget_3);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        title_widget = new QWidget(widget_3);
        title_widget->setObjectName(QString::fromUtf8("title_widget"));
        title_widget->setMinimumSize(QSize(0, 60));
        title_widget->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_3 = new QHBoxLayout(title_widget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, -1, 0);
        widget_7 = new QWidget(title_widget);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        horizontalLayout_4 = new QHBoxLayout(widget_7);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        title_label = new QLabel(widget_7);
        title_label->setObjectName(QString::fromUtf8("title_label"));

        horizontalLayout_4->addWidget(title_label);


        horizontalLayout_3->addWidget(widget_7);


        verticalLayout_4->addWidget(title_widget);

        chat_data_wid = new ChatView(widget_3);
        chat_data_wid->setObjectName(QString::fromUtf8("chat_data_wid"));

        verticalLayout_4->addWidget(chat_data_wid);

        tool_wid = new QWidget(widget_3);
        tool_wid->setObjectName(QString::fromUtf8("tool_wid"));
        tool_wid->setMinimumSize(QSize(0, 60));
        tool_wid->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_2 = new QHBoxLayout(tool_wid);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        emo_lb = new ClickedLabel(tool_wid);
        emo_lb->setObjectName(QString::fromUtf8("emo_lb"));
        emo_lb->setMinimumSize(QSize(40, 40));
        emo_lb->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(emo_lb);

        file_lb = new ClickedLabel(tool_wid);
        file_lb->setObjectName(QString::fromUtf8("file_lb"));
        file_lb->setMinimumSize(QSize(40, 40));
        file_lb->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(file_lb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_4->addWidget(tool_wid);

        chatEdit = new MessageTextEdit(widget_3);
        chatEdit->setObjectName(QString::fromUtf8("chatEdit"));
        chatEdit->setMinimumSize(QSize(0, 200));
        chatEdit->setMaximumSize(QSize(16777215, 200));

        verticalLayout_4->addWidget(chatEdit);

        send_wid = new QWidget(widget_3);
        send_wid->setObjectName(QString::fromUtf8("send_wid"));
        send_wid->setMinimumSize(QSize(0, 60));
        send_wid->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_5 = new QHBoxLayout(send_wid);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        receive_btn = new ClickedBtn(send_wid);
        receive_btn->setObjectName(QString::fromUtf8("receive_btn"));
        receive_btn->setMinimumSize(QSize(80, 40));
        receive_btn->setMaximumSize(QSize(80, 40));

        horizontalLayout_5->addWidget(receive_btn);

        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        send_btn = new ClickedBtn(send_wid);
        send_btn->setObjectName(QString::fromUtf8("send_btn"));
        send_btn->setMinimumSize(QSize(80, 40));
        send_btn->setMaximumSize(QSize(80, 40));

        horizontalLayout_5->addWidget(send_btn);

        horizontalSpacer_4 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_4->addWidget(send_wid);


        horizontalLayout->addWidget(widget_3);


        retranslateUi(ChatPage);

        QMetaObject::connectSlotsByName(ChatPage);
    } // setupUi

    void retranslateUi(QWidget *ChatPage)
    {
        ChatPage->setWindowTitle(QApplication::translate("ChatPage", "Form", nullptr));
        title_label->setText(QApplication::translate("ChatPage", "\346\207\222\346\203\260\347\232\204\345\260\217\350\202\245\347\276\212", nullptr));
        emo_lb->setText(QString());
        file_lb->setText(QString());
        receive_btn->setText(QApplication::translate("ChatPage", "\346\216\245\346\224\266", nullptr));
        send_btn->setText(QApplication::translate("ChatPage", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatPage: public Ui_ChatPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATPAGE_H
