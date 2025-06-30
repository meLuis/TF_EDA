#pragma once

template <typename T>
class NodoAVL {
public:
    T dato;
    NodoAVL* izquierda;
    NodoAVL* derecha;
    int altura;

    NodoAVL(const T& valor) : dato(valor), izquierda(nullptr), derecha(nullptr), altura(1) {}
};