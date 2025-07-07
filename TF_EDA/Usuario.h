#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Usuario
{
protected:
	string nombre;
	string apellido;
	string eMail;
	string contraseña;
public:
	Usuario() {}
	Usuario(string nom, string a, string e, string c) : nombre(nom), apellido(a), eMail(e), contraseña(c) {}

	~Usuario() {}

	string getEmail() const { return eMail; }
	string getNombre() const { return nombre; }
	string getApellido() const { return apellido; }
	string getContraseña() const { return contraseña; }

	virtual string getTipo() { return "sin definir"; };
	virtual void mostrarBienvenida() {
		cout << "Bienvenido " << nombre << " " << apellido << endl;
	};
};
