#pragma once
#include <iostream>
#include "Reserva.h"
#include "Nodo.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace  std;

template <typename T>
class ListaReservas {
private:
    Nodo<T>* cabeza;
    set<int> idsExistentes;

public:
    ListaReservas() : cabeza(nullptr) {
        srand(time(0));
        cargarIdsExistentes();
    }

    ~ListaReservas() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }
    void cargarIdsExistentes() {
        ifstream archivo("Archivos//reserva.txt");
        if (!archivo) return;

        string linea;
        while (getline(archivo, linea)) {
            size_t pos = linea.find('|');
            if (pos != string::npos) {
                int id = stoi(linea.substr(0, pos));
                idsExistentes.insert(id);
            }
        }
        archivo.close();
    }

    int generarIdUnico() {
        int id;
        do {
            id = 1000 + rand() % 9000;
        } while (idsExistentes.find(id) != idsExistentes.end());
        idsExistentes.insert(id);
        return id;
    }

    void agregarReserva(T& reserva) {
        reserva.setIdReserva(generarIdUnico());
        Nodo<T>* nuevoNodo = new Nodo<T>(reserva);
        if (!cabeza) {
            cabeza = nuevoNodo;
        }
        else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    void guardarListaEnArchivo(string& archivoReservas) {
        ofstream archivo(archivoReservas, ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo para guardar reservas." << endl;
            return;
        }
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            actual->dato.guardarEnArchivo(archivo);
            actual = actual->siguiente;
        }
        archivo.close();
    }
};