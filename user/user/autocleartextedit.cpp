#include "autocleartextedit.h"

AutoClearTextEdit::AutoClearTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}


AutoClearTextEdit::AutoClearTextEdit(const QString &text, QWidget *parent)
    : QTextEdit(parent)
{
    setPlainText(text);//设置初始文本
}


void AutoClearTextEdit::focusOutEvent(QFocusEvent *event)
{
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();//取消选择
    this->setTextCursor(cursor);//更新光标状态
    QTextEdit::focusOutEvent(event);//调用基类的事件处理
}
