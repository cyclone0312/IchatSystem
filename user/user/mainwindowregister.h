#ifndef MAINWINDOWREGISTER_H
#define MAINWINDOWREGISTER_H

#include <QMainWindow>

namespace Ui {
class MainWindowregister;
}

class MainWindowregister : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowregister(QWidget *parent = nullptr);
    ~MainWindowregister();

private:
    Ui::MainWindowregister *ui;
};

#endif // MAINWINDOWREGISTER_H
