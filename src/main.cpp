#include "undoredo.hpp"
#include <iostream>
#include <vector>
#include <string>

struct CasoPrueba {
    std::string archivoEntrada;
    std::string archivoSalida;
    int capacidadInicial;
};

int main() {
     std::vector<CasoPrueba> casos = {
          {"tests/caso1_secuencia_normal.txt", "results/caso1_salida.txt" , 4},
          {"tests/caso2_undo_vacio.txt", "results/caso2_salida.txt" , 4},
          {"tests/caso3_edicion_undo_redo.txt", "results/caso3_salida.txt", 4},
          {"tests/caso4_edicion_tras_undo.txt", "results/caso4_salida.txt", 4},
          {"tests/caso5_n_ediciones_n_deshacer.txt" , "results/caso5_salida.txt", 4},
          {"tests/caso6_redo_exceso.txt", "results/caso6_salida", 4},
          {"tests/caso7_crecimiento_capacidad.txt", "results/caso7_salida.txt", 2}

     };

  for (const auto& caso : casos) {
    GestorUndoRedo gestor(caso.capacidadInicial);
    gestor.procesarArchivo(caso.archivoEntrada, caso.archivoSalida);
    std::cout <<"Procesado: " << caso.archivoEntrada << std::endl;

  }

  return 0;

   
}

