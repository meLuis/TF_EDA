#pragma once
#include "Usuario.h"

class Cliente : public Usuario
{
private:
    string idOriginal;   // ID secuencial único
    string idHash;       // ID hasheado para búsquedas
    int puntosAcumulados;
    int vuelosRealizados;

public:
    Cliente() : idOriginal(""), idHash(""), puntosAcumulados(0), vuelosRealizados(0) {}

    Cliente(string nom, string a, string e, string c) :
        Usuario(nom, a, e, c), idOriginal(""), idHash(""), puntosAcumulados(0), vuelosRealizados(0) {
    }

    Cliente(string id, string idH, string nom, string a, string e, string c, int puntos = 0, int vuelos = 0) :
        Usuario(nom, a, e, c), idOriginal(id), idHash(idH), puntosAcumulados(puntos), vuelosRealizados(vuelos) {
    }

    ~Cliente() {}

    // Getters y setters para los nuevos campos
    string getIdOriginal() const { return idOriginal; }
    string getIdHash() const { return idHash; }
    int getPuntosAcumulados() const { return puntosAcumulados; }
    int getVuelosRealizados() const { return vuelosRealizados; }

    void setIdOriginal(const string& id) { idOriginal = id; }
    void setIdHash(const string& id) { idHash = id; }
    void setPuntosAcumulados(int puntos) { puntosAcumulados = puntos; }
    void incrementarVuelos() { vuelosRealizados++; }

    string getTipo() override {
        return "Cliente";
    }

    void mostrarBienvenida() override {
        cout << "Bienvenido Cliente " << nombre << " " << apellido << endl;
        if (puntosAcumulados > 0) {
            cout << "\t\t\tPuntos acumulados: " << puntosAcumulados << endl;
        }
    }

    // Método para acumular puntos basado en gasto
    void acumularPuntos(float monto) {
        puntosAcumulados += static_cast<int>(monto * 1.2f);
    }
};