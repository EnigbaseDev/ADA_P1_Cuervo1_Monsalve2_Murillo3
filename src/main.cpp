#include "undoredo.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>

struct CasoPrueba {
    std::string archivoEntrada;
    std::string archivoSalida;
    int capacidadInicial;
};

void correrCasosDePrueba() {
    std::vector<CasoPrueba> casos = {
        {"tests/caso1_secuencia_normal.txt", "results/caso1_salida.txt", 4},
        {"tests/caso2_undo_vacio.txt", "results/caso2_salida.txt", 4},
        {"tests/caso3_edicion_undo_redo.txt", "results/caso3_salida.txt", 4},
        {"tests/caso4_edicion_tras_undo.txt", "results/caso4_salida.txt", 4},
        {"tests/caso5_n_ediciones_n_deshacer.txt", "results/caso5_salida.txt", 4},
        {"tests/caso6_redo_exceso.txt", "results/caso6_salida.txt", 4},
        {"tests/caso7_crecimiento_capacidad.txt", "results/caso7_salida.txt", 2}
    };

    for (const auto& caso : casos) {
        GestorUndoRedo gestor(caso.capacidadInicial);
        gestor.procesarArchivo(caso.archivoEntrada, caso.archivoSalida);
        std::cout << "Procesado: " << caso.archivoEntrada << std::endl;
    }
}

void medirTiempos() {
    std::vector<std::pair<int, std::string>> tamanos = {
        {1000, "data/generados/n1000.txt"},
        {10000, "data/generados/n10000.txt"},
        {100000, "data/generados/n100000.txt"},
        {1000000, "data/generados/n1000000.txt"}
    };

    std::ofstream reporte("results/experimentacion.txt");
    reporte << "n_eventos,tiempo_ms_promedio,desviacion_estandar_ms" << std::endl;

    for (const auto& [n, rutaEntrada] : tamanos) {
        std::vector<double> tiempos;

        for (int repeticion = 0; repeticion < 5; repeticion++) {
            auto inicio = std::chrono::high_resolution_clock::now();

            GestorUndoRedo gestor(4);
            gestor.procesarArchivo(rutaEntrada, "results/temp_salida.txt");

            auto fin = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(fin - inicio).count();
            tiempos.push_back(ms);
        }

        double suma = 0;
        for (double t : tiempos) suma += t;
        double promedio = suma / tiempos.size();

        double sumaCuadrados = 0;
        for (double t : tiempos) sumaCuadrados += (t - promedio) * (t - promedio);
        double desviacion = std::sqrt(sumaCuadrados / tiempos.size());

        reporte << n << "," << promedio << "," << desviacion << std::endl;
        std::cout << "n=" << n << " -> promedio=" << promedio << "ms, desv=" << desviacion << "ms" << std::endl;
    }
}

int main() {
    std::cout << "=== Corriendo casos de prueba (Seccion 11) ===" << std::endl;
    correrCasosDePrueba();

    std::cout << "\n=== Corriendo experimentacion de tiempos (Seccion 9) ===" << std::endl;
    medirTiempos();

    return 0;
}