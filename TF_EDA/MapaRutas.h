#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

using namespace std;

class MapaRutas {
private:
    struct Ciudad {
        string nombre;
        string codigo;
        int indice;
        int paisIndice;
    };
    
    struct Pais {
        string nombre;
        vector<Ciudad> ciudades;
    };
    
    vector<Pais> paises;
    vector<vector<int>> matrizConexiones;
    int totalCiudades;
    unordered_map<string, int> codigoACiudadIndice;
    unordered_map<string, int> nombreACiudadIndice;
    
    void agregarCiudad(Pais& pais, const string& nombre, const string& codigo) {
        Ciudad ciudad;
        ciudad.nombre = nombre;
        ciudad.codigo = codigo;
        ciudad.indice = totalCiudades++;
        ciudad.paisIndice = paises.size();
        
        pais.ciudades.push_back(ciudad);
        codigoACiudadIndice[codigo] = ciudad.indice;
        nombreACiudadIndice[nombre] = ciudad.indice;
    }
    
    void conectar(const string& codigo1, const string& codigo2) {
        auto it1 = codigoACiudadIndice.find(codigo1);
        auto it2 = codigoACiudadIndice.find(codigo2);
        
        if (it1 != codigoACiudadIndice.end() && it2 != codigoACiudadIndice.end()) {
            int idx1 = it1->second;
            int idx2 = it2->second;
            
            matrizConexiones[idx1][idx2] = 1;
            matrizConexiones[idx2][idx1] = 1;
        }
    }
    
    void inicializarPaisesYCiudades() {
        totalCiudades = 0;
        
        Pais peru;
        peru.nombre = "Peru";
        agregarCiudad(peru, "Tarapoto", "TPP");
        agregarCiudad(peru, "Piura", "PIU");
        agregarCiudad(peru, "Chiclayo", "CIX");
        agregarCiudad(peru, "Cajamarca", "CJA");
        agregarCiudad(peru, "Trujillo", "TRU");
        agregarCiudad(peru, "Lima", "LIM");
        agregarCiudad(peru, "Cusco", "CUZ");
        agregarCiudad(peru, "Arequipa", "AQP");
        paises.push_back(peru);
        
        Pais chile;
        chile.nombre = "Chile";
        agregarCiudad(chile, "Santiago", "SCL");
        agregarCiudad(chile, "Arica", "ARI");
        agregarCiudad(chile, "Iquique", "IQQ");
        agregarCiudad(chile, "Calama", "CJC");
        agregarCiudad(chile, "Antofagasta", "ANF");
        agregarCiudad(chile, "La Serena", "LSC");
        agregarCiudad(chile, "Concepcion", "CCP");
        agregarCiudad(chile, "Temuco", "ZCO");
        agregarCiudad(chile, "Puerto Montt", "PMC");
        agregarCiudad(chile, "Balmaceda", "BBA");
        paises.push_back(chile);
        
        Pais ecuador;
        ecuador.nombre = "Ecuador";
        agregarCiudad(ecuador, "Quito", "UIO");
        paises.push_back(ecuador);
        
        Pais argentina;
        argentina.nombre = "Argentina";
        agregarCiudad(argentina, "Buenos Aires", "BUE");
        agregarCiudad(argentina, "San Martin de los Andes", "CPC");
        agregarCiudad(argentina, "Salta", "SLA");
        agregarCiudad(argentina, "Iguazu", "IGR");
        agregarCiudad(argentina, "Tucuman", "TUC");
        agregarCiudad(argentina, "Cordoba", "COR");
        agregarCiudad(argentina, "Mendoza", "MDZ");
        agregarCiudad(argentina, "Neuquen", "NQN");
        agregarCiudad(argentina, "Bariloche", "BRC");
        agregarCiudad(argentina, "Comodoro Rivadavia", "CRD");
        agregarCiudad(argentina, "El Calafate", "FTE");
        agregarCiudad(argentina, "Ushuaia", "USH");
        agregarCiudad(argentina, "Trelew", "REL");
        agregarCiudad(argentina, "Resistencia", "RES");
        paises.push_back(argentina);
        
        Pais brasil;
        brasil.nombre = "Brasil";
        agregarCiudad(brasil, "Rio de Janeiro", "RIO");
        agregarCiudad(brasil, "Foz de Iguazu", "IGU");
        agregarCiudad(brasil, "Florianopolis", "FLN");
        agregarCiudad(brasil, "Sao Paulo", "GRU");
        agregarCiudad(brasil, "Recife", "REC");
        paises.push_back(brasil);
        
        Pais colombia;
        colombia.nombre = "Colombia";
        agregarCiudad(colombia, "San Andres", "ADZ");
        agregarCiudad(colombia, "Bogota", "BOG");
        agregarCiudad(colombia, "Cali", "CLO");
        agregarCiudad(colombia, "Cartagena de Indias", "CTG");
        agregarCiudad(colombia, "Cucuta", "CUC");
        agregarCiudad(colombia, "Medellin", "MDE");
        agregarCiudad(colombia, "Pereira", "PEI");
        agregarCiudad(colombia, "Santa Marta", "SMR");
        agregarCiudad(colombia, "Monteria", "MTR");
        agregarCiudad(colombia, "Barranquilla", "BAQ");
        paises.push_back(colombia);
        
        Pais uruguay;
        uruguay.nombre = "Uruguay";
        agregarCiudad(uruguay, "Montevideo", "MVD");
        paises.push_back(uruguay);
        
        Pais paraguay;
        paraguay.nombre = "Paraguay";
        agregarCiudad(paraguay, "Asuncion", "ASU");
        paises.push_back(paraguay);
        
        matrizConexiones.resize(totalCiudades, vector<int>(totalCiudades, 0));
    }
    
