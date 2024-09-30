#include "chatview.h"
#include <QScrollBar>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
ChatView::ChatView(QWidget *parent) : QWidget(parent),isAppended(false)
{
    QVBoxLayout *pMainlayout = new QVBoxLayout();
    this->setLayout(pMainlayout);
    //设置布局的边距为0，这意味着布局中的控件将紧贴ChatView的边缘
    pMainlayout->setMargin(0);

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainlayout->addWidget(m_pScrollArea);
    //聊天背景
    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");
    //背景自动填充
    w->setAutoFillBackground(true);
    //向这个layout中添加聊天条目
    QVBoxLayout* pVLayout_1 = new QVBoxLayout();
    pVLayout_1->addWidget(new QWidget(),100000);
    w->setLayout(pVLayout_1);
    m_pScrollArea->setWidget(w);

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //获取滚动区域的垂直滚动条
    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
    //当滚动条范围改变时，将调用该槽函数
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);
    //将滚动条和放了聊天条目的垂直布局进行水平布局
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setMargin(0);
    m_pScrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    m_pScrollArea->setWidgetResizable(true);
    //安装事件过滤器
    m_pScrollArea->installEventFilter(this);
    initStyleSheet();

}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout *v1 = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    v1->insertWidget(v1->count()-1,item);
    isAppended = true;

}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

void ChatView::removeAllItem()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());

   int count = layout->count();

    for (int i = 0; i < count - 1; ++i) {
        QLayoutItem *item = layout->takeAt(0); // 始终从第一个控件开始删除
        if (item) {
            if (QWidget *widget = item->widget()) {
                delete widget;
            }
            delete item;
        }
    }
}

bool ChatView::eventFilter(QObject *o, QEvent *e)
{
    if(o == m_pScrollArea && e->type() == QEvent::Enter){
        //当鼠标进入滚动区域时，检查该区域中的最大条目数是否为0，是则隐藏
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
    }
    else if(o == m_pScrollArea && e->type() == QEvent::Leave){
        //当鼠标离开该区域，垂直滚动条直接隐藏
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(o,e);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended){
        QScrollBar *pVScrollbar = m_pScrollArea->verticalScrollBar();
        pVScrollbar->setSliderPosition(pVScrollbar->maximum());
        QTimer::singleShot(500,[this](){
            isAppended = false;
        });
    }
}

void ChatView::initStyleSheet()
{

}
