#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP
#include "undoredo.hpp"

class StackArray {
private:
    Operacion* datos;
    int tamano;
    int capacidad;

public:
    StackArray(int capacidadInicial);
    ~StackArray();

    void push(Operacion valor);
    bool pop(Operacion& valorSacado);
    bool peek(Operacion& valor) const;
    bool isEmpty() const;
    int size() const;
};

#endif