    void inicializarConexionesNacionalesPeru() {
        string codigosPeru[] = {"TPP", "PIU", "CIX", "CJA", "TRU", "LIM", "CUZ", "AQP"};
        
        int matrizOriginal[8][8] = {
            {0, 0, 1, 0, 0, 1, 1, 1},
            {0, 0, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 1, 1, 1, 1, 0},
            {1, 1, 1, 0, 1, 1, 1, 1},
            {0, 1, 1, 1, 0, 0, 1, 1},
            {1, 1, 1, 1, 0, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 0}
        };
        
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (matrizOriginal[i][j] == 1) {
                    conectar(codigosPeru[i], codigosPeru[j]);
                }
            }
        }
    }
    
    void inicializarConexiones() {
        inicializarConexionesNacionalesPeru();
        
        string ciudadesPeru[] = {"TPP", "PIU", "CIX", "CJA", "TRU", "LIM", "CUZ", "AQP"};
        string destinosComunes[] = {"SCL", "UIO", "BUE", "MDE"};
        
        for (const auto& origen : ciudadesPeru) {
            for (const auto& destino : destinosComunes) {
                conectar(origen, destino);
            }
        }
        
        conectar("TPP", "PEI");
        conectar("PIU", "PEI");
        conectar("CIX", "CTG");
        conectar("TRU", "BOG");
        
        string limaDestinos[] = {"IQQ", "ANF", "COR", "MDZ", "RIO", "GRU", "BOG", 
                               "CLO", "CTG", "BAQ", "MVD", "ASU"};
        for (const auto& destino : limaDestinos) {
            conectar("LIM", destino);
        }
        
        conectar("CUZ", "BOG");
        conectar("AQP", "GRU");
        
        conectar("SCL", "BUE"); conectar("SCL", "MDZ"); conectar("SCL", "COR");
        conectar("IQQ", "SLA");
        
        conectar("SCL", "GRU"); conectar("SCL", "RIO");
        
        conectar("BUE", "GRU"); conectar("BUE", "RIO");
        conectar("IGR", "IGU");
        
        conectar("BOG", "GRU"); conectar("BOG", "UIO"); conectar("BOG", "RIO"); 
        conectar("BOG", "MVD"); conectar("MDE", "UIO"); conectar("MVD", "BUE"); 
        conectar("MVD", "COR"); conectar("MVD", "GRU"); conectar("MVD", "LIM");
        conectar("ASU", "BUE"); conectar("ASU", "SCL"); conectar("ASU", "GRU"); 
        conectar("ASU", "LIM"); conectar("ASU", "RES");
        
        conectar("ARI", "LIM"); conectar("ARI", "BUE"); conectar("ARI", "SLA");
        conectar("IQQ", "CUZ");
        conectar("CJC", "LIM"); conectar("CJC", "SLA");
        conectar("ANF", "BUE");
        conectar("LSC", "BUE"); conectar("LSC", "LIM");
        conectar("CCP", "MVD"); conectar("CCP", "BUE");
        conectar("ZCO", "BUE"); conectar("ZCO", "ASU");
        conectar("PMC", "BUE"); conectar("PMC", "BRC");
        conectar("BBA", "BRC"); conectar("BBA", "FTE");
        
        conectar("CPC", "SCL"); conectar("CPC", "PMC");
        conectar("SLA", "SCL");
        conectar("IGR", "RIO"); conectar("IGR", "GRU");
        conectar("TUC", "SCL"); conectar("TUC", "LIM");
        conectar("COR", "GRU");
        conectar("NQN", "SCL"); conectar("NQN", "BBA");
        conectar("CRD", "SCL"); conectar("CRD", "BBA");
        conectar("FTE", "SCL");
        conectar("USH", "SCL"); conectar("USH", "FTE");
        conectar("REL", "SCL"); conectar("REL", "CRD");
        conectar("RES", "BUE");
        
        conectar("FLN", "BUE"); conectar("FLN", "GRU");
        conectar("REC", "GRU"); conectar("REC", "BOG");
        
        conectar("ADZ", "BOG"); conectar("ADZ", "CTG");
        conectar("CLO", "LIM"); conectar("CLO", "UIO");
        conectar("CTG", "UIO");
        conectar("CUC", "BOG"); conectar("CUC", "UIO");
        conectar("MDE", "BUE");
        conectar("PEI", "BOG"); conectar("PEI", "UIO");
        conectar("SMR", "BOG"); conectar("SMR", "UIO");
        conectar("MTR", "BOG"); conectar("MTR", "BAQ");
        conectar("BAQ", "UIO");
        
        string capitales[] = {"SCL", "UIO", "BUE", "GRU", "BOG", "MVD", "ASU"};
        for (size_t i = 0; i < sizeof(capitales)/sizeof(capitales[0]); ++i) {
            for (size_t j = i + 1; j < sizeof(capitales)/sizeof(capitales[0]); ++j) {
                conectar(capitales[i], capitales[j]);
            }
        }
        
        conectar("LIM", "SCL"); conectar("LIM", "ARI"); conectar("LIM", "IQQ");
        conectar("LIM", "BUE"); conectar("LIM", "COR"); conectar("LIM", "MDZ");
        conectar("LIM", "GRU"); conectar("LIM", "RIO"); conectar("LIM", "FLN");
        conectar("LIM", "BOG"); conectar("LIM", "MDE"); conectar("LIM", "CTG");
        conectar("LIM", "UIO");
        
        conectar("SCL", "LIM"); conectar("SCL", "CUZ"); conectar("SCL", "AQP");
        conectar("SCL", "BUE"); conectar("SCL", "COR"); conectar("SCL", "MDZ");
        conectar("SCL", "GRU"); conectar("SCL", "RIO"); conectar("SCL", "FLN");
        conectar("SCL", "BOG"); conectar("SCL", "MDE"); conectar("SCL", "CTG");
        conectar("SCL", "MVD");
        
        conectar("BUE", "LIM"); conectar("BUE", "CUZ"); conectar("BUE", "AQP");
        conectar("BUE", "SCL"); conectar("BUE", "ARI"); conectar("BUE", "IQQ");
        conectar("BUE", "GRU"); conectar("BUE", "RIO"); conectar("BUE", "FLN");
        conectar("BUE", "BOG"); conectar("BUE", "MDE"); conectar("BUE", "CTG");
        conectar("BUE", "MVD");
        
        conectar("GRU", "LIM"); conectar("GRU", "CUZ"); conectar("GRU", "AQP");
        conectar("GRU", "SCL"); conectar("GRU", "ARI"); conectar("GRU", "IQQ");
        conectar("GRU", "BUE"); conectar("GRU", "COR"); conectar("GRU", "MDZ");
        conectar("GRU", "BOG"); conectar("GRU", "MDE"); conectar("GRU", "CTG");
        conectar("GRU", "MVD");
        
        conectar("BOG", "LIM"); conectar("BOG", "CUZ"); conectar("BOG", "AQP");
        conectar("BOG", "SCL"); conectar("BOG", "ARI"); conectar("BOG", "IQQ");
        conectar("BOG", "BUE"); conectar("BOG", "COR"); conectar("BOG", "MDZ");
        conectar("BOG", "GRU"); conectar("BOG", "RIO"); conectar("BOG", "FLN");
        conectar("BOG", "UIO");
        
        conectar("UIO", "LIM"); conectar("UIO", "CUZ"); conectar("UIO", "AQP");
        conectar("UIO", "BOG"); conectar("UIO", "MDE"); conectar("UIO", "CTG");
        conectar("UIO", "BUE"); conectar("UIO", "COR"); conectar("UIO", "MDZ");
        conectar("UIO", "GRU"); conectar("UIO", "RIO"); conectar("UIO", "FLN");
        conectar("UIO", "SCL"); conectar("UIO", "ARI"); conectar("UIO", "IQQ");
        
        conectar("MVD", "LIM"); conectar("MVD", "CUZ"); conectar("MVD", "AQP");
        conectar("MVD", "SCL"); conectar("MVD", "ARI"); conectar("MVD", "IQQ");
        conectar("MVD", "BUE"); conectar("MVD", "COR"); conectar("MVD", "MDZ");
        conectar("MVD", "GRU"); conectar("MVD", "RIO"); conectar("MVD", "FLN");
        conectar("MVD", "BOG"); conectar("MVD", "MDE"); conectar("MVD", "CTG");
        
        conectar("ASU", "LIM"); conectar("ASU", "CUZ"); conectar("ASU", "AQP");
        conectar("ASU", "SCL"); conectar("ASU", "ARI"); conectar("ASU", "IQQ");
        conectar("ASU", "BUE"); conectar("ASU", "COR"); conectar("ASU", "MDZ");
        conectar("ASU", "GRU"); conectar("ASU", "RIO"); conectar("ASU", "FLN");
        conectar("ASU", "BOG"); conectar("ASU", "MDE"); conectar("ASU", "CTG");
        
        conectar("TPP", "SCL"); conectar("TPP", "BUE"); conectar("TPP", "GRU"); conectar("TPP", "BOG");
        conectar("PIU", "SCL"); conectar("PIU", "BUE"); conectar("PIU", "GRU"); conectar("PIU", "BOG");
        conectar("CIX", "SCL"); conectar("CIX", "BUE"); conectar("CIX", "GRU"); conectar("CIX", "BOG");
        conectar("CJA", "SCL"); conectar("CJA", "BUE"); conectar("CJA", "GRU"); conectar("CJA", "BOG");
        conectar("TRU", "SCL"); conectar("TRU", "BUE"); conectar("TRU", "GRU"); conectar("TRU", "BOG");
        conectar("CUZ", "SCL"); conectar("CUZ", "BUE"); conectar("CUZ", "GRU"); conectar("CUZ", "BOG");
        conectar("AQP", "SCL"); conectar("AQP", "BUE"); conectar("AQP", "GRU"); conectar("AQP", "BOG");
        
        conectar("ARI", "LIM"); conectar("ARI", "BUE"); conectar("ARI", "GRU"); conectar("ARI", "BOG");
        conectar("IQQ", "LIM"); conectar("IQQ", "BUE"); conectar("IQQ", "GRU"); conectar("IQQ", "BOG");
        conectar("CJC", "LIM"); conectar("CJC", "BUE"); conectar("CJC", "GRU"); conectar("CJC", "BOG");
        conectar("ANF", "LIM"); conectar("ANF", "BUE"); conectar("ANF", "GRU"); conectar("ANF", "BOG");
        conectar("LSC", "LIM"); conectar("LSC", "BUE"); conectar("LSC", "GRU"); conectar("LSC", "BOG");
        conectar("CCP", "LIM"); conectar("CCP", "BUE"); conectar("CCP", "GRU"); conectar("CCP", "BOG");
        conectar("ZCO", "LIM"); conectar("ZCO", "BUE"); conectar("ZCO", "GRU"); conectar("ZCO", "BOG");
        conectar("PMC", "LIM"); conectar("PMC", "BUE"); conectar("PMC", "GRU"); conectar("PMC", "BOG");
        conectar("BBA", "LIM"); conectar("BBA", "BUE"); conectar("BBA", "GRU"); conectar("BBA", "BOG");
        
        conectar("CPC", "LIM"); conectar("CPC", "SCL"); conectar("CPC", "GRU"); conectar("CPC", "BOG");
        conectar("SLA", "LIM"); conectar("SLA", "SCL"); conectar("SLA", "GRU"); conectar("SLA", "BOG");
        conectar("IGR", "LIM"); conectar("IGR", "SCL"); conectar("IGR", "GRU"); conectar("IGR", "BOG");
        conectar("TUC", "LIM"); conectar("TUC", "SCL"); conectar("TUC", "GRU"); conectar("TUC", "BOG");
        conectar("COR", "LIM"); conectar("COR", "SCL"); conectar("COR", "GRU"); conectar("COR", "BOG");
        conectar("MDZ", "LIM"); conectar("MDZ", "SCL"); conectar("MDZ", "GRU"); conectar("MDZ", "BOG");
        conectar("NQN", "LIM"); conectar("NQN", "SCL"); conectar("NQN", "GRU"); conectar("NQN", "BOG");
        conectar("BRC", "LIM"); conectar("BRC", "SCL"); conectar("BRC", "GRU"); conectar("BRC", "BOG");
        conectar("CRD", "LIM"); conectar("CRD", "SCL"); conectar("CRD", "GRU"); conectar("CRD", "BOG");
        conectar("FTE", "LIM"); conectar("FTE", "SCL"); conectar("FTE", "GRU"); conectar("FTE", "BOG");
        conectar("USH", "LIM"); conectar("USH", "SCL"); conectar("USH", "GRU"); conectar("USH", "BOG");
        conectar("REL", "LIM"); conectar("REL", "SCL"); conectar("REL", "GRU"); conectar("REL", "BOG");
        conectar("RES", "LIM"); conectar("RES", "SCL"); conectar("RES", "GRU"); conectar("RES", "BOG");
        
        conectar("RIO", "LIM"); conectar("RIO", "SCL"); conectar("RIO", "BUE"); conectar("RIO", "BOG");
        conectar("IGU", "LIM"); conectar("IGU", "SCL"); conectar("IGU", "BUE"); conectar("IGU", "BOG");
        conectar("FLN", "LIM"); conectar("FLN", "SCL"); conectar("FLN", "BUE"); conectar("FLN", "BOG");
        conectar("REC", "LIM"); conectar("REC", "SCL"); conectar("REC", "BUE"); conectar("REC", "BOG");
        
        conectar("ADZ", "LIM"); conectar("ADZ", "SCL"); conectar("ADZ", "BUE"); conectar("ADZ", "GRU");
        conectar("CLO", "LIM"); conectar("CLO", "SCL"); conectar("CLO", "BUE"); conectar("CLO", "GRU");
        conectar("CTG", "LIM"); conectar("CTG", "SCL"); conectar("CTG", "BUE"); conectar("CTG", "GRU");
        conectar("CUC", "LIM"); conectar("CUC", "SCL"); conectar("CUC", "BUE"); conectar("CUC", "GRU");
        conectar("MDE", "LIM"); conectar("MDE", "SCL"); conectar("MDE", "BUE"); conectar("MDE", "GRU");
        conectar("PEI", "LIM"); conectar("PEI", "SCL"); conectar("PEI", "BUE"); conectar("PEI", "GRU");
        conectar("SMR", "LIM"); conectar("SMR", "SCL"); conectar("SMR", "BUE"); conectar("SMR", "GRU");
        conectar("MTR", "LIM"); conectar("MTR", "SCL"); conectar("MTR", "BUE"); conectar("MTR", "GRU");
        conectar("BAQ", "LIM"); conectar("BAQ", "SCL"); conectar("BAQ", "BUE"); conectar("BAQ", "GRU");
        
        conectar("TPP", "PIU"); conectar("TPP", "CUZ");
        conectar("PIU", "CIX"); conectar("PIU", "TRU");
        conectar("CIX", "CJA"); conectar("CIX", "TRU");
        conectar("CJA", "TRU"); conectar("CJA", "CUZ");
        conectar("TRU", "CUZ"); conectar("TRU", "AQP");
        conectar("CUZ", "AQP"); conectar("CUZ", "LIM");
        conectar("AQP", "LIM"); conectar("AQP", "CUZ");
        
        conectar("ARI", "IQQ"); conectar("ARI", "SCL");
        conectar("IQQ", "CJC"); conectar("IQQ", "SCL");
        conectar("CJC", "ANF"); conectar("CJC", "SCL");
        conectar("ANF", "LSC"); conectar("ANF", "SCL");
        conectar("LSC", "CCP"); conectar("LSC", "SCL");
        conectar("CCP", "ZCO"); conectar("CCP", "SCL");
        conectar("ZCO", "PMC"); conectar("ZCO", "SCL");
        conectar("PMC", "BBA"); conectar("PMC", "SCL");
        conectar("BBA", "SCL"); conectar("BBA", "PMC");
        
        conectar("CPC", "BUE"); conectar("CPC", "SLA");
        conectar("SLA", "BUE"); conectar("SLA", "TUC");
        conectar("IGR", "BUE"); conectar("IGR", "COR");
        conectar("TUC", "BUE"); conectar("TUC", "COR");
        conectar("COR", "BUE"); conectar("COR", "MDZ");
        conectar("MDZ", "BUE"); conectar("MDZ", "NQN");
        conectar("NQN", "BUE"); conectar("NQN", "BRC");
        conectar("BRC", "BUE"); conectar("BRC", "CRD");
        conectar("CRD", "BUE"); conectar("CRD", "FTE");
        conectar("FTE", "BUE"); conectar("FTE", "USH");
        conectar("USH", "BUE"); conectar("USH", "REL");
        conectar("REL", "BUE"); conectar("REL", "RES");
        conectar("RES", "BUE"); conectar("RES", "COR");
        
        conectar("RIO", "GRU"); conectar("RIO", "FLN");
        conectar("IGU", "GRU"); conectar("IGU", "RIO");
        conectar("FLN", "GRU"); conectar("FLN", "RIO");
        conectar("REC", "GRU"); conectar("REC", "RIO");
        
        conectar("ADZ", "BOG"); conectar("ADZ", "CTG");
        conectar("CLO", "BOG"); conectar("CLO", "MDE");
        conectar("CTG", "BOG"); conectar("CTG", "MDE");
        conectar("CUC", "BOG"); conectar("CUC", "MDE");
        conectar("MDE", "BOG"); conectar("MDE", "PEI");
        conectar("PEI", "BOG"); conectar("PEI", "SMR");
        conectar("SMR", "BOG"); conectar("SMR", "MTR");
        conectar("MTR", "BOG"); conectar("MTR", "BAQ");
        conectar("BAQ", "BOG"); conectar("BAQ", "ADZ");
        
        conectar("LIM", "TPP"); conectar("LIM", "PIU"); conectar("LIM", "CIX");
        conectar("LIM", "CJA"); conectar("LIM", "TRU"); conectar("LIM", "CUZ");
        conectar("LIM", "AQP");
        
        conectar("SCL", "ARI"); conectar("SCL", "IQQ"); conectar("SCL", "CJC");
        conectar("SCL", "ANF"); conectar("SCL", "LSC"); conectar("SCL", "CCP");
        conectar("SCL", "ZCO"); conectar("SCL", "PMC"); conectar("SCL", "BBA");
        
        conectar("BUE", "CPC"); conectar("BUE", "SLA"); conectar("BUE", "IGR");
        conectar("BUE", "TUC"); conectar("BUE", "COR"); conectar("BUE", "MDZ");
        conectar("BUE", "NQN"); conectar("BUE", "BRC"); conectar("BUE", "CRD");
        conectar("BUE", "FTE"); conectar("BUE", "USH"); conectar("BUE", "REL");
        conectar("BUE", "RES");
        
        conectar("GRU", "RIO"); conectar("GRU", "IGU"); conectar("GRU", "FLN");
        conectar("GRU", "REC");
        
        conectar("BOG", "ADZ"); conectar("BOG", "CLO"); conectar("BOG", "CTG");
        conectar("BOG", "CUC"); conectar("BOG", "MDE"); conectar("BOG", "PEI");
        conectar("BOG", "SMR"); conectar("BOG", "MTR"); conectar("BOG", "BAQ");
    }
    
    vector<int> obtenerPaisesConDestinosDisponibles(int origen) {
        vector<int> paisesConDestinos;
        set<int> paisesIncluidos;
        
        for (int i = 0; i < totalCiudades; ++i) {
            if (matrizConexiones[origen][i] == 1 && i != origen) {
                for (size_t j = 0; j < paises.size(); ++j) {
                    for (const auto& ciudad : paises[j].ciudades) {
                        if (ciudad.indice == i && paisesIncluidos.find(j) == paisesIncluidos.end()) {
                            paisesConDestinos.push_back(j);
                            paisesIncluidos.insert(j);
                            break;
                        }
                    }
                }
            }
        }
        
        return paisesConDestinos;
    }
    
