#include "Pagador.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
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

    string _obtenerIdentificador(const Pagador* pagador) {
        return (pagador->getTipoPagador() == persona) ? pagador->getDni() : pagador->getRuc();
    }

    bool _insertar(NodoAVL*& nodo, Pagador* pagador) {
        if (nodo == nullptr) {
            nodo = new NodoAVL(pagador);
            return true;
        }
        else if (pagador->getTotalPagado() < nodo->elemento->getTotalPagado()) {
            bool resultado = _insertar(nodo->izq, pagador);
            if (resultado) _balanceo(nodo);
            return resultado;
        }
        else if (pagador->getTotalPagado() > nodo->elemento->getTotalPagado()) {
            bool resultado = _insertar(nodo->der, pagador);
            if (resultado) _balanceo(nodo);
            return resultado;
        }
        else {

            string idPagador = _obtenerIdentificador(pagador);
            string idNodo = _obtenerIdentificador(nodo->elemento);

            if (idPagador < idNodo) {
                bool resultado = _insertar(nodo->izq, pagador);
                if (resultado) _balanceo(nodo);
                return resultado;
            }
            else if (idPagador > idNodo) {
                bool resultado = _insertar(nodo->der, pagador);
                if (resultado) _balanceo(nodo);
                return resultado;
            }
            else {

                bool resultado = _insertar(nodo->izq, pagador);
                if (resultado) _balanceo(nodo);
                return resultado;
            }
        }
    }

    void _inOrden(NodoAVL* nodo) {
        if (nodo == nullptr) return;
        _inOrden(nodo->izq);

        string tipoComprobante = (nodo->elemento->getTipoPagador() == persona) ? "Boleta" : "Factura";
        string apellido = (nodo->elemento->getTipoPagador() == persona) ? nodo->elemento->getApellido() : "--\t";
        string identificador = (nodo->elemento->getTipoPagador() == persona) ? nodo->elemento->getDni() : nodo->elemento->getRuc();

        cout << "\t" << nodo->elemento->getIdReserva() << "\t|  "
            << tipoComprobante << "    \t| "
            << nodo->elemento->getNombre() << " \t| "
            << apellido << "\t| "
            << identificador << "\t | S/. "
            << fixed << setprecision(2) << nodo->elemento->getTotalPagado() << "\n";

        _inOrden(nodo->der);
    }

    void _inOrdenFiltrado(NodoAVL* nodo, TipoPagador tipoFiltro) {
        if (nodo == nullptr) return;
        _inOrdenFiltrado(nodo->izq, tipoFiltro);
        if (nodo->elemento->getTipoPagador() == tipoFiltro) {
            string tipoComprobante = (nodo->elemento->getTipoPagador() == persona) ? "Boleta" : "Factura";
            string apellido = (nodo->elemento->getTipoPagador() == persona) ? nodo->elemento->getApellido() : "--";
            string identificador = (nodo->elemento->getTipoPagador() == persona) ? nodo->elemento->getDni() : nodo->elemento->getRuc();

            cout << "\t" << nodo->elemento->getIdReserva() << "\t|  "
                << tipoComprobante << "    \t| "
                << nodo->elemento->getNombre() << " \t| "
                << apellido << "\t| "
                << identificador << "\t | S/. "
                << fixed << setprecision(2) << nodo->elemento->getTotalPagado() << "\n";
        }
        _inOrdenFiltrado(nodo->der, tipoFiltro);
    }

    void _destruir(NodoAVL* nodo) {
        if (nodo) {
            _destruir(nodo->izq);
            _destruir(nodo->der);
            delete nodo->elemento;
            delete nodo;
        }
    }

    Pagador* _buscarPorDNI(NodoAVL* nodo, const string& dni) {
        if (nodo == nullptr) {
            return nullptr;
        }

        if (nodo->elemento->getTipoPagador() == persona && nodo->elemento->getDni() == dni) {
            return nodo->elemento;
        }

        Pagador* encontradoIzq = _buscarPorDNI(nodo->izq, dni);
        if (encontradoIzq != nullptr) {
            return encontradoIzq;
        }

        return _buscarPorDNI(nodo->der, dni);
    }

    Pagador* _buscarPorRUC(NodoAVL* nodo, const string& ruc) {
        if (nodo == nullptr) {
            return nullptr;
        }

        if (nodo->elemento->getTipoPagador() == organizacion && nodo->elemento->getRuc() == ruc) {
            return nodo->elemento;
        }

        Pagador* encontradoIzq = _buscarPorRUC(nodo->izq, ruc);
        if (encontradoIzq != nullptr) {
            return encontradoIzq;
        }

        return _buscarPorRUC(nodo->der, ruc);
    }

    Pagador* _buscarPorCodigoReserva(NodoAVL* nodo, const string& codigoReserva) {
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

    void _obtenerPorTipo(NodoAVL* nodo, vector<Pagador*>& resultado, TipoPagador tipo) {
        if (nodo == nullptr) return;

        _obtenerPorTipo(nodo->izq, resultado, tipo);

        if (nodo->elemento->getTipoPagador() == tipo) {
            resultado.push_back(nodo->elemento);
        }

        _obtenerPorTipo(nodo->der, resultado, tipo);
    }
    void _leerArchivoPagadores(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            if (linea.find("ID Reserva:") != string::npos) {
                int idReserva = stoi(linea.substr(linea.find(":") + 2));

                string nombre, apellido = "", dni = "", ruc = "";
                float totalPagado = 0;
                TipoPagador tipo;

                
                getline(archivo, linea);
                if (linea.find("Nombre del Pagador:") != string::npos) {
                    nombre = linea.substr(linea.find(":") + 2);
                    
                    getline(archivo, linea);
                    if (linea.find("Apellido del Pagador:") != string::npos)
                        apellido = linea.substr(linea.find(":") + 2);

                   
                    getline(archivo, linea);
                    if (linea.find("DNI del Pagador:") != string::npos)
                        dni = linea.substr(linea.find(":") + 2);
                }
                else if (linea.find("Nombre de la organización:") != string::npos) {
                    nombre = linea.substr(linea.find(":") + 2);
                    
                    getline(archivo, linea);
                    if (linea.find("RUC:") != string::npos)
                        ruc = linea.substr(linea.find(":") + 2);
                }

              
                getline(archivo, linea);

                
                getline(archivo, linea);
                if (linea.find("S/.") != string::npos)
                    totalPagado = stof(linea.substr(linea.find("S/.") + 4));

               
                getline(archivo, linea);
                if (linea.find("Persona") != string::npos) {
                    tipo = persona;
                    insertar(Pagador(nombre, apellido, dni, "", totalPagado, idReserva, tipo));
                }
                else {
                    tipo = organizacion;
                    insertar(Pagador(nombre, ruc, "", totalPagado, idReserva, tipo));
                }

               
                getline(archivo, linea);
            }
        }

        archivo.close();
        cout << "Archivo de pagadores cargado exitosamente." << endl;
    }


