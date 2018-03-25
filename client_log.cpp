//C++

//Qt
#include "QString"
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QMessageBox>

//ADT
#include "decoderclient.h"
#include "mainwindow.h"
#include "ui_client_log.h"
#include "user_session.h"
#include "client_log.h"

//Static Storage Definition
QString User_Session::Nickname = "";
QString User_Session::Password = "";
QString User_Session::Name = "";
QString User_Session::EMail = "";

//(^< ............ ............ ............ Constructor: Set
Client_Log::Client_Log(QWidget *parent) : QWidget(parent), ui(new Ui::Client_Log){

    ui->setupUi(this);

    Tcp_Socket_Client = new QTcpSocket(this);

    connect(Tcp_Socket_Client,SIGNAL (readyRead()),this,SLOT (socketReceive()));
    Tcp_Socket_Client->connectToHost(QHostAddress::LocalHost,4328);

    NewUser_State();
}

//(^< ............ ............ ............ Destructor
Client_Log::~Client_Log(){
    delete ui;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Form States
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void Client_Log::Clean_TextBox(){
    ui->tboxNickname->setText("");
    ui->tboxPassword->setText("");
    ui->tboxName->setText("");
    ui->tboxEMail->setText("");
}

void Client_Log::NewUser_State(){

    ui->btnNewUser->setEnabled(false);
    ui->btnPartner->setEnabled(true);

    Clean_TextBox();

    ui->tboxNickname->show();
    ui->tboxPassword->show();
    ui->tboxName->show();
    ui->tboxEMail->show();

    ui->lblNickname->show();
    ui->lblPassword->show();
    ui->lblName->show();
    ui->lblEMail->show();

    ui->btnPlay->hide();
    ui->btnRegister->show();
}

void Client_Log::Partner_State(){

    ui->btnNewUser->setEnabled(true);
    ui->btnPartner->setEnabled(false);

    Clean_TextBox();

    ui->tboxNickname->show();
    ui->tboxPassword->show();
    ui->tboxName->hide();
    ui->tboxEMail->hide();

    ui->lblNickname->show();
    ui->lblPassword->show();
    ui->lblName->hide();
    ui->lblEMail->hide();

    ui->btnPlay->show();
    ui->btnRegister->hide();
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ui Events
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void Client_Log::on_btnPlay_clicked(){

    QString PartnerInfo = "Log_In";
    PartnerInfo.append("#");

    PartnerInfo.append(ui->tboxNickname->text());
    PartnerInfo.append("#");

    PartnerInfo.append(ui->tboxPassword->text());
    PartnerInfo.append("#");

    socketSend(PartnerInfo);

}

void Client_Log::on_btnRegister_clicked(){

    QString NewUserInfo = "Sign_In";

    NewUserInfo.append("#");

    NewUserInfo.append(ui->tboxNickname->text());
    NewUserInfo.append("#");

    NewUserInfo.append(ui->tboxPassword->text());
    NewUserInfo.append("#");

    NewUserInfo.append(ui->tboxName->text());
    NewUserInfo.append("#");

    NewUserInfo.append(ui->tboxEMail->text());
    NewUserInfo.append("#");

    socketSend(NewUserInfo);


}

void Client_Log::on_btnNewUser_clicked(){
    NewUser_State();
}

void Client_Log::on_btnPartner_clicked(){
    Partner_State();
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Client Management
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void Client_Log::Log_In_Req(bool Success){
    if(Success){
        QMessageBox::information(this,"Login","Welcome to Shared Files Management");

        User_Session::Nickname = ui->tboxNickname->text();
        User_Session::Password = ui->tboxPassword->text();

        MainWindow* W = new MainWindow();
        W->show();

        delete this;
    }
    else{
        QMessageBox::information(this,"Login","Password dont Match with Nickname, Please Try Again...");
    }

}

void Client_Log::Sign_In_Req(bool Success){
    if(Success){
        QMessageBox::information(this,"Sign in","New User Successfuly Registered");

        User_Session::Nickname = ui->tboxNickname->text();
        User_Session::Password = ui->tboxPassword->text();
        User_Session::Name = ui->tboxName->text();
        User_Session::EMail = ui->tboxEMail->text();

        Partner_State();
        ui->tboxName->setText(User_Session::Nickname);

        //MainWindow* W = new MainWindow();
        //W->show();

        //delete this;
    }
    else{
        QMessageBox::information(this,"Sign in","New User Nickname is Already in Use, Please Try Again...");
    }

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Sockets Handle
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void Client_Log::socketSend(QString msg){

    //if(Tcp_Socket_Client){
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

void Client_Log::socketReceive(){

    DecoderClient* decoTools = new DecoderClient();

    QByteArray Bf;
    Bf.resize(Tcp_Socket_Client->bytesAvailable());
    Tcp_Socket_Client->read(Bf.data(),Bf.size());

    QString msg = (QString (Bf));
    decoTools->GetStack_of_Answers(msg);
    decoTools->Answer_Request(this);
}

