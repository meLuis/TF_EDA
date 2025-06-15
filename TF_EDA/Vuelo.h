#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <iomanip>
#include <sstream>
#include "MapaRutas.h"
using namespace std;

class Vuelo {
private:
    string horaInicio;
    string horaFin;
    string duracion;
    vector<string> paradas;
    float precio;

    string id;
    int origen;
    int destino;
    string codigoIATAOrigen;
    string codigoIATADestino;
    string ciudadOrigen;
    string ciudadDestino;
    string paisOrigen;
    string paisDestino;
    bool esInternacional;
    string fecha;
    int asientosDisponibles;
    int asientosTotales;

    string generarID() const {
        time_t now = time(0);
        tm timeinfo;
        localtime_s(&timeinfo, &now);

        stringstream ss;
        ss << codigoIATAOrigen << "-" << codigoIATADestino << "-"
            << (1900 + timeinfo.tm_year) % 100
            << setfill('0') << setw(2) << (1 + timeinfo.tm_mon)
            << setfill('0') << setw(2) << timeinfo.tm_mday << "-";

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1000, 9999);
        ss << dis(gen);

        return ss.str();
    }

public:
    Vuelo() :
        horaInicio(""), horaFin(""), duracion(""), precio(0.0f),
        origen(-1), destino(-1), codigoIATAOrigen(""), codigoIATADestino(""),
        ciudadOrigen(""), ciudadDestino(""), paisOrigen(""), paisDestino(""),
        esInternacional(false), fecha(""), asientosDisponibles(0), asientosTotales(0) {
    }

    Vuelo(string horaInicio, string horaFin, string duracion, vector<string> paradas, float precio) :
        horaInicio(horaInicio), horaFin(horaFin), duracion(duracion),
        paradas(paradas), precio(precio), origen(-1), destino(-1),
        codigoIATAOrigen(""), codigoIATADestino(""), ciudadOrigen(""), ciudadDestino(""),
        paisOrigen(""), paisDestino(""), esInternacional(false),
        fecha(""), asientosDisponibles(0), asientosTotales(0) {
    }

    Vuelo(int origen, int destino, const string& fecha, const string& horaInicio,
        const string& horaFin, const string& duracion, float precio,
        int asientosTotales, MapaRutas& mapaRutas) :
        horaInicio(horaInicio), horaFin(horaFin), duracion(duracion), precio(precio),
        origen(origen), destino(destino), fecha(fecha),
        asientosTotales(asientosTotales), asientosDisponibles(asientosTotales) {

        ciudadOrigen = mapaRutas.getNombreCiudad(origen);
        ciudadDestino = mapaRutas.getNombreCiudad(destino);
        paisOrigen = mapaRutas.getNombrePais(origen);
        paisDestino = mapaRutas.getNombrePais(destino);
        codigoIATAOrigen = mapaRutas.getSiglasLugar(origen);
        codigoIATADestino = mapaRutas.getSiglasLugar(destino);
        esInternacional = mapaRutas.esVueloInternacional(origen, destino);

        id = generarID();
    }

    Vuelo(const string& id, int origen, int destino,
        const string& codigoIATAOrigen, const string& codigoIATADestino,
        const string& ciudadOrigen, const string& ciudadDestino,
        const string& paisOrigen, const string& paisDestino,
        bool esInternacional, const string& fecha,
        const string& horaInicio, const string& horaFin,
        const string& duracion, float precio,
        int asientosDisponibles, int asientosTotales) :
        id(id), origen(origen), destino(destino),
        codigoIATAOrigen(codigoIATAOrigen), codigoIATADestino(codigoIATADestino),
        ciudadOrigen(ciudadOrigen), ciudadDestino(ciudadDestino),
        paisOrigen(paisOrigen), paisDestino(paisDestino),
        esInternacional(esInternacional), fecha(fecha),
        horaInicio(horaInicio), horaFin(horaFin),
        duracion(duracion), precio(precio),
        asientosDisponibles(asientosDisponibles),
        asientosTotales(asientosTotales) {
    }

    float getPrecio() const { return precio; }
    string getDuracion() const { return duracion; }
    string getHoraInicio() const { return horaInicio; }
    string getHoraFin() const { return horaFin; }

    string getID() const { return id; }
    int getOrigen() const { return origen; }
    int getDestino() const { return destino; }
    string getCodigoIATAOrigen() const { return codigoIATAOrigen; }
    string getCodigoIATADestino() const { return codigoIATADestino; }
    string getCiudadOrigen() const { return ciudadOrigen; }
    string getCiudadDestino() const { return ciudadDestino; }
    string getPaisOrigen() const { return paisOrigen; }
    string getPaisDestino() const { return paisDestino; }
    bool getEsInternacional() const { return esInternacional; }
    string getFecha() const { return fecha; }
    int getAsientosDisponibles() const { return asientosDisponibles; }
    int getAsientosTotales() const { return asientosTotales; }

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
        cout << " | Precio: $" << precio << endl;

        cout << "\t\t\t\t" << ciudadOrigen << " (" << codigoIATAOrigen << ") ---> "
            << ciudadDestino << " (" << codigoIATADestino << ")" << endl;
        cout << "\t\t\t\tFecha: " << fecha;
        cout << " | Asientos disponibles: " << asientosDisponibles
            << " de " << asientosTotales << endl << endl;
    }

    bool reservarAsiento(const string& numeroAsiento) {
        if (asientosDisponibles <= 0) {
            return false;
        }

        string nombreArchivo = "Archivos//asientos//" + id + ".txt";
        ifstream archivoLectura(nombreArchivo);

        if (!archivoLectura.is_open()) {
            return false;
        }

        vector<vector<string>> asientos;
        string linea;

        while (getline(archivoLectura, linea)) {
            vector<string> fila;
            stringstream ss(linea);
            string asiento;

            while (ss >> asiento) {
                fila.push_back(asiento);
            }

            asientos.push_back(fila);
        }

        archivoLectura.close();

        bool encontrado = false;
        for (auto& fila : asientos) {
            for (auto& asiento : fila) {
                if (asiento == numeroAsiento) {
                    asiento = "XX";
                    encontrado = true;
                    asientosDisponibles--;
                    break;
                }
            }
            if (encontrado) break;
        }

        if (!encontrado) {
            return false;
        }

        ofstream archivoEscritura(nombreArchivo);
        for (const auto& fila : asientos) {
            for (const auto& asiento : fila) {
                archivoEscritura << asiento << " ";
            }
            archivoEscritura << endl;
        }

        return true;
    }

    string serializar() const {
        stringstream ss;
        ss << id << "|"
            << origen << "|"
            << destino << "|"
            << codigoIATAOrigen << "|"
            << codigoIATADestino << "|"
            << ciudadOrigen << "|"
            << ciudadDestino << "|"
            << paisOrigen << "|"
            << paisDestino << "|"
            << (esInternacional ? "1" : "0") << "|"
            << fecha << "|"
            << horaInicio << "|"
            << horaFin << "|"
            << duracion << "|"
            << precio << "|"
            << asientosDisponibles << "|"
            << asientosTotales;

        return ss.str();
    }

    static Vuelo deserializar(const string& datos) {
        stringstream ss(datos);
        string id, codigoIATAOrigen, codigoIATADestino;
        string ciudadOrigen, ciudadDestino, paisOrigen, paisDestino;
        string fecha, horaInicio, horaFin, duracion, esIntStr;
        int origen, destino, asientosDisponibles, asientosTotales;
        float precio;
        bool esInternacional;

        getline(ss, id, '|');
        ss >> origen; ss.ignore();
        ss >> destino; ss.ignore();
        getline(ss, codigoIATAOrigen, '|');
        getline(ss, codigoIATADestino, '|');
        getline(ss, ciudadOrigen, '|');
        getline(ss, ciudadDestino, '|');
        getline(ss, paisOrigen, '|');
        getline(ss, paisDestino, '|');
        getline(ss, esIntStr, '|');
        getline(ss, fecha, '|');
        getline(ss, horaInicio, '|');
        getline(ss, horaFin, '|');
        getline(ss, duracion, '|');
        ss >> precio; ss.ignore();
        ss >> asientosDisponibles; ss.ignore();
        ss >> asientosTotales;

        esInternacional = (esIntStr == "1");

        return Vuelo(id, origen, destino, codigoIATAOrigen, codigoIATADestino,
            ciudadOrigen, ciudadDestino, paisOrigen, paisDestino,
            esInternacional, fecha, horaInicio, horaFin, duracion,
            precio, asientosDisponibles, asientosTotales);
    }
};