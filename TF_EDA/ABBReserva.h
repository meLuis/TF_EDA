
#pragma once
//Esto es para usar un árbol binario para organizar en Administrador y buscar objetos de tipo Reserva.
#include "Reserva.h"

class ABBReserva {
private:
    struct NodoReserva {
        Reserva reserva;
        NodoReserva* izquierda;
        NodoReserva* derecha;

        NodoReserva(const Reserva& r) : reserva(r), izquierda(nullptr), derecha(nullptr) {}
    };

    NodoReserva* raiz;

    NodoReserva* insertar(NodoReserva* nodo, const Reserva& r) {
        if (!nodo) return new NodoReserva(r);

        if (r.getIdReserva() < nodo->reserva.getIdReserva())
            nodo->izquierda = insertar(nodo->izquierda, r);
        else
            nodo->derecha = insertar(nodo->derecha, r);

        return nodo;
    }

    Reserva* buscar(NodoReserva* nodo, int id) {
        if (!nodo) return nullptr;
        if (id == nodo->reserva.getIdReserva()) return &nodo->reserva;
        if (id < nodo->reserva.getIdReserva())
            return buscar(nodo->izquierda, id);
        else
            return buscar(nodo->derecha, id);
    }

    void liberar(NodoReserva* nodo) {
        if (!nodo) return;
        liberar(nodo->izquierda);
        liberar(nodo->derecha);
        delete nodo;
    }

public:
    ABBReserva() : raiz(nullptr) {}
    ~ABBReserva() { liberar(raiz); }

    void insertar(const Reserva& r) {
        raiz = insertar(raiz, r);
    }

    Reserva* buscar(int id) {
        return buscar(raiz, id);
    }
};

