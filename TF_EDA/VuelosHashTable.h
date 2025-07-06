#pragma once
#include "HashTable.h"
#include "Vuelo.h"
#include <map>
#include <ctime>

class VuelosHashTable {
private:
    HashTable<string, Vuelo> vuelosPorId;
    HashTable<string, vector<string>> vuelosPorRuta;
    HashTable<string, vector<string>> vuelosPorCiudad;
    HashTable<string, vector<string>> vuelosPorPais;
    HashTable<string, vector<string>> vuelosPorHorario;

    string archivoVuelos;
    string archivoHasheado;

    static size_t hashVueloId(const string& id) {
        size_t hash = 0;
        for (char c : id) {
            hash = hash * 31 + c;
        }
        return hash;
    }

    void indexarVuelo(const Vuelo& vuelo) {
        string id = vuelo.getID();

        string ruta = vuelo.getCodigoOrigen() + "-" + vuelo.getCodigoDestino();
        auto rutaIds = vuelosPorRuta.find(ruta);
        if (rutaIds) {
            rutaIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorRuta.insert(ruta, ids);
        }

        auto origenIds = vuelosPorCiudad.find(vuelo.getCiudadOrigen());
        if (origenIds) {
            origenIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorCiudad.insert(vuelo.getCiudadOrigen(), ids);
        }

        auto destinoIds = vuelosPorCiudad.find(vuelo.getCiudadDestino());
        if (destinoIds) {
            destinoIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorCiudad.insert(vuelo.getCiudadDestino(), ids);
        }

        auto paisOrigenIds = vuelosPorPais.find(vuelo.getPaisOrigen());
        if (paisOrigenIds) {
            paisOrigenIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorPais.insert(vuelo.getPaisOrigen(), ids);
        }

        auto paisDestinoIds = vuelosPorPais.find(vuelo.getPaisDestino());
        if (paisDestinoIds) {
            paisDestinoIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorPais.insert(vuelo.getPaisDestino(), ids);
        }

        auto horaIds = vuelosPorHorario.find(vuelo.getHoraInicio());
        if (horaIds) {
            horaIds->push_back(id);
        }
        else {
            vector<string> ids = { id };
            vuelosPorHorario.insert(vuelo.getHoraInicio(), ids);
        }
    }

public:
    VuelosHashTable(const string& archivo = "Archivos//vuelos.txt",
        const string& archivoHash = "Archivos//vuelosHash.txt")
        : vuelosPorId(1000, hashVueloId),
        vuelosPorRuta(200),
        vuelosPorCiudad(100),
        vuelosPorPais(50),
        vuelosPorHorario(24),
        archivoVuelos(archivo),
        archivoHasheado(archivoHash) {

        cargarVuelosDesdeArchivo();
    }

    void cargarVuelosDesdeArchivo() {
        ifstream archivo(archivoVuelos);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo " << archivoVuelos << endl;
            return;
        }

        string linea;
        int contador = 0;

        while (getline(archivo, linea) && contador < 3000) {
            Vuelo vuelo = Vuelo::deserializar(linea);
            vuelosPorId.insert(vuelo.getID(), vuelo);
            indexarVuelo(vuelo);
            contador++;
        }

        archivo.close();
        cout << "Se cargaron " << contador << " vuelos en la tabla hash." << endl;

