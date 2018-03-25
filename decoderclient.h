#ifndef DECODERCLIENT_H
#define DECODERCLIENT_H

//C++

//Q

//ADT
#include "client_log.h"
#include "genlist.h"
#include "mystring.h"

class DecoderClient{

public:

    GenList<MyString*>* AnsList;

    DecoderClient(){
        AnsList = new GenList<MyString*>();
    }

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
            else if(Req_Type == "Get_User_Files"){

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

    bool Log_In(){


    }

    bool Sign_In(){

    }


};

#endif // DECODERCLIENT_H
