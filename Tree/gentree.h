#ifndef GENTREE_H
#define GENTREE_H



//C++

//Qt
#include "genlist.h"
#include "QFile"
#include "QTextStream"
#include "QDebug"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QTreeWidget>

//ADT


class DocData{

public:

    int ID;
    int Papa_ID;
    QString Titulo;
    QString Contenido;
    QString Tipo_Seccion;


    DocData(){
        ID = 0;
        Papa_ID = -1;
        Titulo = "";
        Contenido = "";
        Tipo_Seccion = "";
    }

    ~DocData(){

    }
};

class TNode{

public:
    int Level;
    int Parent_ID;
    DocData* Info;
    GenList<TNode*>* TreeList;

    TNode(){
        Level = -1;
        Parent_ID = -1;
        Info = NULL;
        TreeList = NULL;
    }

    ~TNode(){
        delete Info;
    }

};

class GenTree{

public:

    //Retorno
    TNode* Rs = NULL;

    //Contador
    int Size = 0;

    //Raiz
    TNode* RT;

    //QTree
    QTreeWidget* Tv;

    GenTree(){
        int Size = 0;
        RT =  NULL;
        Tv = NULL;
        Rs = NULL;
    }

    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ A C T I O N S
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

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

    void JSon_Tree_Load(QString JSon_Plain_String){

        //(^< ............ User Load

        //QString Input_JSon_URL = "JSon_Temp/Newton.json";

        //QFile MyFile(Input_JSon_URL);
        //MyFile.open(QIODevice::ReadOnly | QIODevice::Text);

        //QTextStream out(&MyFile);
        //QString JSon_Plain_String = out.readAll();

        //MyFile.close();


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

            //GenTree* ARB = new GenTree();
            this->RT = Mn_Node;
            this->Size = Dm + 1;

            SetNode_Levels();
            SetNode_ID();
        }
    }

    void GraphTree(){

        QString GText = "";

        GText.append("digraph DocTree{\n");
        GText.append("\tnode [shape = \"box\"];\n");

        if(RT != NULL){
            GText.append(LinkString(RT));
        }

        GText.append("}");

        QFile Fl("DocTree.dot");
        if(Fl.open(QFile::WriteOnly | QFile::Text)){
            QTextStream DotBf(&Fl);

            DotBf << GText;

            Fl.close();
            system("xdg-open ./DocTree.dot");
        }
    }

    QString LinkString(TNode* Mn){
        QString Aw = "";

        QString ID_F = QString::number(Mn->Info->ID);
        QString Tl = Mn->Info->Titulo;
        Tl.append(" ("+QString::number(Mn->Level)+") ID: "+ID_F+" P: "+QString::number(Mn->Parent_ID));
        Aw.append("\tN"+ID_F+" [label = \""+Tl+"\"];\n");

        int cnt = 0;
        while(cnt < Mn->TreeList->ListSize){

            TNode* Sub_N = Mn->TreeList->GetNode(cnt)->Data;

            QString ID_C = QString::number(Sub_N->Info->ID);
            if(Sub_N->Info->Tipo_Seccion == "viñetas"){
                Tl = Sub_N->Info->Contenido;
            }
            else{
                Tl = Sub_N->Info->Titulo;
            }

            Tl.append(" ("+QString::number(Sub_N->Level)+") ID: "+ID_C+" P: "+QString::number(Sub_N->Parent_ID));

            Aw.append("\tN"+ID_C+" [label =  \""+Tl+"\"];\n");
            Aw.append("\tN"+ID_F+" -> N"+ID_C+";\n");

            if(Sub_N->TreeList > 0){
                Aw.append(LinkString(Sub_N));
            }

            cnt++;
        }
        return Aw;
    }



    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ C R U D
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void TopLevel_ChildInsert(TNode* Child){
        if(RT != NULL){
            RT->TreeList->EndInsert(Child);
        }
        else{
            RT = Child;
            if(Child->TreeList == NULL){
                RT->TreeList = new GenList<TNode*>();
            }
        }
        SetNode_ID();
        SetNode_Levels();
    }

    TNode* GetNodeBy_ID(int ID){

        if(RT != NULL){
            if(ID == RT->Info->ID){
                return RT;
            }

            if(RT->TreeList->ListSize > 0){
                Rs = NULL;
                return GetNodeBy_ID(ID,RT);
            }
        }
    }

    TNode* GetNodeBy_ID(int ID,TNode* Sub){
        int cnt = 0;
        //TNode* Rs = NULL;
        while(cnt < Sub->TreeList->ListSize){

            TNode* Tmp = Sub->TreeList->GetNode(cnt)->Data;
            int Sh = Tmp->Info->ID;

            if(ID == Sh){
                Rs = Tmp;
                return Rs;
            }

            if(Tmp->TreeList != NULL){
                Rs = GetNodeBy_ID(ID,Tmp);
            }

            cnt++;
        }

        return Rs;
    }

    void DeleteNodeBy_ID(int N_ID){

        if(N_ID > 0){

            TNode* RqNode = GetNodeBy_ID(N_ID);
            if(RqNode->TreeList != NULL){
                RqNode->TreeList->Clear_List();
            }

            int P_ID = RqNode->Parent_ID;
            TNode* P_Node = GetNodeBy_ID(P_ID);

            int cnt = 0;
            //int It = 0;
            while(cnt < P_Node->TreeList->ListSize){
                int CM = P_Node->TreeList->GetNode(cnt)->Data->Info->ID;
                if(CM = N_ID){
                    break;
                }
                cnt++;
            }

            cnt = (P_Node->TreeList->ListSize - 1) - cnt;
            P_Node->TreeList->DeleteNode(cnt);

        }
        else if(N_ID == 0){
            if(RT->TreeList != NULL){
                RT->TreeList->Clear_List();
            }

            delete RT;
            RT = NULL;
        }
    }
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ ID Handle
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void SetNode_ID(){
        if(RT != NULL){
            RT->Info->ID = 0;
            RT->Info->Papa_ID = -1;
            RT->Parent_ID = -1;

            int ID = 0;
            int ID_P = 0;
            SetNode_ID(ID,RT,ID_P);
        }
    }

    void SetNode_ID(int &ID,TNode* Sub,int ID_P){

        int cnt = 0;
        TNode* Tmp;
        while(cnt < Sub->TreeList->ListSize){
            ID++;
            Tmp = Sub->TreeList->GetNode(cnt)->Data;
            Tmp->Info->ID =  ID;

            Tmp->Parent_ID = ID_P;
            Tmp->Info->Papa_ID = ID_P;

            if(Tmp->TreeList != NULL){
                int TI = ID_P;
                ID_P = ID;
                SetNode_ID(ID,Tmp,ID_P);
                ID_P = TI;
            }
            cnt++;
        }

    }

    void SetNode_Levels(){

        GenList<TNode*>* Qe = new GenList<TNode*>();
        RT->Level = 0;
        if(RT->TreeList != NULL){
            if(RT->TreeList->ListSize > 0){
                SetNode_Levels(RT->TreeList->First);
            }
        }
    }

    void SetNode_Levels(GeNode<TNode*>* Ps){

        int Lv = 1;

        GenList<GeNode<TNode*>*>* Qe = new GenList <GeNode<TNode*>*>();

        GeNode<TNode*>* TMP = Ps;
        Ps->Data->Level = Lv;
        Qe->EndInsert(Ps);
        //Atender la Cola
        while(Qe->ListSize > 0){
            TMP = Qe->First->Data;
            if(TMP->Next != NULL){
                Qe->EndInsert(TMP->Next);
                Qe->Last->Data->Data->Level = Lv;
            }

            if(TMP->Data->TreeList != NULL){
                if(TMP->Data->TreeList->ListSize > 0){
                    Qe->EndInsert(TMP->Data->TreeList->First);
                    Qe->Last->Data->Data->Level = Lv+1;
                    Lv++;
                }
            }

            Qe->DeleteNode(0);

        }
    }

    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
    //(^< ............ ............ ............ ............ ............ QTreeView Handle
    //(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

    void Set_TreeView(QTreeWidget* Tv){
        this->Tv = Tv;
    }

    void UpdateTreeView(){

        Tv->clear();

        if(RT != NULL){
            QTreeWidgetItem* Main_Item = new QTreeWidgetItem(Tv);

            QString Tp = RT->Info->Tipo_Seccion;

            QString TG = "";

            if(Tp == "viñetas"){
                TG = "Vin";
            }

            if(Tp == "párrafo"){
                TG = "Par";
            }

            if(Tp == "imagen"){
                TG = "Img";
            }


            Main_Item->setText(0,TG+": "+RT->Info->Titulo);
            Main_Item->setText(1,QString::number(RT->Info->ID));

            Tv->addTopLevelItem(Main_Item);

            if(RT->TreeList->ListSize > 0){
                UpdateTreeView(Main_Item,RT);
            }
        }
    }

    void UpdateTreeView(QTreeWidgetItem* Parent,TNode* Sub){

        int cnt = 0;
        TNode* Tmp;
        QTreeWidgetItem* itm;
        while(cnt < Sub->TreeList->ListSize){
            Tmp = Sub->TreeList->GetNode(cnt)->Data;

            QString IDS = QString::number(Tmp->Info->ID);
            QString Tp = Tmp->Info->Tipo_Seccion;
            QString Tl = Tmp->Info->Titulo;
            QString Cn = Tmp->Info->Contenido;

            QString TG = "";

            if(Tp == "viñetas"){
                TG = "Vin: ";
            }

            if(Tp == "párrafo"){
                TG = "Par: ";
            }

            if(Tp == "imagen"){
                //TG = "Img";
            }


            if(Tp == "viñetas" && Tl == ""){
                itm = new QTreeWidgetItem();
                itm->setText(1,IDS);
                itm->setText(0,"♦ "+Cn);
                Parent->addChild(itm);
                //qDebug() << Cn;
            }
            else if(Tp == "viñetas"){
                itm = new QTreeWidgetItem();
                itm->setText(1,IDS);
                //itm->setText(0,TG+": "+Tl);
                itm->setText(0,TG+""+Tl);
                Parent->addChild(itm);
                //qDebug() << Tp;
            }
            else{

                itm = new QTreeWidgetItem();
                itm->setText(1,IDS);
                //itm->setText(0,TG+": "+Tl);
                itm->setText(0,TG+""+Tl);
                Parent->addChild(itm);
                //qDebug() << Tl;
            }

            if(Tmp->TreeList != NULL){

                UpdateTreeView(itm,Tmp);
            }
            cnt++;
        }
    }

    void AddRoot(QString Name){
        QTreeWidgetItem* itm = new QTreeWidgetItem(Tv);
        itm->setText(0,Name);
        Tv->addTopLevelItem(itm);
    }

    void AddChild(QTreeWidgetItem* Parent,QString Name){
        QTreeWidgetItem* itm = new QTreeWidgetItem();
        itm->setText(0,Name);
        Parent->addChild(itm);
    }
};



#endif // GENTREE_H
