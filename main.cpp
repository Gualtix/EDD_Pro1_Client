//C++

//Qt
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

//ADT
#include "mainwindow.h"
#include "client_log.h"
//#include "Tree/gentree.h"
#include "USS/clientworking.h"

/*
GenList<TNode*>* Get_Doc_Subsection(QJsonArray ARR,int &Dm){

    //Dm++;

    GenList<TNode*>* RList = new GenList<TNode*>();
    //GenList<TNode*>* RList = NULL;

    DocData* SubDocInfo = NULL;
    TNode* SubNode = NULL;

    int SN = ARR.count();
    int cnt = 0;
    while (cnt < SN){

        Dm++;
        if(ARR.at(cnt).isObject()){


            //(^< ............ S U B
            QJsonObject Sub_OBJ = ARR.at(cnt).toObject();

            QString Sb_Title        = Sub_OBJ["titulo"].toString();
            QString Sb_Section_Type = Sub_OBJ["tipo_seccion"].toString();
            QString Sb_Content      = Sub_OBJ["contenido"].toString();

            QJsonArray Sb_ARR = Sub_OBJ["subsecciones"].toArray();

            //(^< ............ Main DocData
            SubDocInfo = new DocData();

            SubDocInfo->ID = Dm;
            SubDocInfo->Titulo = Sb_Title;
            SubDocInfo->Tipo_Seccion = Sb_Section_Type;
            SubDocInfo->Contenido = Sb_Content;

            //(^< ............ Main Node
            SubNode = new TNode();
            SubNode->Info = SubDocInfo;

            //(^< ............ I M A G E
            if(Sb_Section_Type == "imagen"){

            }
            //(^< ............ P A R R A F O
            //else if(Sb_Section_Type == "párrafo"){

            //}
            //(^< ............ V I Ñ E T A S
            else if(Sb_Section_Type == "viñetas"){
                SubNode->TreeList = Get_Doc_Subsection(Sub_OBJ["contenido"].toArray(),Dm);
            }
            else{
                if(Sub_OBJ.count() > 0){
                    //GenList<TNode*>* SBN = Get_Doc_Subsection(Sb_ARR);
                    SubNode->TreeList = Get_Doc_Subsection(Sb_ARR,Dm);

                }
            }
        }

        else{

            SubDocInfo = new DocData();

            SubDocInfo->ID = Dm;
            SubDocInfo->Titulo = "";
            SubDocInfo->Tipo_Seccion = "viñetas";
            SubDocInfo->Contenido = ARR.at(cnt).toString();

            SubNode = new TNode();
            SubNode->Info = SubDocInfo;
            SubNode->TreeList = NULL;

        }

        if(SubNode != NULL){
            RList->EndInsert(SubNode);
        }

        cnt++;
    }

    if(RList->ListSize > 0){
        return RList;
    }
    else{
        return NULL;
    }
}

*/
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //MainWindow* w = new MainWindow();
    //w->show();

    /*
    //MainWindow* w = new MainWindow();
    //w->show();

    //(^< ............ User Load
    QString Input_JSon_URL = "JSon_Temp/Newton.json";

    QFile MyFile(Input_JSon_URL);
    MyFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream out(&MyFile);
    QString JSon_Plain_String = out.readAll();

    MyFile.close();

    //(^< ............ Plain Text to JSonDocument
    QJsonDocument JSon_Fl = QJsonDocument::fromJson(JSon_Plain_String.toUtf8());

    //(^< ............ Tree Load
    if(!JSon_Fl.isEmpty()){

        //(^< ............ Doc Header
        QJsonObject OBJ = JSon_Fl.object();

        QString Title        = OBJ["titulo"].toString();
        QString Section_Type = OBJ["tipo_seccion"].toString();
        QString Content      = OBJ["contenido"].toString();

        //(^< ............ Main DocData
        DocData* Mn = new DocData();

        Mn->ID = 0;
        Mn->Titulo = Title;
        Mn->Tipo_Seccion = Section_Type;
        Mn->Contenido = Content;

        //(^< ............ Main Node
        TNode* Mn_Node = new TNode();
        Mn_Node->Info = Mn;

        QJsonArray ARR = OBJ["subsecciones"].toArray();

        //(^< ............ Main Node Childs
        int Dm = 0;
        GenList<TNode*>* Mn_Childs = Get_Doc_Subsection(ARR,Dm);

        Mn_Node->TreeList = Mn_Childs;

        GenTree* ARB = new GenTree();
        ARB->RT = Mn_Node;
        ARB->Size = Dm + 1;

        //ARB->GraphTree();

        DocumentHandle* KM = new DocumentHandle();
        //QString CD = KM->PNG_to_Base64("IMG_Temp/MK.png");
        //KM->Base64_to_PNG("NoManches",CD);
        KM->Arbolito = ARB;
        //KM->GeneratePDF();
        //KM->Arbolito->SetNode_Levels(KM->Arbolito->RT);
        KM->Arbolito->SetNode_Levels();

        KM->Arbolito->GraphTree();
        KM->GeneratePDF();


        int k = 0;

    }

    */

    Client_Log* CL = new Client_Log();
    CL->show();

    return a.exec();
}


