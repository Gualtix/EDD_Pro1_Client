#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//C++

//Qt
#include <QMainWindow>
#include <QTableWidgetItem>

//ADT
#include "USS/clientworking.h"
#include "Canvas/wbutton.h"


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

    void Show_Message(QString Type,QString Tl,QString msg);

    void Archive_Manager_FirstUse();

    void Fill_Archive_Manager(GeNode<ArchiveHandle*>* Gn);

    void Clean_Archive_Manager();

    void Document_Edit_FirstUse();

    void Presentation_Edit_FirstUSe();

    void Canvas_Edit_FirstUse();

    void MainWindow_FirstUse();

    void Load_Available_Users();

    void Get_Available_Users();

    void Fill_Combox_Users();

    void AddTable_US_Item(int Rk,QString Nombre,QString Permiso);

    //void Fill_Combox_Users_With_Exception(QString Rp);

    void Backup_Erasable_NickList();

    void Create_File(ArchiveHandle* Fl_Info);

    void Owner_State(QString Arch_Type);

    void Editor_State(QString Arch_Type);

    void Viewer_State(QString Arch_Type);

    void New_Archive_State();

    void Update_Archives_Table();

    void Delete_Archive(QString Archive_Name);

    void Get_Permission(QString Archive_Name);

    void Delete_Archive_State();

    void Update_Permission(QString Archive_Name);


private slots:

    void Pick_Color();

    void socketReceive();

    void socketSend(QString msg);

    void on_btnUpdate_clicked();

    void on_tblArchives_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnNew_clicked();

    void on_btnPushSingle_clicked();

    void on_btnPushBlock_clicked();

    void on_tboxLoad_clicked();

    void Load_Canvas(ArrayRange* J,ArrayRange* I);
    //void Load_Canvas();

    void on_btnRender_clicked();

    void on_btnSaveCanvas_clicked();

    void on_btnFetch_JSon_clicked();

    void on_tboxNombre_textChanged(const QString &arg1);

    //void on_pushButton_clicked();

    //void on_btnEdit_clicked();



    void on_btnAddPermiso_clicked();

    void on_tblPermisos_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnArchRegis_clicked();

    void on_btnDelete_clicked();

    void on_btnChangePermisos_clicked();

public:
    Ui::MainWindow *ui;
    QTcpSocket* Tcp_Socket_Client;

    ClientWorking* Artist;
    GenList<MyString*>* NickList;
    GenList<MyString*>* Erasable_NickList;
    GenList<MyString*>* PList;
};

#endif // MAINWINDOW_H
