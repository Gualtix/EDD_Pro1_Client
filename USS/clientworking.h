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
#include "Tree/gentree.h"
#include "QLabel"
#include "QDebug"

#include <QPainter>
#include <QPrinter>

class DocumentHandle{

public:
    QString Original_JSonContent;
    QString New_JSonContent;

    GenTree* Arbolito;

    DocumentHandle(){
        Arbolito = new GenTree();
    }

    void Build_JSon_Document(){
        QString Js = "";

        if(Arbolito->RT != NULL){
            Js.append("{\n");

            Js.append("\t\"titulo\": \""+Arbolito->RT->Info->Titulo+"\",\n");
            Js.append("\t\"tipo_seccion\":\""+Arbolito->RT->Info->Tipo_Seccion+"\",\n");
            Js.append("\t\"contenido\": \""+Arbolito->RT->Info->Contenido+"\",\n");
            Js.append("\t\"subsecciones\":\n");
            //Js.append("\t[\n");

            bool Has_Sub = false;
            if(Arbolito->RT->TreeList != NULL){
                if(Arbolito->RT->TreeList->ListSize > 0){
                    Has_Sub = true;
                }
            }

            if(Has_Sub){
                Build_JSon_Document(Arbolito->RT,Js,true);
            }

            //Js.append("\t]\n");
            Js.append("}");
        }
        else{
            New_JSonContent = "Empty";
        }

        New_JSonContent = Js;
    }

    void Build_JSon_Document(TNode* Parent,QString &Js,bool Has_Sub){

        if(Has_Sub){
            setIdent(Parent->Level + 1,Js);
            Js.append("[\n");
        }

        int cnt = 0;
        while(cnt < Parent->TreeList->ListSize){

            TNode* Tmp = Parent->TreeList->GetNode(cnt)->Data;

            QString IDS = QString::number(Tmp->Info->ID);
            QString Tp = Tmp->Info->Tipo_Seccion;
            QString Tl = Tmp->Info->Titulo;
            QString Cn = Tmp->Info->Contenido;
            //QString Level = Tmp->Level;
            //bool Has_Sub = false;

            //(^< ............ ............ ............ P A R R A F O
            if(Tp == "párrafo"){

                setIdent(Tmp->Level,Js);
                Js.append("{\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"titulo\": \""+Tmp->Info->Titulo+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"tipo_seccion\":\""+Tmp->Info->Tipo_Seccion+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"contenido\": \""+Tmp->Info->Contenido+"\"\n");

                if(Tmp->TreeList != NULL){
                    if(Tmp->TreeList->ListSize > 0){
                        Js.chop(1);
                        Js.append(",\n");
                        setIdent(Tmp->Level + 1,Js);
                        Js.append("\"subsecciones\":\n");

                    }
                }
            }

            //(^< ............ ............ ............ I M A G E N
            if(Tp == "imagen"){

                setIdent(Tmp->Level,Js);
                Js.append("{\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"titulo\": \""+Tmp->Info->Titulo+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"tipo_seccion\":\""+Tmp->Info->Tipo_Seccion+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"contenido\": \""+Tmp->Info->Contenido+"\"\n");

            }
            //(^< ............ ............ ............ V I N I E T A
            if(Tp == "viñetas" && Tl != ""){

                setIdent(Tmp->Level,Js);
                Js.append("{\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"titulo\": \""+Tmp->Info->Titulo+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"tipo_seccion\":\""+Tmp->Info->Tipo_Seccion+"\",\n");

                setIdent(Tmp->Level + 1,Js);
                Js.append("\"contenido\":\n");

            }

            //(^< ............ ............ ............ H O J A
            if(Tp == "viñetas" && Tl == ""){
                setIdent(Tmp->Level + 1,Js);
                Js.append("\""+Tmp->Info->Contenido+"\",\n");
            }


            if(Tmp->TreeList != NULL){
                Build_JSon_Document(Tmp,Js,true);

            }

            if(Tp == "párrafo"){
                setIdent(Tmp->Level,Js);
                Js.append("},\n");
            }

            if(Tp == "imagen"){
                setIdent(Tmp->Level,Js);
                Js.append("},\n");
            }

            if(Tp == "viñetas" && Tl != ""){
                setIdent(Tmp->Level,Js);
                Js.append("},\n");
            }

            if(cnt == (Parent->TreeList->ListSize - 1)){
                Js.chop(2);
                Js.append("\n");
            }



            cnt++;
        }

        if(Has_Sub){
            setIdent(Parent->Level + 1,Js);
            Js.append("]\n");
        }
    }

