#pragma once
#include <iomanip>
#include <cmath>
#include "MapaRutas.h"

class Vuelo {
private:

    string id;
    string codigoOrigen;
    string codigoDestino;
    string ciudadOrigen;
    string ciudadDestino;
    string paisOrigen;
    string paisDestino;
    bool esInternacional;
    string horaInicio;
    string horaFin;
    string duracion;
    float precio;

public:

    Vuelo() :
        id(""), codigoOrigen(""), codigoDestino(""),
        ciudadOrigen(""), ciudadDestino(""),
        paisOrigen(""), paisDestino(""),
        esInternacional(false),
        horaInicio(""), horaFin(""), duracion(""),
        precio(0.0f) {
    }


    Vuelo(const string& id,
        const string& codigoOrigen, const string& codigoDestino,
        const string& ciudadOrigen, const string& ciudadDestino,
        const string& paisOrigen, const string& paisDestino,
        bool esInternacional,
        const string& horaInicio, const string& horaFin,
        const string& duracion, float precio) :
        id(id), codigoOrigen(codigoOrigen), codigoDestino(codigoDestino),
        ciudadOrigen(ciudadOrigen), ciudadDestino(ciudadDestino),
        paisOrigen(paisOrigen), paisDestino(paisDestino),
        esInternacional(esInternacional),
        horaInicio(horaInicio), horaFin(horaFin), duracion(duracion),
        precio(precio) {
    }


    string getID() const { return id; }
    string getCodigoOrigen() const { return codigoOrigen; }
    string getCodigoDestino() const { return codigoDestino; }
    string getCiudadOrigen() const { return ciudadOrigen; }
    string getCiudadDestino() const { return ciudadDestino; }
    string getPaisOrigen() const { return paisOrigen; }
    string getPaisDestino() const { return paisDestino; }
    bool getEsInternacional() const { return esInternacional; }
    string getHoraInicio() const { return horaInicio; }
    string getHoraFin() const { return horaFin; }
    string getDuracion() const { return duracion; }
    float getPrecio() const { return precio; }

    int getDuracionEnMinutos() const {
        int posH = duracion.find('h');
        int posM = duracion.find('m');
        int horas = stoi(duracion.substr(0, posH));
        int minutos = stoi(duracion.substr(posH + 1, posM - posH - 1));
        return horas * 60 + minutos;
    }

    void mostrar() const {
        cout << horaInicio << " - " << horaFin;
        cout << " | Duracion: " << duracion;
        cout << " | Precio: $" << fixed << setprecision(2) << precio << endl;

        cout << "\t\t\t\t" << ciudadOrigen << " (" << codigoOrigen << ") ---> "
            << ciudadDestino << " (" << codigoDestino << ")" << endl << endl;
    }


    static Vuelo deserializar(const string& datos) {
        stringstream ss(datos);
        string id, codigoOrigen, codigoDestino;
        string ciudadOrigen, ciudadDestino, paisOrigen, paisDestino;
        string horaInicio, horaFin, duracion, esIntStr;
        float precio = 0.0f;
        bool esInternacional;

        getline(ss, id, '|');
        getline(ss, codigoOrigen, '|');
        getline(ss, codigoDestino, '|');
        getline(ss, ciudadOrigen, '|');
        getline(ss, ciudadDestino, '|');
        getline(ss, paisOrigen, '|');
        getline(ss, paisDestino, '|');
        getline(ss, esIntStr, '|');
        getline(ss, horaInicio, '|');
        getline(ss, horaFin, '|');
        getline(ss, duracion, '|');

        // Lectura segura del precio
        string precioStr;
        getline(ss, precioStr);
        try {
            if (!precioStr.empty()) {
                precio = stof(precioStr);
                if (precio < 0 || !isfinite(precio)) {
                    precio = 0.0f;
                }
            }
        }
        catch (...) {
            precio = 0.0f;
        }

        esInternacional = (esIntStr == "1");

        return Vuelo(id, codigoOrigen, codigoDestino, ciudadOrigen, ciudadDestino,
            paisOrigen, paisDestino, esInternacional, horaInicio, horaFin,
            duracion, precio);
    }


    string serializar() const {
        stringstream ss;
        ss << id << "|"
            << codigoOrigen << "|"
            << codigoDestino << "|"
            << ciudadOrigen << "|"
            << ciudadDestino << "|"
            << paisOrigen << "|"
            << paisDestino << "|"
            << (esInternacional ? "1" : "0") << "|"
            << horaInicio << "|"
            << horaFin << "|"
            << duracion << "|"
            << precio;

        return ss.str();
    }

    void setPrecio(float nuevoPrecio) {
        if (nuevoPrecio >= 0) {
            this->precio = nuevoPrecio;
        }
    }
};