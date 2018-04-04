//C++

//Qt
#include "QString"
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QMessageBox>
#include <QDate>

//ADT
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_session.h"
#include "decoderclient.h"
#include "asklimits.h"
//#include "Canvas/wbutton.h"

//(^< ............ ............ ............ Constructor: Set
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(User_Session::Nickname);


    this->NickList = new GenList<MyString*>();
    this->Erasable_NickList = new GenList<MyString*>();
    this->PList = new GenList<MyString*>();


    //(^< ............ T A B L E
    ui->tblArchives->setColumnWidth(0,135);
    ui->tblArchives->setColumnWidth(3,135);
    ui->tblArchives->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblPermisos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //(^< ............ C L I E N T
    Tcp_Socket_Client = new QTcpSocket(this);

    Tcp_Socket_Client->connectToHost(QHostAddress::LocalHost,4328);
    connect(Tcp_Socket_Client,SIGNAL (readyRead()),this,SLOT (socketReceive()));

    //(^< ............ A R T I S T
    Artist = new ClientWorking();

    //(^< ............ F I R S T  U S E
    MainWindow_FirstUse();

}

//(^< ............ ............ ............ Destructor
MainWindow::~MainWindow(){
    delete ui;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Form States
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::MainWindow_FirstUse(){
    Archive_Manager_FirstUse();
}

void MainWindow::Archive_Manager_FirstUse(){

    Clean_Archive_Manager();
    ui->btnUpdate->setEnabled(true);
    ui->btnNew->setEnabled(true);
    ui->btnArchRegis->setEnabled(false);
    ui->btnDelete->setEnabled(false);
    //ui->btnEdit->setEnabled(false);

    ui->btnUseEditor->setEnabled(false);
    ui->btnFetch_JSon->setEnabled(false);
    ui->btnAddPermiso->setEnabled(false);

    ui->btnChangePermisos->setEnabled(false);
    ui->btnViewPDF->setEnabled(false);

    //Update_Archives_Table();




    /*
    ui->gboxArchInfo->setEnabled(false);
    ui->gboxPermisos->setEnabled(false);

    ui->gboxEdit->setEnabled(false);
    ui->tabManager->setTabEnabled(1,false);
    ui->tabManager->setTabEnabled(2,false);
    ui->tabManager->setTabEnabled(3,false);
    */
}

void MainWindow::New_Archive_State(){

    Clean_Archive_Manager();

    Get_Available_Users();
    Update_Archives_Table();

    //Backup_Erasable_NickList();


    //Load_Available_Users();

    ui->gboxActios->setEnabled(false);
    ui->gboxPermisos->setEnabled(true);
    ui->tboxNombre->setReadOnly(false);
    ui->tboxNombre->setFocus();


    ui->btnUpdate->setEnabled(false);
    ui->btnNew->setEnabled(false);
    ui->btnArchRegis->setEnabled(true);
    //ui->btnEdit->setEnabled(false);
    ui->btnDelete->setEnabled(false);

    ui->btnUseEditor->setEnabled(false);
    ui->btnFetch_JSon->setEnabled(false);
    ui->btnAddPermiso->setEnabled(true);


    ui->tboxCreador->setText(User_Session::Nickname);
    ui->tboxCreado->setText(QDate::currentDate().toString("dd/MM/yy"));
    ui->tboxModificador->setText(User_Session::Nickname);
    ui->tboxModificado->setText(QDate::currentDate().toString("dd/MM/yy"));
    ui->tboxURL->setText("JSon_DIR/.json");
    ui->comboxPermiso->setCurrentIndex(2);

}

void MainWindow::Delete_Archive_State(){

    Clean_Archive_Manager();

    ui->btnDelete->setEnabled(false);
    Update_Archives_Table();
    Show_Message("Info","Archive Manager","Archivo Eliminado Exitosamente");

    ui->gboxPermisos->setEnabled(false);
    ui->gboxActios->setEnabled(false);

}

void MainWindow::Owner_State(QString Arch_Type){

    ui->btnNew->setEnabled(true);
    ui->btnArchRegis->setEnabled(false);
    ui->btnDelete->setEnabled(false);


    ui->gboxPermisos->setEnabled(true);
    ui->gboxActios->setEnabled(true);

    ui->btnAddPermiso->setEnabled(true);

    ui->btnViewPDF->setEnabled(true);
    ui->btnUseEditor->setEnabled(true);
    ui->btnChangePermisos->setEnabled(true);

    ui->btnDelete->setEnabled(true);
}

void MainWindow::Editor_State(QString Arch_Type){

    ui->tblPermisos->setRowCount(0);

    ui->btnNew->setEnabled(true);
    ui->btnArchRegis->setEnabled(false);
    ui->tboxNombre->setReadOnly(true);
    ui->gboxActios->setEnabled(true);

    ui->gboxPermisos->setEnabled(false);

    ui->btnViewPDF->setEnabled(true);
    ui->btnUseEditor->setEnabled(true);
    ui->btnChangePermisos->setEnabled(false);

    ui->btnDelete->setEnabled(false);
}

void MainWindow::Viewer_State(QString Arch_Type){

    ui->tblPermisos->setRowCount(0);

    ui->btnNew->setEnabled(true);
    ui->btnArchRegis->setEnabled(false);
    ui->btnNew->setEnabled(true);
    ui->btnArchRegis->setEnabled(false);
    ui->btnDelete->setEnabled(false);
    //ui->btnEdit->setEnabled(false);

    //ui->gboxArchInfo->setEnabled(false);
    ui->tboxNombre->setReadOnly(true);
    ui->gboxPermisos->setEnabled(false);

    ui->btnViewPDF->setEnabled(true);
    ui->btnUseEditor->setEnabled(false);
    ui->btnChangePermisos->setEnabled(false);

    ui->btnDelete->setEnabled(false);

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Clean
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Clean_Archive_Manager(){

    this->PList->Clear_List();

    ui->tboxNombre->setText("");
    ui->tboxCreador->setText("");
    ui->tboxCreado->setText("");
    ui->tboxModificador->setText("");
    ui->tboxModificado->setText("");
    ui->tboxURL->setText("");

    //(^< ............ C O M B O X
    ui->comboxEstado->setCurrentIndex(0);
    ui->comboxPermiso->setCurrentIndex(0);
    ui->comboxTipo->setCurrentIndex(0);

     ui->comboxUS_AV->clear();

    //(^< ............ T A B L E
    ui->tblArchives->setRowCount(0);
    ui->tblPermisos->setRowCount(0);
}
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Document
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Presentation
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Canvas
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Load_Canvas(ArrayRange* I,ArrayRange* J){
//void MainWindow::Load_Canvas(){
    ui->tboxColor->setText("#cc0099");

    //(^< ............ C O N T R O L S
    ui->spinBoxFila->setMaximum(I->Sup - 1);
    ui->spinBoxFila->setMinimum(I->Low);

    ui->spinBoxColumna->setMaximum(J->Sup - 1);
    ui->spinBoxColumna->setMinimum(J->Low);

    ui->spinBoxSup_Fl->setMaximum(I->Sup - 1);
    ui->spinBoxSup_Fl->setMinimum(I->Low);

    ui->spinBoxSup_Cl->setMaximum(J->Sup - 1);
    ui->spinBoxSup_Cl->setMinimum(J->Low);

    ui->spinBoxInf_Fl->setMaximum(I->Sup - 1);
    ui->spinBoxInf_Fl->setMinimum(I->Low);

    ui->spinBoxInf_Cl->setMaximum(J->Sup - 1);
    ui->spinBoxInf_Cl->setMinimum(J->Low);

    //(^< ............ C O L O R   P I C K E R
    WButton* MyButton;
    int cnt = 0;
    while(cnt < 7){


        int j = (cnt % 7);
        int i = (cnt / 7);

        MyButton = new WButton(ui->gboxEdit);
        MyButton->setGeometry(QRect(500 + 21 * (j),150 + 21 * (i),20,20));

        connect(MyButton, SIGNAL(clicked()), this, SLOT(Pick_Color()));

        if(cnt == 0){
            //Rojo
            MyButton->SetColor("#990000");
        }
        else if(cnt == 1){
            //Verde
            MyButton->SetColor("#006600");
        }
        else if(cnt == 2){
            //Azul
            MyButton->SetColor("#000066");
        }
        else if(cnt == 3){
            //Naranja
            MyButton->SetColor("#e65c00");
        }
        else if(cnt == 4){
            //Amarillo
            MyButton->SetColor("#e6b800");
        }
        else if(cnt == 5){
            //Blanco
            MyButton->SetColor("#ffffff");
        }
        else if(cnt == 6){
            //Negro
            MyButton->SetColor("#000000");
        }

        MyButton->show();
        cnt++;
    }


    //(^< ............ E D G E

    //J
    cnt = 0;
    while(cnt < J->Size){
        MyButton = new WButton(ui->gboxCanvas_Preview);
        MyButton->setGeometry(QRect(26 + 21 * (cnt),25,20,20));
        MyButton->setText(QString::number(cnt + J->Low));
        MyButton->SetColor("#99bbff");
        MyButton->show();
        cnt++;
    }

    //I
    cnt = 0;
    while(cnt < I->Size){
        MyButton = new WButton(ui->gboxCanvas_Preview);
        MyButton->setGeometry(QRect(5,46 + 21 * (cnt),20,20));
        MyButton->setText(QString::number(cnt + I->Low));
        MyButton->SetColor("#ff944d");
        MyButton->show();
        cnt++;
    }

    //(^< ............ P I X E L S
    int Loc = 0;
    int J_Size = J->Size;
    int I_Size = I->Size;
    while(Loc < (J_Size * I_Size)){

        int j = (Loc % J_Size);
        int i = (Loc / J_Size);

        MyButton = new WButton(ui->gboxCanvas_Preview);
        //connect(MyButton, SIGNAL(clicked()), this, SLOT(Show_ID_BT()));

        MyButton->setGeometry(QRect(26 + 21 * (j),46 + 21 * (i),20,20));
        //MyButton->setText(QString::number(Loc));
        MyButton->setText("");
        MyButton->SetColor("#ffffff");

        MyButton->i = i;
        MyButton->j - j;
        MyButton->Loc = Loc;

        MyButton->show();

        //Artist->Selected_Archive = "MarioBros";
        //Artist->ArchList->GetNode_By_String(Artist->Selected_Archive)->Data->Cnv->Btn_List->EndInsert(MyButton);
        Artist->Get_Archive_By_Name("MarioBros")->Data->Cnv->Btn_List->EndInsert(MyButton);


        Loc++;
    }
}

void MainWindow::Pick_Color(){
    WButton *Bt = (WButton *)sender();
    ui->tboxColor->setText(Bt->Current_Color);
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ R E Q U E S T S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Get_User_Files(){

    QString Reqest = "Get_User_Files";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    socketSend(Reqest);
}

void MainWindow::Get_Available_Users(){

    QString Reqest = "Get_Available_Users";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    socketSend(Reqest);
}

void MainWindow::Create_File(ArchiveHandle* Fl_Info){
    QString Reqest = "Create_File";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Fl_Info->Name);
    Reqest.append("#");

    Reqest.append(Fl_Info->Creator);
    Reqest.append("#");

    Reqest.append(Fl_Info->Creation_Date);
    Reqest.append("#");

    Reqest.append(Fl_Info->Modifier_Nickname);
    Reqest.append("#");

    Reqest.append(Fl_Info->Modification_Date);
    Reqest.append("#");

    Reqest.append(Fl_Info->Type);
    Reqest.append("#");

    Reqest.append(Fl_Info->Permiso);
    Reqest.append("#");

    Reqest.append(Fl_Info->State);
    Reqest.append("#");

    Reqest.append(Fl_Info->JSon_URL);
    Reqest.append("#");

    Reqest.append(QString::number(Fl_Info->Permiso_List->ListSize));
    Reqest.append("#");



    //(^< ............ P E R M I S O S
    int cnt = 0;
    while(cnt < Fl_Info->Permiso_List->ListSize){


        QString Us = Fl_Info->Permiso_List->GetNode(cnt)->Data->Usuario;
        QString Pm = Fl_Info->Permiso_List->GetNode(cnt)->Data->Permiso;

        Reqest.append(Us);
        Reqest.append("#");

        Reqest.append(Pm);
        Reqest.append("#");

        cnt++;
    }

    socketSend(Reqest);
}

void MainWindow::Delete_Archive(QString Archive_Name){

    QString Reqest = "Delete_File";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Archive_Name);
    Reqest.append("#");

    socketSend(Reqest);

}

void MainWindow::Get_Permission(QString Archive_Name){
    QString Reqest = "Get_Permission";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Archive_Name);
    Reqest.append("#");

    socketSend(Reqest);
}

