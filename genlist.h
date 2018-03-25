#ifndef GENLIST_H
#define GENLIST_H



//C++

//Qt
#include "QString"

//ADT
#include "genode.h"

template <typename T>
class GenList{

public:

    int ListSize;
    GeNode<T>* First;
    GeNode<T>* Last;

    //(^< ............ ............ ............ Constructor: Default NULL
    GenList(){
        this->ListSize = 0;
        this->First = Last = NULL;
    }

    //(^< ............ ............ ............ InsertWhenEmpty
    bool InsertWhenEmpty(T Dt){
        if(ListSize == 0){
            this->First = Last = new GeNode<T>(Dt);
            this->ListSize++;
            return true;
        }
        return false;
    }

    //(^< ............ ............ ............ Insertar al Frente
    bool FrontInsert(T Dt){
        if(!InsertWhenEmpty(Dt)){
            GeNode<T>* TmpNode = new GeNode<T>(Dt);
            this->First->SetPrevious(TmpNode);
            TmpNode->SetNext(First);

            this->First = TmpNode;
            this->ListSize++;
        }
        return true;
    }

    //(^< ............ ............ ............ Insertar al Final
    bool EndInsert(T Dt){
        if(!InsertWhenEmpty(Dt)){
            GeNode<T>* TmpNode = new GeNode<T>(Dt);
            this->Last->SetNext(TmpNode);
            TmpNode->SetPrevious(Last);

            this->Last = TmpNode;
            this->ListSize++;
        }
        return true;
    }

    //(^< ............ ............ ............ Obtener Nodo Espesifico
    GeNode<T>* GetNode(int index){
        if(this->ListSize > 0 && index < this->ListSize){
            int cnt = 0;
            GeNode<T>* TmpNode = First;
            while (cnt < index) {
                TmpNode = TmpNode->GetNext();
                cnt++;
            }
            return TmpNode;
        }
        return NULL;
    }

    //(^< ............ ............ ............ Obtener Nodo Espesifico por Parametro String
    GeNode<T>* GetNode_By_String(QString Param){

        if(this->ListSize > 0){
            int cnt = 0;
            GeNode<T>* TmpNode = First;
            while (cnt < this->ListSize) {
                if(TmpNode->TAG == Param){

                    TmpNode->CORREL = cnt;
                    return TmpNode;
                }
                TmpNode = TmpNode->GetNext();
                cnt++;
            }
        }
        return NULL;
    }

    //(^< ............ ............ ............ Obtener Nodo Espesifico por Parametro String
    GeNode<T>* GetNode_By_Nickname(QString Nick){

        if(this->ListSize > 0){
            int cnt = 0;
            GeNode<T>* TmpNode = First;
            while (cnt < this->ListSize) {
                if(TmpNode->NICK == Nick){

                    TmpNode->CORREL = cnt;
                    return TmpNode;
                }
                TmpNode = TmpNode->GetNext();
                cnt++;
            }
        }
        return NULL;
    }

    //(^< ............ ............ ............ Insertar en Indice Especifico
    bool InsertAt(int index,T Dt){
        if(index >= 0 && index <= this->ListSize){
            //Insertar al Frente de la Lista
            if(index == 0){
                FrontInsert(Dt);
                return true;
            }
            //Insertar al Final de la Lista
            else if(index == this->ListSize){
                EndInsert(Dt);
                return true;
            }
            //Insertar en Indice Intermedio
            GeNode<T>* TmpNode = GetNode(index-1);
            GeNode<T>* NewNode = new GeNode<T>(Dt);

            NewNode->SetNext(TmpNode->GetNext());
            NewNode->SetPrevious(TmpNode);

            TmpNode->GetNext()->SetPrevious(NewNode);
            TmpNode->SetNext(NewNode);

            ListSize++;
            return true;
        }
        return false;
    }

    //(^< ............ ............ ............ Eliminar Nodo segun Indice
    bool DeleteNode(int index){
        if(index >= 0 && index < this->ListSize){

            GeNode<T>* TmpNode = GetNode(index);

            //Eliminar Primer Nodo
            if(index == 0 && this->ListSize == 1){
                this->First = NULL;
                this->Last = NULL;
                delete(TmpNode);

                ListSize--;
                return true;
            }

            else if(index == 0){

                //this->First->Next->Previous = NULL;
                this->First = this->First->Next;
                this->First->Previous = NULL;

                delete(TmpNode);
                ListSize--;
                return true;
            }
            //Eliminar Ultimo Nodo
            else if(index == (this->ListSize-1)){
                this->Last = this->Last->Previous;
                this->Last->Next = NULL;

                delete(TmpNode);
                ListSize--;
                return true;

            }
            //Eliminar Nodo Intermedio
            TmpNode->Previous->Next = TmpNode->Next;
            TmpNode->Next->Previous = TmpNode->Previous;

            delete(TmpNode);
            ListSize--;
            return true;
        }
        return false;
    }

    //(^< ............ ............ ............ GetFirst
    GeNode<T>* GetFirst()const{
        return this->First;
    }

    //(^< ............ ............ ............ GetLast
    GeNode<T>* GetLast()const{
        return this->Last;
    }

    //(^< ............ ............ ............ SetFirst
    void SetFirst(T Dt){
        FrontInsert(Dt);
    }

    //(^< ............ ............ ............ SetLast
    void SetLast(T Dt){
        EndInsert(Dt);
    }

};

#endif // GENLIST_H
