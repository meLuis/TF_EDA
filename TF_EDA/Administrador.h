#pragma once
#include "Usuario.h"

class Administrador : public Usuario
{
public:
	Administrador() {}
	Administrador(string nom, string a, string e, string c) : Usuario(nom, a, e, c) {}

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


};
