#pragma once
#include "Ruta.h"
#include <fstream>
#include <functional>

class GestorRutas {
private:
    vector<Ruta> rutas;

    void selectionSort(vector<Ruta>& arr, function<bool(Ruta&, Ruta&)> criterio) {
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

    void bubbleSort(vector<Ruta>& arr, function<bool(Ruta&, Ruta&)> criterio) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (!criterio(arr[j], arr[j + 1])) {
                    Ruta temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }


public:
    Ruta getRuta(int indice) {
        if (indice < 0 || indice >= rutas.size()) {
            cout << "Índice fuera de rango." << endl;
            return Ruta();
        }
        return rutas[indice];
    }

    void cargarDesdeArchivo(string archivoNombre, string origenDestinoID) {
        rutas.clear();

        fstream archivo(archivoNombre, ios::in);
        if (!archivo.is_open()) {
            cout << "Error abriendo el archivo." << endl;
            return;
        }

        string linea;
        bool leyendo = false;

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            if (linea[0] == '#') {
                leyendo = (linea == "#" + origenDestinoID);
                continue;
            }

            if (leyendo) {
                vector<string> datos;
                string actual = "";

                for (int i = 0; i < linea.length(); i++) {
                    if (linea[i] == ',') {
                        datos.push_back(actual);
                        actual = "";
                    }
                    else {
                        actual += linea[i];
                    }
                }
                datos.push_back(actual);

                if (datos.size() >= 5) {
                    string horaInicio = datos[0];
                    string horaFin = datos[1];
                    string duracion = datos[2];
                    float precio = stof(datos.back());

                    vector<string> paradas;
                    for (int j = 3; j < datos.size() - 1; j++) {
                        paradas.push_back(datos[j]);
                    }

                    Ruta r(horaInicio, horaFin, duracion, paradas, precio);
                    rutas.push_back(r);
                }
            }
        }

        archivo.close();
    }
    void mostrarRutas(int index) {
        if (index >= rutas.size()) return;
        cout << "\n\n\t\t\t\t[" << index + 1 << "] ";
        rutas[index].mostrar();
        mostrarRutas(index + 1);
    }
    void imprimirRutas() {
        cout << "\t\t\t----------------------------- Vuelos Disponibles ------------------------------- ";
        mostrarRutas(0);
        cout << "\t\t\tElige tu vuelo: ";
    }
    void selectionSegunCriterio(function<bool(Ruta&, Ruta&)> criterio) {
        selectionSort(rutas, criterio);
    }

    void bubbleSegunCriterio(function<bool(Ruta&, Ruta&)> criterio) {
        bubbleSort(rutas, criterio);
    }

    void ordenarPorPrecio() {
        selectionSegunCriterio([](Ruta& a, Ruta& b) {
            return a.getPrecio() < b.getPrecio();
            });
    }

    void ordenarPorDuracion() {
        bubbleSegunCriterio([](Ruta& a, Ruta& b) {
            return a.getDuracionEnMinutos() < b.getDuracionEnMinutos();
            });
    }

    void guardarRutaEnArchivo(int indice, const string& nombreArchivo) {
        Ruta ruta = getRuta(indice);
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo.is_open()) {
            cout << "Error abriendo el archivo para guardar" << endl;
            return;
        }

        archivo << ruta.getHoraInicio() << "|" << ruta.getHoraFin() << "|" << ruta.getDuracion() << "|" << ruta.getPrecio() << "\n";
        archivo.close();
    }
};
