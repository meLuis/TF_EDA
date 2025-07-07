#pragma once
#include "ListaReservas.h"
#include "Administrador.h"
#include "GestorVuelos.h"
#include "GestorPasajero.h"
#include "Cliente.h"
#include "ABBReserva.h"
#include "ColaPagos.h"
#include "GrafoVuelos.h"
#include "GestorClientes.h"
class Menu
{
private:
    ColaPagos <Pago>colaPagos;
    ListaReservas<Reserva> listaReservas;
    string fecha;
    string opcionAdicionales;
    int opcionVuelo;
    int origen, destino;
    int cantPasajeros;
    string archivoReservas = "Archivos//reserva.txt";
    string archivoPasajeros = "Archivos//pasajeros.txt";
    MapaRutas vuelos;
    GestorVuelos gestorVuelos;
    GrafoVuelos* grafoVuelos;
    int diaActual, mesActual, anioActual;
    GestorClientes gestorClientes;
    void obtenerFechaActual() {
        time_t tiempoActual = time(nullptr);
        tm tiempoLocal;
        localtime_s(&tiempoLocal, &tiempoActual);
        diaActual = tiempoLocal.tm_mday;
        mesActual = tiempoLocal.tm_mon + 1;
        anioActual = tiempoLocal.tm_year + 1900;
    }

public:
    Menu() {
        obtenerFechaActual();
        grafoVuelos = new GrafoVuelos(&vuelos);
    }

    ~Menu() {
        if (grafoVuelos) delete grafoVuelos;
    }
    void guardarUsuarioArchivo(Usuario& user) {
        ofstream archivo("Archivos//usuarios.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }
        archivo << endl << user.getNombre() << "|" << user.getApellido() << "|"
            << user.getEmail() << "|"
            << user.getContraseña() << "|"
            << user.getTipo() << endl;
        archivo.close();
        cout << "Usuario registrado con exito." << endl;
    }


    void registro() {
        string nombre, apellido, email, dni, contraseña;

        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t------------ REGISTRO DE USUARIO ------------" << endl;
        cout << "" << endl;

        // Función lambda para validar campos
        auto leerCampo = [](const string& mensaje) -> string {
            string valor;
            do {
                cout << mensaje;
                getline(cin, valor);
                if (valor.empty()) {
                    cout << "\t\t\tEs obligatorio llenar este campo.\n";
                }
            } while (valor.empty());
            return valor;
            };

        nombre = leerCampo("\t\t\tNombre: ");
        apellido = leerCampo("\t\t\tApellido: ");
        dni = leerCampo("\t\t\tDNI: ");
        email = leerCampo("\t\t\tEmail: ");
        contraseña = leerCampo("\t\t\tContrasena: ");
        Cliente nuevoCliente = gestorClientes.agregarCliente(nombre, apellido, email, contraseña);
        cout << "Usuario registrado con éxito con ID: " << nuevoCliente.getIdOriginal() << endl;

    }
    Usuario* iniciarSesion() {
        string emailIngresado, contrasenaIngresada;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t------------ INICIO DE SESION ------------" << endl;
        cout << " " << endl;

        do {
            cout << "\t\t Email: ";
            getline(cin, emailIngresado);
            if (emailIngresado.empty()) {
                cout << "Es obligatorio llenar este campo.\n";
            }
        } while (emailIngresado.empty());

        do {
            cout << "\t\t Contrasena: ";
            getline(cin, contrasenaIngresada);
            if (contrasenaIngresada.empty()) {
                cout << "Es obligatorio llenar este campo.\n";
            }
        } while (contrasenaIngresada.empty());

        // Intentar encontrar un cliente
        Cliente* cliente = gestorClientes.buscarClientePorEmail(emailIngresado);
        if (cliente && cliente->getContraseña() == contrasenaIngresada) {
            return new Cliente(*cliente);
        }

        // Si no es cliente, buscar si es admin (los admins siguen con el formato original)
        ifstream archivo("Archivos//usuarios.txt");
        if (!archivo) {
            cout << "No hay usuarios registrados." << endl;
            return nullptr;
        }

        string linea;
        for (int i = 0; i < 3; i++) { // Solo leer las primeras 3 líneas (admins)
            if (getline(archivo, linea)) {
                size_t pos1 = linea.find('|');
                size_t pos2 = linea.find('|', pos1 + 1);
                size_t pos3 = linea.find('|', pos2 + 1);
                size_t pos4 = linea.find('|', pos3 + 1);

                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos && pos4 != string::npos) {
                    string nombre = linea.substr(0, pos1);
                    string apellido = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                    string emailArchivo = linea.substr(pos2 + 1, pos3 - pos2 - 1);
                    string contrasenaArchivo = linea.substr(pos3 + 1, pos4 - pos3 - 1);
                    string tipo = linea.substr(pos4 + 1);

                    if (emailIngresado == emailArchivo && contrasenaIngresada == contrasenaArchivo && tipo == "Administrador") {
                        archivo.close();
                        return new Administrador(nombre, apellido, emailArchivo, contrasenaArchivo);
                    }
                }
            }
        }

        archivo.close();
        cout << "Usuario no encontrado, intente nuevamente.\n";
        return nullptr;
    }

