//C++

//Qt
#include "QString"
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QMessageBox>
#include <QDate>
#include <QMessageBox>
#include <QFileDialog>

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

    ui->tabManager->setTabEnabled(3,false);
    ui->tabManager->setTabEnabled(2,false);
    ui->tabManager->setTabEnabled(1,false);
    ui->tabManager->setTabEnabled(0,true);
    ui->tabManager->setCurrentIndex(0);

    Archive_Manager_FirstUse();
    DocEdit_FirstUse();
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

void MainWindow::DocEdit_FirstUse(){

    ui->tvDocTree->header()->resizeSection(0,200);

    //(^< ............ ............ ............ DocEmulator
    //Doc_Emu = new DocumentHandle();
    //Doc_Emu->Arbolito->Set_TreeView(ui->tvDocTree);

    ui->btnADD->setEnabled(true);
    ui->btnUP->setEnabled(false);
    ui->btnDEL->setEnabled(false);

    ui->btnAddSubsection->setEnabled(false);
    ui->btnDeleteNode->setEnabled(false);

    Parragraph_State();
}

void MainWindow::Parragraph_State(){

    Clean_DocEdit();

    ui->btnPR->setEnabled(false);
    ui->btnIMG->setEnabled(true);
    ui->btnVIN->setEnabled(true);
    ui->tboxTitle->setEnabled(true);
    ui->tboxContent->setEnabled(true);
    ui->tboxContent->setReadOnly(false);
    ui->listboxVin->setEnabled(false);
    ui->tboxVin->setEnabled(false);
    ui->tboxIMG->setEnabled(false);
    ui->btnDirIMG->setEnabled(false);
}

void MainWindow::Image_State(){

    Clean_DocEdit();

    ui->btnPR->setEnabled(true);
    ui->btnIMG->setEnabled(false);
    ui->btnVIN->setEnabled(true);
    ui->tboxTitle->setEnabled(true);
    ui->tboxContent->setEnabled(true);
    ui->tboxContent->setReadOnly(true);
    ui->listboxVin->setEnabled(false);
    ui->tboxVin->setEnabled(false);
    ui->tboxIMG->setEnabled(true);
    ui->btnDirIMG->setEnabled(true);

}

