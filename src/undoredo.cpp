#include "undoredo.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> // Necesario para std::min

GestorUndoRedo::GestorUndoRedo(int capacidadInicial)
    : pilaUndo(capacidadInicial), pilaRedo(capacidadInicial) {}

void GestorUndoRedo::registrarEdicion(Operacion op) {
    pilaUndo.push(op);

    Operacion temp;
    while (!pilaRedo.isEmpty()) {
        pilaRedo.pop(temp);
    }
}

bool GestorUndoRedo::deshacer(Operacion& opDesecha) {
    if (pilaUndo.isEmpty()) {
        return false;
    }
    Operacion op;
    pilaUndo.pop(op);
    pilaRedo.push(op);
    opDesecha = op;
    return true;
}

bool GestorUndoRedo::rehacer(Operacion& opRehecha) {
    if (pilaRedo.isEmpty()) {
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

int GestorUndoRedo::tamanoRedo() const {
    return pilaRedo.size();
}

static void aplicarOperacion(std::string& documento, Operacion op) {
    // Garantizar que la posición nunca sea out_of_range
    size_t pos = std::min((size_t)op.posicion, documento.size());

    if (op.tipo == INSERT) {
        documento.insert(pos, op.contenido);
    } else if (op.tipo == DELETE) {
        size_t len = std::min(op.contenido.size(), documento.size() - pos);
        documento.erase(pos, len);
    } else if (op.tipo == REPLACE) {
        size_t len = std::min(op.contenido.size(), documento.size() - pos);
        documento.erase(pos, len);
        documento.insert(pos, op.contenido);
    }
}

static void revertirOperacion(std::string& documento, Operacion op) {
    size_t pos = std::min((size_t)op.posicion, documento.size());

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

void GestorUndoRedo::procesarArchivo(const std::string& rutaEntrada, const std::string& rutaSalida) {
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
        if (linea.empty()) continue;

        std::istringstream iss(linea);
        std::string comando;
        iss >> comando;

        if (comando == "EDIT") {
            std::string tipoStr;
            int posicion;
            std::string contenido;
            iss >> tipoStr >> posicion;
            std::getline(iss, contenido);

            if (!contenido.empty() && contenido[0] == ' ') {
                contenido = contenido.substr(1);
            }

            // Ajustar posición si excede el tamaño actual de documento
            if (posicion < 0) posicion = 0;
            size_t posValida = std::min((size_t)posicion, documento.size());

            TipoEdicion tipo;
            if (tipoStr == "INSERT") tipo = INSERT;
            else if (tipoStr == "DELETE") tipo = DELETE;
            else tipo = REPLACE;

            if (tipo == DELETE) {
                int longitud = 0;
                try {
                    longitud = std::stoi(contenido);
                } catch (...) {
                    longitud = 1;
                }
                size_t lenValida = std::min((size_t)longitud, documento.size() - posValida);
                contenido = documento.substr(posValida, lenValida);
            }

            std::string contenidoPrevio = "";
            if (tipo == REPLACE) {
                size_t lenValida = std::min(contenido.size(), documento.size() - posValida);
                contenidoPrevio = documento.substr(posValida, lenValida);
            }

            Operacion op = {tipo, (int)posValida, contenido, contenidoPrevio};
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