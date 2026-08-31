#ifndef STACK_LIST_HPP
#define STACK_LIST_HPP
#include "stack_array.hpp"

// Nodo de la lista enlazada simple usada como representacion interna de la Pila.
// Guarda una Operacion (mismo tipo que StackArray) para que ambas representaciones
// expongan exactamente la misma interfaz (mismo TAD Pila, dos implementaciones distintas).
struct NodoPila {
    Operacion valor;
    NodoPila* siguiente;
};

class StackList {
private:
    NodoPila* cabeza;
    int tamano;

public:
    // Se acepta un parametro entero (con valor por defecto) unicamente para
    // que StackArray y StackList compartan la misma firma de constructor y
    // puedan instanciarse indistintamente como PilaT en GestorUndoRedo<PilaT>.
    // En esta representacion no existe el concepto de "capacidad inicial":
    // cada nodo se reserva individualmente en cada push, sin redimensionamiento.
    explicit StackList(int capacidadInicialIgnorada = 0);
    ~StackList();

    // Prohibimos copia para evitar doble liberacion de memoria
    // (la clase administra memoria dinamica manualmente).
    StackList(const StackList&) = delete;
    StackList& operator=(const StackList&) = delete;

    void push(Operacion valor);
    bool pop(Operacion& valorSacado);
    bool peek(Operacion& valor) const;
    bool isEmpty() const;
    int size() const;
};

#endif