    void anularReserva(const vector<Reserva>& reservas, const string& archivoReservas) {
        vector<string> todasLasLineas;
        ifstream archivoLectura(archivoReservas);
        if (archivoLectura.is_open()) {
            string linea;
            while (getline(archivoLectura, linea)) {
                todasLasLineas.push_back(linea);
            }
            archivoLectura.close();
        }
        else
        {
            cout << "\t\t\t Error al abrir el archivo." << endl;
            return;
        }

        //IA
        // Ahora abrir el archivo para escribir
        ofstream archivoEscritura(archivoReservas, ios::trunc);

        if (!archivoEscritura.is_open()) {
            cout << "\t\t\t Error al abrir el archivo para escritura." << endl;
            return;
        }

        // Mantener un registro de los IDs de reserva que han sido anulados
        vector<int> idsAnulados;
        for (const Reserva& r : reservas) {
            if (r.getEstado() == "Anulada") {
                idsAnulados.push_back(r.getIdReserva());
            }
        }

        // Procesar cada línea del archivo
        for (const string& linea : todasLasLineas) {
            // Verificar si esta línea es una reserva (contiene suficientes '|')
            int contadorDelimitadores = 0;
            for (char c : linea) {
                if (c == '|') contadorDelimitadores++;
            }

            // Si tiene suficientes delimitadores para ser una reserva
            if (contadorDelimitadores >= 5) {
                // Extraer el ID de la reserva
                size_t pos = linea.find('|');
                if (pos != string::npos) {
                    string idStr = linea.substr(0, pos);
                    try {
                        int id = stoi(idStr);

                        // Si esta reserva debe ser anulada
                        bool debeAnular = false;
                        for (int idAnulado : idsAnulados) {
                            if (id == idAnulado) {
                                debeAnular = true;
                                break;
                            }
                        }

                        if (debeAnular) {
                            // Reconstruir la línea con el estado "Anulada"
                            vector<string> campos;
                            string temp = linea;
                            size_t posTemp;

                            // Dividir la línea en campos
                            while ((posTemp = temp.find('|')) != string::npos) {
                                campos.push_back(temp.substr(0, posTemp));
                                temp = temp.substr(posTemp + 1);
                            }
                            campos.push_back(temp); // Último campo

                            // Cambiar el campo de estado (6º elemento, índice 5)
                            if (campos.size() > 5) {
                                campos[5] = "Anulada";
                            }

                            // Reconstruir la línea
                            string nuevaLinea = "";
                            for (size_t i = 0; i < campos.size(); i++) {
                                nuevaLinea += campos[i];
                                if (i < campos.size() - 1) nuevaLinea += "|";
                            }

                            archivoEscritura << nuevaLinea << endl;
                            continue; // Pasar a la siguiente línea
                        }
                    }
                    catch (const std::invalid_argument& e) {
                        // No es un ID válido, mantener la línea original
                    }
                }
            }

            // Si no es una reserva o no necesita ser modificada, escribir la línea original
            archivoEscritura << linea << endl;
        }

        archivoEscritura.close();
    }
    void buscarReservaPorID() {
        ABBReserva arbol;
        arbol.cargarDesdeArchivo(archivoReservas); // Cargar reservas desde el archivo

        int id;
        cout << "\t\t\tIngrese el ID de la reserva a buscar: ";
        cin >> id;
        cin.ignore();

        Reserva* encontrada = arbol.buscar(id);
        if (encontrada) {
            cout << "\n\t\t\tReserva encontrada:\n";
            encontrada->mostrar();

            int opcion;
            cout << "\n\t\t\t1. Anular reserva" << endl;
            cout << "\t\t\t2. Retroceder" << endl;
            cout << "\t\t\tSeleccione una opción: ";
            cin >> opcion;

            if (opcion == 1) {
                if (encontrada->getEstado() != "Anulada") {
                    encontrada->setEstado("Anulada");
                    arbol.guardarEnArchivo(archivoReservas); // Guardar el árbol actualizado
                    cout << "\t\t\tReserva anulada correctamente." << endl;
                }
                else {
                    cout << "\t\t\tLa reserva ya está anulada." << endl;
                }
            }
            else if (opcion == 2) {
                cout << "\t\t\tRetrocediendo..." << endl;
            }
            else {
                cout << "\t\t\tOpción inválida." << endl;
            }
        }
        else {
            cout << "\t\t\tReserva no encontrada." << endl;
        }

        system("pause>0");
    }

