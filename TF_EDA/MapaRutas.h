#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;

class MapaRutas {
private:
    struct Ciudad {
        string nombre;
        string codigo;
        int indice;
        int paisIndice;
    };

    struct Pais {
        string nombre;
        vector<Ciudad> ciudades;
    };

    vector<Pais> paises;
    vector<vector<int>> matrizConexiones;
    int totalCiudades;
    unordered_map<string, int> codigoACiudadIndice;
    unordered_map<string, int> nombreACiudadIndice;

    void agregarCiudad(Pais& pais, const string& nombre, const string& codigo) {
        Ciudad ciudad;
        ciudad.nombre = nombre;
        ciudad.codigo = codigo;
        ciudad.indice = totalCiudades++;
        ciudad.paisIndice = paises.size();

        pais.ciudades.push_back(ciudad);
        codigoACiudadIndice[codigo] = ciudad.indice;
        nombreACiudadIndice[nombre] = ciudad.indice;
    }

    void conectar(const string& codigo1, const string& codigo2) {
        auto it1 = codigoACiudadIndice.find(codigo1);
        auto it2 = codigoACiudadIndice.find(codigo2);

        if (it1 != codigoACiudadIndice.end() && it2 != codigoACiudadIndice.end()) {
            int idx1 = it1->second;
            int idx2 = it2->second;

            matrizConexiones[idx1][idx2] = 1;
            matrizConexiones[idx2][idx1] = 1;
        }
    }

    void inicializarPaisesYCiudades() {
        totalCiudades = 0;

        Pais peru;
        peru.nombre = "Peru";
        agregarCiudad(peru, "Tarapoto", "TPP");
        agregarCiudad(peru, "Piura", "PIU");
        agregarCiudad(peru, "Chiclayo", "CIX");
        agregarCiudad(peru, "Cajamarca", "CJA");
        agregarCiudad(peru, "Trujillo", "TRU");
        agregarCiudad(peru, "Lima", "LIM");
        agregarCiudad(peru, "Cusco", "CUZ");
        agregarCiudad(peru, "Arequipa", "AQP");
        paises.push_back(peru);

        Pais chile;
        chile.nombre = "Chile";
        agregarCiudad(chile, "Santiago", "SCL");
        agregarCiudad(chile, "Arica", "ARI");
        agregarCiudad(chile, "Iquique", "IQQ");
        agregarCiudad(chile, "Calama", "CJC");
        agregarCiudad(chile, "Antofagasta", "ANF");
        agregarCiudad(chile, "La Serena", "LSC");
        agregarCiudad(chile, "Concepcion", "CCP");
        agregarCiudad(chile, "Temuco", "ZCO");
        agregarCiudad(chile, "Puerto Montt", "PMC");
        agregarCiudad(chile, "Balmaceda", "BBA");
        paises.push_back(chile);

        Pais ecuador;
        ecuador.nombre = "Ecuador";
        agregarCiudad(ecuador, "Quito", "UIO");
        paises.push_back(ecuador);

        Pais argentina;
        argentina.nombre = "Argentina";
        agregarCiudad(argentina, "Buenos Aires", "BUE");
        agregarCiudad(argentina, "San Martin de los Andes", "CPC");
        agregarCiudad(argentina, "Salta", "SLA");
        agregarCiudad(argentina, "Iguazu", "IGR");
        agregarCiudad(argentina, "Tucuman", "TUC");
        agregarCiudad(argentina, "Cordoba", "COR");
        agregarCiudad(argentina, "Mendoza", "MDZ");
        agregarCiudad(argentina, "Neuquen", "NQN");
        agregarCiudad(argentina, "Bariloche", "BRC");
        agregarCiudad(argentina, "Comodoro Rivadavia", "CRD");
        agregarCiudad(argentina, "El Calafate", "FTE");
        agregarCiudad(argentina, "Ushuaia", "USH");
        agregarCiudad(argentina, "Trelew", "REL");
        agregarCiudad(argentina, "Resistencia", "RES");
        paises.push_back(argentina);

        Pais brasil;
        brasil.nombre = "Brasil";
        agregarCiudad(brasil, "Rio de Janeiro", "RIO");
        agregarCiudad(brasil, "Foz de Iguazu", "IGU");
        agregarCiudad(brasil, "Florianopolis", "FLN");
        agregarCiudad(brasil, "Sao Paulo", "GRU");
        agregarCiudad(brasil, "Recife", "REC");
        paises.push_back(brasil);

        Pais colombia;
        colombia.nombre = "Colombia";
        agregarCiudad(colombia, "San Andres", "ADZ");
        agregarCiudad(colombia, "Bogota", "BOG");
        agregarCiudad(colombia, "Cali", "CLO");
        agregarCiudad(colombia, "Cartagena de Indias", "CTG");
        agregarCiudad(colombia, "Cucuta", "CUC");
        agregarCiudad(colombia, "Medellin", "MDE");
        agregarCiudad(colombia, "Pereira", "PEI");
        agregarCiudad(colombia, "Santa Marta", "SMR");
        agregarCiudad(colombia, "Monteria", "MTR");
        agregarCiudad(colombia, "Barranquilla", "BAQ");
        paises.push_back(colombia);

        Pais uruguay;
        uruguay.nombre = "Uruguay";
        agregarCiudad(uruguay, "Montevideo", "MVD");
        paises.push_back(uruguay);

        Pais paraguay;
        paraguay.nombre = "Paraguay";
        agregarCiudad(paraguay, "Asuncion", "ASU");
        paises.push_back(paraguay);

        matrizConexiones.resize(totalCiudades, vector<int>(totalCiudades, 0));
    }