        guardarVuelosHasheados();
    }

    void guardarVuelosHasheados() {
        ofstream archivo(archivoHasheado);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo " << archivoHasheado << " para escritura." << endl;
            return;
        }

        auto vuelos = vuelosPorId.values();
        for (const auto& vuelo : vuelos) {
            archivo << vuelo.serializar() << endl;
        }

        archivo.close();
        cout << "\t\t\tVuelos guardados en formato hasheado: " << archivoHasheado << endl;
    }

    Vuelo* buscarVueloPorID(const string& id) {
        return vuelosPorId.find(id);
    }

    vector<Vuelo> buscarVuelosPorRuta(const string& origen, const string& destino) {
        string ruta = origen + "-" + destino;
        vector<Vuelo> resultado;

        auto ids = vuelosPorRuta.find(ruta);
        if (ids) {
            for (const auto& id : *ids) {
                auto vuelo = vuelosPorId.find(id);
                if (vuelo) {
                    resultado.push_back(*vuelo);
                }
            }
        }

        return resultado;
    }

    vector<Vuelo> buscarVuelosPorCiudad(const string& ciudad) {
        vector<Vuelo> resultado;

        auto ids = vuelosPorCiudad.find(ciudad);
        if (ids) {
            for (const auto& id : *ids) {
                auto vuelo = vuelosPorId.find(id);
                if (vuelo) {
                    resultado.push_back(*vuelo);
                }
            }
        }

        return resultado;
    }

    vector<Vuelo> buscarVuelosPorPais(const string& pais) {
        vector<Vuelo> resultado;

        auto ids = vuelosPorPais.find(pais);
        if (ids) {
            for (const auto& id : *ids) {
                auto vuelo = vuelosPorId.find(id);
                if (vuelo) {
                    if (vuelo->getPaisOrigen() != pais && vuelo->getPaisDestino() == pais || vuelo->getPaisOrigen() == pais && vuelo->getPaisDestino() != pais) {
                        resultado.push_back(*vuelo);
                    }
                }
            }
        }

        return resultado;
    }

    vector<Vuelo> buscarVuelosPorHora(const string& hora) {
        vector<Vuelo> resultado;

        auto ids = vuelosPorHorario.find(hora);
        if (ids) {
            for (const auto& id : *ids) {
                auto vuelo = vuelosPorId.find(id);
                if (vuelo) {
                    resultado.push_back(*vuelo);
                }
            }
        }

        return resultado;
    }

    vector<Vuelo> buscarVuelosPorRangoPrecio(float precioMin, float precioMax) {
        return vuelosPorId.filter([precioMin, precioMax](const Vuelo& v) {
            return v.getPrecio() >= precioMin && v.getPrecio() <= precioMax;
            });
    }

    vector<Vuelo> buscarVuelosPorDuracion(int minutosMax) {
        return vuelosPorId.filter([minutosMax](const Vuelo& v) {
            return v.getDuracionEnMinutos() <= minutosMax;
            });
    }

    bool modificarPrecioVuelo(const string& id, float nuevoPrecio) {
        if (nuevoPrecio < 0) return false;

        Vuelo* vuelo = vuelosPorId.find(id);
        if (!vuelo) return false;

        Vuelo vueloModificado = *vuelo;

        vuelosPorId.insert(id, vueloModificado);

        guardarVuelosHasheados();

        return true;
    }

    int aplicarDescuento(function<bool(const Vuelo&)> condicion, float porcentajeDescuento) {
        if (porcentajeDescuento <= 0 || porcentajeDescuento > 100) return 0;

        float factor = 1.0f - (porcentajeDescuento / 100.0f);
        int contador = 0;

        vuelosPorId.forEach([&](Vuelo& v) {
            if (condicion(v)) {
                contador++;
            }
            });

        if (contador > 0) {
            guardarVuelosHasheados();
        }

        return contador;
    }

    void generarReportePorPais(const string& nombreArchivo = "Archivos//reporte_paises.txt") {
        map<string, int> contadorPorPais;
        map<string, float> ingresoPorPais;

        vuelosPorId.forEach([&](Vuelo& v) {
            contadorPorPais[v.getPaisOrigen()]++;
            ingresoPorPais[v.getPaisOrigen()] += v.getPrecio();

            contadorPorPais[v.getPaisDestino()]++;
            ingresoPorPais[v.getPaisDestino()] += v.getPrecio();
            });

        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo crear el archivo de reporte." << endl;
            return;
        }

        archivo << "\t\t\t\t\tREPORTE DE VUELOS POR PAÍS\n";
        archivo << "\t\t\t------------------------------------------\n\n";
        archivo << left << setw(20) << "País"
            << right << setw(10) << "Vuelos"
            << right << setw(15) << "Ingresos($)" << endl;
        archivo << string(45, '-') << endl;

        for (const auto& par : contadorPorPais) {
            archivo << left << setw(20) << par.first
                << right << setw(10) << par.second
                << right << setw(15) << fixed << setprecision(2)
                << ingresoPorPais[par.first] << endl;
        }

        archivo.close();
        cout << "\t\t\tReporte generado: " << nombreArchivo << endl;
    }

    void generarReportePorRangoPrecio(const string& nombreArchivo = "Archivos//reporte_precios.txt") {
        struct RangoPrecio {
            float min;
            float max;
            int contador;
            float ingresoTotal;

            RangoPrecio(float _min, float _max) : min(_min), max(_max), contador(0), ingresoTotal(0.0f) {}
        };

        vector<RangoPrecio> rangos = {
            RangoPrecio(0, 100),
            RangoPrecio(100, 200),
            RangoPrecio(200, 300),
            RangoPrecio(300, 400),
            RangoPrecio(400, 500),
            RangoPrecio(500, 600),
            RangoPrecio(600, 700),
            RangoPrecio(700, 800),
            RangoPrecio(800, 900),
            RangoPrecio(900, 1000),
        };

        vuelosPorId.forEach([&](Vuelo& v) {
            float precio = v.getPrecio();
            for (auto& rango : rangos) {
                if (precio >= rango.min && precio < rango.max) {
                    rango.contador++;
                    rango.ingresoTotal += precio;
                    break;
                }
            }
            });

        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo crear el archivo de reporte." << endl;
            return;
        }

        archivo << "\t\t\tREPORTE DE VUELOS POR RANGO DE PRECIO\n";
        archivo << "\t\t\t---------------------------------------------------\n\n";
        archivo << left << setw(20) << "Rango($)"
            << right << setw(10) << "Vuelos"
            << right << setw(15) << "Ingresos($)"
            << right << setw(15) << "Precio Prom($)" << endl;
        archivo << string(60, '-') << endl;

        for (const auto& rango : rangos) {
            string rangoStr;
            if (rango.max == numeric_limits<float>::max()) {
                rangoStr = to_string(static_cast<int>(rango.min)) + "+";
            }
            else {
                rangoStr = to_string(static_cast<int>(rango.min)) + "-" + to_string(static_cast<int>(rango.max));
            }

            float promedio = (rango.contador > 0) ? (rango.ingresoTotal / rango.contador) : 0.0f;

            archivo << left << setw(20) << rangoStr
                << right << setw(10) << rango.contador
                << right << setw(15) << fixed << setprecision(2) << rango.ingresoTotal
                << right << setw(15) << fixed << setprecision(2) << promedio << endl;
        }

        archivo.close();
        cout << "\t\t\tReporte generado: " << nombreArchivo << endl;
    }

    void mostrarEstadisticas() {
        int totalVuelos = vuelosPorId.size();
        int vuelosInternacionales = 0;
        int vuelosNacionales = 0;
        float precioPromedio = 0.0f;
        float precioMinimo = numeric_limits<float>::max();
        float precioMaximo = 0.0f;

        vuelosPorId.forEach([&](Vuelo& v) {
            if (v.getEsInternacional()) {
                vuelosInternacionales++;
            }
            else {
                vuelosNacionales++;
            }

            float precio = v.getPrecio();
            precioPromedio += precio;
            precioMinimo = min(precioMinimo, precio);
            precioMaximo = max(precioMaximo, precio);
            });

        if (totalVuelos > 0) {
            precioPromedio /= totalVuelos;
        }

        cout << "\n\t\t\t------ ESTADÍSTICAS DE VUELOS ------\n";
        cout << "\t\t\tTotal de vuelos: " << totalVuelos << endl;
        cout << "\t\t\tVuelos internacionales: " << vuelosInternacionales << " ("
            << (totalVuelos > 0 ? (vuelosInternacionales * 100.0 / totalVuelos) : 0) << "%)" << endl;
        cout << "\t\t\tVuelos nacionales: " << vuelosNacionales << " ("
            << (totalVuelos > 0 ? (vuelosNacionales * 100.0 / totalVuelos) : 0) << "%)" << endl;
        cout << "\t\t\tPrecio promedio: $" << fixed << setprecision(2) << precioPromedio << endl;
        cout << "\t\t\tPrecio mínimo: $" << precioMinimo << endl;
        cout << "\t\t\tPrecio máximo: $" << precioMaximo << endl;

        cout << "\n\t\t\t------- ESTADÍSTICAS DE TABLA HASH -------\n";
        vuelosPorId.imprimirEstadisticas();
    }

    vector<Vuelo> busquedaAvanzada(
        const string& origen = "",
        const string& destino = "",
        float precioMin = 0.0f,
        float precioMax = numeric_limits<float>::max(),
        const string& horaInicioMin = "",
        const string& horaInicioMax = "",
        int duracionMaxMinutos = numeric_limits<int>::max(),
        bool soloInternacionales = false
    ) {
        return vuelosPorId.filter([&](const Vuelo& v) {
            if (!origen.empty() && v.getCiudadOrigen() != origen && v.getCodigoOrigen() != origen) {
                return false;
            }

            if (!destino.empty() && v.getCiudadDestino() != destino && v.getCodigoDestino() != destino) {
                return false;
            }

            if (v.getPrecio() < precioMin || v.getPrecio() > precioMax) {
                return false;
            }

            if (!horaInicioMin.empty() && v.getHoraInicio() < horaInicioMin) {
                return false;
            }

            if (!horaInicioMax.empty() && v.getHoraInicio() > horaInicioMax) {
                return false;
            }

            if (v.getDuracionEnMinutos() > duracionMaxMinutos) {
                return false;
            }

            if (soloInternacionales && !v.getEsInternacional()) {
                return false;
            }

            return true;
            });
    }
};
