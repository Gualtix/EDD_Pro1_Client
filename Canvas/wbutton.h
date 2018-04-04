#ifndef WBUTTON_H
#define WBUTTON_H

//C++

//Qt
#include "QString"
#include "QPushButton"
#include "QPalette"
#include "QApplication"

//ADT

class ArrayRange{

public:
    int Sup;
    int Low;
    int Size;



    ArrayRange(){

    }

    ArrayRange(int Lw,int Sp){
        this->Low = Lw;
        this->Sup = Sp;
        this->Size = Sup - Low;
    }

    void SetSize(){
        this->Size = Sup - Low;
    }

};

class WButton : public QPushButton{
public:

    int j;
    int i;
    int Loc;

    QString Current_Color;

    QString BTN_TAG;

    WButton(QWidget *parent): QPushButton(parent){
        QPalette Pl= QApplication::palette();
        Pl.setColor(QPalette::Button, QColor("#ff0066"));
        this->setPalette(Pl);
        this->Current_Color = "#ff0066";
    }

    void SetColor(QString Cl){
        QPalette Pl= QApplication::palette();
        Pl.setColor(QPalette::Button, QColor(Cl));
        this->setPalette(Pl);
        this->Current_Color = Cl;
    }
};



#endif // WBUTTON_H