    void setIdent(int Lv,QString &Js){

        int cnt = 0;
        while(cnt <= Lv){
            Js.append("\t");
            cnt++;
        }
    }

    void GeneratePDF(){

        QString Pdf_Report = "PDF_Tmp/TreeDoc.pdf";


        QPrinter Printer;
        Printer.setOutputFormat(QPrinter::PdfFormat);
        Printer.setOutputFileName(Pdf_Report);
        QPainter Painter;
        if (!Painter.begin(&Printer)){
            qWarning("failed to open file, is it writable?");
            return;
        }

        int Top = 75;

        //------------ Escritura del PDF
        QString Mn_Tl = Arbolito->RT->Info->Titulo;
        Painter.setFont(QFont("MathJax_SansSerif",22,QFont::Bold));
        Painter.drawText(330 - ((Mn_Tl.size() / 2) * 8),Top,Mn_Tl);



        QString Mn_Cont = Arbolito->RT->Info->Contenido;
        int cnt = 0;
        int Lm  = Mn_Cont.size() / 90;
        while(cnt < Lm){
            Mn_Cont.insert(90 * (cnt+1),"#");
            cnt++;
        }

        QStringList Pr = Mn_Cont.split("#");

        cnt = 0;
        Top = 110;
        Painter.setFont(QFont("MathJax_SansSerif",14));
        while(cnt < Pr.size()){
            Painter.drawText(50,Top + (cnt * 20),Pr.at(cnt));
            Top += cnt * 20;
            cnt++;
        }

        Top += 30;

        FillPrinter(Painter,Arbolito->RT,Top);
        Painter.end();
        QString cmd("xdg-open ");
        cmd.append(Pdf_Report);
        qDebug() << cmd;
        system(cmd.toLatin1().data());

    }

    void FillPrinter(QPainter &Painter,TNode* Mn, int &Top){

        int cnt = 0;
        while(cnt < Mn->TreeList->ListSize){
            TNode* Sub_N = Mn->TreeList->GetNode(cnt)->Data;

            int ID = Sub_N->Info->ID;
            QString Titulo = Sub_N->Info->Titulo;
            QString Contenido = Sub_N->Info->Contenido;
            QString Tipo_Seccion = Sub_N->Info->Tipo_Seccion;

            int XPos = 50 + (Sub_N->Level * 25);
            int YPos = Top + (Sub_N->Info->ID * 20);

            Top += 10;

            if(Titulo == "" && Tipo_Seccion == "viñetas"){
                Contenido = "♦ " + Contenido;
                if(cnt > 0){

                    Painter.drawText(XPos,(YPos - (10 * cnt)),Contenido);
                }
                else{
                    Painter.drawText(XPos,YPos,Contenido);
                }
            }
            else if(Tipo_Seccion == "imagen"){

                QPixmap image;
                QTextStream Stm(&Contenido);
                QByteArray base64Data = Stm.readAll().toUtf8();
                image.loadFromData(QByteArray::fromBase64(base64Data));

                Painter.drawPixmap(QRect(XPos,(YPos - (image.height()/3)),image.width(),image.height()),image);

                Painter.setFont(QFont("MathJax_SansSerif",10));
                Painter.drawText(XPos,(YPos + (image.height()/2)),Titulo);

                Top += (image.height() / 2);

            }
            else{
                Painter.setFont(QFont("MathJax_SansSerif",14,QFont::Bold));
                Painter.drawText(XPos,YPos,Titulo);

                YPos += 20;

                Painter.setFont(QFont("MathJax_SansSerif",14));

                int mct = 0;
                int Lm  = Contenido.size() / 60;
                while(mct < Lm){
                    Contenido.insert(60 * (mct+1),"#");
                    mct++;
                }

                QStringList Pr = Contenido.split("#");

                mct = 0;
                while(mct < Pr.size()){
                    Painter.drawText(XPos,YPos + (mct * 20),Pr.at(mct));
                    Top += 20;
                    mct++;
                }
            }

            if(Sub_N->TreeList > 0){

                FillPrinter(Painter,Sub_N,Top);
            }

            cnt++;
        }

    }

