#include "stack_array.hpp"
#include <iostream>

int main() {
    StackArray pila(2);
    
    pila.push(10);
    pila.push(20);
    pila.push(30);

    std::cout << "Pila creada y push funcionando." << std::endl;
    return 0;
}