    void menuConsultasPagadores() {
        AVLPagadores<Pagador> arbol("Archivos//pagantes.txt");

        int opcion;
        do {
            system("cls");
            cout << "  " << endl;
            cout << "  " << endl;
            cout << "\t\t\t------------ CONSULTAS DE PAGADORES ------------";
            cout << "  " << endl;
            cout << "\t\t\t       [1]. Mostrar todos los pagadores\n";
            cout << "\t\t\t       [2]. Mostrar solo personas (Boletas)\n";
            cout << "\t\t\t       [3]. Mostrar solo organizaciones (Facturas)\n";
            cout << "\t\t\t       [4]. Buscar pagador específico\n";
            cout << "\t\t\t       [0]. Volver al menú anterior\n";
            cout << "  " << endl;
            cout << "\t\t\t        Seleccione opción: ";
            cin >> opcion;
            cin.ignore();

            switch (opcion) {
            case 1:
                arbol.mostrar();
                break;
            case 2:
                arbol.mostrarPorTipo(persona);
                break;
            case 3:
                arbol.mostrarPorTipo(organizacion);
                break;
            case 4: {
                int tipo;
                string criterio;
                cout << " " << endl;
                cout << "\t\t\t    Buscar por:\n";
                cout << " " << endl;
                cout << "\t\t\t    [1]. DNI\n";
                cout << "\t\t\t    [2]. Codigo de reserva\n";
                cout << "\t\t\t    [3]. RUC\n";
                cout << "\t\t\t    [4]. Retornar\n";
                cout << " " << endl;
                cout << "\t\t\t    Seleccione tipo de busqueda: ";
                cin >> tipo;
                cin.ignore();
                cout << "\t\t\tIngrese valor: ";
                getline(cin, criterio);
                Pagador* encontrado = arbol.buscar(criterio, tipo);
                if (encontrado) {
                    cout << "\n\t\t\tPagador encontrado:\n";
                    cout << "\t\t\tNombre: " << encontrado->getNombre() << "\n";
                    if (encontrado->getTipoPagador() == persona) {
                        cout << "\t\t\tApellido: " << encontrado->getApellido() << "\n";
                        cout << "\t\t\tDNI: " << encontrado->getDni() << "\n";
                    }
                    else {
                        cout << "\t\t\tRUC: " << encontrado->getRuc() << "\n";
                    }
                    cout << "\t\t\tTotal pagado: S/. " << encontrado->getTotalPagado() << "\n";
                    cout << "\t\t\tID Reserva: " << encontrado->getIdReserva() << "\n";
                }
                else {
                    cout << "\t\t\tNo se encontró el pagador.\n";
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "\t\t\tOpción inválida.\n";
            }

            if (opcion != 0) {
                system("pause");
            }

        } while (opcion != 0);
    }
    void menuAdministrador(Usuario* user) {
        Administrador* admin = dynamic_cast<Administrador*>(user);

        int opcion;
        do {
            system("cls");
            cout << "\t\t\t" << endl;
            user->mostrarBienvenida();
            cout << "\t\t\t" << endl;
            cout << "\t\t\t------------ MENU ADMINISTRADOR ------------" << endl;
            cout << "  " << endl;
            cout << "\t\t\t            [1] Buscar reserva               " << endl;
            cout << "\t\t\t            [2] Gestionar vuelos             " << endl;
            cout << "\t\t\t            [3] Gestionar promociones        " << endl;
            cout << "\t\t\t            [4] Generar reportes             " << endl;
            cout << "\t\t\t            [5] Búsqueda avanzada de vuelos  " << endl;
            cout << "\t\t\t            [6] Consultar pagadores          " << endl;
            cout << "\t\t\t            [7] Análisis de grafos de vuelos " << endl;
            cout << "\t\t\t            [8] Salir                        " << endl;
            cout << "\t\t\t " << endl;
            cout << "\t\t\t-------------------------------------------" << endl;
            cout << "\t\t\t           Seleccione una opción: ";

            string input;
            getline(cin, input);
            try {
                opcion = stoi(input);
            }
            catch (...) {
                opcion = -1;
            }

            switch (opcion) {
            case 1:
                buscarReservaPorID();
                system("pause>0");
                break;
            case 2: {
                int subopcion;
                do {
                    system("cls");
                    cout << "\n\t\t\t------------ GESTION DE VUELOS ------------\n";
                    cout << "  " << endl;
                    cout << "\t\t\t        [1]. Buscar vuelo por ID\n";
                    cout << "\t\t\t        [2]. Modificar precio de vuelo\n";
                    cout << "\t\t\t        [3]. Buscar vuelos por hora de salida\n";
                    cout << "\t\t\t        [4]. Buscar vuelos por duracion maxima\n";
                    cout << "\t\t\t        [5]. Buscar vuelos entre paises\n";
                    cout << "\t\t\t        [0]. Volver al menu anterior\n";
                    cout << "  " << endl;
                    cout << "\t\t\t           Seleccione una opcion: ";
                    cin >> subopcion;
                    cin.ignore();

                    switch (subopcion) {
                    case 1:
                        admin->buscarVuelo();
                        system("pause>0");
                        break;
                    case 2:
                        admin->modificarPrecioVuelo();
                        system("pause>0");
                        break;
                    case 3: {

                        string hora;
                        cout << "\t\t\tIngrese hora de salida (formato HH:MM): ";
                        getline(cin, hora);

                        auto vuelos = admin->buscarVuelosPorHora(hora);

                        cout << "\n\t\t\tSe encontraron " << vuelos.size() << " vuelos con salida a las " << hora << ":\n\n";
                        for (const auto& vuelo : vuelos) {

                            cout << "\t\t\t";
                            vuelo.mostrar();
                            cout << "\n";
                        }
                        system("pause>0");
                        break;
                    }
                    case 4: {

                        int minutos;
                        cout << "\t\t\tIngrese duracion maxima en minutos: ";
                        cin >> minutos;
                        cin.ignore();

                        auto vuelos = admin->buscarVuelosPorDuracion(minutos);

                        cout << "\n\t\t\tSe encontraron " << vuelos.size() << " vuelos con duracion menor a "
                            << minutos << " minutos:\n\n";
                        for (const auto& vuelo : vuelos) {
                            cout << "\t\t\t";
                            vuelo.mostrar();
                            cout << "\n";
                        }
                        system("pause>0");
                        break;
                    }
                    case 5: {

                        string pais;
                        cout << "\t\t\tIngrese pais de origen o destino: ";
                        getline(cin, pais);

                        auto vuelos = admin->buscarVuelosPorPais(pais);

                        cout << "\n\t\t\tSe encontraron " << vuelos.size() << " vuelos con origen o destino en "
                            << pais << ":\n\n";
                        for (const auto& vuelo : vuelos) {
                            cout << "\t\t\t";
                            vuelo.mostrar();
                            cout << "\n";
                        }
                        system("pause>0");
                        break;
                    }
                    case 0:
                        break;
                    default:
                        cout << "\t\t\tOpción invalida.\n";
                        system("pause>0");
                    }
                } while (subopcion != 0);
                break;
            }
            case 3: {
                int subopcion;
                system("cls");
                cout << "\n\t\t\t------------ GESTION DE PROMOCIONES ------------\n";
                cout << "  " << endl;
                cout << "\t\t\t       [1]. Aplicar descuento a vuelos internacionales\n";
                cout << "\t\t\t       [2]. Aplicar descuento a vuelos a un país especifico\n";
                cout << "\t\t\t       [0]. Volver al menu anterior\n";
                cout << "  " << endl;
                cout << "\t\t\t           Seleccione una opcion: ";
                cin >> subopcion;
                cin.ignore();
                if (subopcion == 0) { break; }
                float porcentaje;
                cout << "\t\t\tIngrese porcentaje de descuento (1-50): ";
                cin >> porcentaje;
                cin.ignore();

                int vuelosAfectados = 0;

                switch (subopcion) {
                case 1:
                    vuelosAfectados = admin->aplicarDescuentoInternacionales(porcentaje);
                    break;
                case 2: {
                    string pais;
                    cout << "\t\t\tIngrese país: ";
                    getline(cin, pais);
                    vuelosAfectados = admin->aplicarDescuentoPorPais(pais, porcentaje);
                    break;
                }
                default:
                    cout << "\t\t\tOpción invalida.\n";
                }

                if (subopcion >= 1 && subopcion <= 3) {
                    cout << "\t\t\t\tSe aplico descuento del " << porcentaje << "% a "
                        << vuelosAfectados << " vuelos." << endl;
                }
                system("pause>0");
                break;
            }
            case 4:
                admin->generarReportes();
                system("pause>0");
                break;
            case 5:
                system("cls");
                cout << "\t\t\t ------------BUSQUEDA AVANZADA------------\t\t\t\n";
                admin->busquedaAvanzada();
                system("pause>0");
                break;
            case 6:
                menuConsultasPagadores();
                break;
            case 7: {
                system("cls");
                cout << "\n\t\t\t------------ ANÁLISIS DE GRAFOS DE VUELOS ------------\n";
                cout << "  " << endl;
                cout << "\t\t\t            [1]. Visualizar grafo de conexiones\n";
                cout << "\t\t\t            [2]. Analizar centralidad de aeropuertos\n";
                cout << "\t\t\t            [3]. Analizar eficiencia de la red\n";
                cout << "\t\t\t            [0]. Volver al menu anterior\n";
                cout << "  " << endl;
                cout << "\t\t\t           Seleccione una opcion: ";
                int subopcion;
                cin >> subopcion;
                cin.ignore();

                switch (subopcion) {
                case 1:
                    grafoVuelos->visualizarGrafo();
                    break;
                case 2:
                    grafoVuelos->analizarCentralidad();
                    break;
                case 3:
                    grafoVuelos->analizarEficienciaRed();
                    break;
                case 0:
                    break;
                default:
                    cout << "\t\t\tOpcion invalida.\n";
                }
                system("pause>0");
                break;
            }
            case 8:
                break;
            default:
                cout << "\t\t\tOpcion invalida." << endl;
                system("pause>0");
            }
        } while (opcion != 8);
    }


    bool confirmarDatos(const string& mensaje) {
        string confirmacion;

        do {
            cout << "\t\t\t" << mensaje << " (Si/No): ";
            cin >> confirmacion;


            transform(confirmacion.begin(), confirmacion.end(), confirmacion.begin(), ::tolower);

        } while (confirmacion != "s" && confirmacion != "n" && confirmacion != "si" && confirmacion != "no");

        return (confirmacion == "s" || confirmacion == "si");
    }

    void realizarReservas() {
        string _fecha;
        Vuelo vueloSeleccionado;
        vector<Pasajero> pasajeros;
        int etapa = 0;
        bool cd = false;
        int origen, destino, cantPasajeros;

        while (etapa < 5) {
            switch (etapa) {
            case 0:
                cout << "\t\t\t------------------------- RESERVA -------------------------  " << endl;
                if (!validarFecha(_fecha)) return;
                this->fecha = _fecha;
                origen = seleccionarOrigen();
                if (origen == -1) return;
                system("pause>0");
                system("cls");

                destino = seleccionarDestino(origen);
                if (destino == -1) return;

                cantPasajeros = ingresarCantidadPasajeros();
                if (cantPasajeros <= 0) return;

                if (confirmarDatos("\t Los datos ingresados son correctos?")) {
                    cd = true;
                    cout << "\t\t\t Buscar.... " << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                    system("cls");
                    etapa++;
                }
                else {
                    cout << "\t\t\tPor favor, ingrese los datos nuevamente." << endl;
                    system("pause>0");
                    system("cls");
                }
                break;

            case 1:
                vueloSeleccionado = seleccionarVuelo(origen, destino);
                if (confirmarDatos("\t Los datos ingresados son correctos?")) {
                    cd = true;
                    system("pause>0");
                    system("cls");
                    etapa++;
                }
                else {
                    cout << "\t\t\tPor favor, ingrese los datos nuevamente." << endl;
                    system("pause>0");
                    system("cls");
                }
                break;

            case 2:
                pasajeros = manejarEquipajeYAsientos(cantPasajeros, vueloSeleccionado);
                if (pasajeros.empty()) return;

                if (confirmarDatos("\t Los datos ingresados son correctos?")) {
                    cd = true;
                    system("pause>0");
                    system("cls");
                    etapa++;
                }
                else {
                    cout << "\t\t\tPor favor, ingrese los datos nuevamente." << endl;
                    system("pause>0");
                    system("cls");
                }
                break;

            case 3:
                cin.ignore();
                registrarPasajeros(pasajeros);

                if (confirmarDatos("\t Los datos ingresados son correctos?")) {
                    cd = true;
                    system("pause>0");
                    system("cls");
                    etapa++;
                }
                else {
                    cout << "\t\t\tPor favor, ingrese los datos nuevamente." << endl;
                    system("pause>0");
                    system("cls");
                }
                break;
            case 4:
                float precioTotal = calcularPrecioTotal(pasajeros);
                procesarPago(precioTotal, pasajeros, vueloSeleccionado);
                etapa++;
                break;
            }
        }
    }


    bool validarFecha(string& fecha) {
        auto validar = [this](string& fecha) {
            if (fecha.length() != 10 || fecha[2] != '/' || fecha[5] != '/') {
                cout << "\t\t\tFecha invalida. Formato correcto: DD/MM/AAAA" << endl;
                return false;
            }
            int dia = stoi(fecha.substr(0, 2));
            int mes = stoi(fecha.substr(3, 2));
            int anio = stoi(fecha.substr(6, 4));

            if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || anio < 2023 || anio > 2026) {
                cout << "\t\t\t\tFecha invalida. " << endl;
                return false;
            }
            if (anio < anioActual ||
                (anio == anioActual && mes < mesActual) ||
                (anio == anioActual && mes == mesActual && dia <= diaActual)) {
                cout << "\t\t\t\tLa fecha debe ser posterior a la fecha actual." << endl;
                return false;
            }
            return true;
            };
        while (true) {
            cout << "\t\t\t   Ingrese fecha del vuelo (DD/MM/AAAA): ";
            cin >> fecha;
            if (validar(fecha)) {
                return true;
            }
        }
    }


    int seleccionarOrigen() {
        cout << "\t\t\t   Seleccione ciudad de origen: " << endl;
        int origen = vuelos.seleccionarOrigen();
        if (origen == -1) {
            cout << "\t\t\t   Reserva cancelada. " << endl;
        }
        return origen;
    }

    int seleccionarDestino(int origen) {
        cout << "\t\t\t   Seleccione ciudad de destino:    " << endl;
        int destino = vuelos.seleccionarDestino(origen);
        if (destino == -1) {
            cout << "\t\t\t| Reserva cancelada." << endl;
        }
        return destino;
    }

    int ingresarCantidadPasajeros() {
        int cantPasajeros;
        cout << " " << endl;
        cout << "\t\t\t           Ingrese la cantidad de pasajeros: ";
        cin >> cantPasajeros;
        return cantPasajeros;
    }


    Vuelo seleccionarVuelo(int origen, int destino) {
        string claveVuelo = vuelos.getSiglasLugar(origen) + "-" + vuelos.getSiglasLugar(destino);
        gestorVuelos.cargarDesdeArchivo("Archivos//vuelos.txt", claveVuelo);

        // Mostrar opciones de ordenamiento
        cout << "" << endl;
        cout << "\n\t\t\t------------------------- OPCIONES DE ORDENAMIENTO ------------------------- " << endl;
        cout << "" << endl;
        cout << "\t\t\t                      [1] Ordenar por precio (ascendente)" << endl;
        cout << "\t\t\t                      [2] Ordenar por precio (descendente)" << endl;
        cout << "\t\t\t                      [3] Ordenar por duracion (ascendente)" << endl;
        cout << "\t\t\t                      [4] Ordenar por duracion (descendente)" << endl;
        cout << "\t\t\t                      [5] No ordenar (mostrar como están)" << endl;
        cout << "" << endl;
        cout << "\t\t\t------------------------------------------------------------------------------" << endl;
        cout << "\t\t\t                    Selecciona una opción de ordenamiento: ";

        int opcionOrdenamiento;
        cin >> opcionOrdenamiento;


        switch (opcionOrdenamiento) {
        case 1:
            gestorVuelos.selectionSegunCriterio([](Vuelo& a, Vuelo& b) {
                return a.getPrecio() < b.getPrecio();
                });
            system("cls");
            cout << "\t\t\t Vuelos ordenados por precio (menor a mayor)" << endl;
            break;

        case 2:
            gestorVuelos.selectionSegunCriterio([](Vuelo& a, Vuelo& b) {
                return a.getPrecio() > b.getPrecio();
                });
            system("cls");
            cout << "\t\t\t Vuelos ordenados por precio (mayor a menor)" << endl;
            break;

        case 3:
            gestorVuelos.bubbleSegunCriterio([](Vuelo& a, Vuelo& b) {
                return a.getDuracionEnMinutos() < b.getDuracionEnMinutos();
                });
            system("cls");
            cout << "\t\t\t Vuelos ordenados por duración (menor a mayor)" << endl;
            break;

        case 4:
            gestorVuelos.bubbleSegunCriterio([](Vuelo& a, Vuelo& b) {
                return a.getDuracionEnMinutos() > b.getDuracionEnMinutos();
                });
            system("cls");
            cout << "\t\t\t Vuelos ordenados por duracion (mayor a menor)" << endl;
            break;

        case 5:
            system("cls");
            cout << "\t\t\t Mostrando vuelos en orden original" << endl;
            break;

        default:
            cout << "\t\t\t Opción no valida. Mostrando vuelos sin ordenar." << endl;
            break;
        }

        gestorVuelos.imprimirVuelos();


        int opcionVuelo;
        cin >> opcionVuelo;

        return gestorVuelos.getVuelo(opcionVuelo - 1);
    }
    vector<Pasajero> manejarEquipajeYAsientos(int cantPasajeros, Vuelo& vueloSeleccionado) {
        vector<Pasajero> pasajeros;
        string opcionAdicionales;

        cout << "" << endl;

        cout << "\t\t\t------------------------- Equipajes y Asientos -------------------------";
        cout << "" << endl;
        cout << "\t\t\t    ----------------------------------------------------------------" << endl;
        cout << "\t\t\t   |      Deseas agregar equipaje personalizado por pasajero ?      |" << endl;
        cout << "\t\t\t    ----------------------------------------------------------------" << endl;
        cout << "\t\t\t\t\t\t     Si/No: ";
        cin.ignore();
        getline(cin, opcionAdicionales);

        for (char& c : opcionAdicionales) c = toupper(c);

        if (opcionAdicionales == "SI") {
            for (int i = 0; i < cantPasajeros; i++) {
                cout << "\n\t\t\tPasajero " << i + 1 << ": " << endl;
                Pasajero pasajero(vueloSeleccionado.getPrecio(), 0, vueloSeleccionado.getID());
                pasajero.seleccionarEquipaje();

                pasajero.seleccionarAsiento(cantPasajeros);
                pasajeros.push_back(pasajero);
            }
        }
        else if (opcionAdicionales == "NO") {
            Pasajero pasajero(vueloSeleccionado.getPrecio(), 0, vueloSeleccionado.getID());
            pasajero.seleccionarEquipaje();
            pasajero.seleccionarAsiento(cantPasajeros);
            pasajeros.push_back(pasajero);

            for (int i = 0; i < cantPasajeros - 1; i++) {
                Pasajero clon(vueloSeleccionado.getPrecio(), pasajero.getIdReserva(), vueloSeleccionado.getID());
                pasajeros.push_back(clon);
            }
        }
        return pasajeros;
    }

    void registrarPasajeros(vector<Pasajero>& pasajeros) {
        cout << "" << endl;
        cout << "\t\t\t----------------------------- Registro Pasajeros -------------------------------" << endl;
        for (int i = 0; i < pasajeros.size(); i++) {
            cout << "\n\t\t\tPasajero " << i + 1 << ": " << endl;
            pasajeros[i].pedirDatosPersonales();
        }
    }
    float calcularPrecioTotal(vector<Pasajero>& pasajeros) {
        float precioTotal = 0.0f;
        for (auto& pasajero : pasajeros) {
            precioTotal += pasajero.calcularPrecioPasajero();
        }
        return precioTotal;
    }

    void procesarPago(float precioTotal, vector<Pasajero>& pasajeros, Vuelo& vueloSeleccionado) {
        Reserva nuevaReserva(fecha, vueloSeleccionado.getCiudadDestino(), vueloSeleccionado.getCiudadOrigen(), pasajeros.size());
        listaReservas.agregarReserva(nuevaReserva);

        Pago pago(precioTotal, pasajeros, nuevaReserva, vueloSeleccionado, nuevaReserva.getIdReserva());
        cout << "" << endl;

        cout << "\t\t\t--------------------------------- PROMOCIONES -----------------------------------" << endl;
        cout << "" << endl;


        cout << "\t\t\tTiene un codigo de descuento (si/no): ";
        string respuesta;
        cin.ignore();
        getline(cin, respuesta);

        if (respuesta == "si") {
            cout << "\t\t\tIngrese su codigo de descuento: ";
            string codigoDescuento;
            getline(cin, codigoDescuento);

            pago.aplicarDescuento(codigoDescuento);
        }
        system("cls");
        cout << "" << endl;
        cout << "\t\t\t--------------------------------- PAGO -----------------------------------" << endl;
        cout << "" << endl;
        pago.ingresarDatosPagador();

        colaPagos.enqueue(pago);

        cout << "\t\t\tPago agregado a la cola." << endl;

        colaPagos.procesarPagos();

        listaReservas.guardarListaEnArchivo(archivoReservas);
        gestorVuelos.guardarVueloEnArchivo(vueloSeleccionado);
        GestorPasajero gestorPasajero(pasajeros);
        gestorPasajero.guardarPasajerosEnArchivo(archivoPasajeros);
    }

    void menuPricipal() {
        system("cls");
        string titulo = R"(                     _         _     ___   __  __     _     ___   _____ 
                  _ | |  ___  | |_  / __| |  \/  |   /_\   | _ \ |_   _|
                 | || | / -_) |  _| \__ \ | |\/| |  / _ \  |   /   | |  
                  \__/  \___|  \__| |___/ |_|  |_| /_/ \_\ |_|_\   |_|
 )";

