#include "mainwindow.h"
#include "client_log.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Client_Log* CL = new Client_Log();
    CL->show();



    //MainWindow w;
    //w.show();


    return a.exec();
}
