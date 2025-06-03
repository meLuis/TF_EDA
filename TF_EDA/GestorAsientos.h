#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class GestorAsientos {
public:

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

            if (count % 3 == 0) cout << "  | "; // Pasillo visual después de 3 asientos
            if (count % 6 == 0) cout << endl;  // Nueva fila cada 6 asientos
        }

        cout << "\n";
        archivo.close();
    }

    static bool ocuparAsiento(const string& codigoAsiento) {
        ifstream archivoLectura("Archivos//asientos.txt");
        vector<string> asientos;
        string asiento;
        bool encontrado = false;

        if (!archivoLectura.is_open()) {
            cout << "\t\t\tError al abrir el archivo de asientos.\n";
            return false;
        }

        while (archivoLectura >> asiento) {
            if (asiento == codigoAsiento && asiento != "XX") {
                asientos.push_back("XX");
                encontrado = true;
            }
            else {
                asientos.push_back(asiento);
            }
        }
        archivoLectura.close();

        if (!encontrado) {
            cout << "\t\t\tEse asiento no está disponible o no existe. Intente otro.\n";
            return false;
        }

        ofstream archivoEscritura("Archivos//asientos.txt");
        for (size_t i = 0; i < asientos.size(); ++i) {
            archivoEscritura << asientos[i] << " ";
            if ((i + 1) % 6 == 0) archivoEscritura << endl; // 6 asientos por fila
        }
        archivoEscritura.close();

        return true;
    }
};