void MainWindow::Update_Permission(QString Archive_Name){
    QString Reqest = "Update_Permission";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Archive_Name);
    Reqest.append("#");

    Reqest.append(QString::number(ui->tblPermisos->rowCount()));
    Reqest.append("#");

    //(^< ............ P E R M I S O S
    int cnt = 0;
    while(cnt < ui->tblPermisos->rowCount()){


        QString Us = ui->tblPermisos->item(cnt,0)->text();
        QString Pm = ui->tblPermisos->item(cnt,1)->text();

        Reqest.append(Us);
        Reqest.append("#");

        Reqest.append(Pm);
        Reqest.append("#");

        cnt++;
    }

    socketSend(Reqest);
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
//(^< ............ ............ ............ ............ ............ Actions
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Show_Message(QString Type,QString Tl,QString msg){
    if(Type == "Info"){
        QMessageBox::information(this,Tl,msg);
    }
    else{
        QMessageBox::critical(this,Tl,msg);
    }
}

void MainWindow::AddTableItem(int Rk,QString Nombre,QString Tipo,QString Permiso,QString Fecha){

    ui->tblArchives->insertRow(Rk);

    ui->tblArchives->setItem(Rk,0,new QTableWidgetItem(Nombre));
    ui->tblArchives->setItem(Rk,1,new QTableWidgetItem(Tipo));
    ui->tblArchives->setItem(Rk,2,new QTableWidgetItem(Permiso));
    ui->tblArchives->setItem(Rk,3,new QTableWidgetItem(Fecha));


}

