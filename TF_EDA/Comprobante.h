#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Vuelo.h"
#include "Reserva.h"
#include "GestorPasajero.h"
#include "Pagador.h"
using namespace std;

class Comprobante {
private:
    vector<Pasajero> pasajeros;
    Reserva reserva;
    Vuelo vueloSeleccionado;
    Pagador pagador;
    float total;
    int idReserva;
    string tipoComprobante; // "Boleta" o "Factura"

public:
    Comprobante() : total(0.0f), idReserva(0), tipoComprobante("Boleta") {}

    Comprobante(const vector<Pasajero>& pasajeros, const Reserva& reserva,
        const Vuelo& vuelo, const Pagador& pagador, float total,
        int idReserva, const string& tipo = "Boleta")
        : pasajeros(pasajeros), reserva(reserva), vueloSeleccionado(vuelo),
        pagador(pagador), total(total), idReserva(idReserva), tipoComprobante(tipo) {}

    // Getters
    float getTotal() const { return total; }
    int getIdReserva() const { return idReserva; }
    string getTipoComprobante() const { return tipoComprobante; }
    Pagador getPagador() const { return pagador; }

    // Setters
    void setTipoComprobante(const string& tipo) { tipoComprobante = tipo; }

    void mostrarComprobante() {
        cout << "\t\t\t--------------------- COMPROBANTE DE PAGO ---------------------\n";
        cout << "\t\t\tTipo: " << tipoComprobante << "\n";
        cout << "\t\t\tID Reserva: " << idReserva << "\n";
        cout << "\t\t\t\t--------------------------------------------------\n";
        cout << "\t\t\tDatos del Pagador:\n";
        cout << "\t\t\tNombre: " << pagador.getNombre() << "\n";
        cout << "\t\t\tApellido: " << pagador.getApellido() << "\n";
        cout << "\t\t\tDNI: " << pagador.getDni() << "\n";
        cout << "\t\t\t\t--------------------------------------------------\n\n";
        cout << "\t\t\tFecha: " << reserva.getFecha() << "\n";
        cout << "\t\t\tOrigen: " << reserva.getOrigen() << "\t Destino: " << reserva.getDestino() << "\n";
        cout << "\t\t\tHora Salida: " << vueloSeleccionado.getHoraInicio() << "\tHora llegada: " << vueloSeleccionado.getHoraFin() << "\n";
        cout << "\t\t\tDetalles de Pasajeros:\n";

        for (auto& pasajero : pasajeros) {
            cout << "\t\t\t\t--------------------------------------------------\n";
            cout << "\t\t\tNombre: " << pasajero.getNombre() << "\n";
            cout << "\t\t\tApellido: " << pasajero.getApellido() << "\n";
            cout << "\t\t\tDNI: " << pasajero.getDni() << "\n";
            cout << "\t\t\tAsiento: " << pasajero.getNumeroAsiento() << "\n";
            cout << "\t\t\tPrecio Ruta: S/. " << pasajero.getPrecioRuta() << "\n";
            if (pasajero.getPrecioCabina() > 0) {
                cout << "\t\t\tEquipaje de Cabina: S/. " << pasajero.getPrecioCabina() << "\n";
            }
            if (pasajero.getPrecioBodega() > 0) {
                cout << "\t\t\tEquipaje de Bodega: S/. " << pasajero.getPrecioBodega() << "\n";
            }
            if (pasajero.getPrecioAsiento() > 0) {
                cout << "\t\t\tPrecio Asiento: S/. " << pasajero.getPrecioAsiento() << "\n";
            }
            float IGV = 0.18f * (pasajero.getPrecioRuta() + pasajero.calcularPrecioAdicional());
            cout << "\t\t\tImpuesto General a la Venta (18%): S/. " << IGV << "\n";
            cout << "\t\t\tCargo Administrativo: S/. 45.00\n";
            cout << "\t\t\tCargo Tarifa unificada de uso de aeropuerto: S/.30\n";
            cout << "\t\t\tPrecio Total: S/. " << pasajero.calcularPrecioPasajero() << "\n";
        }

        cout << "\t\t\t\t\\--------------------------------------------------\n";
        cout << "\t\t\tTotal a Pagar: S/. " << total << "\n";
        cout << "\t\t\tGracias por su compra!\n";
    }

    void generarComprobante() {
        ofstream archivo("Archivos//comprobante_pago.txt", ios::app);
        if (!archivo) {
            cout << "Error al crear el comprobante." << endl;
            return;
        }

        archivo << "\t\t\t--------------------- COMPROBANTE DE PAGO ---------------------\n";
        archivo << "\t\t\tTipo: " << tipoComprobante << "\n";
        archivo << "\t\t\tID Reserva: " << idReserva << "\n";
        archivo << "\t\t\t\t--------------------------------------------------\n";
        archivo << "\t\t\tDatos del Pagador:\n";
        archivo << "\t\t\tNombre: " << pagador.getNombre() << "\n";
        archivo << "\t\t\tApellido: " << pagador.getApellido() << "\n";
        archivo << "\t\t\tDNI: " << pagador.getDni() << "\n";
        archivo << "\t\t\t\t--------------------------------------------------\n\n";
        archivo << "\t\t\tFecha: " << reserva.getFecha() << "\n";
        archivo << "\t\t\tOrigen: " << reserva.getOrigen() << "\t Destino: " << reserva.getDestino() << "\n";
        archivo << "\t\t\tHora Salida: " << vueloSeleccionado.getHoraInicio() << "\tHora llegada: " << vueloSeleccionado.getHoraFin() << "\n";
        archivo << "\t\t\tDetalles de Pasajeros:\n";

        for ( auto& pasajero : pasajeros) {
            archivo << "\t\t\t\t--------------------------------------------------\n";
            archivo << "\t\t\tNombre: " << pasajero.getNombre() << "\n";
            archivo << "\t\t\tApellido: " << pasajero.getApellido() << "\n";
            archivo << "\t\t\tDNI: " << pasajero.getDni() << "\n";
            archivo << "\t\t\tAsiento: " << pasajero.getNumeroAsiento() << "\n";
            archivo << "\t\t\tPrecio Ruta: S/. " << pasajero.getPrecioRuta() << "\n";
            if (pasajero.getPrecioCabina() > 0) {
                archivo << "\t\t\tEquipaje de Cabina: S/. " << pasajero.getPrecioCabina() << "\n";
            }
            if (pasajero.getPrecioBodega() > 0) {
                archivo << "\t\t\tEquipaje de Bodega: S/. " << pasajero.getPrecioBodega() << "\n";
            }
            if (pasajero.getPrecioAsiento() > 0) {
                archivo << "\t\t\tPrecio Asiento: S/. " << pasajero.getPrecioAsiento() << "\n";
            }
            float IGV = 0.18f * (pasajero.getPrecioRuta() + pasajero.calcularPrecioAdicional());
            archivo << "\t\t\tImpuesto General a la Venta (18%): S/. " << IGV << "\n";
            archivo << "\t\t\tCargo Administrativo: S/. 45.00\n";
            archivo << "\t\t\tCargo Tarifa unificada de uso de aeropuerto: S/.30\n";
            archivo << "\t\t\tPrecio Total: S/. " << pasajero.calcularPrecioPasajero() << "\n";
        }

        archivo << "\t\t\t\t\\--------------------------------------------------\n";
        archivo << "\t\t\tTotal a Pagar: S/. " << total << "\n";
        archivo << "\t\t\tGracias por su compra!\n\n\n\n";
        archivo.close();
    }
};