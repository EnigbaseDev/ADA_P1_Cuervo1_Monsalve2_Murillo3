#ifndef STACK_LIST_HPP
#define STACK_LIST_HPP

struct Nodo{
    int valor;
    Nodo* siguiente;
};

class StackList{
    private:
    Nodo* cabeza;
    int tamano;

    public :
    StackList();
    ~StackList();

    void push (int valor);
    bool pop (int& valorSacado);
    bool peek(int& valor) const;
    bool isEmpty() const;
    int size () const;
};

StackList::StackList(){
    cabeza = nullptr;
    tamano = 0;
}
StackList::~StackList(){
    while (cabeza != nullptr){
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente; //avanzamos al siguiente nodo
        delete temp; //liberamos el nodo viejo
    }
}

void StackList::push(int valor){
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->valor = valor;
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    tamano++;
}

bool StackList::pop(int& valorSacado){
   if(cabeza == nullptr){
    return false;
   }
   else{
    valorSacado = cabeza->valor;
    Nodo* temp = cabeza;
    cabeza = temp->siguiente;
    delete temp;
    tamano--;
    return true;
   }
}

bool StackList::peek(int& valor) const{
    if(cabeza == nullptr){
        return false;
    }
    valor = cabeza->valor;
    return true;
}

bool StackList::isEmpty() const{
    return cabeza == nullptr;
}

int StackList:: size() const{
    return tamano;
}
#endif

