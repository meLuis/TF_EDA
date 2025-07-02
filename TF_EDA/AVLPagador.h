#pragma once
#include "Pagador.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

struct NodoAVL {
    Pagador* elemento;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(Pagador* pag) : elemento(pag), izq(nullptr), der(nullptr), altura(1) {}
};

class AVLPagadores {
private:
    NodoAVL* raiz;

    // Métodos privados
    int _altura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0;
    }

    void _rotarDerecha(NodoAVL*& nodo) {
        NodoAVL* p = nodo->izq;
        nodo->izq = p->der;
        p->der = nodo;
        nodo->altura = 1 + max(_altura(nodo->izq), _altura(nodo->der));
        p->altura = 1 + max(_altura(p->izq), _altura(p->der));
        nodo = p;
    }

    void _rotarIzquierda(NodoAVL*& nodo) {
        NodoAVL* p = nodo->der;
        nodo->der = p->izq;
        p->izq = nodo;
        nodo->altura = 1 + max(_altura(nodo->izq), _altura(nodo->der));
        p->altura = 1 + max(_altura(p->izq), _altura(p->der));
        nodo = p;
    }

    void _balanceo(NodoAVL*& nodo) {
        int fb = _altura(nodo->der) - _altura(nodo->izq);

        if (fb > 1) {
            if (_altura(nodo->der->izq) > _altura(nodo->der->der)) {
                _rotarDerecha(nodo->der);
            }
            _rotarIzquierda(nodo);
        }
        else if (fb < -1) {
            if (_altura(nodo->izq->der) > _altura(nodo->izq->izq)) {
                _rotarIzquierda(nodo->izq);
            }
            _rotarDerecha(nodo);
        }
        nodo->altura = 1 + max(_altura(nodo->izq), _altura(nodo->der));
    }

    bool _insertar(NodoAVL*& nodo, Pagador* pagador) {
        if (nodo == nullptr) {
            nodo = new NodoAVL(pagador);
            return true;
        }
        else if (pagador->getTotalPagado() < nodo->elemento->getTotalPagado()) {
            _insertar(nodo->izq, pagador);
        }
        else if (pagador->getTotalPagado() > nodo->elemento->getTotalPagado()) {
            _insertar(nodo->der, pagador);
        }
        else {
            // Si tienen el mismo total pagado, ordenar por DNI como criterio secundario
            if (pagador->getDni() < nodo->elemento->getDni()) {
                _insertar(nodo->izq, pagador);
            }
            else if (pagador->getDni() > nodo->elemento->getDni()) {
                _insertar(nodo->der, pagador);
            }
            else {
                // Mismo DNI, actualizar el total pagado
                nodo->elemento->setTotalPagado(nodo->elemento->getTotalPagado() + pagador->getTotalPagado());
                return false;
            }
            _balanceo(nodo);
            return true;
        }
    }
        void _inOrden(NodoAVL * nodo) {
            if (nodo == nullptr) return;
            _inOrden(nodo->izq);
            cout << "\t\t\tTotal Pagado: S/. " << nodo->elemento->getTotalPagado()
                << " | Nombre: " << nodo->elemento->getNombre() << " " << nodo->elemento->getApellido()
                << " | DNI: " << nodo->elemento->getDni()
                << " | ID Reserva: " << nodo->elemento->getIdReserva() << "\n";
            _inOrden(nodo->der);
        }

        void _destruir(NodoAVL * nodo) {
            if (nodo) {
                _destruir(nodo->izq);
                _destruir(nodo->der);
                delete nodo->elemento;
                delete nodo;
            }
        }

        void _guardarEnArchivo(NodoAVL * nodo, ofstream & archivo) {
            if (nodo) {
                _guardarEnArchivo(nodo->izq, archivo);
                if (nodo->elemento->getTipoPagador() == persona) {
                    archivo << "ID Reserva: " << nodo->elemento->getIdReserva() << "\n";
                    archivo << "Nombre del Pagador: " << nodo->elemento->getNombre() << "\n";
                    archivo << "Apellido del Pagador: " << nodo->elemento->getApellido() << "\n";
                    archivo << "DNI del Pagador: " << nodo->elemento->getDni() << "\n";
                    archivo << "Tarjeta (últimos 4 dígitos): **** **** **** "
                        << to_string(nodo->elemento->getNumTarjeta()).substr(max(0, (int)to_string(nodo->elemento->getNumTarjeta()).length() - 4)) << "\n";
                    archivo << "Total Pagado: S/. " << nodo->elemento->getTotalPagado() << "\n";
                    archivo << "-----------------------------------------------\n";
                }
                else if (nodo->elemento->getTipoPagador() == organizacion) {
                    archivo << "ID Reserva: " << nodo->elemento->getIdReserva() << "\n";
                    archivo << "Nombre de la organizacion: " << nodo->elemento->getNombre() << "\n";
                    archivo << "RUC: " << nodo->elemento->getRuc() << "\n";
                    archivo << "Tarjeta (últimos 4 dígitos): **** **** **** "
                        << to_string(nodo->elemento->getNumTarjeta()).substr(max(0, (int)to_string(nodo->elemento->getNumTarjeta()).length() - 4)) << "\n";
                    archivo << "Total Pagado: S/. " << nodo->elemento->getTotalPagado() << "\n";
                    archivo << "-----------------------------------------------\n";
                }
                _guardarEnArchivo(nodo->der, archivo);
            }
        }

        Pagador* _buscarPorDNI(NodoAVL * nodo, const string & dni) {
            if (nodo == nullptr) {
                return nullptr;
            }
            if (dni < nodo->elemento->getDni()) {
                return _buscarPorDNI(nodo->izq, dni);
            }
            else if (dni > nodo->elemento->getDni()) {
                return _buscarPorDNI(nodo->der, dni);
            }
            else {
                return nodo->elemento;
            }
        }

        Pagador* _buscarPorCodigoReserva(NodoAVL * nodo, const string & codigoReserva) {
            if (nodo == nullptr) {
                return nullptr;
            }


            if (to_string(nodo->elemento->getIdReserva()) == codigoReserva) {
                return nodo->elemento;
            }


            Pagador* encontradoIzq = _buscarPorCodigoReserva(nodo->izq, codigoReserva);
            if (encontradoIzq != nullptr) {
                return encontradoIzq;
            }

            return _buscarPorCodigoReserva(nodo->der, codigoReserva);
        }

