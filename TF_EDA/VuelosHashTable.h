#pragma once
#include "HashTable.h"
#include "Vuelo.h"
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <ctime>
#include <iomanip>

// Clase que gestiona los vuelos mediante tablas hash para un acceso eficiente
class VuelosHashTable {
private:
    // Tabla hash principal para almacenar vuelos indexados por ID
    HashTable<std::string, Vuelo> vuelosPorId;

    // Tablas hash secundarias para búsquedas específicas
    HashTable<std::string, std::vector<std::string>> vuelosPorRuta;
    HashTable<std::string, std::vector<std::string>> vuelosPorCiudad;
    HashTable<std::string, std::vector<std::string>> vuelosPorPais;
    HashTable<std::string, std::vector<std::string>> vuelosPorHorario;

    // Rutas de archivos para la persistencia de datos
    std::string archivoVuelos;
    std::string archivoHasheado;

    // Función hash personalizada para optimizar la distribución de IDs de vuelo
    static size_t hashVueloId(const std::string& id) {
        size_t hash = 0;
        for (char c : id) {
            hash = hash * 31 + c;
        }
        return hash;
    }

    // Agrega un vuelo a todos los índices secundarios para facilitar búsquedas
    void indexarVuelo(const Vuelo& vuelo) {
        std::string id = vuelo.getID();

        std::string ruta = vuelo.getCodigoOrigen() + "-" + vuelo.getCodigoDestino();
        auto rutaIds = vuelosPorRuta.find(ruta);
        if (rutaIds) {
            rutaIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorRuta.insert(ruta, ids);
        }

        auto origenIds = vuelosPorCiudad.find(vuelo.getCiudadOrigen());
        if (origenIds) {
            origenIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorCiudad.insert(vuelo.getCiudadOrigen(), ids);
        }

        auto destinoIds = vuelosPorCiudad.find(vuelo.getCiudadDestino());
        if (destinoIds) {
            destinoIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorCiudad.insert(vuelo.getCiudadDestino(), ids);
        }

        auto paisOrigenIds = vuelosPorPais.find(vuelo.getPaisOrigen());
        if (paisOrigenIds) {
            paisOrigenIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorPais.insert(vuelo.getPaisOrigen(), ids);
        }

        auto paisDestinoIds = vuelosPorPais.find(vuelo.getPaisDestino());
        if (paisDestinoIds) {
            paisDestinoIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorPais.insert(vuelo.getPaisDestino(), ids);
        }

        auto horaIds = vuelosPorHorario.find(vuelo.getHoraInicio());
        if (horaIds) {
            horaIds->push_back(id);
        }
        else {
            std::vector<std::string> ids = { id };
            vuelosPorHorario.insert(vuelo.getHoraInicio(), ids);
        }
    }

public:
    // Constructor que inicializa todas las tablas hash con tamaños apropiados
    VuelosHashTable(const std::string& archivo = "Archivos//vuelos.txt",
        const std::string& archivoHash = "Archivos//vuelosHash.txt")
        : vuelosPorId(1000, hashVueloId),
        vuelosPorRuta(200),
        vuelosPorCiudad(100),
        vuelosPorPais(50),
        vuelosPorHorario(24),
        archivoVuelos(archivo),
        archivoHasheado(archivoHash) {

        cargarVuelosDesdeArchivo();
    }

    // Carga todos los vuelos desde el archivo de texto y los indexa en las tablas hash
    void cargarVuelosDesdeArchivo() {
        std::ifstream archivo(archivoVuelos);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << archivoVuelos << std::endl;
            return;
        }

        std::string linea;
        int contador = 0;

        while (std::getline(archivo, linea) && contador < 3000) {
            Vuelo vuelo = Vuelo::deserializar(linea);
            vuelosPorId.insert(vuelo.getID(), vuelo);
            indexarVuelo(vuelo);
            contador++;
        }

        archivo.close();
        std::cout << "Se cargaron " << contador << " vuelos en la tabla hash." << std::endl;

