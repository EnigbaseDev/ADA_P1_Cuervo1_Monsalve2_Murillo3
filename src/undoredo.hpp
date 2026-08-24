#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP
#include "stack_array.hpp"

class GestorUndoRedo {
    private: 
    StackArray pilaUndo;
    StackArray pilaRedo;

    public:
    GestorUndoRedo(int capacidadInicial);

    void registrarEdicion(Operacion op); //llamamos al ocurro un EDIT
    bool deshacer (Operacion& opDesecha);
    bool rehacer (Operacion& opResecha);

    int tamanoUndo() const;
    int tamanoRedo() const;
};
#endif