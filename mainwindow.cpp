#include "mainwindow.h"
#include "ui_mainwindow.h"


//C++
//Qt
//ADT
#include "user_session.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
}
