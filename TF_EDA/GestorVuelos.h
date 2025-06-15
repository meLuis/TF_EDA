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

    void cargarDesdeArchivo(string archivoNombre, string origenDestinoID) {
        cargarVuelosDesdeArchivo("Archivos//vuelos.txt", origenDestinoID);
    }

    void guardarVuelosEnArchivo() {
        string archivoNombre = "Archivos//vuelos.txt";
        ofstream archivo(archivoNombre);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar" << endl;
            return;
        }

        for (const auto& vuelo : vuelos) {
            archivo << vuelo.serializar() << endl;
        }

        archivo.close();
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

    void guardarVueloEnArchivo(const Vuelo& vuelo) {
        string archivoNombre = "Archivos//vuelos.txt";

        vector<string> lineas;
        bool encontrado = false;

        ifstream archivoLectura(archivoNombre);
        if (archivoLectura.is_open()) {
            string linea;
            while (getline(archivoLectura, linea)) {
                if (linea.find(vuelo.getID() + "|") == 0) {
                    // Reemplazar con la nueva información
                    lineas.push_back(vuelo.serializar());
                    encontrado = true;
                }
                else {
                    lineas.push_back(linea);
                }
            }
            archivoLectura.close();
        }

        // Escribir todos los vuelos de vuelta
        ofstream archivoEscritura(archivoNombre);
        if (!archivoEscritura.is_open()) {
            cout << "Error al abrir el archivo para guardar" << endl;
            return;
        }

        for (const auto& linea : lineas) {
            archivoEscritura << linea << endl;
        }

        if (!encontrado) {
            archivoEscritura << vuelo.serializar() << endl;
        }

        archivoEscritura.close();
    }

    void guardarRutaEnArchivo(int indice, const string& nombreArchivo) {
        Vuelo vuelo = getVuelo(indice);
        guardarVueloEnArchivo(vuelo);
    }

    bool reservarAsiento(int indiceVuelo, const string& numeroAsiento) {
        if (indiceVuelo < 0 || indiceVuelo >= vuelos.size()) {
            return false;
        }

        bool resultado = vuelos[indiceVuelo].reservarAsiento(numeroAsiento);

        if (resultado) {
            guardarVueloEnArchivo(vuelos[indiceVuelo]);
        }

        return resultado;
    }
};