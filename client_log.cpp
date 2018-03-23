//C++

//Qt
#include "QString"
#include "mainwindow.h"
#include "ui_client_log.h"
//Local --> #include <QLocalSocket>
#include <QTcpSocket>
#include <QTextStream>

//ADT
#include "user_session.h"
#include "client_log.h"

QString User_Session::Nickname = "";
QString User_Session::Password = "";
QString User_Session::Name = "";
QString User_Session::EMail = "";

Client_Log::Client_Log(QWidget *parent) : QWidget(parent), ui(new Ui::Client_Log){

    ui->setupUi(this);
    // Local --> mSocket = new QLocalSocket(this);
    mSocket = new QTcpSocket(this);
    //Local ---> connect(mSocket, &QLocalSocket::readyRead, [&](){
    connect(mSocket, &QTcpSocket::readyRead, [&](){
        QTextStream T(mSocket);
        //Lectura del Servidor

        ui->tboxEMail->setText(T.readAll());
    });


    NewUser_State();
}

void Client_Log::NewUser_State(){

    ui->btnPartner->setEnabled(false);
    ui->btnNewUser->setEnabled(true);

    ui->tboxName->setEnabled(false);
    ui->tboxEMail->setEnabled(false);

    ui->btnRegister->setEnabled(false);
    ui->btnPlay->setEnabled(true);
}

void Client_Log::Partner_State(){

    ui->btnPartner->setEnabled(true);
    ui->btnNewUser->setEnabled(false);

    ui->tboxName->setEnabled(true);
    ui->tboxEMail->setEnabled(true);

    ui->btnRegister->setEnabled(true);
    ui->btnPlay->setEnabled(false);
}



Client_Log::~Client_Log(){
    delete ui;
}

void Client_Log::on_btnPlay_clicked(){


    MainWindow* w = new MainWindow();
    w->show();

    delete this;
}

void Client_Log::on_btnRegister_clicked(){

}

void Client_Log::on_btnNewUser_clicked(){
    //Local ---> mSocket->connectToServer("Shared_Files_Server");

    //mSocket->connectToHost("190.149.42.96",1234);

    /mSocket->connectToHost("localhost",4328);

    Partner_State();


    //ui->tboxEMail->setText(QString::number(ClientConfig::HOLAS));
    //ui->tboxName->setText(ClientConfig.USR.Name);


}

void Client_Log::on_btnPartner_clicked(){
    NewUser_State();
    //ClientConfig.USR.Name = "Nomanches xD Parner";
    //ui->tboxName->setText(ClientConfig.USR.Name);
}
