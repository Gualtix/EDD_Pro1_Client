#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//C++

//Qt
#include <QMainWindow>

//ADT

namespace Ui {
class MainWindow;
}

class QTcpSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent,QTcpSocket* mSocket_Client);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //explicit MainWindow(QTcpSocket* Tcp_Socket_Client);

    void Get_User_Files();

    void AddTableItem(int Rk,QString Nombre,QString Tipo,QString Permiso,QString Fecha);

    void Show_Message(QString Tl,QString msg);    

private slots:
    void socketReceive();

    void socketSend(QString msg);

    void on_btnUpdate_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket* Tcp_Socket_Client;
};

#endif // MAINWINDOW_H
