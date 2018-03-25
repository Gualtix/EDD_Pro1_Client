#ifndef MYSTRING_H
#define MYSTRING_H

#include "QString"

class MyString{

public:

    QString Cad = "";

    MyString(QString Value){
        this->Cad = Value;
    }
};

#endif // MYSTRING_H
