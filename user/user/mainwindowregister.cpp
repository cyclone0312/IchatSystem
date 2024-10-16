#include "mainwindowregister.h"
#include "ui_mainwindowregister.h"

MainWindowregister::MainWindowregister(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowregister)
{
    ui->setupUi(this);
}

MainWindowregister::~MainWindowregister()
{
    delete ui;
}
