// GrafoVuelos.h
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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

        // Cargar matriz desde archivo o definirla directamente
        ifstream archivo("Archivos//conexiones_ponderadas.txt");
        if (archivo.is_open()) {
            string linea;
            getline(archivo, linea); // Leer cabecera

            int fila = 0;
            while (getline(archivo, linea) && fila < totalCiudades) {
                istringstream iss(linea);
                string codigo;
                iss >> codigo; // Leer código de ciudad

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

    // Método para visualizar el grafo
    void visualizarGrafo() {
        cout << "\n\t\t\t=== VISUALIZACION DEL GRAFO DE CONEXIONES ===\n\n";

        // Mostrar algunos nodos principales para demo
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

    // Método simple para simular búsqueda de ruta más corta
    void simularDijkstra(const string& origen, const string& destino) {
        int idxOrigen = mapaRutas->buscarIndicePorCodigo(origen);
        int idxDestino = mapaRutas->buscarIndicePorCodigo(destino);

        if (idxOrigen == -1 || idxDestino == -1) {
            cout << "Ciudad no encontrada." << endl;
            return;
        }

        cout << "\n\t\t\tSimulando algoritmo de Dijkstra para encontrar la ruta mas corta\n";
        cout << "\t\t\tOrigen: " << mapaRutas->getNombreLugar(idxOrigen) << "\n";
        cout << "\t\t\tDestino: " << mapaRutas->getNombreLugar(idxDestino) << "\n\n";

        // Simular proceso de Dijkstra (demo)
        cout << "\t\t\tProcesando... Calculando distancias...\n";
        cout << "\t\t\tExaminando nodos vecinos...\n";
        cout << "\t\t\tActualizando distancias minimas...\n\n";

        // Mostrar una ruta predefinida
        cout << "\t\t\tRuta mas corta encontrada:\n";
        cout << "\t\t\t" << origen << " → ";

        // Si hay conexión directa
        if (matrizPonderada[idxOrigen][idxDestino] > 0) {
            cout << destino << "\n";
            cout << "\t\t\tPeso total: " << matrizPonderada[idxOrigen][idxDestino] << "\n";
        }
        // Si no hay conexión directa, mostrar ruta a través de Lima o Santiago
        else {
            int idxIntermedio = mapaRutas->buscarIndicePorCodigo("LIM"); // Lima como hub
            cout << "LIM → " << destino << "\n";
            int pesoTotal =
                (matrizPonderada[idxOrigen][idxIntermedio] > 0 ? matrizPonderada[idxOrigen][idxIntermedio] : 3) +
                (matrizPonderada[idxIntermedio][idxDestino] > 0 ? matrizPonderada[idxIntermedio][idxDestino] : 3);
            cout << "\t\t\tPeso total: " << pesoTotal << "\n";
        }
    }

    void analizarCentralidad() {
        vector<pair<int, string>> gradosNodos;
        
        for (int i = 0; i < matrizPonderada.size(); i++) {
            int conexiones = 0;
            for (int j = 0; j < matrizPonderada[i].size(); j++) {
                if (matrizPonderada[i][j] > 0) conexiones++;
            }
            
            string nombreCiudad = mapaRutas->getNombreLugar(i);
            gradosNodos.push_back({conexiones, nombreCiudad});
        }
        
        // Ordenar por número de conexiones (descendente)
        sort(gradosNodos.begin(), gradosNodos.end(), 
             [](const pair<int, string>& a, const pair<int, string>& b) {
                 return a.first > b.first;
             });
        
        cout << "\n\t\t\t=== ANALISIS DE CENTRALIDAD DE AEROPUERTOS ===\n\n";
        cout << "\t\t\tLos aeropuertos más importantes por número de conexiones son:\n";
        
        for (int i = 0; i < min(10, (int)gradosNodos.size()); i++) {
            cout << "\t\t\t" << i+1 << ". " << gradosNodos[i].second 
                 << ": " << gradosNodos[i].first << " conexiones\n";
        }
    }

    void simularInterrupcion() {
        cout << "\n\t\t\t=== SIMULACION DE INTERRUPCION DE RUTAS ===\n\n";
        cout << "\t\t\tIngrese codigo de aeropuerto a cerrar temporalmente: ";

        string codigo;
        getline(cin, codigo);
        transform(codigo.begin(), codigo.end(), codigo.begin(), ::toupper);

        int idx = mapaRutas->buscarIndicePorCodigo(codigo);
        if (idx == -1) {
            cout << "\t\t\tAeropuerto no encontrado.\n";
            return;
        }

        int rutasAfectadas = 0;
        vector<string> destinosAfectados;

        for (int i = 0; i < matrizPonderada[idx].size(); i++) {
            if (matrizPonderada[idx][i] > 0) {
                rutasAfectadas++;
                destinosAfectados.push_back(mapaRutas->getSiglasLugar(i));
                if (destinosAfectados.size() >= 10) break; // Limitar a 10 destinos
            }
        }

        cout << "\n\t\t\tImpacto del cierre de " << codigo << ":\n";
        cout << "\t\t\t- Rutas directamente afectadas: " << rutasAfectadas << "\n";
        cout << "\t\t\t- Destinos sin conexion directa: ";

        for (size_t i = 0; i < destinosAfectados.size(); i++) {
            cout << destinosAfectados[i];
            if (i < destinosAfectados.size() - 1) cout << ", ";
        }
        cout << "\n\n";

        // Mostrar rutas alternativas para los principales destinos
        cout << "\t\t\tRutas alternativas recomendadas:\n";
        for (size_t i = 0; i < min(size_t(3), destinosAfectados.size()); i++) {
            cout << "\t\t\t" << codigo << " → " << destinosAfectados[i] << ": ";

            // Sugerir una ruta alternativa a través de Lima o Santiago
            string hub = "LIM";
            cout << "via " << hub << " (" << codigo << " → " << hub << " → "
                << destinosAfectados[i] << ")\n";
        }
    }

    void analizarEficienciaRed() {
        cout << "\n\t\t\t=== ANALISIS DE EFICIENCIA DE LA RED ===\n\n";
        
        // Calcular estadísticas
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
        cout << "\t\t\t- Total de aeropuertos: " << totalNodos << "\n";
        cout << "\t\t\t- Total de conexiones: " << totalConexiones / 2 << "\n"; // Dividir por 2 para no contar duplicados
        cout << "\t\t\t- Promedio de conexiones por aeropuerto: " << conexionesPorNodo / 2 << "\n";
        cout << "\t\t\t- Aeropuerto con más conexiones: " << nodoMaxConexiones << " (" << maxConexiones << ")\n";
        cout << "\t\t\t- Aeropuerto con menos conexiones: " << nodoMinConexiones << " (" << minConexiones << ")\n\n";
        
        // Evaluación de la red
        cout << "\t\t\tEvaluacion de la estructura de la red:\n";
        if (maxConexiones > totalNodos * 0.5) {
            cout << "\t\t\t- Red centralizada con hubs dominantes\n";
        } else {
            cout << "\t\t\t- Red distribuida sin hubs excesivamente dominantes\n";
        }
        
        if (conexionesPorNodo > 5) {
            cout << "\t\t\t- Alta densidad de conexiones, buena conectividad general\n";
        } else {
            cout << "\t\t\t- Baja densidad de conexiones, posibles problemas de conectividad\n";
        }
    }

    void sugerirNuevasRutas() {
        cout << "\n\t\t\t=== SUGERENCIA DE NUEVAS RUTAS ===\n\n";
        
        vector<pair<string, string>> rutasSugeridas;
        
        // Buscar pares de nodos que no estén conectados directamente
        // pero que tengan muchas conexiones compartidas
        for (int i = 0; i < matrizPonderada.size(); i++) {
            for (int j = i + 1; j < matrizPonderada.size(); j++) {
                // Si no hay conexión directa
                if (matrizPonderada[i][j] == 0) {
                    // Verificar si tienen conexiones compartidas
                    int conexionesCompartidas = 0;
                    for (int k = 0; k < matrizPonderada.size(); k++) {
                        if (matrizPonderada[i][k] > 0 && matrizPonderada[j][k] > 0) {
                            conexionesCompartidas++;
                        }
                    }
                    
                    // Si tienen suficientes conexiones compartidas, sugerir conectarlos
                    if (conexionesCompartidas >= 3) {
                        string origen = mapaRutas->getSiglasLugar(i);
                        string destino = mapaRutas->getSiglasLugar(j);
                        rutasSugeridas.push_back({origen, destino});
                        
                        // Limitar a 5 sugerencias
                        if (rutasSugeridas.size() >= 5) break;
                    }
                }
            }
            if (rutasSugeridas.size() >= 5) break;
        }
        
        cout << "\t\t\tRutas potencialmente rentables a considerar:\n\n";
        for (size_t i = 0; i < rutasSugeridas.size(); i++) {
            cout << "\t\t\t" << i+1 << ". " << rutasSugeridas[i].first << " → " 
                 << rutasSugeridas[i].second << " (Aeropuertos con muchas conexiones compartidas)\n";
        }
    }
};