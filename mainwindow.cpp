//C++

//Qt
#include "QString"
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QMessageBox>

//ADT
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_session.h"
#include "decoderclient.h"

//(^< ............ ............ ............ Constructor: Set
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(User_Session::Nickname);

    //(^< ............ T A B L E
    ui->tblArchives->setColumnWidth(0,135);
    ui->tblArchives->setColumnWidth(3,135);
    ui->tblArchives->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //(^< ............ C L I E N T
    Tcp_Socket_Client = new QTcpSocket(this);

    Tcp_Socket_Client->connectToHost(QHostAddress::LocalHost,4328);
    connect(Tcp_Socket_Client,SIGNAL (readyRead()),this,SLOT (socketReceive()));

}


//(^< ............ ............ ............ Destructor
MainWindow::~MainWindow(){
    delete ui;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Form States
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Document
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Presentation
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Canvas
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//void MainWindow::Load_Canvas(int j, int i){

//}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Form States
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Client Management
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Get_User_Files(){

    QString GetArch = "Get_User_Files";
    GetArch.append("#");

    GetArch.append(User_Session::Nickname);
    GetArch.append("#");

    socketSend(GetArch);
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Sockets Handle
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::socketSend(QString msg){

    bool IsConnected = (Tcp_Socket_Client->state() == QTcpSocket::ConnectedState);
    if(IsConnected){
        QTextStream T(Tcp_Socket_Client);
        T << msg;
        Tcp_Socket_Client->flush();
    }
    else{
        QMessageBox::critical(this,"Error","Server is Not Listening");
    }
}

void MainWindow::socketReceive(){

    DecoderClient* decoTools = new DecoderClient();

    QByteArray Bf;
    Bf.resize(Tcp_Socket_Client->bytesAvailable());
    Tcp_Socket_Client->read(Bf.data(),Bf.size());

    QString msg = (QString (Bf));
    decoTools->GetStack_of_Answers(msg);
    decoTools->Answer_Request(this);
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ui Events
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void MainWindow::on_btnUpdate_clicked()
{
    ui->tblArchives->setRowCount(0);
    Get_User_Files();
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Actions
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void MainWindow::Show_Message(QString Tl,QString msg){
    QMessageBox::information(this,Tl,msg);
}

void MainWindow::AddTableItem(int Rk,QString Nombre,QString Tipo,QString Permiso,QString Fecha){

    ui->tblArchives->insertRow(Rk);

    ui->tblArchives->setItem(Rk,0,new QTableWidgetItem(Nombre));
    ui->tblArchives->setItem(Rk,1,new QTableWidgetItem(Tipo));
    ui->tblArchives->setItem(Rk,2,new QTableWidgetItem(Permiso));
    ui->tblArchives->setItem(Rk,3,new QTableWidgetItem(Fecha));

}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this,"Tl","msg");
}