        cout << "\t\t\t" << endl;
        cout << titulo << endl;
        cout << "\t\t\t  ------------------------------------ " << endl;
        cout << "\t\t\t |                                    |" << endl;
        cout << "\t\t\t |   [1] Iniciar sesion               |" << endl;
        cout << "\t\t\t |   [2] Iniciar Reserva sin cuenta   |" << endl;
        cout << "\t\t\t |   [3] Salir                        |" << endl;
        cout << "\t\t\t |                                    |" << endl;
        cout << "\t\t\t  ------------------------------------ " << endl;
        cout << " " << endl;
        cout << "\t\t\t      Seleccione una opcion: ";
    }

    void deseasRegistrarte() {
        system("cls");
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;
        cout << "\t\t\t" << endl;

        cout << "\t\t\t  ------------------------------------------------------------ " << endl;
        cout << "\t\t\t |  Deseas registrarte en JetSMART?                          |" << endl;
        cout << "\t\t\t |  Al registrarte, podras acceder a ofertas exclusivas,     |" << endl;
        cout << "\t\t\t |  recibir novedades sobre vuelos, promociones especiales   |" << endl;
        cout << "\t\t\t |  y disfrutar de una experiencia personalizada.            |" << endl;
        cout << "\t\t\t |  Si prefieres, tambien puedes continuar sin registrarse   |" << endl;
        cout << "\t\t\t  ----------------------------------------------------------- " << endl;

        cout << "\t\t\t    SI/NO: ";
    }
};



