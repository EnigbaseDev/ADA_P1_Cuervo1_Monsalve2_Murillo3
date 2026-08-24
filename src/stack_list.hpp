#ifndef STACK_LIST_HPP
#define STACK_LIST_HPP
#include "stack_array.hpp"

struct Nodo {
    Operacion valor;
    Nodo* siguiente;
};

class StackList{
    private:
    Nodo* cabeza;
    int tamano;
  
    public:
    StackList();
    ~StackList();

    void push(Operacion valor);
    bool pop(Operacion& valorSacado);
    bool peek(Operacion& valor) const;
    bool isEmpty() const;
    int size() const;
};
#endif