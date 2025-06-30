#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "MapaRutas.h"

using namespace std;

class Reserva
{
private:
    int idReserva;
    string fecha;
    string destino;
    string origen;
    string estado;
    int cantPasajeros;
public:
    Reserva() : fecha(""), destino(""), origen(""), cantPasajeros(0), estado("Activo") {}
    Reserva(string f, string d, string o, int c) : fecha(f), destino(d), origen(o), cantPasajeros(c), estado("Activo") {
        idReserva = 0;
    }

    ~Reserva() {}
    void setIdReserva(int id) { idReserva = id; }
    int getIdReserva() const { return idReserva; }
    string getFecha() { return fecha; }
    string getDestino() { return destino; }
    string getOrigen() { return origen; }
    int getCantPasajeros() { return cantPasajeros; }
    void setEstado(const string& nuevoEstado) { estado = nuevoEstado; }
    string getEstado() const { return estado; }

    void guardarEnArchivo(ofstream& archivo) const {
        archivo << endl << idReserva << "|" << fecha << "|" << destino << "|" << origen << "|" << cantPasajeros << "|" << estado << "|";
    }
    static vector<Reserva> leerReservasDesdeArchivo(const string& archivoReservas) {
        vector<Reserva> reservas;
        ifstream archivo(archivoReservas);
        string linea;

        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string parte;
            vector<string> partes;

            while (getline(ss, parte, '|')) {
                partes.push_back(parte);
            }

            if (partes.size() >= 6) {
                Reserva r(partes[1], partes[2], partes[3], stoi(partes[4]));
                r.setIdReserva(stoi(partes[0]));
                r.setEstado(partes[5]);
                reservas.push_back(r);
            }
        }

        return reservas;
    }

    void mostrar() const {
        cout << "\t\t\tID Reserva: " << idReserva << endl;
        cout << "\t\t\tFecha: " << fecha << endl;
        cout << "\t\t\tOrigen: " << origen << endl;
        cout << "\t\t\tDestino: " << destino << endl;
        cout << "\t\t\tCantidad de Pasajeros: " << cantPasajeros << endl;
        cout << "\t\t\tEstado: " << estado << endl;
    }


    static Reserva* buscarPorID(vector<Reserva>& reservas, int id) {
        for (auto& r : reservas) {
            if (r.getIdReserva() == id) {
                return &r;
            }
        }
        return nullptr;
    }


};
