#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP
#include "stack_array.hpp"
#include <string>

// GestorUndoRedo es el TAD de historial de edicion, construido sobre DOS PILAS
// (Undo y Redo). Se parametriza por el tipo de pila (PilaT) para poder
// instanciarse indistintamente con StackArray (arreglo dinamico) o StackList
// (lista enlazada) sin cambiar una sola linea de la logica de negocio,
// demostrando que ambas representaciones satisfacen la misma interfaz de Pila.
//
// PilaT debe exponer: PilaT(int capacidadInicial), push(Operacion),
// pop(Operacion&), peek(Operacion&) const, isEmpty() const, size() const.
template <typename PilaT>
class GestorUndoRedo {
private:
    PilaT pilaUndo;
    PilaT pilaRedo;

public:
    explicit GestorUndoRedo(int capacidadInicial);

    void registrarEdicion(Operacion op); // se llama al ocurrir un EDIT
    bool deshacer(Operacion& opDeshecha);
    bool rehacer(Operacion& opRehecha);

    int tamanoUndo() const;
    int tamanoRedo() const;

    void procesarArchivo(const std::string& rutaEntrada, const std::string& rutaSalida);
};

#endif
