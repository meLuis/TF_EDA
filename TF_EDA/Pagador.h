#pragma once
#include <string>
using namespace std;

class Pagador {
private:
    string nombre;
    string apellido;
    string dni;
    int numTarjeta;
    float totalPagado;
    int idReserva;

public:
    Pagador() : numTarjeta(0), totalPagado(0.0f), idReserva(0) {}

    Pagador(const string& nombre, const string& apellido, const string& dni,
        int numTarjeta, float totalPagado, int idReserva)
        : nombre(nombre), apellido(apellido), dni(dni), numTarjeta(numTarjeta),
        totalPagado(totalPagado), idReserva(idReserva) {}


    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    string getDni() const { return dni; }
    int getNumTarjeta() const { return numTarjeta; }
    float getTotalPagado() const { return totalPagado; }
    int getIdReserva() const { return idReserva; }


    void setNombre(const string& nombre) { this->nombre = nombre; }
    void setApellido(const string& apellido) { this->apellido = apellido; }
    void setDni(const string& dni) { this->dni = dni; }
    void setNumTarjeta(int numTarjeta) { this->numTarjeta = numTarjeta; }
    void setTotalPagado(float totalPagado) { this->totalPagado = totalPagado; }
    void setIdReserva(int idReserva) { this->idReserva = idReserva; }


    bool operator<(const Pagador& other) const {
        return totalPagado < other.totalPagado;
    }

    bool operator>(const Pagador& other) const {
        return totalPagado > other.totalPagado;
    }

    bool operator==(const Pagador& other) const {
        return dni == other.dni;
    }
};