public:
    MapaRutas() {
        inicializarPaisesYCiudades();
        inicializarConexiones();
    }
    
    ~MapaRutas() {}
    
    int seleccionarPais(const vector<int>& paisesDisponibles = {}) {
        int opcion;
        
        while (true) {
            cout << "\t\t\t\t\t";
            
            // Mostrar opciones
            if (!paisesDisponibles.empty()) {
                for (size_t i = 0; i < paisesDisponibles.size(); ++i) {
                    int indicePais = paisesDisponibles[i];
                    cout << (i + 1) << ". " << paises[indicePais].nombre << endl << "\t\t\t\t\t";
                }
            }
            else {
                for (size_t i = 0; i < paises.size(); ++i) {
                    cout << (i + 1) << ". " << paises[i].nombre << endl << "\t\t\t\t\t";
                }
            }
            
            cout << "0. Volver" << endl << endl;
            cout << "\t\t\t\t\tSeleccione una opcion: ";
            cin >> opcion;
            
            if (opcion == 0) return -1;
            
            // Validar opción
            if (!paisesDisponibles.empty()) {
                if (opcion < 1 || opcion > (int)paisesDisponibles.size()) {
                    cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
                    continue; // Volver al inicio del bucle
                }
                return paisesDisponibles[opcion - 1];
            }
            else {
                if (opcion < 1 || opcion > (int)paises.size()) {
                    cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
                    continue; // Volver al inicio del bucle
                }
                return opcion - 1;
            }
        }
    }
    
    int seleccionarCiudadDePais(int indicePais) {
        if (indicePais < 0 || indicePais >= (int)paises.size()) {
            return -1;
        }
        
        for (size_t i = 0; i < paises[indicePais].ciudades.size(); ++i) {
            cout << "\t\t\t\t\t" << (i + 1) << ". " 
                << paises[indicePais].ciudades[i].nombre << endl;
        }
        cout << "\t\t\t\t\t0. Volver" << endl << endl;
        
        int opcion;
        cout << "\t\t\t\t\tSeleccione una opcion: ";
        cin >> opcion;
        
        if (opcion == 0) return -2;
        if (opcion < 1 || opcion > (int)paises[indicePais].ciudades.size()) {
            cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
            return seleccionarCiudadDePais(indicePais);
        }
        
        return paises[indicePais].ciudades[opcion - 1].indice;
    }
    
    int seleccionarOrigen() {
        cout << "\t\t\t\t\t--- SELECCIONE ORIGEN ---" << endl;
        int paisOrigen = seleccionarPais();
        if (paisOrigen == -1) return -1;
        
        int ciudadOrigen = seleccionarCiudadDePais(paisOrigen);
        while (ciudadOrigen == -2) {
            paisOrigen = seleccionarPais();
            if (paisOrigen == -1) return -1;
            ciudadOrigen = seleccionarCiudadDePais(paisOrigen);
        }
        
        return ciudadOrigen;
    }
    
    int seleccionarDestino(int origen) {
        if (origen < 0 || origen >= totalCiudades) {
            return -1;
        }
        
        string nombreOrigen = "Desconocido";
        string nombrePaisOrigen = "Desconocido";
        
        for (size_t i = 0; i < paises.size(); ++i) {
            for (const auto& ciudad : paises[i].ciudades) {
                if (ciudad.indice == origen) {
                    nombreOrigen = ciudad.nombre;
                    nombrePaisOrigen = paises[i].nombre;
                    break;
                }
            }
            if (nombreOrigen != "Desconocido") break;
        }
        
        cout << "\t\t\t\t\t--- SELECCIONE DESTINO ---" << endl;
        cout << "\t\t\t\t\tOrigen: " << nombreOrigen << ", " << nombrePaisOrigen << endl << endl;
        
        vector<int> paisesDisponibles = obtenerPaisesConDestinosDisponibles(origen);
        
        if (paisesDisponibles.empty()) {
            cout << "\t\t\t\t\tNo hay destinos disponibles para este origen." << endl;
            return -1;
        }
        
        int paisDestino = seleccionarPais(paisesDisponibles);
        if (paisDestino == -1) return -1;
        
        vector<int> ciudadesDisponibles;
        
        for (const auto& ciudad : paises[paisDestino].ciudades) {
            if (matrizConexiones[origen][ciudad.indice] == 1 && ciudad.indice != origen) {
                ciudadesDisponibles.push_back(ciudad.indice);
                cout << "\t\t\t\t\t" << ciudadesDisponibles.size() << ". " 
                    << ciudad.nombre << endl;
            }
        }
        
        cout << "\t\t\t\t\t0. Volver" << endl << endl;
        
        int opcion;
        cout << "\t\t\t\t\tSeleccione una opcion: ";
        cin >> opcion;
        
        if (opcion == 0) return seleccionarDestino(origen);
        if (opcion < 1 || opcion > (int)ciudadesDisponibles.size()) {
            cout << "\t\t\t\t\tOpcion invalida. Intente nuevamente." << endl;
            return seleccionarDestino(origen);
        }
        
        return ciudadesDisponibles[opcion - 1];
    }
    
    bool esVueloInternacional(int origen, int destino) {
        if (origen < 0 || origen >= totalCiudades || destino < 0 || destino >= totalCiudades) {
            return false;
        }
        
        int paisOrigen = -1;
        int paisDestino = -1;
        
        for (size_t i = 0; i < paises.size(); ++i) {
            for (const auto& ciudad : paises[i].ciudades) {
                if (ciudad.indice == origen) paisOrigen = i;
                if (ciudad.indice == destino) paisDestino = i;
            }
            if (paisOrigen != -1 && paisDestino != -1) break;
        }
        
        return paisOrigen != paisDestino && paisOrigen != -1 && paisDestino != -1;
    }
    
    string getNombreLugar(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.nombre + " (" + pais.nombre + ")";
                }
            }
        }
        return "No valido";
    }
    
    string getNombreCiudad(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.nombre;
                }
            }
        }
        return "Desconocida";
    }
    
    string getNombrePais(int indice) const {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return pais.nombre;
                }
            }
        }
        return "Desconocido";
    }
    
    string getSiglasLugar(int indice) {
        for (const auto& pais : paises) {
            for (const auto& ciudad : pais.ciudades) {
                if (ciudad.indice == indice) {
                    return ciudad.codigo;
                }
            }
        }
        return "???";
    }
    
    int getNumLugares() const {
        return totalCiudades;
    }
    
    string getIDRuta(int origen, int destino) {
        return getSiglasLugar(origen) + "-" + getSiglasLugar(destino);
    }
    
    bool existeRuta(int origen, int destino) {
        if (origen < 0 || origen >= totalCiudades || destino < 0 || destino >= totalCiudades) {
            return false;
        }
        return matrizConexiones[origen][destino] == 1;
    }
    
    int buscarIndicePorCodigo(const string& codigo) const {
        auto it = codigoACiudadIndice.find(codigo);
        if (it != codigoACiudadIndice.end()) {
            return it->second;
        }
        return -1;
    }
    
    int buscarIndicePorNombre(const string& nombre) const {
        auto it = nombreACiudadIndice.find(nombre);
        if (it != nombreACiudadIndice.end()) {
            return it->second;
        }
        return -1;
    }
};