//C++

//Qt
#include <QApplication>

//ADT
//#include "mainwindow.h"
#include "client_log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client_Log* CL = new Client_Log();

    CL->show();

    return a.exec();
}
