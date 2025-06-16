#include <iostream>
#include "Menu.h"

int main()
{
    system("color 74");
    int opcion;
    string opcionRegistro;
    Menu menu;


    while (true) {
        menu.menuPricipal();
        cin >> opcion;
        cin.ignore();
        Usuario* user = nullptr;

        switch (opcion)
        {
        case 1:
            system("cls");
            user = menu.iniciarSesion();
            if (user != nullptr) {
                system("cls");
                user->mostrarBienvenida();
                if (user->getTipo() == "Cliente") {
                    menu.realizarReservas();
                    system("pause>0");
                }
                else if (user->getTipo() == "Administrador") {
                    menu.menuAdministrador(user);
                }

                delete user;
            }
            else {
                cout << "Error al iniciar sesion." << endl;
                system("pause>0");
                system("cls");
            }
            break;
        case 2:
            menu.deseasRegistrarte();
            getline(cin, opcionRegistro);
            for (int i = 0; i < opcionRegistro.length(); i++) {
                opcionRegistro[i] = toupper(opcionRegistro[i]);
            }
            if (opcionRegistro == "SI") {
                menu.registro();
                system("pause>0");
                system("cls");
                menu.realizarReservas();
                system("pause>0");
            }
            else if (opcionRegistro == "NO") {
                system("cls");
                menu.realizarReservas();
                system("pause>0");
            }
            else {
                cout << "Opcion invalida. Intente nuevamente.\n";
            }
            break;
        case 3:
            cout << "Saliendo del sistema." << endl;
            return 0;
        }
    }

    return 0;
}

