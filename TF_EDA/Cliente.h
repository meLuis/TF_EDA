#pragma once
#include "Usuario.h"

class Cliente : public Usuario
{
public:
	Cliente() {}
	Cliente(string nom, string a, string e, string c) : Usuario(nom, a, e, c) {}

	~Cliente() {}

	string getTipo() {
		return "Cliente";
	}
	void mostrarBienvenida() override {
		cout << "Bienvenido Cliente " << nombre << " " << apellido << endl;
	};

};
