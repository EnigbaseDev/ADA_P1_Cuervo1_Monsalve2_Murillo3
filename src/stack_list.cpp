#include "stack_list.hpp"

StackList::StackList(int capacidadInicialIgnorada) {
    (void)capacidadInicialIgnorada; // no aplica en esta representacion
    cabeza = nullptr;
    tamano = 0;
}

StackList::~StackList() {
    while (cabeza != nullptr) {
        NodoPila* temp = cabeza;
        cabeza = cabeza->siguiente; // avanzamos al siguiente nodo
        delete temp;                // liberamos el nodo viejo
    }
}

void StackList::push(Operacion valor) {
    NodoPila* nuevoNodo = new NodoPila;
    nuevoNodo->valor = valor;
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    tamano++;
}

bool StackList::pop(Operacion& valorSacado) {
    if (cabeza == nullptr) {
        return false;
    }
    valorSacado = cabeza->valor;
    NodoPila* temp = cabeza;
    cabeza = temp->siguiente;
    delete temp;
    tamano--;
    return true;
}

bool StackList::peek(Operacion& valor) const {
    if (cabeza == nullptr) {
        return false;
    }
    valor = cabeza->valor;
    return true;
}

bool StackList::isEmpty() const {
    return cabeza == nullptr;
}

int StackList::size() const {
    return tamano;
}
