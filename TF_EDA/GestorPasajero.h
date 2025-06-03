#pragma once
#include "Pasajero.h"
#include <fstream>

class GestorPasajero {
private:
	vector<Pasajero> pasajeros;
	float precioTotal;
public:
	GestorPasajero() {}
	GestorPasajero(vector<Pasajero>pasajeros) {
		this->pasajeros = pasajeros;
	}

	float getPrecioTotal() {
		precioTotal = 0;
		for (auto& pasajero : this->pasajeros) {
			precioTotal += pasajero.calcularPrecioPasajero();
		}
	}

	void guardarPasajerosEnArchivo(const string& nombreArchivo) {
		ofstream archivo(nombreArchivo, ios::app);
		if (!archivo) {
			cout << "Error al abrir el archivo." << endl;
			return;
		}
		for (auto& pasajero : this->pasajeros) {
			archivo << pasajero.getNombre() << "|" << pasajero.getApellido() << "|"
				<< pasajero.getDni() << "|" << pasajero.getNumeroAsiento() << "|"
				<< pasajero.getPrecioCabina() << "|" << pasajero.getPrecioBodega() << "|"
				<< pasajero.getPrecioAsiento() << "|" << pasajero.getPrecioRuta() << "|"
				<< pasajero.getIdReserva() << endl;
		}
		archivo.close();
	}

};
