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

	string getEmail() { return eMail; }
	string getNombre() { return nombre; }
	string getApellido() { return apellido; }
	string getContraseña() { return contraseña; }

	virtual string getTipo() { return "sin definir"; };
	virtual void mostrarBienvenida() {
		cout << "Bienvenido " << nombre << " " << apellido << endl;
	};
};
