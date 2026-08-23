#include "stack_array.hpp"

StackArray::StackArray(int capacidadInicial) {
    tamano = 0;
    capacidad = capacidadInicial;
    datos = new int[capacidad];
}

StackArray::~StackArray() {
    delete[] datos;
}

void StackArray::push(int valor) {
    if(tamano ==capacidad) {
        int nuevaCapacidad = capacidad * 2;
        int *  nuevoArreglo = new int [nuevaCapacidad];

        for(int i = 0; i < tamano; i++){
            nuevoArreglo[i] = datos[i];
        }
        delete [] datos;
        datos = nuevoArreglo;
        capacidad = nuevaCapacidad; 
    }
    
    datos[tamano] = valor;
    tamano = tamano + 1;

}
bool StackArray::pop(int& valorSacado){
    if (tamano == 0){
        return false;
    }
    valorSacado = datos[tamano -1];
    tamano = tamano -1;
    return true;
}