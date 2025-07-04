#pragma once
#include "Usuario.h"
#include "VuelosHashTable.h"

class Administrador : public Usuario {
private:
    VuelosHashTable vuelosHash;

public:
    Administrador() {}
    Administrador(const string& nombre, const string& apellido, const string& email, const string& contraseña)
        : Usuario(nombre, apellido, email, contraseña),
        vuelosHash() {
    }

    ~Administrador() {}

    string getTipo() {
        return "Administrador";
    }
    void mostrarBienvenida() override {
        cout << "Bienvenido Administrador " << nombre << " " << apellido << endl;
    };

    void guardarUsuarioArchivo(Usuario& user) {
        ofstream archivo("Archivos//usuarios.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }
        archivo << user.getNombre() << "|" << user.getApellido() << "|"
            << user.getEmail() << "|"
            << user.getContraseña() << "|"
            << user.getTipo() << endl;
        archivo.close();
        cout << "Usuario registrado con exito." << endl;
    }

    void buscarVuelo() {
        string id;
        cout << "\t\t\tIngrese ID del vuelo: ";
        cin >> id;

        Vuelo* vuelo = vuelosHash.buscarVueloPorID(id);
        if (vuelo) {
            cout << "\n\t\t\tVuelo encontrado:\n";
            vuelo->mostrar();
        }
        else {
            cout << "\t\t\tVuelo no encontrado.\n";
        }
    }

    void modificarPrecioVuelo() {
        string id;
        float nuevoPrecio;

        cout << "\t\t\tIngrese ID del vuelo: ";
        cin >> id;
        cout << "\t\t\tIngrese nuevo precio: $";
        cin >> nuevoPrecio;

        if (vuelosHash.modificarPrecioVuelo(id, nuevoPrecio)) {
            cout << "\t\t\tPrecio modificado exitosamente.\n";
        }
        else {
            cout << "\t\t\tNo se pudo modificar el precio.\n";
        }
    }

    void generarReportes() {
        int opcion;
        cout << "\n\t\t\t=== GENERACIÓN DE REPORTES ===\n";
        cout << "\t\t\t1. Reporte por país\n";
        cout << "\t\t\t2. Reporte por rango de precio\n";
        cout << "\t\t\t3. Estadísticas generales\n";
        cout << "\t\t\tSeleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            vuelosHash.generarReportePorPais();
            break;
        case 2:
            vuelosHash.generarReportePorRangoPrecio();
            break;
        case 3:
            vuelosHash.mostrarEstadisticas();
            break;
        default:
            cout << "\t\t\tOpción inválida.\n";
        }
    }

    void busquedaAvanzada() {
        string origen, destino, horaMin, horaMax;
        float precioMin = 0, precioMax = std::numeric_limits<float>::max();
        int duracionMax = std::numeric_limits<int>::max();
        bool soloInternacionales = false;

        cin.ignore();
        cout << "\t\t\tOrigen (dejar vacío para cualquiera): ";
        getline(cin, origen);

        cout << "\t\t\tDestino (dejar vacío para cualquiera): ";
        getline(cin, destino);

        cout << "\t\t\tPrecio mínimo ($): ";
        cin >> precioMin;

        cout << "\t\t\tPrecio máximo ($): ";
        cin >> precioMax;

        cin.ignore();
        cout << "\t\t\tHora de salida mínima (HH:MM): ";
        getline(cin, horaMin);

        cout << "\t\t\tHora de salida máxima (HH:MM): ";
        getline(cin, horaMax);

        cout << "\t\t\tDuración máxima (minutos): ";
        cin >> duracionMax;

        cout << "\t\t\tSolo vuelos internacionales (1=Sí, 0=No): ";
        cin >> soloInternacionales;

        auto vuelos = vuelosHash.busquedaAvanzada(origen, destino, precioMin, precioMax,
            horaMin, horaMax, duracionMax, soloInternacionales);

        cout << "\n\t\t\tSe encontraron " << vuelos.size() << " vuelos:\n\n";
        for (const auto& vuelo : vuelos) {
            vuelo.mostrar();
            cout << "\n";
        }
    }

    // Métodos para búsquedas especializadas
    std::vector<Vuelo> buscarVuelosPorHora(const std::string& hora) {
        return vuelosHash.buscarVuelosPorHora(hora);
    }

    std::vector<Vuelo> buscarVuelosPorDuracion(int minutos) {
        return vuelosHash.buscarVuelosPorDuracion(minutos);
    }

    std::vector<Vuelo> buscarVuelosPorPais(const std::string& pais) {
        return vuelosHash.buscarVuelosPorPais(pais);
    }

    // Métodos para aplicar descuentos
    int aplicarDescuentoInternacionales(float porcentaje) {
        return vuelosHash.aplicarDescuento(
            [](const Vuelo& v) { return v.getEsInternacional(); },
            porcentaje
        );
    }

    int aplicarDescuentoCortos(float porcentaje) {
        return vuelosHash.aplicarDescuento(
            [](const Vuelo& v) { return v.getDuracionEnMinutos() < 120; },
            porcentaje
        );
    }

    int aplicarDescuentoPorPais(const std::string& pais, float porcentaje) {
        return vuelosHash.aplicarDescuento(
            [pais](const Vuelo& v) {
                return v.getPaisDestino() == pais || v.getPaisOrigen() == pais;
            },
            porcentaje
        );
    }
};
