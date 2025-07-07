#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <direct.h>  // Para _mkdir en Windows
#include "Cliente.h"
#include "ABB.h"

using namespace std;

class GestorClientes {
private:
    ABB<Cliente> arbolClientes;
    string archivoUsuarios;
    string carpetaClientes;
    int ultimoId;

    // Genera un ID secuencial para clientes
    string generarIdOriginal() {
        ultimoId++;

        // Formatear el ID con ceros a la izquierda
        stringstream ss;
        ss << "CLI" << setw(5) << setfill('0') << ultimoId;

        // Guardar el nuevo ID como último usado
        ofstream salida("Archivos//ultimo_id_cliente.txt");
        if (salida.is_open()) {
            salida << ultimoId;
            salida.close();
        }

        return ss.str();
    }

    // Genera un hash simple basado en el ID y nombre
    string generarIdHash(const string& idOriginal, const string& nombre) {
        // Usar iniciales y parte del ID
        string iniciales = "";
        if (!nombre.empty()) iniciales += toupper(nombre[0]);

        // Extraer números del ID original
        string numeros = "";
        for (char c : idOriginal) {
            if (isdigit(c)) numeros += c;
        }

        // Calcular hash como combinación
        unsigned long hash = 0;
        for (char c : numeros) {
            hash = hash * 31 + c;
        }
        for (char c : iniciales) {
            hash = hash * 37 + c;
        }

        // Formatear el hash
        hash = hash % 1000000;
        stringstream ss;
        ss << "H" << setw(6) << setfill('0') << hash;
        return ss.str();
    }

    // Crear o actualizar el archivo de historial de cliente
    void actualizarArchivoCliente(const Cliente& cliente) {
        ofstream archivo(carpetaClientes + cliente.getIdHash() + ".txt");
        if (archivo.is_open()) {
            archivo << "Nombre=" << cliente.getNombre() << endl;
            archivo << "Apellido=" << cliente.getApellido() << endl;
            archivo << "Email=" << cliente.getEmail() << endl;
            archivo << "VuelosRealizados=" << cliente.getVuelosRealizados() << endl;
            archivo << "PuntosAcumulados=" << cliente.getPuntosAcumulados() << endl;
            archivo.close();
        }
    }

    // Cargar el último ID usado
    void cargarUltimoId() {
        ifstream archivo("Archivos//ultimo_id_cliente.txt");
        if (archivo.is_open()) {
            archivo >> ultimoId;
            archivo.close();
        }
        else {
            ultimoId = 10000; // Valor inicial si no existe el archivo
        }
    }

    // Leer los clientes existentes desde el archivo (salta las 3 primeras líneas - admins)
    void cargarClientesDesdeArchivo() {
        ifstream archivo(archivoUsuarios);
        if (!archivo.is_open()) {
            return;
        }

        // Saltear las primeras 3 líneas (administradores)
        string linea;
        for (int i = 0; i < 3; i++) {
            getline(archivo, linea);
        }

        // Leer los clientes
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            size_t pos1 = linea.find('|');

            // Si el formato ya incluye ID (nuevo formato)
            if (pos1 != string::npos && linea.substr(0, 1) == "H") { // Si empieza con H, asumimos que es un ID hasheado
                string idHash = linea.substr(0, pos1);
                linea = linea.substr(pos1 + 1); // Quitamos el ID hash

                pos1 = linea.find('|');
                string idOriginal = linea.substr(0, pos1);
                linea = linea.substr(pos1 + 1); // Quitamos el ID original

                // Ahora procesamos el resto del formato antiguo
                pos1 = linea.find('|');
                size_t pos2 = linea.find('|', pos1 + 1);
                size_t pos3 = linea.find('|', pos2 + 1);
                size_t pos4 = linea.find('|', pos3 + 1);

                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos && pos4 != string::npos) {
                    string nombre = linea.substr(0, pos1);
                    string apellido = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                    string email = linea.substr(pos2 + 1, pos3 - pos2 - 1);
                    string contrasena = linea.substr(pos3 + 1, pos4 - pos3 - 1);
                    string tipo = linea.substr(pos4 + 1);

                    if (tipo == "Cliente") {
                        // Intentar cargar datos adicionales del archivo de cliente
                        int puntosAcumulados = 0;
                        int vuelosRealizados = 0;

                        ifstream archivoCliente(carpetaClientes + idHash + ".txt");
                        if (archivoCliente.is_open()) {
                            string lineaCliente;
                            while (getline(archivoCliente, lineaCliente)) {
                                if (lineaCliente.find("PuntosAcumulados=") == 0) {
                                    puntosAcumulados = stoi(lineaCliente.substr(17));
                                }
                                else if (lineaCliente.find("VuelosRealizados=") == 0) {
                                    vuelosRealizados = stoi(lineaCliente.substr(17));
                                }
                            }
                            archivoCliente.close();
                        }

                        Cliente cliente(idOriginal, idHash, nombre, apellido, email, contrasena, puntosAcumulados, vuelosRealizados);
                        arbolClientes.insertar(cliente, idHash); // Usamos el ID hasheado como clave
                    }
                }
            }
            // Si es formato antiguo (sin ID)
            else {
                pos1 = linea.find('|');
                size_t pos2 = linea.find('|', pos1 + 1);
                size_t pos3 = linea.find('|', pos2 + 1);
                size_t pos4 = linea.find('|', pos3 + 1);

                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos && pos4 != string::npos) {
                    string nombre = linea.substr(0, pos1);
                    string apellido = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                    string email = linea.substr(pos2 + 1, pos3 - pos2 - 1);
                    string contrasena = linea.substr(pos3 + 1, pos4 - pos3 - 1);
                    string tipo = linea.substr(pos4 + 1);

                    if (tipo == "Cliente") {
                        // Generar nuevos IDs
                        string idOriginal = generarIdOriginal();
                        string idHash = generarIdHash(idOriginal, nombre);

                        Cliente cliente(idOriginal, idHash, nombre, apellido, email, contrasena);
                        arbolClientes.insertar(cliente, idHash); // Usamos el ID hasheado como clave

                        // Crear archivo de historial
                        actualizarArchivoCliente(cliente);
                    }
                }
            }
        }

        archivo.close();
    }

