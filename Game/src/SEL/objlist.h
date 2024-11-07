#ifndef SEL_OBJLIST_H
#define SEL_OBJLIST_H

#include <iostream>

template<class T>
class ObjList;

template<class T>
class ColCell;

template<class T>
class ObjList
{
public:
    ColCell<T> *cell;
    T *object;
    ObjList *next;
    ObjList *prev;
    int id;

    ObjList(int str_id = 0);

    ~ObjList();

    bool Remove();

    T* get() const { return object;}
};

template<class T>
class ColCell
{
    ObjList<T> *top;

public:

    ColCell()
    : top(nullptr)
    {
    }

    void reset();
    
    void push(ObjList<T> *obj)
    {
        if(!obj) return;
        if(obj == top) return;

        if(!top) top = obj;
        else
        {
            obj->next = top;
            top->prev = obj;
            top = obj;
        }

        obj->cell = this;
    }

    void onRemove(ObjList<T> *obj);

    ObjList<T> *get() const { return top; }
};

#endif // SEL_OBJLIST_H