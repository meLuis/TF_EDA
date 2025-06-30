#pragma once
#include "NodoAVL.h"
#include "Pagador.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class AVLPagadores {
private:
    NodoAVL<Pagador>* raiz;

    int obtenerAltura(NodoAVL<Pagador>* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int obtenerBalance(NodoAVL<Pagador>* nodo) {
        return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
    }

    void actualizarAltura(NodoAVL<Pagador>* nodo) {
        if (nodo) {
            nodo->altura = 1 + max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
        }
    }

    NodoAVL<Pagador>* rotarDerecha(NodoAVL<Pagador>* y) {
        NodoAVL<Pagador>* x = y->izquierda;
        NodoAVL<Pagador>* T2 = x->derecha;

        x->derecha = y;
        y->izquierda = T2;

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    NodoAVL<Pagador>* rotarIzquierda(NodoAVL<Pagador>* x) {
        NodoAVL<Pagador>* y = x->derecha;
        NodoAVL<Pagador>* T2 = y->izquierda;

        y->izquierda = x;
        x->derecha = T2;

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }

    NodoAVL<Pagador>* insertar(NodoAVL<Pagador>* nodo, const Pagador& pagador) {
        // Inserción normal del BST
        if (!nodo) {
            return new NodoAVL<Pagador>(pagador);
        }

        if (pagador < nodo->dato) {
            nodo->izquierda = insertar(nodo->izquierda, pagador);
        }
        else if (pagador > nodo->dato) {
            nodo->derecha = insertar(nodo->derecha, pagador);
        }
        else {
            // Si el pagador ya existe (mismo DNI), actualizar el total pagado
            nodo->dato.setTotalPagado(nodo->dato.getTotalPagado() + pagador.getTotalPagado());
            return nodo;
        }

        // Actualizar altura
        actualizarAltura(nodo);

        // Obtener balance
        int balance = obtenerBalance(nodo);

        // Casos de rotación
        // Izquierda Izquierda
        if (balance > 1 && pagador < nodo->izquierda->dato) {
            return rotarDerecha(nodo);
        }

        // Derecha Derecha
        if (balance < -1 && pagador > nodo->derecha->dato) {
            return rotarIzquierda(nodo);
        }

        // Izquierda Derecha
        if (balance > 1 && pagador > nodo->izquierda->dato) {
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
            return rotarDerecha(nodo);
        }

        // Derecha Izquierda
        if (balance < -1 && pagador < nodo->derecha->dato) {
            nodo->derecha = rotarDerecha(nodo->derecha);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    void inOrden(NodoAVL<Pagador>* nodo) const {
        if (nodo) {
            inOrden(nodo->izquierda);
            cout << "\t\t\tNombre: " << nodo->dato.getNombre() << " " << nodo->dato.getApellido()
                << " | DNI: " << nodo->dato.getDni()
                << " | Total Pagado: S/. " << nodo->dato.getTotalPagado() << "\n";
            inOrden(nodo->derecha);
        }
    }

    void destruir(NodoAVL<Pagador>* nodo) {
        if (nodo) {
            destruir(nodo->izquierda);
            destruir(nodo->derecha);
            delete nodo;
        }
    }

    void guardarEnArchivo(NodoAVL<Pagador>* nodo, ofstream& archivo) const {
        if (nodo) {
            guardarEnArchivo(nodo->izquierda, archivo);
            archivo << "ID Reserva: " << nodo->dato.getIdReserva() << "\n";
            archivo << "Nombre del Pagador: " << nodo->dato.getNombre() << "\n";
            archivo << "Apellido del Pagador: " << nodo->dato.getApellido() << "\n";
            archivo << "DNI del Pagador: " << nodo->dato.getDni() << "\n";
            archivo << "Tarjeta (últimos 4 dígitos): **** **** **** "
                << to_string(nodo->dato.getNumTarjeta()).substr(max(0, (int)to_string(nodo->dato.getNumTarjeta()).length() - 4)) << "\n";
            archivo << "Total Pagado: S/. " << nodo->dato.getTotalPagado() << "\n";
            archivo << "-----------------------------------------------\n";
            guardarEnArchivo(nodo->derecha, archivo);
        }
    }

public:
    AVLPagadores() : raiz(nullptr) {}

    ~AVLPagadores() {
        destruir(raiz);
    }

    void agregarPagador(const Pagador& pagador) {
        raiz = insertar(raiz, pagador);
    }

    void mostrarPagadoresOrdenados() const {
        if (!raiz) {
            cout << "\t\t\tNo hay pagadores registrados.\n";
            return;
        }
        cout << "\t\t\t========== PAGADORES ORDENADOS POR TOTAL PAGADO ==========\n";
        inOrden(raiz);
        cout << "\t\t\t=========================================================\n";
    }

    void guardarPagadores() const {
        ofstream archivo("Archivos//pagantes.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir archivo de pagantes." << endl;
            return;
        }

        archivo << "========== REGISTRO DE PAGADORES ==========\n";
        guardarEnArchivo(raiz, archivo);
        archivo << "==========================================\n\n";
        archivo.close();
    }
};