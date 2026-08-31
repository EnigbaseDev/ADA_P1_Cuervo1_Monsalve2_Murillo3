#include "undoredo.hpp"
#include "stack_list.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace {

std::string trim(const std::string& texto) {
    size_t inicio = 0;
    while (inicio < texto.size() && std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        ++inicio;
    }

    size_t fin = texto.size();
    while (fin > inicio && std::isspace(static_cast<unsigned char>(texto[fin - 1]))) {
        --fin;
    }

    return texto.substr(inicio, fin - inicio);
}

bool parseLongitud(const std::string& texto, size_t& longitud) {
    if (texto.empty()) {
        return false;
    }

    try {
        size_t pos = 0;
        unsigned long long valor = std::stoull(texto, &pos);
        if (pos == texto.size()) {
            longitud = static_cast<size_t>(valor);
            return true;
        }
    } catch (const std::exception&) {
        return false;
    }

    return false;
}

void aplicarOperacion(std::string& documento, Operacion op) {
    size_t pos = std::min(static_cast<size_t>(op.posicion), documento.size());

    if (op.tipo == INSERT) {
        documento.insert(pos, op.contenido);
    } else if (op.tipo == DELETE) {
        size_t len = std::min(op.contenido.size(), documento.size() - pos);
        documento.erase(pos, len);
    } else if (op.tipo == REPLACE) {
        size_t len = std::min(op.contenidoPrevio.size(), documento.size() - pos);
        documento.erase(pos, len);
        documento.insert(pos, op.contenido);
    }
}

void revertirOperacion(std::string& documento, Operacion op) {
    size_t pos = std::min(static_cast<size_t>(op.posicion), documento.size());

    if (op.tipo == INSERT) {
        size_t len = std::min(op.contenido.size(), documento.size() - pos);
        documento.erase(pos, len);
    } else if (op.tipo == DELETE) {
        documento.insert(pos, op.contenido);
    } else if (op.tipo == REPLACE) {
        size_t len = std::min(op.contenido.size(), documento.size() - pos);
        documento.erase(pos, len);
        documento.insert(pos, op.contenidoPrevio);
    }
}

} // namespace

template <typename PilaT>
GestorUndoRedo<PilaT>::GestorUndoRedo(int capacidadInicial)
    : pilaUndo(capacidadInicial), pilaRedo(capacidadInicial) {}

template <typename PilaT>
void GestorUndoRedo<PilaT>::registrarEdicion(Operacion op) {
    pilaUndo.push(op);

    Operacion temp;
    while (!pilaRedo.isEmpty()) {
        pilaRedo.pop(temp);
    }
}

template <typename PilaT>
bool GestorUndoRedo<PilaT>::deshacer(Operacion& opDeshecha) {
    if (pilaUndo.isEmpty()) {
        return false;
    }

    Operacion op;
    pilaUndo.pop(op);
    pilaRedo.push(op);
    opDeshecha = op;
    return true;
}

template <typename PilaT>
bool GestorUndoRedo<PilaT>::rehacer(Operacion& opRehecha) {
    if (pilaRedo.isEmpty()) {
        return false;
    }

    Operacion op;
    pilaRedo.pop(op);
    pilaUndo.push(op);
    opRehecha = op;
    return true;
}

template <typename PilaT>
int GestorUndoRedo<PilaT>::tamanoUndo() const {
    return pilaUndo.size();
}

template <typename PilaT>
int GestorUndoRedo<PilaT>::tamanoRedo() const {
    return pilaRedo.size();
}

template <typename PilaT>
void GestorUndoRedo<PilaT>::procesarArchivo(const std::string& rutaEntrada, const std::string& rutaSalida) {
    std::ifstream entrada(rutaEntrada);
    std::ofstream salida(rutaSalida);

    if (!entrada.is_open()) {
        std::cerr << "ERROR: no se pudo abrir el archivo de entrada: " << rutaEntrada << std::endl;
        return;
    }
    if (!salida.is_open()) {
        std::cerr << "ERROR: no se pudo abrir el archivo de salida: " << rutaSalida << std::endl;
        return;
    }

    std::string documento = "";
    std::string linea;

    while (std::getline(entrada, linea)) {
        if (linea.empty()) {
            continue;
        }

        std::istringstream iss(linea);
        std::string comando;
        iss >> comando;

        if (comando == "EDIT") {
            std::string tipoStr;
            int posicion = 0;
            std::string contenido;
            iss >> tipoStr >> posicion;
            std::getline(iss, contenido);
            contenido = trim(contenido);

            if (posicion < 0) {
                posicion = 0;
            }

            size_t posValida = std::min(static_cast<size_t>(posicion), documento.size());

            TipoEdicion tipo = INSERT;
            if (tipoStr == "DELETE") {
                tipo = DELETE;
            } else if (tipoStr == "REPLACE") {
                tipo = REPLACE;
            }

            std::string contenidoPrevio = "";
            if (tipo == DELETE) {
                size_t longitud = 0;
                if (parseLongitud(contenido, longitud)) {
                    size_t maxLongitud = documento.size() - posValida;
                    longitud = std::min(longitud, maxLongitud);
                    contenido = documento.substr(posValida, longitud);
                } else {
                    size_t maxLongitud = documento.size() - posValida;
                    size_t longitudTexto = std::min(contenido.size(), maxLongitud);
                    contenido = documento.substr(posValida, longitudTexto);
                }
            } else if (tipo == REPLACE) {
                size_t maxLongitud = documento.size() - posValida;
                size_t longitudAntes = std::min(contenido.size(), maxLongitud);
                contenidoPrevio = documento.substr(posValida, longitudAntes);
            }

            Operacion op = {tipo, static_cast<int>(posValida), contenido, contenidoPrevio};
            aplicarOperacion(documento, op);
            registrarEdicion(op);
            salida << "EDIT aplicado correctamente" << std::endl;

        } else if (comando == "UNDO") {
            Operacion op;
            if (deshacer(op)) {
                revertirOperacion(documento, op);
                salida << "UNDO exitoso" << std::endl;
            } else {
                salida << "UNDO no-op: pila Undo vacia" << std::endl;
            }

        } else if (comando == "REDO") {
            Operacion op;
            if (rehacer(op)) {
                aplicarOperacion(documento, op);
                salida << "REDO exitoso" << std::endl;
            } else {
                salida << "REDO no-op: pila Redo vacia" << std::endl;
            }
        }
    }

    salida << "Estado final: " << std::endl;
    salida << "Documento: " << documento << std::endl;
    salida << "Elementos en pila Undo: " << tamanoUndo() << std::endl;
    salida << "Elementos en pila Redo: " << tamanoRedo() << std::endl;
}

// Instanciacion explicita: GestorUndoRedo se usa con las dos representaciones
// del TAD Pila (arreglo dinamico y lista enlazada), sin duplicar logica.
template class GestorUndoRedo<StackArray>;
template class GestorUndoRedo<StackList>;
