#include "stack_array.hpp"

StackArray::StackArray(int capacidadInicial) {
    tamano = 0;
    capacidad = capacidadInicial;
    datos = new Operacion[capacidad];
}

StackArray::~StackArray() {
    delete[] datos;
}

void StackArray::push(Operacion valor){
    if(tamano==capacidad){
        int nuevaCapacidad = capacidad *2;
        Operacion* nuevoArreglo = new Operacion[nuevaCapacidad];

        for (int i = 0; i <tamano; i++){
            nuevoArreglo[i] = datos [i];
        }
        delete[]datos;
        datos = nuevoArreglo;
        capacidad = nuevaCapacidad;
    }

    datos[tamano] = valor;
    tamano = tamano + 1;
}
bool StackArray::pop(Operacion& valorSacado){
    if(tamano ==0){
        return false;
    }
    valorSacado= datos [tamano -1];
    tamano = tamano -1;
    return true;
}
bool StackArray::peek(Operacion& valor) const {
    if (tamano ==0){
        return false;
    }
    valor = datos [tamano - 1];
    return true;
}
bool StackArray::isEmpty() const {
    return tamano ==0;
}
int StackArray::size() const {
    return tamano;
}