    void inicializarConexiones() {
        // Abrir el archivo de conexiones
        ifstream archivo("Archivos//conexiones.txt");
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo 'conexiones.txt'" << endl;
            return;
        }
        string linea;
        getline(archivo, linea);

        // Procesar cada línea de la matriz
        int fila = 0;
        while (getline(archivo, linea) && fila < totalCiudades) {
            istringstream iss(linea);
            string codigo;
            iss >> codigo; // Leer el código de la ciudad

            for (int col = 0; col < totalCiudades; col++) {
                int valor;
                iss >> valor;

                if (valor == 1) {
                    matrizConexiones[fila][col] = 1;
                }
            }

            fila++;
        }

        archivo.close();
    }

    vector<int> obtenerPaisesConDestinosDisponibles(int origen) {
        vector<int> paisesConDestinos;
        set<int> paisesIncluidos;

        for (int i = 0; i < totalCiudades; ++i) {
            if (matrizConexiones[origen][i] == 1 && i != origen) {
                for (size_t j = 0; j < paises.size(); ++j) {
                    for (const auto& ciudad : paises[j].ciudades) {
                        if (ciudad.indice == i && paisesIncluidos.find(j) == paisesIncluidos.end()) {
                            paisesConDestinos.push_back(j);
                            paisesIncluidos.insert(j);
                            break;
                        }
                    }
                }
            }
        }

        return paisesConDestinos;
    }

