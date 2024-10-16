#ifndef AUTOCLEARTEXTEDIT_H
#define AUTOCLEARTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <QTextEdit>
#include <QFocusEvent>

class AutoClearTextEdit : public QTextEdit//可以自动取消选择文字的textedit
{
    Q_OBJECT

public:
    explicit AutoClearTextEdit(QWidget *parent = nullptr);
    explicit AutoClearTextEdit(const QString &text, QWidget *parent = nullptr);//新增构造函数

protected:
    void focusOutEvent(QFocusEvent *event) override;
};

#endif // AUTOCLEARTEXTEDIT_H