void MainWindow::AddTable_US_Item(int Rk,QString Nombre,QString Permiso){

    ui->tblPermisos->insertRow(Rk);

    ui->tblPermisos->setItem(Rk,0,new QTableWidgetItem(Nombre));
    ui->tblPermisos->setItem(Rk,1,new QTableWidgetItem(Permiso));
}

void MainWindow::Fill_Archive_Manager(GeNode<ArchiveHandle*>* Gn){

    if(Gn){
        Artist->Selected_Archive = Gn->Data->Name;

        ui->tboxNombre->setText(Gn->Data->Name);
        ui->tboxCreador->setText(Gn->Data->Creator);
        ui->tboxCreado->setText(Gn->Data->Creation_Date);
        ui->tboxModificador->setText(Gn->Data->Modifier_Nickname);
        ui->tboxModificado->setText(Gn->Data->Modification_Date);
        //ui->tboxTipo->setText(Gn->Data->Type);
        ui->tboxURL->setText(Gn->Data->JSon_URL);

        //(^< ............ C O M B O X
        QString Tp = Gn->Data->Type;
        if(Tp == "lienzo"){
            ui->comboxTipo->setCurrentIndex(0);
        }
        else if(Tp == "documento"){
            ui->comboxTipo->setCurrentIndex(1);
        }
        else{
            ui->comboxTipo->setCurrentIndex(2);
        }

        QString Pm = Gn->Data->Permiso;
        if(Pm == "ver"){
            ui->comboxPermiso->setCurrentIndex(0);
        }
        else if(Pm == "editar"){
            ui->comboxPermiso->setCurrentIndex(1);
        }
        else{
            ui->comboxPermiso->setCurrentIndex(2);
        }

        QString St = Gn->Data->State;
        if(St == "disponible" || St == ""){
            ui->comboxEstado->setCurrentIndex(0);
        }
        else{
            ui->comboxEstado->setCurrentIndex(1);
        }
    }
}

