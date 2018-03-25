#ifndef CLIENT_LOG_H
#define CLIENT_LOG_H

//C++

//QT
#include <QWidget>

//ADT
//#include "decoderclient.h"

namespace Ui {
class Client_Log;
}

class QTcpSocket;

class Client_Log : public QWidget
{
    Q_OBJECT

public:
    explicit Client_Log(QWidget *parent = 0);
    ~Client_Log();

private slots:
    void on_btnPlay_clicked();

    void on_btnRegister_clicked();

    void on_btnNewUser_clicked();

    void NewUser_State();

    void Partner_State();

    void on_btnPartner_clicked();

    void socketSend(QString msg);

    void socketReceive();

    void Clean_TextBox();

public:
    void Log_In_Req(bool Success);
    void Sign_In_Req(bool Success);


private:
    Ui::Client_Log *ui;

    QTcpSocket* Tcp_Socket_Client;
    //DecoderClient* decoTools;
};

#endif