public:

    AVLPagadores(const string& nombreArchivo) : raiz(nullptr) {
        cargarDesdeArchivo(nombreArchivo);
    }
    ~AVLPagadores() {
        _destruir(raiz);
    }
    void cargarDesdeArchivo(const string& nombreArchivo) {
        _leerArchivoPagadores(nombreArchivo);
    }

    bool insertar(const Pagador& pagador) {
        Pagador* nuevoPagador = new Pagador(pagador);
        return _insertar(raiz, nuevoPagador);
    }

    void agregarPagador(const Pagador& pagador) {
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
        cout << "\t========== PAGADORES ORDENADOS POR TOTAL PAGADO ==========\n";
        cout << "\tIDRESERVA | COMPROBANTE | NOMBRE\t | APELLIDO\t | IDENTIFICADOR | MONTO PAGADO\n";
        cout << "\t--------------------------------------------------------------------------------\n";
        inOrden();

    }


    void mostrarPorTipo(TipoPagador tipo) {
        if (!raiz) {
            cout << "\t\t\tNo hay pagadores registrados.\n";
            return;
        }


        string tipoStr = (tipo == persona) ? "PERSONAS (BOLETAS)" : "ORGANIZACIONES (FACTURAS)";
        cout << "\t========== " << tipoStr << " ==========\n";
        cout << "\tIDRESERVA | COMPROBANTE | NOMBRE\t | APELLIDO\t | IDENTIFICADOR | MONTO PAGADO\n";
        cout << "\t--------------------------------------------------------------------------------\n";

        _inOrdenFiltrado(raiz, tipo);

    }
    Pagador* buscar(const string& criterio, int tipoBusqueda = 1) {
        switch (tipoBusqueda) {
        case 1: return _buscarPorDNI(raiz, criterio);
        case 2: return _buscarPorCodigoReserva(raiz, criterio);
        case 3: return _buscarPorRUC(raiz, criterio);
        default:
            cout << "\t\t\tTipo de búsqueda no válido.\n";
            return nullptr;
        }
    }


    int altura() {
        return raiz ? raiz->altura : 0;
    }

    Pagador* buscarPorDNI(const string& dni) {
        return _buscarPorDNI(raiz, dni);
    }

    Pagador* buscarPorRUC(const string& ruc) {
        return _buscarPorRUC(raiz, ruc);
    }

    Pagador* buscarPorCodigoReserva(const string& codigoReserva) {
        return _buscarPorCodigoReserva(raiz, codigoReserva);
    }
};