void MainWindow::Vinieta_State(){

    Clean_DocEdit();

    ui->btnPR->setEnabled(true);
    ui->btnIMG->setEnabled(true);
    ui->btnVIN->setEnabled(false);
    ui->tboxTitle->setEnabled(true);
    ui->tboxContent->setEnabled(false);
    ui->listboxVin->setEnabled(true);
    ui->tboxVin->setEnabled(true);
    ui->tboxIMG->setEnabled(false);
    ui->btnDirIMG->setEnabled(false);
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

void MainWindow::Clean_DocEdit(){
    //ui->tvDocTree->clear();
    ui->listboxVin->clear();

    ui->tboxTitle->setText("");
    ui->tboxContent->setText("");
    ui->tboxIMG->setText("");
    ui->tboxVin->setText("");
    ui->tboxJSonDocEdit->setText("");



}

void MainWindow::Clean_Canvas(){
    QString FlName = ui->tboxNombre->text();
    CanvasHadle * Fl = Artist->Get_Archive_By_Name(FlName)->Data->Cnv;

    Artist->Get_Archive_By_Name(FlName)->Data->Cnv->Remove_WButtons();
    Artist->Get_Archive_By_Name(FlName)->Data->Cnv->Btn_List->Clear_List();
    foreach (QObject* OB, ui->gboxCanvas_Preview->children()) {
        WButton* Bt = (WButton*)OB;
        delete Bt;
    }
    ui->listActions->clear();

}
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Document
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void MainWindow::Load_Doc(){
    //QString FlName = ui->tboxNombre->text();
    //GeNode<ArchiveHandle*>* Fl = Artist->Get_Archive_By_Name(FlName);

}

void MainWindow::StartDoc(bool FirstDoc,QString Order){
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    Doc_Emu->Arbolito->Set_TreeView(ui->tvDocTree);
    QString JS_Content = Doc_Emu->Original_JSonContent;
    SelectedNode = NULL;

    if(FirstDoc){
        ui->tabManager->setTabEnabled(0,false);
        ui->tabManager->setTabEnabled(1,true);
        ui->tabManager->setCurrentIndex(1);
    }
    else{
        Doc_Emu->Arbolito->JSon_Tree_Load(JS_Content);

        if(Order != "View"){
            ui->tabManager->setTabEnabled(0,false);
            ui->tabManager->setTabEnabled(1,true);
            ui->tabManager->setCurrentIndex(1);
            Doc_Emu->Arbolito->UpdateTreeView();
        }
        else{
            Doc_Emu->GeneratePDF();
            Doc_Emu->Arbolito->GraphTree();
        }
    }
}



//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Presentation
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::StartPresentation(bool FistPres){

}
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ Canvas
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::Load_Canvas(ArrayRange* I,ArrayRange* J,bool FirstDraw){

    QString FlName = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* Fl = Artist->Get_Archive_By_Name(FlName);

    if(FirstDraw){
        ArrayRange* Itt = new ArrayRange(I->Low,I->Sup);
        ArrayRange* Jtt = new ArrayRange(J->Low,J->Sup);

        Itt->SetSize();
        Jtt->SetSize();

        I = Itt;
        J = Jtt;

        //QString FlName = ui->tboxNombre->text();
        //GeNode<ArchiveHandle*>* Fl = Artist->Get_Archive_By_Name(FlName);
        Fl->Data->Cnv->SetCanvasLimits(I->Low,I->Sup,J->Low,J->Sup);
    }
    else{

        I = Fl->Data->Cnv->I;
        J = Fl->Data->Cnv->J;

        I->SetSize();
        J->SetSize();
    }



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
        Artist->Get_Archive_By_Name(FlName)->Data->Cnv->Btn_List->EndInsert(MyButton);


        Loc++;
    }
}

void MainWindow::Pick_Color(){
    WButton *Bt = (WButton *)sender();
    ui->tboxColor->setText(Bt->Current_Color);
}

void MainWindow::StartCanvas(bool FirstDraw,QString Order){
    if(FirstDraw){


        QString Fl_Name = ui->tboxNombre->text();
        AskLimits* Ak = new AskLimits(0,this,Fl_Name);

        ui->tabManager->setTabEnabled(3, false);
        ui->tabManager->setTabEnabled(0,false);
        ui->tabManager->setCurrentIndex(3);
        bool Rs = Ak->exec();

        if(Rs == QDialog::Accepted){
            ui->tabManager->setTabEnabled(3,true);
        }
        else{
            ui->tabManager->setTabEnabled(0,true);
            ui->tabManager->setCurrentIndex(0);
        }


    }
    else{

        QString FlName = ui->tboxNombre->text();
        Artist->Get_Archive_By_Name(FlName)->Data->Cnv->LoadFrom_JSon(false);
        Load_Canvas(NULL,NULL,false);
        Artist->Get_Archive_By_Name(FlName)->Data->Cnv->LoadFrom_JSon(true);

        if(Order != "View"){
            ui->tabManager->setTabEnabled(0,false);
            ui->tabManager->setTabEnabled(3,true);
            ui->tabManager->setCurrentIndex(3);
        }
        else{
            QString FlName = ui->tboxNombre->text();
            GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name(FlName);
            WFile->Data->Cnv->Render();
            Clean_Canvas();
        }
    }
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

void MainWindow::Get_File_Content(QString Archive_Name,QString JSon_URL,QString Order,QString Type){

    QString Reqest = "Get_File_Content";
    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Archive_Name);
    Reqest.append("#");

    Reqest.append(JSon_URL);
    Reqest.append("#");

    Reqest.append(Order);
    Reqest.append("#");

    Reqest.append(Type);
    Reqest.append("#");

    socketSend(Reqest);

}

