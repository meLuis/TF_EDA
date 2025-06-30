#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "Reserva.h"
#include "GestorAsientos.h"
using namespace std;

class Pasajero {
private:
	string nombre;
	string apellido;
	string dni;
	string numeroAsiento;
	float precioCabina;
	float precioBodega;
	float precioAsiento;
	float precioRuta;
	int idReserva;
	string idVuelo; // Añadir ID del vuelo

	bool pedirOpcion(string mensaje) {
		string opcion;
		cout << mensaje; cin >> opcion;
		for (auto& c : opcion) c = toupper(c);
		return (opcion == "SI");
	}

public:

	Pasajero(float precioRuta, int idReserva, const string& idVuelo) : nombre(""), apellido(""), dni(""), numeroAsiento(""),
		precioCabina(0.0f), precioBodega(0.0f), precioAsiento(0.0f), precioRuta(precioRuta), idReserva(idReserva), idVuelo(idVuelo) {
	}

	Pasajero(float precioRuta, float precioCabina, float precioBodega, float precioAsiento, int idReserva, const string& idVuelo)
		: nombre(""), apellido(""), dni(""), numeroAsiento(""),
		precioCabina(precioCabina), precioBodega(precioBodega),
		precioAsiento(precioAsiento), precioRuta(precioRuta), idReserva(idReserva), idVuelo(idVuelo) {
	}

	void setNombre(string nombre) { this->nombre = nombre; }
	void setApellido(string apellido) { this->apellido = apellido; }
	void setDni(int dni) { this->dni = dni; }
	void setNumeroAsiento(string numeroAsiento) { this->numeroAsiento = numeroAsiento; }
	void setPrecioCabina(float precioCabina) { this->precioCabina = precioCabina; }
	void setPrecioBodega(float precioBodega) { this->precioBodega = precioBodega; }
	void setPrecioAsiento(float precioAsiento) { this->precioAsiento = precioAsiento; }

	string getNombre() { return this->nombre; }
	string getApellido() { return this->apellido; }
	string getDni() { return this->dni; }
	string getNumeroAsiento() { return this->numeroAsiento; }
	float getPrecioCabina() { return this->precioCabina; }
	float getPrecioBodega() { return this->precioBodega; }
	float getPrecioAsiento() { return this->precioAsiento; }
	float getPrecioRuta() { return this->precioRuta; }
	int getIdReserva() { return this->idReserva; }
	string getIdVuelo() const { return idVuelo; }

	void seleccionarEquipaje() {
		if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de cabina? (SI/NO): ")) this->precioCabina = 97.04f;
		if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de bodega? (SI/NO): ")) this->precioBodega = 102.54f;
	}

	void seleccionarAsiento() {
		cin.ignore(); // Limpiar buffer de entrada
		cout << "\t\t\t\t\t Desea elegir un asiento? (SI/NO): ";
		string respuesta;
		getline(cin, respuesta);
		for (auto& c : respuesta) c = toupper(c);

		if (respuesta == "SI" || respuesta == "S") {
			string asiento;
			while (true) {
				cout << "\t\tAsientos disponibles para el vuelo " << idVuelo << ":" << endl;
				GestorAsientos::mostrarAsientosVuelo(idVuelo);
				cout << "\t\t\t\tIngrese el numero de asiento (ej: 1A): ";
				cin >> asiento;
				if (GestorAsientos::ocuparAsientoVuelo(idVuelo, asiento)) {
					this->numeroAsiento = asiento;
					this->precioAsiento = 50.0f;
					break;
				}
			}
		}
		else {
			cout << "\t\t\t\t\tSe asignara un asiento aleatorio más adelante." << endl;
		}
	}

	float calcularPrecioAdicional() {
		float precioAdicional = 0.0f;
		if (this->precioCabina) precioAdicional += this->precioCabina;
		if (this->precioBodega) precioAdicional += this->precioBodega;
		if (!this->numeroAsiento.empty()) precioAdicional += this->precioAsiento;
		return precioAdicional;
	}

	float calcularPrecioPasajero() {
		float precioAdicional = this->calcularPrecioAdicional();
		return this->precioRuta + precioAdicional + (0.18f * (this->precioRuta + precioAdicional)) + 45.0f + 30.0f;// 0.18IGV + 45.0 cargo administrativo + 30.0 cargo TUUA Tarifa unificada de uso de aeropuerto 
	}
	void pedirDatosPersonales() {
		cout << "\t\t\tNombre: ";
		getline(cin, nombre);

		cout << "\t\t\tApellido: ";
		getline(cin, apellido);

		do {
			cout << "\t\t\tDNI (8 dígitos): ";
			cin >> dni;
			cin.ignore();

			if (dni.length() != 8 || !all_of(dni.begin(), dni.end(), ::isdigit)) {
				cout << "\t\t\tDNI inválido. Debe contener exactamente 8 dígitos numéricos.\n";
			}
			else {
				break;
			}

		} while (true);
	}
	void mostrarResumen() {
		cout << "\t\t\tNombre: " << this->nombre << endl;
		cout << "\t\t\tApellido: " << this->apellido << endl;
		cout << "\t\t\tDNI: " << this->dni << endl;
		cout << "\t\t\tEquipaje de cabina: " << (this->precioCabina ? "Si" : "No") << endl;
		cout << "\t\t\tEquipaje de bodega: " << (this->precioBodega ? "Si" : "No") << endl;
		cout << "\t\t\tAsiento elegido: " << (this->numeroAsiento.empty() ? "No" : this->numeroAsiento) << endl;
		cout << "\t\t\tPrecio total: S/." << this->calcularPrecioPasajero() << endl << endl;
	}

};
