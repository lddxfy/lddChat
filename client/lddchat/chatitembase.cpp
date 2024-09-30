#include "chatitembase.h"
#include <QGridLayout>
ChatItemBase::ChatItemBase(ChatRole role,QWidget *parent) : QWidget(parent),m_role(role)
{
    //设置名称
    m_pNameLabel = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name_lb");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);
    //设置头像
    m_pIconLabel    = new QLabel();
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42, 42);
    //聊天气泡
    m_pBubble       = new QWidget();
    //设置网格布局
    QGridLayout* pGlayout = new QGridLayout();
    //设置边距
    pGlayout->setVerticalSpacing(3);
    pGlayout->setHorizontalSpacing(3);
    pGlayout->setMargin(3);
    //设置弹簧
    QSpacerItem* pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    if(m_role == Self){
        //名称和头像之间间距为8
        m_pNameLabel->setContentsMargins(0,0,8,0);
        //设置名称标签右对齐
        m_pNameLabel->setAlignment(Qt::AlignRight);
        //设置布局占位
        pGlayout->addWidget(m_pNameLabel,0,1,1,1);
        pGlayout->addWidget(m_pIconLabel,0,2,2,1,Qt::AlignTop);
        pGlayout->addItem(pSpacer, 1, 0, 1, 1);
        pGlayout->addWidget(m_pBubble, 1,1, 1,1);
        //设置变化比例
        pGlayout->setColumnStretch(0, 2);
        pGlayout->setColumnStretch(1, 3);

    }else{
        //名称和头像之间间距为8
        m_pNameLabel->setContentsMargins(8,0,0,0);
        //设置名称标签左对齐
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        //设置布局占位
        pGlayout->addWidget(m_pNameLabel,0,1,1,1);
        pGlayout->addWidget(m_pIconLabel,0,0,2,1,Qt::AlignTop);
        pGlayout->addItem(pSpacer, 2, 2, 1, 1);
        pGlayout->addWidget(m_pBubble, 1,1, 1,1);
        //设置变化比例
        pGlayout->setColumnStretch(1, 3);
        pGlayout->setColumnStretch(2, 2);

    }
    this->setLayout(pGlayout);
}

void ChatItemBase::setUserName(const QString &name)
{
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon)
{
    m_pIconLabel->setPixmap(icon);
}


void ChatItemBase::setWidget(QWidget *w)
{
   QGridLayout *pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
   pGLayout->replaceWidget(m_pBubble, w);
   delete m_pBubble;
   m_pBubble = w;
}
