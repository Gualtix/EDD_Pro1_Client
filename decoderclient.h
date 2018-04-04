#ifndef DECODERCLIENT_H
#define DECODERCLIENT_H

//C++

//Q

//ADT
#include "client_log.h"
#include "mainwindow.h"
#include "genlist.h"
#include "mystring.h"

#include "USS/clientworking.h"

class DecoderClient{

public:

    GenList<MyString*>* AnsList;

    //(^< ............ ............ ............ Constructor: Default
    DecoderClient(){
        AnsList = new GenList<MyString*>();
    }

    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ Tools
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void GetStack_of_Answers(QString msg){

        int cnt = 0;
        QString Tomcat = "";
        QString Ch = "";

        while(cnt < msg.size()){

            Ch = msg.mid(cnt,1);

            if(Ch == "#"){
                AnsList->EndInsert(new MyString(Tomcat));
                Tomcat = "";
            }
            else{
                Tomcat.append(Ch);
            }
            cnt++;
        }
    }

    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ App
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void Answer_Request(MainWindow* frmApp){

        if(AnsList->ListSize > 0){

            QString Req_Type = AnsList->First->Data->Cad;

            if(Req_Type == "Get_User_Files"){

                int Nm = (AnsList->GetNode(1)->Data->Cad.toInt());
                AnsList->DeleteNode(0);
                AnsList->DeleteNode(0);

                if(Nm > 0){

                    int cnt = 0;
                    frmApp->Artist->ArchList->Clear_List();
                    while(cnt < Nm){

                        ArchiveHandle* Ar_in = new ArchiveHandle();

                        Ar_in->Permiso = AnsList->GetNode(0 + (9 * cnt))->Data->Cad;
                        Ar_in->Name = AnsList->GetNode(1 + (9 * cnt))->Data->Cad;
                        Ar_in->Type = AnsList->GetNode(2 + (9 * cnt))->Data->Cad;
                        Ar_in->Creator  = AnsList->GetNode(3 + (9 * cnt))->Data->Cad;
                        Ar_in->Creation_Date = AnsList->GetNode(4 + (9 * cnt))->Data->Cad;
                        Ar_in->State = AnsList->GetNode(5 + (9 * cnt))->Data->Cad;
                        Ar_in->Modifier_Nickname = AnsList->GetNode(6 + (9 * cnt))->Data->Cad;
                        Ar_in->Modification_Date = AnsList->GetNode(7 + (9 * cnt))->Data->Cad;
                        Ar_in->JSon_URL = AnsList->GetNode(8 + (9 * cnt))->Data->Cad;

                        frmApp->Artist->Add_New_Archive(Ar_in);

                        QString Permiso = AnsList->GetNode(0 + (9 * cnt))->Data->Cad;
                        QString Nombre = AnsList->GetNode(1 + (9 * cnt))->Data->Cad;
                        QString Tipo = AnsList->GetNode(2 + (9 * cnt))->Data->Cad;
                        QString Fecha = AnsList->GetNode(4 + (9 * cnt))->Data->Cad;

                        frmApp->AddTableItem(cnt,Nombre,Tipo,Permiso,Fecha);

                        cnt++;
                    }

                }
                else{
                    frmApp->Show_Message("Critical","Get User Files","El Usuario no dispone de Archivos");
                }

            }
            else if(Req_Type == "Get_File_Content"){

                //QString Req_Type = AnsList->GetNode(0)->Data->Cad;
                QString Ans_Type = AnsList->GetNode(1)->Data->Cad;

                if(Ans_Type == "SUCCESS"){
                    frmApp->Artist->Get_Archive_By_Name(frmApp->Artist->Selected_Archive)->Data->Server_JSonContent = AnsList->GetNode(3)->Data->Cad;

                }
                else{
                    frmApp->Show_Message("Critical","Get JSon Content","La Direccion URL NO es Valida");
                }
            }
            else if(Req_Type == "Get_Available_Users"){

                frmApp->NickList->Clear_List();
                frmApp->Erasable_NickList->Clear_List();

                AnsList->DeleteNode(0);
                int cnt = 0;
                while(cnt < AnsList->ListSize){

                    frmApp->NickList->EndInsert(new MyString(AnsList->GetNode(cnt)->Data->Cad));
                    frmApp->NickList->Last->NICK = AnsList->GetNode(cnt)->Data->Cad;

                    frmApp->Erasable_NickList->EndInsert(new MyString(AnsList->GetNode(cnt)->Data->Cad));
                    frmApp->Erasable_NickList->Last->NICK = AnsList->GetNode(cnt)->Data->Cad;

                    cnt++;
                }
                frmApp->Fill_Combox_Users();
            }

            //else if(Req_Type == "Delete_File"){
                //frmApp->Show_Message("Info","Archive Manager","Archivo Eliminado Exitosamente");
            //}

            else if(Req_Type == "Delete_File"){
                //frmApp->Show_Message("Info","Archive Manager","Archivo Eliminado Exitosamente");
                //return;
            }

            else if(Req_Type == "Create_File"){

            }

            else if(Req_Type == "Get_Permission"){
                int cnt = 3;
                int NP = AnsList->GetNode(2)->Data->Cad.toInt();
                frmApp->PList->Clear_List();
                while(cnt < (NP * 2) + 3){
                    QString USR = AnsList->GetNode(cnt)->Data->Cad;
                    QString TYP = AnsList->GetNode(cnt+1)->Data->Cad;


                    if(frmApp->Erasable_NickList->ListSize > 0){
                        //QString Us = ui->comboxUS_AV->currentText();
                        //QString Pm = ui->comboxPerm->currentText();

                        //AddTable_US_Item(0,Us,Pm);

                        frmApp->PList->EndInsert(new MyString(USR,TYP));
                        frmApp->PList->Last->NICK = TYP;

                        frmApp->Erasable_NickList->DeleteNode_By_Nickname(USR);

                    }



                    frmApp->AddTable_US_Item(0,USR,TYP);
                    frmApp->Fill_Combox_Users();
                    cnt++;
                    cnt++;
                }
            }







            else if(Req_Type == "Block_File"){

            }
            else if(Req_Type == "Release_File"){

            }
            else if(Req_Type == "Save_File"){

            }
        }

    }

    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ Login
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void Answer_Request(Client_Log* frmLogin){

        if(AnsList->ListSize > 0){

            QString Req_Type = AnsList->First->Data->Cad;
            QString Ans_Type = AnsList->First->Next->Data->Cad;

            if(Req_Type == "Log_In"){

                if(Ans_Type == "SUCCESS"){
                    frmLogin->Log_In_Req(true);
                    return;
                }

                frmLogin->Log_In_Req(false);
                return;

            }
            else if(Req_Type == "Sign_In"){

                if(Ans_Type == "SUCCESS"){
                    frmLogin->Sign_In_Req(true);
                    return;
                }

                frmLogin->Sign_In_Req(false);
                return;

            }
        }
    }
};

#endif // DECODERCLIENT_H
