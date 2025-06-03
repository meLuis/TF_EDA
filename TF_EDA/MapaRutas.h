#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class MapaRutas {
private:
    enum Lugares {
        TPP = 0,
        PIU,
        CIX,
        CJA,
        TRU,
        LIM,
        CUZ,
        AQP,
        NUM_LUGARES
    };

    string nombres_lugares[NUM_LUGARES] = {
        "Tarapoto (TPP)",
        "Piura (PIU)",
        "Chiclayo (CIX)",
        "Cajamarca (CJA)",
        "Trujillo (TRU)",
        "Lima (LIM)",
        "Cusco (CUZ)",
        "Arequipa (AQP)"
    };

    int matriz_conexiones[NUM_LUGARES][NUM_LUGARES] = {
        {  0,  0,  1,  0,  0,  1,  1,  1 },
        {  0,  0,  1,  1,  1,  1,  1,  1 },
        {  1,  1,  0,  1,  1,  1,  1,  0 },
        {  1,  1,  1,  0,  1,  1,  1,  1 },
        {  0,  1,  1,  1,  0,  0,  1,  1 },
        {  1,  1,  1,  1,  0,  0,  1,  1 },
        {  1,  1,  1,  1,  1,  1,  0,  1 },
        {  1,  1,  0,  1,  1,  1,  1,  0 }
    };

public:
    MapaRutas() {}
    ~MapaRutas() {}

    int seleccionarOrigen() {
        int origen;
        bool seleccion_valida = false;

        auto mostrarLugares = [&]() {
            for (int i = 0; i < NUM_LUGARES; i++) {
                cout << "\t\t\t\t\t" << i + 1 << ". " << nombres_lugares[i] << endl;
            }
            cout << "\t\t\t\t\t0. Salir" << endl;
            };

        auto entradaValida = [&](int entrada) {
            return entrada >= 1 && entrada <= NUM_LUGARES;
            };

        while (!seleccion_valida) {
            mostrarLugares();
            cout << "\t\t\t\t\tIngrese su eleccion: ";
            cin >> origen;

            if (origen == 0) return -1;
            else if (entradaValida(origen)) seleccion_valida = true;
            else cout << "\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
        }

        return origen - 1;
    }

    int seleccionarDestino(int origen) {
        int destino;
        bool seleccion_valida = false;
        vector<int> destinos_disponibles;

        auto mostrarDestinosDisponibles = [&]() {
            for (int i = 0; i < NUM_LUGARES; i++) {
                if (matriz_conexiones[origen][i] == 1 && i != origen) {
                    cout << "\t\t\t\t\t" << i + 1 << ". " << nombres_lugares[i] << endl;
                    destinos_disponibles.push_back(i);
                }
            }
            cout << "\t\t\t\t\t0. Volver al menu principal" << endl;
            };

        mostrarDestinosDisponibles();

        while (!seleccion_valida) {
            cout << "\t\t\t\t\tIngrese su eleccion: ";
            cin >> destino;

            if (destino == 0) return -1;

            auto esDestinoValido = [&](int valor) {
                return valor == destino - 1;
                };

            for (int i : destinos_disponibles) {
                if (esDestinoValido(i)) {
                    seleccion_valida = true;
                    break;
                }
            }

            if (!seleccion_valida) {
                cout << "\t\t\t\tOpción invalida. Intente nuevamente." << endl;
            }
        }

        return destino - 1;
    }

    string getNombreLugar(int indice) const {
        if (indice >= 0 && indice < NUM_LUGARES) {
            return nombres_lugares[indice];
        }
        return "No valido";
    }

    string getSiglasLugar(int indice) {
        switch (indice) {
        case TPP: return "TPP";
        case PIU: return "PIU";
        case CIX: return "CIX";
        case CJA: return "CJA";
        case TRU: return "TRU";
        case LIM: return "LIM";
        case CUZ: return "CUZ";
        case AQP: return "AQP";
        default: return "Desconocido";
        }
    }

    int getNumLugares() {
        return NUM_LUGARES;
    }

    string getIDRuta(int origen, int destino) {
        return to_string(origen + 1) + "-" + to_string(destino + 1);
    }

    bool existeRuta(int origen, int destino) {
        return matriz_conexiones[origen][destino] == 1;
    }
};