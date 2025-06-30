#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "Vuelo.h"
#include "Pagador.h"
#include "Comprobante.h"
#include "AVLPagador.h"

using namespace std;

class Pago {
private:
    vector<Pasajero> pasajeros;
    Reserva reserva;
    Vuelo vueloSeleccionado;
    Pagador pagador;
    float total;
    int idReserva;
    string tipoComprobante;

    // Referencia al AVL de pagadores (est�tico para compartir entre instancias)
    static AVLPagadores* avlPagadores;

public:
    Pago() : total(0.0f), idReserva(0), tipoComprobante("Boleta") {}

    Pago(float total, const vector<Pasajero>& pasajeros, const Reserva& reserva,
        const Vuelo& vueloSeleccionado, int idReserva)
        : total(total), pasajeros(pasajeros), reserva(reserva),
        vueloSeleccionado(vueloSeleccionado), idReserva(idReserva), tipoComprobante("Boleta") {}

    // Getters
    float getTotal() const { return total; }
    int getIdReserva() const { return idReserva; }
    string getTipoComprobante() const { return tipoComprobante; }
    Pagador getPagador() const { return pagador; }

    // M�todo est�tico para inicializar el AVL
    static void inicializarAVL() {
        if (!avlPagadores) {
            avlPagadores = new AVLPagadores();
        }
    }

    // M�todo est�tico para obtener referencia al AVL
    static AVLPagadores* getAVLPagadores() {
        return avlPagadores;
    }

    void ingresarDatosPagador() {
        cin.ignore();
        cout << "\n\t\t\tIngrese los datos del pagador:\n";

        string nombre, apellido, dni;
        long long numTarjeta;
        int opcionComprobante;

        cout << "\t\t\tNombre: ";
        getline(cin, nombre);
        cout << "\t\t\tApellido: ";
        getline(cin, apellido);


        do {
            cout << "\t\t\tDNI (8 d�gitos): ";
            cin >> dni;

            if (dni.length() != 8 || !all_of(dni.begin(), dni.end(), ::isdigit)) {
                cout << "\t\t\tDNI inv�lido. Debe contener exactamente 8 d�gitos num�ricos.\n";
            }
            else {
                break;
            }
        } while (true);

     
        cout << "\t\t\tTarjeta (16 d�gitos d�bito y 15 d�gitos AMEX): ";
        cin >> numTarjeta;
      
        cout << "\t\t\tTipo de comprobante:\n";
        cout << "\t\t\t1. Boleta\n";
        cout << "\t\t\t2. Factura\n";
        cout << "\t\t\tSeleccione opci�n: ";
 
        cin >> opcionComprobante;

            if (opcionComprobante == 1) {
                tipoComprobante = "Boleta";
               
            }
            else if (opcionComprobante == 2) {
                tipoComprobante = "Factura";
               
            }
            else {
                cout << "\t\t\tOpci�n inv�lida. Seleccione 1 o 2.\n";
            }

       
        pagador = Pagador(nombre, apellido, dni, numTarjeta, total, idReserva);

    
        if (avlPagadores) {
            avlPagadores->agregarPagador(pagador);
        }
    }

    void mostrarComprobante() const {
        Comprobante comprobante(pasajeros, reserva, vueloSeleccionado, pagador, total, idReserva, tipoComprobante);
        comprobante.mostrarComprobante();
    }

    void generarComprobante() const {
        Comprobante comprobante(pasajeros, reserva, vueloSeleccionado, pagador, total, idReserva, tipoComprobante);
        comprobante.generarComprobante();
    }

    void guardarDatosPagador() const {
        if (avlPagadores) {
            avlPagadores->guardarPagadores();
        }
    }

    // M�todo est�tico para mostrar todos los pagadores ordenados
    static void mostrarPagadoresOrdenados() {
        if (avlPagadores) {
            avlPagadores->mostrarPagadoresOrdenados();
        }
        else {
            cout << "\t\t\tNo hay sistema de pagadores inicializado.\n";
        }
    }

    // M�todo est�tico para limpiar memoria al finalizar el programa
    static void limpiarAVL() {
        delete avlPagadores;
        avlPagadores = nullptr;
    }
};

// Definici�n de la variable est�tica
AVLPagadores* Pago::avlPagadores = nullptr;