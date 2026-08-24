#include "undoredo.hpp"

GestorUndoRedo::GestorUndoRedo(int capacidadInicial)
    : pilaUndo(capacidadInicial), pilaRedo(capacidadInicial) {

    }
void GestorUndoRedo::registrarEdicion(Operacion op) {
    pilaUndo.push(op);

    Operacion temp;
    while (!pilaRedo.isEmpty()) {
        pilaRedo.pop(temp);
    }
}
bool GestorUndoRedo::deshacer(Operacion& opDesecha){
    if(pilaUndo.isEmpty()) {
        return false;
    }
    Operacion op;
    pilaRedo.pop(op);
    pilaRedo.push(op);
    opDesecha = op;
    return true;
}
bool GestorUndoRedo::rehacer(Operacion& opRehecha) {
    if(pilaRedo.isEmpty()) {
        return false;
    }
    Operacion op;
    pilaRedo.pop(op);
    pilaUndo.push(op);
    opRehecha = op;
    return true;
}
int GestorUndoRedo::tamanoUndo() const {
    return pilaUndo.size();
}
int GestorUndoRedo::tamanoRedo() const{
    return pilaRedo.size ();
}