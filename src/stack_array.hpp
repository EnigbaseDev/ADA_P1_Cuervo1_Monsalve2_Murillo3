#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP

//Rep del arreglo dinámico
class StackArray {
    private:
        int* datos;
        int tamano;
        int capacidad;

    public:
        StackArray(int capacidadInicial); //constructor
      
    
        ~StackArray(); //destructor
           


        void push(int valor);
        bool pop(int& valorSacado); //puntero pq estas operaciones necesitan conocer la posición
        bool peek(int& valor) const;
        bool isEmpty() const;
        int size() const;
};

#endif