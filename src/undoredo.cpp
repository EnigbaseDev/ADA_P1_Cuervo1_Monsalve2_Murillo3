#include "undoredo.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

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
    pilaUndo.pop(op);
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

static void aplicarOperacion(std::string& documento, Operacion op){
    if(op.tipo ==INSERT){
        documento.insert(op.posicion, op.contenido);
    } else if (op.tipo == DELETE){
        documento.erase(op.posicion, op.contenido.size());
    } else if (op.tipo == REPLACE) {
        documento.erase(op.posicion, op.contenido.size());
        documento.insert(op.posicion, op.contenido);
    }
}

static void revertirOperacion(std::string& documento, Operacion op) {
    if (op.tipo == INSERT){
        documento.erase(op.posicion, op.contenido.size());
    } else if (op.tipo == DELETE) {
        documento.insert(op.posicion, op.contenido);
    } else if (op.tipo == REPLACE){
    }
}
void GestorUndoRedo::procesarArchivo(const std::string& rutaEntrada, const std::string& rutaSalida){
    std::ifstream entrada(rutaEntrada);
    std::ofstream salida(rutaSalida);
    std::string documento ="";
    std::string linea;

    while(std::getline(entrada, linea)) {
        std::istringstream iss(linea);
        std::string comando;
        iss >> comando;

        if(comando == "EDIT") {
            std::string tipoStr;
            int posicion;
            std::string contenido;
            iss >> tipoStr >> posicion;
            std::getline(iss, contenido);
            if(!contenido.empty() && contenido[0] == ' '){
                contenido = contenido.substr(1);
            }

            TipoEdicion tipo;
            if(tipoStr == "INSERT") tipo = INSERT;
            else if(tipoStr == "DELETE") tipo = DELETE;
            else tipo = REPLACE;

            if(tipo ==DELETE) {
                int longitud = std::stoi(contenido);
                contenido = documento.substr(posicion, longitud);
            }

            Operacion op = {tipo, posicion, contenido};
            aplicarOperacion(documento, op);
            registrarEdicion(op);
            salida <<"EDIT apliaco correctamente"<<std::endl;

        } else if (comando == "UNDO"){
            Operacion op;
            if (deshacer(op)) {
                revertirOperacion(documento, op);
                salida <<"UNDO exitoso" <<std::endl;
            } else {
                salida <<"UNDO no-op: pila Undo vacia" <<std::endl;
            }

        } else if (comando == "REDO") {
            Operacion op;
            if(rehacer(op)) {
                aplicarOperacion(documento, op);
                salida <<"REDO exitoso" <<std::endl;
            } else {
                salida << "REDO no-op: pila Redo vacía" <<std::endl;
            }
        }
    }
    salida << "Esatdo final: " << std::endl;
    salida << "Documento: "<< documento << std::endl;
    salida << "Elementos en pila Undo: " << tamanoUndo() << std::endl;
    salida << "Elementos en pila Redo: " << tamanoRedo() << std::endl;
}