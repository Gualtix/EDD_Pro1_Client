#ifndef CLIENT_LOG_H
#define CLIENT_LOG_H

//C++

//QT
#include <QWidget>


//ADT

namespace Ui {
class Client_Log;
}

//Local --> class QLocalSocket;
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

private:
    Ui::Client_Log *ui;

    //Local --> QLocalSocket* mSocket;
    QTcpSocket* mSocket;
};

#endif // CLIENT_LOG_H