        guardarVuelosHasheados();
    }

    // Guarda los vuelos en un formato optimizado para futuras cargas
    void guardarVuelosHasheados() {
        std::ofstream archivo(archivoHasheado);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << archivoHasheado << " para escritura." << std::endl;
            return;
        }

        auto vuelos = vuelosPorId.values();
        for (const auto& vuelo : vuelos) {
            archivo << vuelo.serializar() << std::endl;
        }

        archivo.close();
        std::cout << "Vuelos guardados en formato hasheado: " << archivoHasheado << std::endl;
    }

    // Busca un vuelo específico por su ID en O(1)
    Vuelo* buscarVueloPorID(const std::string& id) {
        return vuelosPorId.find(id);
    }

    // Encuentra todos los vuelos entre un origen y destino específicos
    std::vector<Vuelo> buscarVuelosPorRuta(const std::string& origen, const std::string& destino) {
        std::string ruta = origen + "-" + destino;
        std::vector<Vuelo> resultado;

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

    // Encuentra todos los vuelos que salen o llegan a una ciudad específica
    std::vector<Vuelo> buscarVuelosPorCiudad(const std::string& ciudad) {
        std::vector<Vuelo> resultado;

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

    // Encuentra todos los vuelos relacionados con un país específico
    std::vector<Vuelo> buscarVuelosPorPais(const std::string& pais) {
        std::vector<Vuelo> resultado;

        auto ids = vuelosPorPais.find(pais);
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

    // Encuentra todos los vuelos que salen a una hora específica
    std::vector<Vuelo> buscarVuelosPorHora(const std::string& hora) {
        std::vector<Vuelo> resultado;

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

    // Encuentra vuelos dentro de un rango de precio específico
    std::vector<Vuelo> buscarVuelosPorRangoPrecio(float precioMin, float precioMax) {
        return vuelosPorId.filter([precioMin, precioMax](const Vuelo& v) {
            return v.getPrecio() >= precioMin && v.getPrecio() <= precioMax;
            });
    }

    // Encuentra vuelos con duración menor o igual a un límite en minutos
    std::vector<Vuelo> buscarVuelosPorDuracion(int minutosMax) {
        return vuelosPorId.filter([minutosMax](const Vuelo& v) {
            return v.getDuracionEnMinutos() <= minutosMax;
            });
    }

    // Modifica el precio de un vuelo específico
    bool modificarPrecioVuelo(const std::string& id, float nuevoPrecio) {
        if (nuevoPrecio < 0) return false;

        Vuelo* vuelo = vuelosPorId.find(id);
        if (!vuelo) return false;

        Vuelo vueloModificado = *vuelo;

        vuelosPorId.insert(id, vueloModificado);

        guardarVuelosHasheados();

        return true;
    }

    // Aplica un descuento a todos los vuelos que cumplan una condición específica
    int aplicarDescuento(std::function<bool(const Vuelo&)> condicion, float porcentajeDescuento) {
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

    // Genera un informe detallado de vuelos por país
    void generarReportePorPais(const std::string& nombreArchivo = "Archivos//reporte_paises.txt") {
        std::map<std::string, int> contadorPorPais;
        std::map<std::string, float> ingresoPorPais;

        vuelosPorId.forEach([&](Vuelo& v) {
            contadorPorPais[v.getPaisOrigen()]++;
            ingresoPorPais[v.getPaisOrigen()] += v.getPrecio();

            contadorPorPais[v.getPaisDestino()]++;
            ingresoPorPais[v.getPaisDestino()] += v.getPrecio();
            });

        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo crear el archivo de reporte." << std::endl;
            return;
        }

        archivo << "REPORTE DE VUELOS POR PAÍS\n";
        archivo << "=========================\n\n";
        archivo << std::left << std::setw(20) << "País"
            << std::right << std::setw(10) << "Vuelos"
            << std::right << std::setw(15) << "Ingresos($)" << std::endl;
        archivo << std::string(45, '-') << std::endl;

        for (const auto& par : contadorPorPais) {
            archivo << std::left << std::setw(20) << par.first
                << std::right << std::setw(10) << par.second
                << std::right << std::setw(15) << std::fixed << std::setprecision(2)
                << ingresoPorPais[par.first] << std::endl;
        }

        archivo.close();
        std::cout << "Reporte generado: " << nombreArchivo << std::endl;
    }

    // Genera un informe de vuelos agrupados por rangos de precio
    void generarReportePorRangoPrecio(const std::string& nombreArchivo = "Archivos//reporte_precios.txt") {
        struct RangoPrecio {
            float min;
            float max;
            int contador;
            float ingresoTotal;

            RangoPrecio(float _min, float _max) : min(_min), max(_max), contador(0), ingresoTotal(0.0f) {}
        };

        std::vector<RangoPrecio> rangos = {
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
            RangoPrecio(1000, std::numeric_limits<float>::max())
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

        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo crear el archivo de reporte." << std::endl;
            return;
        }

        archivo << "REPORTE DE VUELOS POR RANGO DE PRECIO\n";
        archivo << "====================================\n\n";
        archivo << std::left << std::setw(20) << "Rango($)"
            << std::right << std::setw(10) << "Vuelos"
            << std::right << std::setw(15) << "Ingresos($)"
            << std::right << std::setw(15) << "Precio Prom($)" << std::endl;
        archivo << std::string(60, '-') << std::endl;

        for (const auto& rango : rangos) {
            std::string rangoStr;
            if (rango.max == std::numeric_limits<float>::max()) {
                rangoStr = std::to_string(static_cast<int>(rango.min)) + "+";
            }
            else {
                rangoStr = std::to_string(static_cast<int>(rango.min)) + "-" + std::to_string(static_cast<int>(rango.max));
            }

            float promedio = (rango.contador > 0) ? (rango.ingresoTotal / rango.contador) : 0.0f;

            archivo << std::left << std::setw(20) << rangoStr
                << std::right << std::setw(10) << rango.contador
                << std::right << std::setw(15) << std::fixed << std::setprecision(2) << rango.ingresoTotal
                << std::right << std::setw(15) << std::fixed << std::setprecision(2) << promedio << std::endl;
        }

        archivo.close();
        std::cout << "Reporte generado: " << nombreArchivo << std::endl;
    }

    // Muestra estadísticas generales sobre los vuelos y la tabla hash
    void mostrarEstadisticas() {
        int totalVuelos = vuelosPorId.size();
        int vuelosInternacionales = 0;
        int vuelosNacionales = 0;
        float precioPromedio = 0.0f;
        float precioMinimo = std::numeric_limits<float>::max();
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
            precioMinimo = std::min(precioMinimo, precio);
            precioMaximo = std::max(precioMaximo, precio);
            });

        if (totalVuelos > 0) {
            precioPromedio /= totalVuelos;
        }

        std::cout << "\n=== ESTADÍSTICAS DE VUELOS ===\n";
        std::cout << "Total de vuelos: " << totalVuelos << std::endl;
        std::cout << "Vuelos internacionales: " << vuelosInternacionales << " ("
            << (totalVuelos > 0 ? (vuelosInternacionales * 100.0 / totalVuelos) : 0) << "%)" << std::endl;
        std::cout << "Vuelos nacionales: " << vuelosNacionales << " ("
            << (totalVuelos > 0 ? (vuelosNacionales * 100.0 / totalVuelos) : 0) << "%)" << std::endl;
        std::cout << "Precio promedio: $" << std::fixed << std::setprecision(2) << precioPromedio << std::endl;
        std::cout << "Precio mínimo: $" << precioMinimo << std::endl;
        std::cout << "Precio máximo: $" << precioMaximo << std::endl;

        std::cout << "\n=== ESTADÍSTICAS DE TABLA HASH ===\n";
        vuelosPorId.imprimirEstadisticas();
    }

    // Realiza una búsqueda combinada con múltiples criterios
    std::vector<Vuelo> busquedaAvanzada(
        const std::string& origen = "",
        const std::string& destino = "",
        float precioMin = 0.0f,
        float precioMax = std::numeric_limits<float>::max(),
        const std::string& horaInicioMin = "",
        const std::string& horaInicioMax = "",
        int duracionMaxMinutos = std::numeric_limits<int>::max(),
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
