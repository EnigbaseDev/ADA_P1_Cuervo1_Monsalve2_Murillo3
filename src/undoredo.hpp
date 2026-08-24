#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP
#include <string>

enum TipoEdicion {INSERT, DELETE, REPLACE};

struct Operacion {
    TipoEdicion tipo;
    int posicion;
    std::string contenido;
};

#endif