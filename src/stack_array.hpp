#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP
#include <string>

enum TipoEdicion {INSERT, DELETE, REPLACE };

struct Operacion {
    TipoEdicion tipo;
    int posicion;
    std::string contenido;  
    std::string contenidoPrevio; 
};

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