public:
    GestorClientes(const string& archivo = "Archivos//usuarios.txt", const string& carpeta = "Archivos//Clientes//")
        : archivoUsuarios(archivo), carpetaClientes(carpeta), ultimoId(0)
    {
        _mkdir("Archivos//Clientes"); // Crear carpeta si no existe
        cargarUltimoId();
        cargarClientesDesdeArchivo();
    }

    // Agrega un nuevo cliente
    Cliente agregarCliente(const string& nombre, const string& apellido, const string& email, const string& contrasena) {
        // Generar IDs para el nuevo cliente
        string idOriginal = generarIdOriginal();
        string idHash = generarIdHash(idOriginal, nombre);

        // Crear cliente
        Cliente cliente(idOriginal, idHash, nombre, apellido, email, contrasena);

        // Añadir al árbol
        arbolClientes.insertar(cliente, idHash);

        // Guardar en archivo de usuarios
        ofstream archivo(archivoUsuarios, ios::app);
        if (archivo.is_open()) {
            archivo << idHash << "|" << idOriginal << "|"
                << nombre << "|" << apellido << "|"
                << email << "|" << contrasena << "|"
                << "Cliente" << endl;
            archivo.close();
        }

        // Crear archivo de historial
        actualizarArchivoCliente(cliente);

        return cliente;
    }

    // Busca un cliente por ID
    Cliente* buscarClientePorId(const string& id) {
        return arbolClientes.buscar(id);
    }

    // Busca un cliente por email
    Cliente* buscarClientePorEmail(const string& email) {
        vector<Cliente> todos = arbolClientes.obtenerTodosEnOrden();
        for (auto& cliente : todos) {
            if (cliente.getEmail() == email) {
                return buscarClientePorId(cliente.getIdHash());
            }
        }
        return nullptr;
    }

    // Actualiza puntos y vuelos de un cliente
    bool actualizarCliente(Cliente& cliente, float monto = 0.0f, bool incrementarVuelo = false) {
        if (incrementarVuelo) {
            cliente.incrementarVuelos();
        }

        if (monto > 0) {
            cliente.acumularPuntos(monto);
        }

        // Actualizar en el árbol
        arbolClientes.insertar(cliente, cliente.getIdHash());

        // Actualizar archivo de historial
        actualizarArchivoCliente(cliente);

        return true;
    }

    // Actualiza el archivo de usuarios para reflejar los cambios en clientes
    void actualizarArchivoUsuarios() {
        vector<string> admins;

        // Leer administradores (primeras 3 líneas)
        ifstream lecturaAdmins(archivoUsuarios);
        string linea;
        for (int i = 0; i < 3; i++) {
            if (getline(lecturaAdmins, linea)) {
                admins.push_back(linea);
            }
        }
        lecturaAdmins.close();

        // Reescribir el archivo
        ofstream archivo(archivoUsuarios);
        if (!archivo.is_open()) return;

        // Escribir admins primero
        for (const auto& admin : admins) {
            archivo << admin << endl;
        }

        // Escribir clientes
        vector<Cliente> clientes = arbolClientes.obtenerTodosEnOrden();
        for (const auto& cliente : clientes) {
            archivo << cliente.getIdHash() << "|" << cliente.getIdOriginal() << "|"
                << cliente.getNombre() << "|" << cliente.getApellido() << "|"
                << cliente.getEmail() << "|" << cliente.getContraseña() << "|"
                << "Cliente" << endl;
        }

        archivo.close();
    }

    // Obtener todos los clientes ordenados
    vector<Cliente> obtenerTodosLosClientes() const {
        return arbolClientes.obtenerTodosEnOrden();
    }
};