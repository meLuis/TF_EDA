#pragma once
#include <iostream>
#include <vector>
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

	bool pedirOpcion(string mensaje) {
		char opcion;
		cout << mensaje; cin >> opcion;
		return (opcion == 's' || opcion == 'S');
	}

public:

	Pasajero(float precioRuta, int idReserva) : nombre(""), apellido(""), dni(""), numeroAsiento(""),
		precioCabina(0.0f), precioBodega(0.0f), precioAsiento(0.0f), precioRuta(precioRuta), idReserva(idReserva) {
	}

	Pasajero(float precioRuta, float precioCabina, float precioBodega, float precioAsiento, int idReserva)
		: nombre(""), apellido(""), dni(""), numeroAsiento(""),
		precioCabina(precioCabina), precioBodega(precioBodega),
		precioAsiento(precioAsiento), precioRuta(precioRuta), idReserva(idReserva) {
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

	void seleccionarEquipaje() {
		if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de cabina? (s/n): ")) this->precioCabina = 97.04f;
		if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de bodega? (s/n): ")) this->precioBodega = 102.54f;
	}

	void seleccionarAsiento() {
		cin.ignore(); // Limpiar buffer de entrada
		cout << "\t\t\t\t\t Desea elegir un asiento? (SI/NO): ";
		string respuesta;
		getline(cin, respuesta);
		for (auto& c : respuesta) c = toupper(c);

		if (respuesta == "SI") {
			string asiento;
			while (true) {
				cout << "\t\tAsientos:" << endl;
				GestorAsientos::mostrarAsientos();
				cout << "\t\t\t\tIngrese el numero de asiento (ej: 1A): ";
				cin >> asiento;
				if (GestorAsientos::ocuparAsiento(asiento)) {
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
		cin >> nombre;
		cout << "\t\t\tApellido: ";
		cin >> apellido;
		cout << "\t\t\tDNI: ";
		cin >> dni;

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
