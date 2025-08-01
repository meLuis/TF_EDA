﻿#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class GestorAsientos {
public:
    static void mostrarAsientosVuelo(const string& idVuelo) {
        system("cls");
        string archivoRuta = "Archivos//asientos//" + idVuelo + ".txt";
        ifstream archivo(archivoRuta);
        string asiento;
        int count = 0;

        if (!archivo.is_open()) {
            cout << "\t\t\tError al abrir el archivo de asientos para el vuelo " << idVuelo << ".\n";
            return;
        }

        cout << "\n";
        cout << setw(60) << "ASIENTOS DEL VUELO " << idVuelo << "\n\n";
        cout << "   A  B  C    D  E  F\n";

        int fila = 1;
        string linea;
        while (getline(archivo, linea)) {
            cout << setw(2) << fila << " ";

            stringstream ss(linea);
            int columna = 0;
            while (ss >> asiento) {
                if (columna == 3) cout << "   ";

                if (asiento == "XX") {
                    cout << " X ";
                }
                else {
                    cout << " O ";
                }

                columna++;
                count++;
            }
            cout << endl;
            fila++;
        }

        cout << "\n";
        cout << " O - Asiento disponible   X - Asiento ocupado\n";
        archivo.close();
    }

    static void mostrarAsientos() {
        system("cls");
        ifstream archivo("Archivos//asientos.txt");
        string asiento;
        int count = 0;

        if (!archivo.is_open()) {
            cout << "\t\t\tError al abrir el archivo de asientos.\n";
            return;
        }

        cout << "\n";
        cout << setw(60) << "ASIENTOS \n\n";

        while (archivo >> asiento) {
            cout << setw(15) << asiento;
            count++;

            if (count % 3 == 0) cout << "  | ";
            if (count % 6 == 0) cout << endl;
        }

        cout << "\n";
        archivo.close();
    }

    static bool ocuparAsientoVuelo(const string& idVuelo, const string& codigoAsiento) {
        string archivoRuta = "Archivos//asientos//" + idVuelo + ".txt";
        ifstream archivoLectura(archivoRuta);
        vector<vector<string>> asientos;
        bool encontrado = false;

        if (!archivoLectura.is_open()) {
            cout << "\t\t\tError al abrir el archivo de asientos para el vuelo " << idVuelo << ".\n";
            return false;
        }

        string linea;
        while (getline(archivoLectura, linea)) {
            vector<string> fila;
            stringstream ss(linea);
            string asiento;

            while (ss >> asiento) {
                if (asiento == codigoAsiento) {
                    fila.push_back("XX");
                    encontrado = true;
                }
                else {
                    fila.push_back(asiento);
                }
            }

            asientos.push_back(fila);
        }
        archivoLectura.close();

        if (!encontrado) {
            cout << "\t\t\tEse asiento no está disponible o no existe. Intente otro.\n";
            return false;
        }

        ofstream archivoEscritura(archivoRuta);
        for (const auto& fila : asientos) {
            for (const auto& asiento : fila) {
                archivoEscritura << asiento << " ";
            }
            archivoEscritura << endl;
        }
        archivoEscritura.close();

        return true;
    }

    static string asignarAsientoAleatorioVuelo(const string& idVuelo) {

        string archivoRuta = "Archivos//asientos//" + idVuelo + ".txt";
        ifstream archivo(archivoRuta);
        string asiento;
        int count = 0;

        if (!archivo.is_open()) {
            cout << "\t\t\tError al abrir el archivo de asientos para el vuelo " << idVuelo << ".\n";
            return "";
        }

        string linea;

        int cantidadLineas = 0;

        while (getline(archivo, linea)) {
            cantidadLineas++;
        }

        archivo.clear();                 // Limpia EOF y otros flags
        archivo.seekg(0, ios::beg);      // Regresa al principio del archivo

        int filaAleatoria = 0;
        int contadorTemporal = 1;

        while (true) {
            filaAleatoria = rand() % (cantidadLineas + 1);
            while (getline(archivo, linea)) {

                if (contadorTemporal == filaAleatoria) {

                    stringstream ss(linea);
                    while (ss >> asiento) {
                        if (asiento != "XX") {

                            archivo.close();
                            return asiento;

                        }

                    }
                }
                contadorTemporal++;
            }
        }
        archivo.close();
        return "";
    }
};