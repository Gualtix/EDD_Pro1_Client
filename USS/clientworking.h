#ifndef CLIENTWORKING_H
#define CLIENTWORKING_H

//C++

//Qt
#include "QString"
#include "QFile"
#include "QTextStream"
#include "QDebug"

//ADT
#include "Canvas/wbutton.h"
#include "genlist.h"
#include "mystring.h"

class DocumentHandle{

public:
    QString JSonContent;
};

class PresentationHandle{

public:
    QString JSonContent;
};

class CanvasHadle{

private:
    ArrayRange* I;
    ArrayRange* J;

public:

    GenList<WButton*>* Btn_List;
    GenList<MyString*>* JSon_Paint_List;

    QString Original_JSonContent;
    QString New_JSonContent;

    CanvasHadle(){
        Btn_List = new GenList<WButton*>();
        JSon_Paint_List = new GenList<MyString*>();

        Original_JSonContent = "";
        New_JSonContent = "";

        I = new ArrayRange();
        J = new ArrayRange();
    }

    ArrayRange* Get_I_Range(){
        return I;
    }

    ArrayRange* Get_J_Range(){
        return J;
    }

    void SetCanvasLimits(int I_Lw,int I_Sp,int J_Lw,int J_Sp){
        I->Sup = I_Sp;
        I->Low = I_Lw;
        I->SetSize();

        J->Sup = J_Sp;
        J->Low = J_Lw;
        J->SetSize();
    }

    void Paint_Pixel(QString Color,int i,int j){

        i = i - I->Low;
        j = j - J->Low;
        int Loc = i * J->Size + j;
        if(Loc >= 0 && Loc < Btn_List->ListSize){
            Btn_List->GetNode(Loc)->Data->SetColor(Color);
        }
    }

    void Paint_Block(QString Color,int HP_i,int HP_j,int LP_i,int LP_j){

        //(^< ............ P I X E L
        if(HP_i == LP_i && HP_j == LP_j){
            Paint_Pixel(Color,HP_i,HP_j);
        }

        //(^< ............ Mas a la Izquierda j
        if(HP_j > LP_j){
            int Tmp = HP_j;
            HP_j = LP_j;
            LP_j = Tmp;
        }

        //(^< ............ Mas hacia Arriba i
        if(HP_i > LP_i){
            int Tmp = HP_i;
            HP_i = LP_i;
            LP_i = Tmp;
        }

        //(^< ............ A J U S T E
        LP_i++;
        LP_j++;

        //(^< ............ M A P E O
        int JLong = LP_j - HP_j;
        int ILong = LP_i - HP_i;

        int Loc = 0;
        while(Loc < (JLong * ILong)){

            int i = (Loc / JLong) + HP_i;
            int j = (Loc % JLong) + HP_j;

            Paint_Pixel(Color,i,j);

            Loc++;
        }
    }


    void Insert_Single_Paint_Instruction(QString Color,QString Fl,QString Cl){
        MyString* Ms = new MyString();
        Ms->Cad = "\t\t{\"color\":\""+Color+"\",\"fila\":"+Fl+",\"columna\":"+Cl+"}";
        JSon_Paint_List->FrontInsert(Ms);
    }

    void Insert_Block_Paint_Instruction(QString Color,QString Sup_Fl,QString Sup_Cl,QString Inf_Fl,QString Inf_Cl){
        MyString* Ms = new MyString();
        Ms->Cad = "\t\t{\"color\":\""+Color+"\",\"fila_inicial\":"+Sup_Fl+",\"columna_inicial\":"+Sup_Cl+",\"fila_final\":"+Inf_Fl+",\"columna_final\":"+Inf_Cl+"}";
        JSon_Paint_List->FrontInsert(Ms);
    }

    void Build_JSon_Canvas(){
        QString NewPaint = "";

        NewPaint.append("{\n");

        NewPaint.append("\t\"lienzo_fila_inf\":"+QString::number(I->Low)+",\n");
        NewPaint.append("\t\"lienzo_fila_sup\":"+QString::number(I->Sup)+",\n");
        NewPaint.append("\t\"lienzo_columna_inf\":"+QString::number(J->Low)+",\n");
        NewPaint.append("\t\"lienzo_columna_sup\":"+QString::number(J->Sup)+",\n");

        NewPaint.append("\t\"lienzo_bloques\":\n\t[\n");

        //(^< ............ Lista de Instrucciones
        int cnt = JSon_Paint_List->ListSize - 1;
        while(cnt >= 0){
            NewPaint.append(JSon_Paint_List->GetNode(cnt)->Data->Cad);

            if(cnt != 0){
                NewPaint.append(",\n");
            }
            else{
                NewPaint.append("\n");
            }
            cnt--;
        }
        NewPaint.append("\t]\n");
        NewPaint.append("}");

        New_JSonContent = NewPaint;

        QString Output_JSon_URL = "JSon_Temp/NewCanvas.json";
        QFile Fl(Output_JSon_URL);
        if (Fl.open(QFile::WriteOnly)) {
            QTextStream stream(&Fl);
            stream << New_JSonContent << endl;
            Fl.close();
        }
    }