void MainWindow::Save_File(QString Archive_Name,QString JSon_URL,QString JSon_Content){
    QString Reqest = "Save_File";

    Reqest.append("#");

    Reqest.append(User_Session::Nickname);
    Reqest.append("#");

    Reqest.append(Archive_Name);
    Reqest.append("#");

    Reqest.append(JSon_URL);
    Reqest.append("#");

    Reqest.append(JSon_Content);
    Reqest.append("#");

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

void MainWindow::Load_Editor(QString Cs,QString Order,QString Type){
    QString Tp = ui->comboxTipo->currentText();

    //(^< ............ ............ ............ E M P T Y
    if(Cs == "Empty"){
        Show_Message("Info","Carga de Contenido JSon","Nuevo Archivo Cargado Correctamente");

        if(Tp == "Lienzo"){
            if(Order == "View"){
                Show_Message("Info","Carga de Contenido JSon","El Lienzo esta Vacio, nada que mostrar...");
            }
            else{
                StartCanvas(true,Order);
            }
        }
        else if(Tp == "Documento"){
            if(Order == "View"){
                Show_Message("Info","Carga de Contenido JSon","El Documento esta Vacio, sin PDF que mostrar...");
            }
            else{
                StartDoc(true,Order);
            }

        }
        else{
            if(Order == "View"){
                Show_Message("Info","Carga de Contenido JSon","La Presentacion esta Vacia, sin PDF que mostrar...");
            }
            else{
                StartPresentation(true);
            }
        }
    }

    //(^< ............ ............ ............ E R R O R : 4 0 4
    else if(Cs == "ERROR"){
        Show_Message("Critical","Carga de Contenido JSon","El Archivo NO fue Encontrado");
    }

    //(^< ............ ............ ............ S U C C E S S
    else if(Cs == "SUCCESS"){
        Show_Message("Info","Carga de Contenido JSon","Archivo Cargado Correctamente");

        if(Tp == "Lienzo"){
            StartCanvas(false,Order);
        }
        else if(Tp == "Documento"){
            StartDoc(false,Order);
        }
        else{
            StartPresentation(false);
        }
    }
}





//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ui Events
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void MainWindow::on_btnUpdate_clicked(){

    Update_Archives_Table();
    ui->btnUpdate->setEnabled(false);
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

    QString FlName = ui->tboxNombre->text();
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
        GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name(FlName);
        WFile->Data->Cnv->Paint_Pixel(Color,ui->spinBoxFila->value(),ui->spinBoxColumna->value());

        WFile->Data->Cnv->Insert_Single_Paint_Instruction(Color,Fl,Cl);
    }
}

void MainWindow::on_btnPushBlock_clicked()
{

    QString FlName = ui->tboxNombre->text();

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

        GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name(FlName);
        WFile->Data->Cnv->Paint_Block(Color,HP_i,HP_j,LP_i,LP_j);
        WFile->Data->Cnv->Insert_Block_Paint_Instruction(Color,Sup_Fl,Sup_Cl,Inf_Fl,Inf_Cl);

    }

}

void MainWindow::on_tboxLoad_clicked()
{
    QString Nm = ui->tboxNombre->text();
    //Artist->Get_Archive_By_Name(Nm)->Data->Cnv->LoadFrom_JSon();
}

void MainWindow::on_btnRender_clicked(){
    QString FlName = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* WFile = Artist->Get_Archive_By_Name(FlName);
    WFile->Data->Cnv->Render();
}

