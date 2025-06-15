#pragma once
#include "Vuelo.h"
#include <fstream>
#include <functional>
#include <sstream>

class GestorVuelos {
private:
    vector<Vuelo> vuelos;

    void selectionSort(vector<Vuelo>& arr, function<bool(Vuelo&, Vuelo&)> criterio) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                if (!criterio(arr[minIdx], arr[j])) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                swap(arr[i], arr[minIdx]);
            }
        }
    }

    void bubbleSort(vector<Vuelo>& arr, function<bool(Vuelo&, Vuelo&)> criterio) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (!criterio(arr[j], arr[j + 1])) {
                    Vuelo temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

public:
    GestorVuelos() {}

    ~GestorVuelos() {}

    Vuelo getVuelo(int indice) {
        if (indice < 0 || indice >= vuelos.size()) {
            cout << "Índice fuera de rango." << endl;
            return Vuelo();
        }
        return vuelos[indice];
    }

    // Cargar vuelos desde el archivo vuelos.txt
    void cargarVuelosDesdeArchivo(const string& archivoNombre, const string& rutaCodigo) {
        vuelos.clear();

        ifstream archivo(archivoNombre);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de vuelos." << endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            string idVuelo = linea.substr(0, linea.find('|'));
            if (idVuelo.find(rutaCodigo) != string::npos) {
                vuelos.push_back(Vuelo::deserializar(linea));
            }
        }

        archivo.close();
    }

    // Para compatibilidad con el código anterior
    void cargarDesdeArchivo(string archivoNombre, string origenDestinoID) {
        cargarVuelosDesdeArchivo("Archivos//vuelos.txt", origenDestinoID);
    }

    void mostrarVuelos(int index) {
        if (index >= vuelos.size()) return;
        cout << "\n\n\t\t\t\t[" << index + 1 << "] ";
        vuelos[index].mostrar();
        mostrarVuelos(index + 1);
    }

    void imprimirVuelos() {
        cout << "\t\t\t----------------------------- Vuelos Disponibles ------------------------------- ";
        mostrarVuelos(0);
        cout << "\t\t\tElige tu vuelo: ";
    }

    // Para compatibilidad con el código anterior
    void imprimirRutas() {
        imprimirVuelos();
    }

    void selectionSegunCriterio(function<bool(Vuelo&, Vuelo&)> criterio) {
        selectionSort(vuelos, criterio);
    }

    void bubbleSegunCriterio(function<bool(Vuelo&, Vuelo&)> criterio) {
        bubbleSort(vuelos, criterio);
    }

    void ordenarPorPrecio() {
        selectionSegunCriterio([](Vuelo& a, Vuelo& b) {
            return a.getPrecio() < b.getPrecio();
            });
    }

    void ordenarPorDuracion() {
        bubbleSegunCriterio([](Vuelo& a, Vuelo& b) {
            return a.getDuracionEnMinutos() < b.getDuracionEnMinutos();
            });
    }

    void guardarVueloEnArchivo(int indice, const string& nombreArchivo) {
        Vuelo vuelo = getVuelo(indice);
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar" << endl;
            return;
        }

        archivo << vuelo.getHoraInicio() << "|" << vuelo.getHoraFin() << "|"
            << vuelo.getDuracion() << "|" << vuelo.getPrecio() << "\n";
        archivo.close();
    }

    // Para compatibilidad con el código anterior
    void guardarRutaEnArchivo(int indice, const string& nombreArchivo) {
        guardarVueloEnArchivo(indice, nombreArchivo);
    }

    // Nuevos métodos específicos para Vuelo
    bool reservarAsiento(int indiceVuelo, const string& numeroAsiento) {
        if (indiceVuelo < 0 || indiceVuelo >= vuelos.size()) {
            return false;
        }

        return vuelos[indiceVuelo].reservarAsiento(numeroAsiento);
    }
};