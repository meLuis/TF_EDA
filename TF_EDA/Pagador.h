#pragma once
#include <string>
using namespace std;
enum TipoPagador {
    persona,
    organizacion
};
class Pagador {
private:
    string nombre;
    string ruc;
    string apellido;
    string dni;
    int numTarjeta;
    float totalPagado;
    int idReserva;
    TipoPagador tipoPagador;

public:
    Pagador() : numTarjeta(0), totalPagado(0.0f), idReserva(0) {}

    Pagador(const string& nombre, const string& apellido, const string& dni,
        int numTarjeta, float totalPagado, int idReserva, TipoPagador tipopagador)
        : nombre(nombre), apellido(apellido), dni(dni), numTarjeta(numTarjeta),
        totalPagado(totalPagado), idReserva(idReserva), tipoPagador(tipopagador) {}

    Pagador(const string& nombre, const string& ruc, int numTarjeta, float totalPagado, int idReserva, TipoPagador tipopagador)
        : nombre(nombre), ruc(ruc), numTarjeta(numTarjeta), totalPagado(totalPagado), idReserva(idReserva), tipoPagador(tipopagador) {}

    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    string getDni() const { return dni; }
    string getRuc() const { return ruc; }
    int getNumTarjeta() const { return numTarjeta; }
    float getTotalPagado() const { return totalPagado; }
    int getIdReserva() const { return idReserva; }
    TipoPagador getTipoPagador() const { return tipoPagador; }

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