void MainWindow::on_btnSaveCanvas_clicked(){
    Show_Message("Info","L I E N Z O","Archivo Guardado Exitosamente");
    QString FlName = ui->tboxNombre->text();
    Artist->Get_Archive_By_Name(FlName)->Data->Cnv->Build_JSon_Canvas();
    QString JsUrl = ui->tboxURL->text();
    QString JsContent = Artist->Get_Archive_By_Name(FlName)->Data->Cnv->New_JSonContent;
    Save_File(FlName,JsUrl,JsContent);

    Clean_Canvas();

    ui->tabManager->setTabEnabled(0,true);
    ui->tabManager->setTabEnabled(3,false);
    ui->tabManager->setCurrentIndex(0);
    Archive_Manager_FirstUse();


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

void MainWindow::on_btnIMG_clicked()
{
    Image_State();
}

void MainWindow::on_btnPR_clicked()
{
    Parragraph_State();
}

void MainWindow::on_btnVIN_clicked()
{
    Vinieta_State();
}

void MainWindow::on_tboxVin_returnPressed()
{
    if(ui->tboxVin->text().trimmed() != ""){
        QString Vn = ui->tboxVin->text();
        ui->listboxVin->insertItem(0,Vn);
        ui->tboxVin->setText("");
    }
}

void MainWindow::on_listboxVin_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listboxVin->removeItemWidget(item);
    delete item;
}

