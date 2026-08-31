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

    // Prohibimos copia: la clase administra memoria dinamica cruda (new[]/delete[])
    // y una copia superficial del puntero causaria doble liberacion al destruir
    // ambas instancias (mismo motivo por el que QueueCircular la prohibe).
    StackArray(const StackArray&) = delete;
    StackArray& operator=(const StackArray&) = delete;

    void push(Operacion valor);
    bool pop(Operacion& valorSacado);
    bool peek(Operacion& valor) const;
    bool isEmpty() const;
    int size() const;
};

#endif