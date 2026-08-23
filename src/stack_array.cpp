#include "stack_array.hpp"

StackArray::StackArray(int capacidadInicial) {
    tamano = 0;
    capacidad = capacidadInicial;
    datos = new int[capacidad];
}

StackArray::~StackArray() {
    delete[] datos;
}