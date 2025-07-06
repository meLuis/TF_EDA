
#pragma once
#include "MapaRutas.h"

class GrafoVuelos {
private:
    vector<vector<int>> matrizPonderada;
    MapaRutas* mapaRutas;

public:
    GrafoVuelos(MapaRutas* mapa) {
        mapaRutas = mapa;
        cargarMatrizPonderada();
    }

    void cargarMatrizPonderada() {
        int totalCiudades = mapaRutas->getNumLugares();
        matrizPonderada.resize(totalCiudades, vector<int>(totalCiudades, 0));


        ifstream archivo("Archivos//conexiones_ponderadas.txt");
        if (archivo.is_open()) {
            string linea;
            getline(archivo, linea);

            int fila = 0;
            while (getline(archivo, linea) && fila < totalCiudades) {
                istringstream iss(linea);
                string codigo;
                iss >> codigo;

                for (int col = 0; col < totalCiudades; col++) {
                    int valor;
                    iss >> valor;
                    matrizPonderada[fila][col] = valor;
                }
                fila++;
            }
            archivo.close();
        }
    }


    void visualizarGrafo() {
        cout << "\n\t\t\t---------- VISUALIZACION DEL GRAFO DE CONEXIONES ----------\n\n";


        int numNodosMostrados = 50;
        cout << "    ";
        for (int i = 0; i < numNodosMostrados; i++) {
            cout << mapaRutas->getSiglasLugar(i) << " ";
        }
        cout << endl;

        for (int i = 0; i < numNodosMostrados; i++) {
            cout << mapaRutas->getSiglasLugar(i) << " | ";
            for (int j = 0; j < numNodosMostrados; j++) {
                if (matrizPonderada[i][j] > 0) {
                    cout << matrizPonderada[i][j] << " ";
                }
                else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }

        cout << "\nLeyenda: 0 = Sin conexion, 1-2 = Conexion cercana, 3-4 = Conexion media, 5-6 = Conexion lejana" << endl;
    }


    void analizarCentralidad() {
        vector<pair<int, string>> gradosNodos;

        for (int i = 0; i < matrizPonderada.size(); i++) {
            int conexiones = 0;
            for (int j = 0; j < matrizPonderada[i].size(); j++) {
                if (matrizPonderada[i][j] > 0) conexiones++;
            }

            string nombreCiudad = mapaRutas->getNombreLugar(i);
            gradosNodos.push_back({ conexiones, nombreCiudad });
        }


        sort(gradosNodos.begin(), gradosNodos.end(),
            [](const pair<int, string>& a, const pair<int, string>& b) {
                return a.first > b.first;
            });

        cout << "\n\t\t\t-------- ANALISIS DE CENTRALIDAD DE AEROPUERTOS --------\n\n";
        cout << "\t\t\tLos aeropuertos más importantes por número de conexiones son:\n";

        for (int i = 0; i < min(10, (int)gradosNodos.size()); i++) {
            cout << "\t\t\t" << i + 1 << ". " << gradosNodos[i].second
                << ": " << gradosNodos[i].first << " conexiones\n";
        }
    }


    void analizarEficienciaRed() {
        cout << "\n\t\t\t-------- ANALISIS DE EFICIENCIA DE LA RED --------\n\n";


        int totalNodos = matrizPonderada.size();
        int totalConexiones = 0;
        int maxConexiones = 0;
        int minConexiones = INT_MAX;
        string nodoMaxConexiones = "";
        string nodoMinConexiones = "";

        for (int i = 0; i < totalNodos; i++) {
            int conexiones = 0;
            for (int j = 0; j < totalNodos; j++) {
                if (matrizPonderada[i][j] > 0) conexiones++;
            }

            totalConexiones += conexiones;

            if (conexiones > maxConexiones) {
                maxConexiones = conexiones;
                nodoMaxConexiones = mapaRutas->getSiglasLugar(i);
            }

            if (conexiones < minConexiones && conexiones > 0) {
                minConexiones = conexiones;
                nodoMinConexiones = mapaRutas->getSiglasLugar(i);
            }
        }

        float conexionesPorNodo = (float)totalConexiones / totalNodos;

        cout << "\t\t\tEstadisticas de la red:\n";
        cout << "\t\t\t- Total de ciudad: " << totalNodos << "\n";
        cout << "\t\t\t- Total de conexiones: " << totalConexiones / 2 << "\n"; // Dividir por 2 para no contar duplicados
        cout << "\t\t\t- Promedio de conexiones por ciudad: " << conexionesPorNodo / 2 << "\n";
        cout << "\t\t\t- Ciudad con más conexiones: " << nodoMaxConexiones << " (" << maxConexiones << ")\n";
        cout << "\t\t\t- Ciudad con menos conexiones: " << nodoMinConexiones << " (" << minConexiones << ")\n\n";


    }
};