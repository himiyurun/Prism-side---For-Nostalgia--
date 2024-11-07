#include "objlist.h"

template<class T>
ObjList<T>::ObjList(int str_id)
    : cell(cell)
    , object(nullptr)
    , next(nullptr)
    , prev(nullptr)
    , id(str_id)
{
}

template<class T>
ObjList<T>::~ObjList()
{
}

//  リストから外れる
template <class T>
bool ObjList<T>::Remove()
{
    if(!cell) return false;
    cell->onRemove(this);

    if(prev) prev->next = next;
    if(next) next->prev = prev;
    prev = nullptr;
    next = nullptr;
    id = 0;
    object = nullptr;
}

template<class T>
void ColCell<T>::reset()
{
    if(top->next) {
        reset(top->next);
    }
    top = nullptr;
}

template <class T>
void ColCell<T>::onRemove(ObjList<T> *obj)
{
    if(obj)
    {
        if(obj == top) top = obj->next;
    }
}
