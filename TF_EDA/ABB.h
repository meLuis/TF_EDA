#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

template <typename T>
class NodoABB {
public:
    T dato;
    string clave;
    NodoABB<T>* izquierda;
    NodoABB<T>* derecha;

    NodoABB(const T& _dato, const string& _clave) :
        dato(_dato), clave(_clave), izquierda(nullptr), derecha(nullptr) {
    }
};

template <typename T>
class ABB {
private:
    NodoABB<T>* raiz;

    // Método recursivo para insertar
    NodoABB<T>* insertarRecursivo(NodoABB<T>* nodo, const T& dato, const string& clave) {
        if (nodo == nullptr) {
            return new NodoABB<T>(dato, clave);
        }

        if (clave < nodo->clave) {
            nodo->izquierda = insertarRecursivo(nodo->izquierda, dato, clave);
        }
        else if (clave > nodo->clave) {
            nodo->derecha = insertarRecursivo(nodo->derecha, dato, clave);
        }

        // Si la clave ya existe, se puede actualizar el dato
        else {
            nodo->dato = dato;
        }

        return nodo;
    }

    // Método recursivo para buscar
    NodoABB<T>* buscarRecursivo(NodoABB<T>* nodo, const string& clave) const {
        if (nodo == nullptr || nodo->clave == clave) {
            return nodo;
        }

        if (clave < nodo->clave) {
            return buscarRecursivo(nodo->izquierda, clave);
        }
        else {
            return buscarRecursivo(nodo->derecha, clave);
        }
    }

    // Método recursivo para recorrer en orden
    void enOrdenRecursivo(NodoABB<T>* nodo, vector<T>& resultado) const {
        if (nodo != nullptr) {
            enOrdenRecursivo(nodo->izquierda, resultado);
            resultado.push_back(nodo->dato);
            enOrdenRecursivo(nodo->derecha, resultado);
        }
    }

    // Método recursivo para liberar memoria
    void liberarMemoria(NodoABB<T>* nodo) {
        if (nodo != nullptr) {
            liberarMemoria(nodo->izquierda);
            liberarMemoria(nodo->derecha);
            delete nodo;
        }
    }

public:
    ABB() : raiz(nullptr) {}

    ~ABB() {
        liberarMemoria(raiz);
    }

    // Insertar un nuevo elemento
    void insertar(const T& dato, const string& clave) {
        raiz = insertarRecursivo(raiz, dato, clave);
    }

    // Buscar un elemento por su clave
    T* buscar(const string& clave) const {
        NodoABB<T>* resultado = buscarRecursivo(raiz, clave);
        return (resultado != nullptr) ? &(resultado->dato) : nullptr;
    }

    // Obtener todos los elementos en orden
    vector<T> obtenerTodosEnOrden() const {
        vector<T> resultado;
        enOrdenRecursivo(raiz, resultado);
        return resultado;
    }

    // Verificar si el árbol está vacío
    bool estaVacio() const {
        return raiz == nullptr;
    }
};