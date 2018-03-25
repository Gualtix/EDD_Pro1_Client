#ifndef GENODE_H
#define GENODE_H

//C++

//Qt
#include "QString"

//ADT

template <typename T>
class GeNode{

public:

    T Data;

    int CORREL;
    QString TAG;
    QString NICK;

    GeNode<T>* Previous;
    GeNode<T>* Next;

    //(^< ............ ............ ............ Constructor: Default NULL
    GeNode(T Dt){
        this->Data     = Dt;
        this->CORREL   = -1;
        this->TAG      = "";
        this->NICK     = "";
        this->Previous = NULL;
        this->Next     = NULL;
    }

    //(^< ............ ............ ............ Constructor: Set
    /*
    GeNode(T Dt,GeNode<T>* Ntx){
        this->Data     = Dt;
        this->Previous = NULL;
        this->Next     = Ntx;
    }
    */

    //(^< ............ ............ ............ Destructor
    ~GeNode(){
        //if (this->Data != NULL){
        //    delete Data;
        //}
        this->Previous = NULL;
        this->Next = NULL;
        //delete this;

    }


    //(^< ............ ............ ............ Get Data
    T GetData()const{
        return this->Data;
    }

    //(^< ............ ............ ............ Get Previous
    GeNode<T>* GetPrevious()const{
        return this->Previous;
    }

    //(^< ............ ............ ............ Get Next
    GeNode<T>* GetNext()const{
        return this->Next;
    }

    //(^< ............ ............ ............ Get Data
    void SetData(T Dt){
        this->Data = Dt;
    }

    //(^< ............ ............ ............ SetPrevious
    void SetPrevious(GeNode<T>* Prev){
        this->Previous = Prev;
    }

    //(^< ............ ............ ............ SetNext
    void SetNext(GeNode<T>* Ntx){
        this->Next =Ntx;
    }
};

#endif // GENODE_H