public:
    MapaRutas() {
        inicializarPaisesYCiudades();
        inicializarConexiones();
    }

    ~MapaRutas() {}

    int seleccionarPais(const vector<int>& paisesDisponibles = {}) {
        int opcion;

        while (true) {
            cout << "\t\t\t\t\t";

            // Mostrar opciones
            if (!paisesDisponibles.empty()) {
                for (size_t i = 0; i < paisesDisponibles.size(); ++i) {
                    int indicePais = paisesDisponibles[i];
                    cout << (i + 1) << ". " << paises[indicePais].nombre << endl << "\t\t\t\t\t";
                }
            }
            else {
                for (size_t i = 0; i < paises.size(); ++i) {
                    cout << (i + 1) << ". " << paises[i].nombre << endl << "\t\t\t\t\t";
                }
            }

            cout << "0. Volver" << endl << endl;
            cout << "\t\t\t\t\tSeleccione una opcion: ";
            cin >> opcion;

            if (opcion == 0) return -1;

            // Validar opción
            if (!paisesDisponibles.empty()) {
                if (opcion < 1 || opcion >(int)paisesDisponibles.size()) {
                    cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
                    continue; // Volver al inicio del bucle
                }
                return paisesDisponibles[opcion - 1];
            }
            else {
                if (opcion < 1 || opcion >(int)paises.size()) {
                    cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
                    continue; // Volver al inicio del bucle
                }
                return opcion - 1;
            }
        }
    }

    int seleccionarCiudadDePais(int indicePais) {
        if (indicePais < 0 || indicePais >= (int)paises.size()) {
            return -1;
        }

        for (size_t i = 0; i < paises[indicePais].ciudades.size(); ++i) {
            cout << "\t\t\t\t\t" << (i + 1) << ". "
                << paises[indicePais].ciudades[i].nombre << endl;
        }
        cout << "\t\t\t\t\t0. Volver" << endl << endl;

        int opcion;
        cout << "\t\t\t\t\tSeleccione una opcion: ";
        cin >> opcion;

        if (opcion == 0) return -2;
        if (opcion < 1 || opcion >(int)paises[indicePais].ciudades.size()) {
            cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
            return seleccionarCiudadDePais(indicePais);
        }

        return paises[indicePais].ciudades[opcion - 1].indice;
    }

    int seleccionarOrigen() {
        cout << "\t\t\t" << endl;
        cout << "\t\t\t   ----------------- SELECCIONE ORIGEN -----------------  " << endl;
        cout << " " << endl;
        int paisOrigen = seleccionarPais();
        if (paisOrigen == -1) return -1;
        cout << " " << endl;
        int ciudadOrigen = seleccionarCiudadDePais(paisOrigen);
        while (ciudadOrigen == -2) {
            paisOrigen = seleccionarPais();
            if (paisOrigen == -1) return -1;
            ciudadOrigen = seleccionarCiudadDePais(paisOrigen);
        }

        return ciudadOrigen;
    }

    int seleccionarDestino(int origen) {
        if (origen < 0 || origen >= totalCiudades) {
            return -1;
        }

        string nombreOrigen = "Desconocido";
        string nombrePaisOrigen = "Desconocido";

        for (size_t i = 0; i < paises.size(); ++i) {
            for (const auto& ciudad : paises[i].ciudades) {
                if (ciudad.indice == origen) {
                    nombreOrigen = ciudad.nombre;
                    nombrePaisOrigen = paises[i].nombre;
                    break;
                }
            }
            if (nombreOrigen != "Desconocido") break;
        }

        cout << "\t\t\t" << endl;
        cout << "\t\t\t   ----------------- SELECCIONE DESTINO ----------------- " << endl;
        cout << " " << endl;
        cout << "\t\t\t\t\t   Origen: " << nombreOrigen << ", " << nombrePaisOrigen << endl << endl;

        vector<int> paisesDisponibles = obtenerPaisesConDestinosDisponibles(origen);

        if (paisesDisponibles.empty()) {
            cout << "\t\t\t\t\tNo hay destinos disponibles para este origen." << endl;
            return -1;
        }

        int paisDestino = seleccionarPais(paisesDisponibles);
        if (paisDestino == -1) return -1;
        cout << " " << endl;
        vector<int> ciudadesDisponibles;

        for (const auto& ciudad : paises[paisDestino].ciudades) {
            if (matrizConexiones[origen][ciudad.indice] == 1 && ciudad.indice != origen) {
                ciudadesDisponibles.push_back(ciudad.indice);
                cout << "\t\t\t\t\t" << ciudadesDisponibles.size() << ". "
                    << ciudad.nombre << endl;
            }
        }

        cout << "\t\t\t\t\t0. Volver" << endl << endl;

        int opcion;
        cout << "\t\t\t\t\tSeleccione una opcion: ";
        cin >> opcion;

        if (opcion == 0) return seleccionarDestino(origen);
        if (opcion < 1 || opcion >(int)ciudadesDisponibles.size()) {
            cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
            return seleccionarDestino(origen);
        }
        cout << " " << endl;

        return ciudadesDisponibles[opcion - 1];
    }

    bool esVueloInternacional(int origen, int destino) {
        if (origen < 0 || origen >= totalCiudades || destino < 0 || destino >= totalCiudades) {
            return false;
        }

        int paisOrigen = -1;
        int paisDestino = -1;

        for (size_t i = 0; i < paises.size(); ++i) {
            for (const auto& ciudad : paises[i].ciudades) {
                if (ciudad.indice == origen) paisOrigen = i;
                if (ciudad.indice == destino) paisDestino = i;
            }
            if (paisOrigen != -1 && paisDestino != -1) break;
        }

        return paisOrigen != paisDestino && paisOrigen != -1 && paisDestino != -1;
    }

    string getNombreLugar(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.nombre + " (" + pais.nombre + ")";
                }
            }
        }
        return "No valido";
    }

    string getNombreCiudad(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.nombre;
                }
            }
        }
        return "Desconocida";
    }

    string getNombrePais(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return pais.nombre;
                }
            }
        }
        return "Desconocido";
    }

    string getSiglasLugar(int indice) {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.codigo;
                }
            }
        }
        return "???";
    }

    int getNumLugares() const {
        return totalCiudades;
    }

    string getIDRuta(int origen, int destino) {
        return getSiglasLugar(origen) + "-" + getSiglasLugar(destino);
    }

    bool existeRuta(int origen, int destino) {
        if (origen < 0 || origen >= totalCiudades || destino < 0 || destino >= totalCiudades) {
            return false;
        }
        return matrizConexiones[origen][destino] == 1;
    }

    int buscarIndicePorCodigo(const string& codigo) const {
        auto it = codigoACiudadIndice.find(codigo);
        if (it != codigoACiudadIndice.end()) {
            return it->second;
        }
        return -1;
    }

    int buscarIndicePorNombre(const string& nombre) const {
        auto it = nombreACiudadIndice.find(nombre);
        if (it != nombreACiudadIndice.end()) {
            return it->second;
        }
        return -1;
    }
};