#include "textbubble.h"
#include <QTextBlock>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent) : BubbleFrame (role,parent)
{
   m_TextEdit = new QTextEdit();
   //设置只读
   m_TextEdit->setReadOnly(true);
   //隐藏垂直和水平滚动条
   m_TextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   m_TextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   m_TextEdit->installEventFilter(this);
   QFont font("Microsoft YaHei");
   font.setPointSize(12);
   m_TextEdit->setFont(font);
   setPlainText(text);
   setWidget(m_TextEdit);
   initStyleSheet();
}
//设置文本的最大宽度
void TextBubble::setPlainText(const QString &text)
{
    m_TextEdit->setPlainText(text);
    //m_pTextEdit->setHtml(text);
    //找到段落中最大宽度
    qreal doc_margin = m_TextEdit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetricsF fm(m_TextEdit->font());
    QTextDocument *doc = m_TextEdit->document();
    int max_width = 0;
    //遍历每一段找到 最宽的那一段
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    //字体总长
    {
        int txtW = int(fm.width(it.text()));
        max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
    }
    //设置这个气泡的最大宽度 只需要设置一次
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));        //设置最大宽度
}

void TextBubble::initStyleSheet()
{
    //设置无边框
    m_TextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}

bool TextBubble::eventFilter(QObject *o, QEvent *e)
{
    if(m_TextEdit == o && e->type() == QEvent::Paint)
    {
        adjustTextHeight(); //PaintEvent中设置
    }
    return BubbleFrame::eventFilter(o, e);
}

//动态调整高度
void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_TextEdit->document()->documentMargin();    //字体到边框的距离默认为4
    QTextDocument *doc = m_TextEdit->document();
    qreal text_height = 0;
    //把每一段的高度相加=文本高
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();                             //这段的rect
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();
    //设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    setFixedHeight(text_height + doc_margin *2 + vMargin*2 );
}