void MainWindow::Load_Available_Users(){

}

void MainWindow::Fill_Combox_Users(){
    ui->comboxUS_AV->clear();

    if(Erasable_NickList->ListSize > 0){
        int cnt = 0;
        while (cnt < Erasable_NickList->ListSize) {
            ui->comboxUS_AV->addItem(Erasable_NickList->GetNode(cnt)->Data->Cad);
            cnt++;
        }
    }
}

void MainWindow::Backup_Erasable_NickList(){
    if(NickList->ListSize > 0){
        int cnt = 0;
        while(cnt < NickList->ListSize){
            QString St = NickList->GetNode(cnt)->Data->Cad;
            Erasable_NickList->EndInsert(new MyString(St));
            cnt++;
        }
    }
}

void MainWindow::Update_Archives_Table(){
    ui->tblArchives->setRowCount(0);
    Get_User_Files();
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ui Events
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::on_btnUpdate_clicked(){

    Update_Archives_Table();
    //Update_Archives_Table();
    //ui->btnUpdate->setEnabled(false);
    //QMessageBox::information(this,"Tl","msg");
}

void MainWindow::on_tblArchives_itemDoubleClicked(QTableWidgetItem *item){
    int Rw = ui->tblArchives->currentRow();
    QString Nm = ui->tblArchives->item(Rw,0)->text();
    GeNode<ArchiveHandle*>* Gn = Artist->Get_Archive_By_Name(Nm);
    Artist->Selected_Archive = Nm;

    Fill_Archive_Manager(Gn);

    //(^< ............ Filter Actios
    QString Tipo = ui->tblArchives->item(Rw,1)->text();
    QString Permiso = ui->tblArchives->item(Rw,2)->text();
    if(Permiso == "ver"){
        Viewer_State(Tipo);
    }
    else if(Permiso == "editar"){
        Editor_State(Tipo);
    }
    if(Permiso == "dueño"){
        Owner_State(Tipo);
        Get_Available_Users();
        Get_Permission(Nm);
    }

    //(^< ............ Fill Permission


}

void MainWindow::on_btnNew_clicked(){



    New_Archive_State();



    //    ui->tboxNF_Fecha->setText(QDate::currentDate().toString("dd/MM/yy"));

    //ui->tboxCreador->setText(User_Session::Nickname);
    //ui->gboxArchInfo->setEnabled(false);
    //ui->gboxArchInfo->setDisabled(true);
}

void MainWindow::on_btnPushSingle_clicked()
{
    QString Fl =  QString::number(ui->spinBoxFila->value());
    QString Cl =  QString::number(ui->spinBoxColumna->value());

    QString Color = ui->tboxColor->text();

    QString Last_Inst = "";
    if(ui->listActions->count() > 0){
        Last_Inst = ui->listActions->item(0)->text();
    }

    QString New_Ins = Color+","+Fl+","+Cl;


    if(New_Ins != Last_Inst){

        ui->listActions->insertItem(0,New_Ins);
        GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name("MarioBros");
        WFile->Data->Cnv->Paint_Pixel(Color,ui->spinBoxFila->value(),ui->spinBoxColumna->value());

        WFile->Data->Cnv->Insert_Single_Paint_Instruction(Color,Fl,Cl);
    }
}

void MainWindow::on_btnPushBlock_clicked()
{

    QString Sup_Fl =  QString::number(ui->spinBoxSup_Fl->value());
    QString Sup_Cl =  QString::number(ui->spinBoxSup_Cl->value());

    QString Inf_Fl =  QString::number(ui->spinBoxInf_Fl->value());
    QString Inf_Cl =  QString::number(ui->spinBoxInf_Cl->value());

    QString Color = ui->tboxColor->text();

    QString Last_Inst = "";
    if(ui->listActions->count() > 0){
        Last_Inst = ui->listActions->item(0)->text();
    }

    QString New_Ins = Color+","+Sup_Fl+","+Sup_Cl+","+Inf_Fl+","+Inf_Cl;

    if(New_Ins != Last_Inst){

        ui->listActions->insertItem(0,New_Ins);

        int HP_i = Sup_Fl.toInt();
        int HP_j = Sup_Cl.toInt();

        int LP_i = Inf_Fl.toInt();
        int LP_j = Inf_Cl.toInt();

        GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name("MarioBros");
        WFile->Data->Cnv->Paint_Block(Color,HP_i,HP_j,LP_i,LP_j);
        WFile->Data->Cnv->Insert_Block_Paint_Instruction(Color,Sup_Fl,Sup_Cl,Inf_Fl,Inf_Cl);

    }

}

void MainWindow::on_tboxLoad_clicked()
{
    GeNode<ArchiveHandle*>* Fl = Artist->Get_Archive_By_Name("MarioBros");

    //Fl->Data->Cnv->SetCanvasLimits(-3,4,-9,5);
    Fl->Data->Cnv->SetCanvasLimits(-3,1,-2,4);

    ArrayRange* I = Fl->Data->Cnv->Get_I_Range();
    ArrayRange* J = Fl->Data->Cnv->Get_J_Range();

    Load_Canvas(I,J);

    ui->spinBoxSup_Fl->setValue(-2);
    ui->spinBoxSup_Cl->setValue(-1);

    ui->spinBoxInf_Fl->setValue(-1);
    ui->spinBoxInf_Cl->setValue(1);

    //Artist->Get_Archive_By_Name("MarioBros")->Data->Cnv->SetCanvasLimits(-3,4,-9,5);

    //Artist->Get_Archive_By_Name("MarioBros")->Data->Cnv->I->Sup
    //Load_Canvas(new ArrayRange(-3,4),new ArrayRange(-9,5));
}

void MainWindow::on_btnRender_clicked(){
    GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name("MarioBros");
    WFile->Data->Cnv->Render();
}

void MainWindow::on_btnSaveCanvas_clicked(){
    if(ui->listActions->count() > 0){
        Artist->Get_Archive_By_Name("MarioBros")->Data->Cnv->Build_JSon_Canvas();
    }
}

void MainWindow::on_btnFetch_JSon_clicked(){

    QString Req = "Get_File_Content";
    Req.append("#");

    QString Archive_Name = Artist->Selected_Archive;
    QString JSon_URL = Artist->Get_Archive_By_Name(Archive_Name)->Data->JSon_URL;

    Req.append(JSon_URL);
    Req.append("#");

    socketSend(Req);

}

void MainWindow::on_tboxNombre_textChanged(const QString &arg1)
{
    QString Nm = ui->tboxNombre->text().trimmed();
    ui->tboxURL->setText("JSon_DIR/"+Nm+".json");
}

//void MainWindow::on_pushButton_clicked()
//{
    //Artist->
    //ArrayRange* III = new ArrayRange();
    //ArrayRange* JJJ = new ArrayRange();
    //AskLimits* Ak = new AskLimits(0,III,JJJ);
    //Ak->show();

    //III->SetSize();
    //JJJ->SetSize();
//}

void MainWindow::on_btnAddPermiso_clicked(){

    if(Erasable_NickList->ListSize > 0){
        QString Us = ui->comboxUS_AV->currentText();
        QString Pm = ui->comboxPerm->currentText();

        AddTable_US_Item(0,Us,Pm);

        PList->EndInsert(new MyString(Us,Pm));
        PList->Last->NICK = Pm;

        Erasable_NickList->DeleteNode_By_Nickname(Us);
        Fill_Combox_Users();
    }
}

void MainWindow::on_tblPermisos_itemDoubleClicked(QTableWidgetItem *item){

    int Rw = ui->tblPermisos->currentRow();
    QString Nm = ui->tblPermisos->item(Rw,0)->text();

    Erasable_NickList->EndInsert(new MyString(Nm));
    Erasable_NickList->Last->NICK = Nm;

    PList->DeleteNode_By_Nickname(Nm);

    ui->tblPermisos->removeRow(Rw);
    Fill_Combox_Users();

}

void MainWindow::on_btnArchRegis_clicked(){
    if(ui->tboxNombre->text().trimmed() != ""){

        ArchiveHandle* NewFl = new ArchiveHandle();
        NewFl->Name = ui->tboxNombre->text();
        NewFl->Creator = ui->tboxCreador->text();
        NewFl->Creation_Date = ui->tboxCreado->text();
        NewFl->Modifier_Nickname = ui->tboxModificador->text();
        NewFl->Modification_Date = ui->tboxModificado->text();
        NewFl->Type = ui->comboxTipo->currentText().toLower();
        NewFl->Permiso = ui->comboxPermiso->currentText().toLower();
        NewFl->State = ui->comboxEstado->currentText().toLower();
        NewFl->JSon_URL = ui->tboxURL->text();

        //(^< ............ P E R M I S O S
        NewFl->Permiso_List = this->PList;

        Create_File(NewFl);



        if(ui->comboxTipo->currentText() == "Lienzo"){

        }
        else if(ui->comboxTipo->currentText() == "Documento"){

        }
        else if(ui->comboxTipo->currentText() == "Presentacion"){

        }

        Show_Message("Info","Nuevo Archivo","El Archivo fue Registrado con Exito");
        Archive_Manager_FirstUse();
        ui->btnUpdate->setEnabled(false);
        Update_Archives_Table();

    }
    else{
        Show_Message("Critical","Nuevo Archivo","El Nombre del Nuevo Archivo NO es Valido...");
    }
}

void MainWindow::on_btnDelete_clicked(){
    Delete_Archive(ui->tboxNombre->text());
    Delete_Archive_State();
}

void MainWindow::on_btnChangePermisos_clicked(){
    Update_Permission(ui->tboxNombre->text());
}
