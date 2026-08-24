#include "stack_array.hpp"
#include "undoredo.hpp"
#include <iostream>

int main() {
    StackArray pila(3);
    
    pila.push({INSERT, 0, "hola"});
    pila.push({DELETE, 5, ""});
    pila.push({REPLACE, 2, "mundo"});

   Operacion op;
    bool exito = pila.pop(op);
    std::cout << "pop devolvio:" << exito << ", tipo: " << op.tipo
              << ", posicion:" << op.posicion << ", contenido: " << op.contenido << std::endl;

    std::cout << "Pila creada y push funcionando." << std::endl;
    return 0;
}