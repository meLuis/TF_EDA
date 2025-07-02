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


    static AVLPagadores* avlPagadores;

public:
    Pago() : total(0.0f), idReserva(0), tipoComprobante("Boleta") {}

    Pago(float total, const vector<Pasajero>& pasajeros, const Reserva& reserva,
        const Vuelo& vueloSeleccionado, int idReserva)
        : total(total), pasajeros(pasajeros), reserva(reserva),
        vueloSeleccionado(vueloSeleccionado), idReserva(idReserva), tipoComprobante("Boleta") {}


    float getTotal() const { return total; }
    int getIdReserva() const { return idReserva; }
    string getTipoComprobante() const { return tipoComprobante; }
    Pagador getPagador() const { return pagador; }


    static void inicializarAVL() {
        if (!avlPagadores) {
            avlPagadores = new AVLPagadores();
        }
    }

    static AVLPagadores* getAVLPagadores() {
        return avlPagadores;
    }

    void ingresarDatosPagador() {

        string nombre, apellido, dni, ruc;
        long long numTarjeta;
        int opcionComprobante;

        cin.ignore();
        cout << "\t\t\tTipo de comprobante:\n";
        cout << "\t\t\t1. Boleta\n";
        cout << "\t\t\t2. Factura\n";
        cout << "\t\t\tSeleccione opción: ";

        cin >> opcionComprobante;

        if (opcionComprobante == 1) {
            tipoComprobante = "Boleta";

        }
        else if (opcionComprobante == 2) {
            tipoComprobante = "Factura";

        }
        else {
            cout << "\t\t\tOpción inválida. Seleccione 1 o 2.\n";
        }


		if (tipoComprobante == "Boleta") {
			cin.ignore();
            cout << "\n\t\t\tIngrese los datos del pagador:\n";

            cout << "\t\t\tNombre: ";
            getline(cin, nombre);
            cout << "\t\t\tApellido: ";
            getline(cin, apellido);


            do {
                cout << "\t\t\tDNI (8 dígitos): ";
                cin >> dni;

                if (dni.length() != 8 || !all_of(dni.begin(), dni.end(), ::isdigit)) {
                    cout << "\t\t\tDNI inválido. Debe contener exactamente 8 dígitos numéricos.\n";
                }
                else {
                    break;
                }
            } while (true);

            do {
                cout << "\t\t\tTarjeta (15-16 dígitos): ";
                cin >> numTarjeta;

                if (to_string(numTarjeta).length() != 15 && to_string(numTarjeta).length() != 16) {
                    cout << "\t\t\tNúmero de tarjeta inválido. Debe contener 15 o 16 dígitos.\n";
                }
                else {
                    break;
                }
            } while (true);
            pagador = Pagador(nombre, apellido, dni, numTarjeta, total, idReserva,persona);
        }
		else if (tipoComprobante == "Factura") {
            cin.ignore();
			apellido = ""; 
			cout << "\n\t\t\tIngrese los datos del pagador:\n";
			cout << "\t\t\tNombre de organización: ";
			getline(cin, nombre);

			do {
				cout << "\t\t\t¨RUC (11 dígitos): ";
				cin >> ruc;

				if (ruc.length() != 11 || !all_of(ruc.begin(), ruc.end(), ::isdigit)) {
					cout << "\t\t\tRUC invalido. Debe contener exactamente 11 dígitos numéricos.\n";
				}
				else {
					break;
				}
			} while (true);

			do {
				cout << "\t\t\tTarjeta (15 - 16): ";
				cin >> numTarjeta;

				if (to_string(numTarjeta).length() != 15 && to_string(numTarjeta).length() != 16) {
					cout << "\t\t\tNúmero de tarjeta inválido. Debe contener 15 o 16 dígitos.\n";
				}
				else {
					break;
				}
			} while (true);

            pagador = Pagador(nombre, ruc, numTarjeta, total, idReserva,organizacion);
		}


    
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

    void mostrarPagadoresOrdenados() {
        if (avlPagadores) {
            avlPagadores->mostrarPagadoresOrdenados();
        }
        else {
            cout << "\t\t\tNo hay sistema de pagadores inicializado.\n";
        }
    }

   
    void mostrarSoloPersonas() {
        if (avlPagadores) {
            avlPagadores->mostrarPorTipo(persona);
        }
        else {
            cout << "\t\t\tNo hay sistema de pagadores inicializado.\n";
        }
    }

   void mostrarSoloOrganizaciones() {
        if (avlPagadores) {
            avlPagadores->mostrarPorTipo(organizacion);
        }
        else {
            cout << "\t\t\tNo hay sistema de pagadores inicializado.\n";
        }
    }

    static void limpiarAVL() {
        delete avlPagadores;
        avlPagadores = nullptr;
    }
};


AVLPagadores* Pago::avlPagadores = nullptr;