void MainWindow::on_btnDirIMG_clicked()
{
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    QString Fl_Name = QFileDialog::getOpenFileName(NULL, tr("I M A G E   P I C K E R"));

    if(Fl_Name.isEmpty())
    return;

    QFile Fl(Fl_Name);

    if (!Fl.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray Fl_Base = Fl.readAll();

    ui->tboxIMG->setText(Fl_Name);
    QString IMG_64 = Doc_Emu->PNG_to_Base64(Fl_Name);
    ui->tboxContent->setText(IMG_64);

}

void MainWindow::on_btnADD_clicked()
{

    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    QString Titulo = ui->tboxTitle->text();
    QString Contenido = ui->tboxContent->toPlainText();
    QString Fl_Path = ui->tboxIMG->text();

    bool Up = false;

    //(^< ............ ............ ............ V I N
    int Nv = ui->listboxVin->count();
    QStringList* Vn = new QStringList();
    if(Nv > 0){
        int cnt = 0;
        while(cnt < Nv){
            QListWidgetItem* itm = ui->listboxVin->item(cnt);
            Vn->push_back(itm->text());
            cnt++;
        }
    }

    DocData* Dt = new DocData();
    TNode* Tmp = new TNode();

    //(^< ............ ............ ............ P A R R A F O
    if(!ui->btnPR->isEnabled()){

        if(ui->tboxTitle->text().trimmed() != "" && ui->tboxContent->toPlainText().trimmed() != ""){
            Dt->Titulo = Titulo;
            Dt->Contenido = Contenido;
            Dt->Tipo_Seccion = "párrafo";

            TNode* Tmp = new TNode();
            Tmp->Info = Dt;

            Dt->Tipo_Seccion = "párrafo";
            //Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
            if(SelectedNode != NULL){
                if(SelectedNode->TreeList != NULL){
                    SelectedNode->TreeList->EndInsert(Tmp);
                }else{
                    SelectedNode->TreeList = new GenList<TNode*>();
                    SelectedNode->TreeList->EndInsert(Tmp);
                }
            }
            else{
                Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
            }
            Up = true;
        }
    }

    //(^< ............ ............ ............ I M A G E N
    if(!ui->btnIMG->isEnabled()){

        Dt->Titulo = "Imagen: ";
        Dt->Titulo.append(Titulo);
        Dt->Tipo_Seccion = "imagen";
        Dt->Contenido = Contenido;

        Tmp->Info = Dt;

        /*
        if(SelectedNode != NULL){
            if(SelectedNode->TreeList != NULL){
                SelectedNode->TreeList->EndInsert(Tmp);
            }else{
                SelectedNode->TreeList = new GenList<TNode*>();
                SelectedNode->TreeList->EndInsert(Tmp);
            }
        }
        else{
            Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
        }
        */
        //Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
        if(SelectedNode != NULL && SelectedNode->Info->Tipo_Seccion == "párrafo"){
            if(SelectedNode->TreeList != NULL){
                SelectedNode->TreeList->EndInsert(Tmp);
            }else{
                SelectedNode->TreeList = new GenList<TNode*>();
                SelectedNode->TreeList->EndInsert(Tmp);
            }
        }
        else{
            Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
        }


        Up = true;

    }

    //(^< ............ ............ ............ V I N I E T A S
    if(!ui->btnVIN->isEnabled()){

        if(ui->tboxTitle->text().trimmed() != "" && ui->listboxVin->count() > 0){
            Dt->Tipo_Seccion = "viñetas";

            int cnt = 0;
            int Sz = Vn->size();

            if(Sz > 0){
                Tmp->TreeList = new GenList<TNode*>();
            }

            while(cnt < Sz){
                DocData* St = new DocData();
                St->Titulo = "";
                St->Tipo_Seccion = "viñetas";
                St->Contenido = Vn->at(cnt);

                TNode* NVin = new TNode();
                NVin->Info = St;
                NVin->TreeList = NULL;

                Tmp->TreeList->EndInsert(NVin);
                cnt++;
            }

            Dt->Titulo = Titulo;
            Dt->Tipo_Seccion = "viñetas";
            Dt->Contenido = "";

            Tmp->Info = Dt;

            if(SelectedNode != NULL && SelectedNode->Info->Tipo_Seccion == "párrafo"){
                if(SelectedNode->TreeList != NULL){
                    SelectedNode->TreeList->EndInsert(Tmp);
                }else{
                    SelectedNode->TreeList = new GenList<TNode*>();
                    SelectedNode->TreeList->EndInsert(Tmp);
                }
            }
            else{
                Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
            }
            //Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
            /*
            if(SelectedNode != NULL){
                SelectedNode->TreeList->EndInsert(Tmp);
            }
            else{
                Doc_Emu->Arbolito->TopLevel_ChildInsert(Tmp);
            }
            */

            Up = true;
        }

    }

    if(Up){
        Clean_DocEdit();
        SelectedNode = NULL;
        Doc_Emu->Arbolito->SetNode_ID();
        Doc_Emu->Arbolito->SetNode_Levels();
        Doc_Emu->Arbolito->UpdateTreeView();
        ui->tvDocTree->expandAll();
    }
}

void MainWindow::on_btnPDF_clicked()
{
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    if(Doc_Emu->Arbolito->RT != NULL){
        Doc_Emu->GeneratePDF();
    }
}

void MainWindow::on_btnTreeIMG_clicked()
{
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    if(Doc_Emu->Arbolito->RT != NULL){
        Doc_Emu->Arbolito->GraphTree();
    }
}

void MainWindow::on_tvDocTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

    Clean_DocEdit();

    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    bool Ps = false;

    QString T_ID = item->text(1);
    TNode* Rw = Doc_Emu->Arbolito->GetNodeBy_ID(T_ID.toInt());
    SelectedNode = Rw;

    QString Titulo = Rw->Info->Titulo;
    QString Tipo_Seccion = Rw->Info->Tipo_Seccion;
    QString Contenido = Rw->Info->Contenido;

    if(Tipo_Seccion == "párrafo"){
        //Clean_DocEdit();
        Parragraph_State();
        ui->tboxTitle->setText(Titulo);
        ui->tboxContent->setText(Contenido);
        Ps = true;
    }
    else if(Tipo_Seccion == "imagen"){
        //
        Clean_DocEdit();
        Image_State();
        Titulo.remove(0,8);
        ui->tboxTitle->setText(Titulo);
        ui->tboxContent->setText(Contenido);
        ui->tboxContent->setReadOnly(true);
        Ps = true;
    }
    else if(Titulo == "" && Tipo_Seccion == "viñetas"){
        Clean_DocEdit();
        ui->tboxTitle->setText(Contenido);
        Ps = true;
    }
    else{
        Clean_DocEdit();
        ui->tboxTitle->setText(Titulo);
        Ps = true;
    }


    if(Ps){

        if(SelectedNode->Info->Tipo_Seccion != "párrafo"){
            ui->btnAddSubsection->setEnabled(false);
        }
        else{
            ui->btnAddSubsection->setEnabled(true);
        }


        ui->btnDeleteNode->setEnabled(true);

        ui->btnADD->setEnabled(false);
        ui->btnUP->setEnabled(true);
    }
}