    void Base64_to_PNG(QString Pl,QString CD){
        QString FilePath = "output.png";

        QTextStream Stm(&CD);

        QByteArray base64Data = Stm.readAll().toUtf8();
        QImage IMG;

        IMG.loadFromData(QByteArray::fromBase64(base64Data), "PNG");
        //QLabel label(0);
        //label.setPixmap(QPixmap::fromImage(IMG));
        //label.show();

        IMG.save(FilePath, "PNG");

    }

    QString PNG_to_Base64(QString FilePath){
        QString CD = "";

        QFile Fl(FilePath);
        if (Fl.open(QIODevice::ReadOnly)){
            QByteArray IMG = Fl.readAll();
            Fl.close();

            CD = QString(IMG.toBase64());
        }
        return CD;
    }
};

class PresentationHandle{

public:
    QString JSonContent;
};

class CanvasHadle{

public:
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
        New_JSonContent.replace("#","^");

        QString Output_JSon_URL = "JSon_Temp/NewCanvas.json";
        QFile Fl(Output_JSon_URL);
        if (Fl.open(QFile::WriteOnly)) {
            QTextStream stream(&Fl);
            stream << New_JSonContent << endl;
            Fl.close();
        }
    }

    void Remove_WButtons(){
        int cnt = 0;

        while(cnt < Btn_List->ListSize){
            WButton* Tmp = Btn_List->GetNode(cnt)->Data;
            delete Tmp;
            cnt++;
        }

        Btn_List->Clear_List();
    }

    void LoadFrom_JSon(bool PaintBody){

        //(^< ............ User Load

        //QString Input_JSon_URL = "JSon_Temp/Mario.json";

        //QFile MyFile(Input_JSon_URL);
        //MyFile.open(QIODevice::ReadOnly | QIODevice::Text);

        //QTextStream out(&MyFile);
        //QString JSon_Plain_String = out.readAll();
        QString JSon_Plain_String = Original_JSonContent;

        //MyFile.close();


        //(^< ............ Plain Text to JSonDocument
        QJsonDocument JSon_Fl = QJsonDocument::fromJson(JSon_Plain_String.toUtf8());

        //(^< ............ Tree Load
        if(!JSon_Fl.isEmpty()){

            //(^< ............ Doc Header
            QJsonObject OBJ = JSon_Fl.object();

            int I_Low = OBJ["lienzo_fila_inf"].toInt();
            int I_Sup = OBJ["lienzo_fila_sup"].toInt();
            int J_Low = OBJ["lienzo_columna_inf"].toInt();
            int J_Sup = OBJ["lienzo_columna_sup"].toInt();

            I->Low = I_Low;
            I->Sup = I_Sup;

            J->Low = J_Low;
            J->Sup = J_Sup;

            I->SetSize();
            J->SetSize();

            if(PaintBody){
                QJsonArray Inst = OBJ["lienzo_bloques"].toArray();
                int cnt = 0;
                int Lm = Inst.size();
                while(cnt < Lm){

                    QJsonObject Sub = Inst.at(cnt).toObject();
                    int Sub_Size = Sub.size();

                    QString Cl = Sub["color"].toString();

                    //(^< ............ Pixel

                    if(Sub_Size == 3){

                        int IP = Sub["fila"].toInt();
                        int JP = Sub["columna"].toInt();

                        Paint_Pixel(Cl,IP,JP);
                        Insert_Single_Paint_Instruction(Cl,QString::number(IP),QString::number(JP));
                    }

                    //(^< ............ Block
                    else{

                        int Hp_i = Sub["fila_inicial"].toInt();
                        int HP_j = Sub["columna_inicial"].toInt();

                        int Lp_i = Sub["fila_final"].toInt();
                        int Lp_j= Sub["columna_final"].toInt();

                        Paint_Block(Cl,Hp_i,HP_j,Lp_i,Lp_j);
                        Insert_Block_Paint_Instruction(Cl,QString::number(Lp_i),QString::number(Lp_j),QString::number(Hp_i),QString::number(HP_j));


                    }

                    cnt++;
                }
            }
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
            //DotBf<<"                <tr>\n";



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
                DotBf<<"					<td bgcolor=\""+Bn->Data->Current_Color+"\" width=\"30\" height=\"30\">\n";
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
    DocumentHandle* Dc;
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
        Dc = new DocumentHandle();
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