    void Render(){


        QFile Fl("Canvas_Render.dot");
        if(Fl.open(QFile::WriteOnly | QFile::Text)){
            QTextStream DotBf(&Fl);
            DotBf<<"digraph Canvas_Render {\n";
            DotBf<<"    node [shape = plaintext]\n";
            DotBf<<"    same_node\n";
            DotBf<<"    [";

            DotBf<<"        label =\n";
            DotBf<<"		<\n";
            DotBf<<"            <table border=\"0\" cellborder=\"1\" cellspacing=\"2\" cellpadding=\"0\">\n";



            //int j = (cnt_0 % this->Get_J_Range()->Size);
            //int i = (cnt_0 / this->Get_J_Range()->Size);

            //(^< ............ M A P E O
            //int JLong = LP_j - HP_j;
            //int ILong = LP_i - HP_i;
            int JLong = J->Size;
            int ILong = I->Size;

            int Loc = 0;

            int Prev_I = 0;
            bool TopSwitch = true;
            bool BottomSwitch = false;

            while(Loc < (JLong * ILong)){

                int i = (Loc / JLong) + I->Low;
                int j = (Loc % JLong) + J->Low;

                //qDebug() << i << j;

                GeNode<WButton*>* Bn = Btn_List->GetNode(Loc);

                //(^< ............ i T I M E S
                if(i != Prev_I && TopSwitch){
                    DotBf<<"				<!-- ............ ............ ............ ............ ............ F I L A: 0 -->\n";
                    DotBf<<"				<tr>\n";

                    Prev_I = i;
                    //if(Prev_I == 0){
                    //    TopSwitch =
                    //}

                    //Prev_I = i;
                    //qDebug() << "Inicio";
                }

                //(^< ............ j T I M E S
                DotBf<<"					<td bgcolor=\"   "+Bn->Data->Current_Color+"  \" width=\"30\" height=\"30\">\n";
                //DotBf<<"						\n";
                DotBf<<"					</td>\n";


                //(^< ............ i T I M E S
                //if(i != Prev_I){
                if(j == J->Sup - 1){
                    DotBf<<"				</tr>\n";
                    DotBf<<"\n";
                    //Prev_I = i;
                    qDebug() << "Fin";
                }


                Loc++;
            }


                DotBf<<"\n";

                //cnt_0++;




            DotBf<<"			</table>\n";
            DotBf<<"		>\n";

            DotBf<<"    ];\n";
            DotBf<<"}";

            Fl.close();

            system("dot -Tpng Canvas_Render.dot -o Render.png");
            system("xdg-open ./Render.png");
            //system("xdg-open ./Canvas_Render.dot");
        }
    }
};

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ClientWorking
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

class ArchiveHandle{

public:
    //(^< ............ Meta
    QString Name;
    QString Type;
    QString Creator;
    QString Creation_Date;
    QString Modification_Date;
    QString Modifier_Nickname;
    QString State;
    QString Permiso;
    QString JSon_URL;
    QString Server_JSonContent;
    QString New_Client_JSonContent;

    GenList<MyString*>* Permiso_List;

    //(^< ............ Types
    //DocumentHandle* Dc;
    //PresentationHandle* Ps;
    CanvasHadle* Cnv;

    //(^< ............ ............ ............ Constructor: Default NULL
    ArchiveHandle(){
        this->Name = "";
        this->Type = "";
        this->Creator = "";
        this->Creation_Date = "";
        this->Modification_Date = "";
        this->Modifier_Nickname = "";
        this->State = "";
        this->Permiso;
        this->JSon_URL = "";
        this->New_Client_JSonContent = "";

        Permiso_List = new GenList<MyString*>();

        //Dc = NULL;
        //Ps = NULL;
        Cnv = new CanvasHadle();
    }
};

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ClientWorking
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


class ClientWorking{


public:

    QString Selected_Archive;

    //(^< ............ Archive List
    GenList<ArchiveHandle*>* ArchList;

    //(^< ............ ............ ............ Constructor: Default NULL
    ClientWorking(){
        ArchList = new GenList<ArchiveHandle*>();
        Selected_Archive = "";
    }

    //(^< ............ ............ ............ Add New Archive
    void Add_New_Archive(ArchiveHandle* Arch){
        ArchList->FrontInsert(Arch);
        ArchList->First->TAG = Arch->Name;
    }

    //(^< ............ ............ ............ Get Archive By Name
    GeNode<ArchiveHandle*>* Get_Archive_By_Name(QString Sl){
        return ArchList->GetNode_By_String(Sl);
    }

    //(^< ............ ............ ............ Delete Archive By Name
    void Delete_Archive_By_Name(){

    }

    //(^< ............ ............ ............ Delete Archive By Name
    void Update_Archive_By_Name(){

    }
};

#endif // CLIENTWORKING_H
