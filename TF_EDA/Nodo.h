#ifndef __NODO_H__
#define __NODO_H__

template<class T>
class Nodo
{
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T v, Nodo<T>* sig = nullptr)
    {
        dato = v;
        siguiente = sig;
    }
};

#endif
