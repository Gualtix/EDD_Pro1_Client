//C++

//Qt
#include <QApplication>

//ADT
#include "mainwindow.h"
#include "client_log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow* w = new MainWindow();
    //w->show();

    Client_Log* CL = new Client_Log();

    CL->show();

    return a.exec();
}
