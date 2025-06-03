#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Pasajero.h"
#include "Ruta.h"

using namespace std;

class Pago
{
private:
    vector<Pasajero> pasajeros;
    Reserva reserva;
    Ruta rutaSeleccionada;

    float total;
    int idReserva;

    string nombrePagador;
    string apellidoPagador;
    string dniPagador;
    int numTarjeta;
public:
    Pago() : total(0.0f), idReserva(0), numTarjeta(0) {}
    Pago(float total, const vector<Pasajero>& pasajeros, const Reserva& reserva, const Ruta rutaSeleccionada, int idReserva)
        : total(total), pasajeros(pasajeros), reserva(reserva), rutaSeleccionada(rutaSeleccionada), idReserva(idReserva) {
    }

    int getNumTarjeta() { return numTarjeta; }

    void ingresarDatosPagador() {
        cin.ignore();
        cout << "\n\t\t\tIngrese los datos del pagador:\n";
        cout << "\t\t\tNombre: ";
        getline(cin, nombrePagador);
        cout << "\t\t\tApellido: ";
        getline(cin, apellidoPagador);
        cout << "\t\t\tDNI: ";
        getline(cin, dniPagador);
        cout << "\t\t\tTarjeta: ";
        cin >> numTarjeta;
    }
    void mostrarComprobante() {

        cout << "\t\t\t--------------------- COMPROBANTE DE PAGO ---------------------\n";
        cout << "\t\t\tID Reserva: " << idReserva << "\n";
        cout << "\t\t\t\t--------------------------------------------------\n";
        cout << "\t\t\tDatos del Pagador:\n";
        cout << "\t\t\tNombre: " << nombrePagador << "\n";
        cout << "\t\t\tApellido: " << apellidoPagador << "\n";
        cout << "\t\t\tDNI: " << dniPagador << "\n";
        cout << "\t\t\t\t--------------------------------------------------\n\n";
        cout << "\t\t\tFecha: " << reserva.getFecha() << "\n";
        cout << "\t\t\tOrigen: " << reserva.getOrigen() << "\t Destino: " << reserva.getDestino() << "\n";
        cout << "\t\t\tHora Salida: " << rutaSeleccionada.getHoraInicio() << "\tHora llegada: " << rutaSeleccionada.getHoraFin() << "\n";
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

        cout << "\t\t\t\t\--------------------------------------------------\n";
        cout << "\t\t\tTotal a Pagar: S/. " << total << "\n";
        cout << "\t\t\tGracias por su compra!\n";
        ;
    }
    void generarComprobante() {
        ofstream archivo("Archivos//comprobante_pago.txt", ios::app);
        if (!archivo) {
            cout << "Error al crear el comprobante." << endl;
            return;
        }

        archivo << "\t\t\t--------------------- COMPROBANTE DE PAGO ---------------------\n";
        archivo << "\t\t\tID Reserva: " << idReserva << "\n";
        archivo << "\t\t\t\t--------------------------------------------------\n";
        archivo << "\t\t\tDatos del Pagador:\n";
        archivo << "\t\t\tNombre: " << nombrePagador << "\n";
        archivo << "\t\t\tApellido: " << apellidoPagador << "\n";
        archivo << "\t\t\tDNI: " << dniPagador << "\n";
        archivo << "\t\t\t\t--------------------------------------------------\n\n";
        archivo << "\t\t\tFecha: " << reserva.getFecha() << "\n";
        archivo << "\t\t\tOrigen: " << reserva.getOrigen() << "\t Destino: " << reserva.getDestino() << "\n";
        archivo << "\t\t\tHora Salida: " << rutaSeleccionada.getHoraInicio() << "\tHora llegada: " << rutaSeleccionada.getHoraFin() << "\n";
        archivo << "\t\t\tDetalles de Pasajeros:\n";

        for (auto& pasajero : pasajeros) {
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

        archivo << "\t\t\t\t\--------------------------------------------------\n";

        archivo << "\t\t\tTotal a Pagar: S/. " << total << "\n";
        archivo << "\t\t\tGracias por su compra!\n\n\n\n";
        archivo.close();
    }
};
