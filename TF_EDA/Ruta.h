#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Ruta {
private:
    string horaInicio;
    string horaFin;
    string duracion;
    vector<string> paradas;
    float precio;

public:
    Ruta() : horaInicio(""), horaFin(""), duracion(""), precio(0.0) {}

    Ruta(string horaInicio, string horaFin, string duracion, vector<string> paradas, float precio) {
        this->horaInicio = horaInicio;
        this->horaFin = horaFin;
        this->duracion = duracion;
        this->paradas = paradas;
        this->precio = precio;
    }

    void mostrar() {
        cout << horaInicio << " - " << horaFin;
        cout << " | Duracion: " << duracion;
        cout << " | Precio: $" << precio << endl;

        for (int i = 0; i < paradas.size(); i++) {
            cout << "\t\t\t\t" << paradas[i];
            if (i != paradas.size() - 1) cout << " ---> ";
        }
        cout << endl << endl;
    }

    float getPrecio() { return precio; }
    string getDuracion() { return duracion; }
    string getHoraInicio() { return horaInicio; }
    string getHoraFin() { return horaFin; }
    int getDuracionEnMinutos() {
        int posH = duracion.find('h');
        int posM = duracion.find('m');
        int horas = stoi(duracion.substr(0, posH));
        int minutos = stoi(duracion.substr(posH + 1, posM - posH - 1));
        return horas * 60 + minutos;
    }
};
