
#pragma once
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
    bool datosCargados = false;

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

    void guardarReservas(NodoReserva* nodo, ofstream& archivo) {
        if (!nodo) return;
        guardarReservas(nodo->izquierda, archivo);
        nodo->reserva.guardarEnArchivo(archivo);
        guardarReservas(nodo->derecha, archivo);
    }

public:
    ABBReserva() : raiz(nullptr) {}
    ~ABBReserva() { liberar(raiz); }


    void cargarDesdeArchivo(const string& nombreArchivo) {
        if (datosCargados) {
            cerr << "Las reservas ya fueron cargadas desde el archivo.\n";
            return;
        }

        vector<Reserva> reservas = Reserva::leerReservasDesdeArchivo(nombreArchivo);
        for (const Reserva& r : reservas) {
            raiz = insertar(raiz, r);
        }

        datosCargados = true;
    }


    Reserva* buscar(int id) {
        return buscar(raiz, id);
    }

    void guardarEnArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "No se pudo abrir el archivo para guardar: " << nombreArchivo << endl;
            return;
        }
        guardarReservas(raiz, archivo);
        archivo.close();
    }
};