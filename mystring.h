#ifndef MYSTRING_H
#define MYSTRING_H

#include "QString"

class MyString{

public:

    QString Cad = "";

    QString Usuario;
    QString Permiso;

    MyString(){
        Cad = "";
        Usuario = "";
        Permiso = "";
    }

    MyString(QString Value){
        this->Cad = Value;
    }

    MyString(QString Us,QString Pm){
        this->Usuario = Us;
        this->Permiso = Pm;
    }

};

#endif // MYSTRING_H