void MainWindow::on_btnUseEditor_clicked()
{
    QString Fln = ui->tboxNombre->text();
    QString Js_Url = ui->tboxURL->text();
    QString Tp = ui->comboxTipo->currentText();
    Get_File_Content(Fln,Js_Url,"Edit",Tp);
}

void MainWindow::on_btnViewPDF_clicked()
{
    QString Fln = ui->tboxNombre->text();
    QString Js_Url = ui->tboxURL->text();
    QString Tp = ui->comboxTipo->currentText();
    Get_File_Content(Fln,Js_Url,"View",Tp);
}

void MainWindow::on_tboxColor_returnPressed()
{

}

void MainWindow::on_btnUP_clicked()
{
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    QString Titulo = SelectedNode->Info->Titulo;
    QString Tipo_Seccion = SelectedNode->Info->Tipo_Seccion;
    QString Contenido = SelectedNode->Info->Contenido;


    if(Tipo_Seccion == "párrafo"){
        SelectedNode->Info->Titulo = ui->tboxTitle->text();
        SelectedNode->Info->Contenido = ui->tboxContent->toPlainText();
    }

    if(Tipo_Seccion == "imagen"){
        QString IMG_Title = "Imagen: ";
        IMG_Title.append(ui->tboxTitle->text());
        SelectedNode->Info->Titulo = IMG_Title;
        SelectedNode->Info->Contenido = ui->tboxContent->toPlainText();
    }

    if(Titulo == "" && Tipo_Seccion == "viñetas"){
        SelectedNode->Info->Contenido = ui->tboxTitle->text();
    }

    if(Titulo != "" && Tipo_Seccion == "viñetas"){
        SelectedNode->Info->Titulo = ui->tboxTitle->text();
    }

    DocEdit_FirstUse();
    //Doc_Emu->Arbolito->Tv->repaint();
    Doc_Emu->Arbolito->UpdateTreeView();
    Doc_Emu->Arbolito->Tv->expandAll();
}

void MainWindow::on_btnDeleteNode_clicked()
{
    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    int ID_Selected = SelectedNode->Info->ID;
    Doc_Emu->Arbolito->DeleteNodeBy_ID(ID_Selected);

    Doc_Emu->Arbolito->UpdateTreeView();
    Doc_Emu->Arbolito->Tv->expandAll();
    DocEdit_FirstUse();

}

void MainWindow::on_btnAddSubsection_clicked(){

    if(ui->tvDocTree->currentItem() != NULL && SelectedNode != NULL){
        ui->btnAddSubsection->setEnabled(false);
        Parragraph_State();
        ui->btnADD->setEnabled(true);
        ui->btnUP->setEnabled(false);
    }
}

void MainWindow::on_btnSaveDoc_clicked()
{
    Show_Message("Info","D O C U M E N T O","Archivo Guardado Exitosamente");

    QString FlName_Dc = ui->tboxNombre->text();
    GeNode<ArchiveHandle*>* FI_l = Artist->Get_Archive_By_Name(FlName_Dc);
    DocumentHandle* Doc_Emu = FI_l->Data->Dc;

    Doc_Emu->Build_JSon_Document();

    QString JsUrl = ui->tboxURL->text();
    QString JsContent = Doc_Emu->New_JSonContent;
    ui->tboxJSonDocEdit->setText(JsContent);


    Save_File(FlName_Dc,JsUrl,JsContent);


    Clean_DocEdit();

    ui->tabManager->setTabEnabled(0,true);
    ui->tabManager->setTabEnabled(1,false);
    ui->tabManager->setCurrentIndex(0);
    Archive_Manager_FirstUse();


}

void MainWindow::on_btnDEL_clicked()
{

}

void MainWindow::on_actionLog_Out_triggered()
{
    //QString Sl = "Log_Out";
    //Sl.append("#");
    //socketSend(Sl);

    Client_Log* Cl = new Client_Log();
    Cl->show();
    this->hide();

}