public:
    AVLPagadores() : raiz(nullptr) {}

    ~AVLPagadores() {
        _destruir(raiz);
    }

    bool insertar(const Pagador & pagador) {
        Pagador* nuevoPagador = new Pagador(pagador);
        return _insertar(raiz, nuevoPagador);
    }


    void agregarPagador(const Pagador & pagador) {
        insertar(pagador);
    }

    void inOrden() {
        _inOrden(raiz);
    }


    void mostrar() {
        if (!raiz) {
            cout << "\t\t\tNo hay pagadores registrados.\n";
            return;
        }
        cout << "\t\t\t========== PAGADORES ORDENADOS POR TOTAL PAGADO ==========\n";
        inOrden();
        cout << "\t\t\t=========================================================\n";
    }


    void mostrarPagadoresOrdenados() {
        mostrar();
    }

    int altura() {
        return raiz ? raiz->altura : 0;
    }

    Pagador* buscarPorDNI(const string & dni) {
        return _buscarPorDNI(raiz, dni);
    }

    Pagador* buscarPorCodigoReserva(const string & codigoReserva) {
        return _buscarPorCodigoReserva(raiz, codigoReserva);
    }


    Pagador* buscar(const string & criterio, int tipoBusqueda = 1) {
        switch (tipoBusqueda) {
        case 1:
            return buscarPorDNI(criterio);
        case 2:
            return buscarPorCodigoReserva(criterio);
        default:
            cout << "\t\t\tTipo de búsqueda no válido." << endl;
            return nullptr;
        }
    }

    void guardarPagadores() {
        ofstream archivo("Archivos//pagantes.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir archivo de pagantes." << endl;
            return;
        }

        archivo << "========== REGISTRO DE PAGADORES ==========\n";
        _guardarEnArchivo(raiz, archivo);
        archivo << "==========================================\n\n";
        archivo.close();
    }

};