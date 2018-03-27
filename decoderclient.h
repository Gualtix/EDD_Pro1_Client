#ifndef DECODERCLIENT_H
#define DECODERCLIENT_H

//C++

//Q

//ADT
#include "client_log.h"
#include "mainwindow.h"
#include "genlist.h"
#include "mystring.h"

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
                    while(cnt < Nm){

                        QString Nombre = AnsList->GetNode(1 + (9 * cnt))->Data->Cad;
                        QString Tipo = AnsList->GetNode(2 + (9 * cnt))->Data->Cad;
                        QString Permiso = AnsList->GetNode(0 + (9 * cnt))->Data->Cad;
                        QString Fecha = AnsList->GetNode(4 + (9 * cnt))->Data->Cad;

                        frmApp->AddTableItem(cnt,Nombre,Tipo,Permiso,Fecha);

                        cnt++;
                    }

                }
                else{
                    frmApp->Show_Message("Get User Files","El Usuario no dispone de Archivos");
                }

            }
            else if(Req_Type == "Get_File_Content"){

            }
            else if(Req_Type == "Create_File"){

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
