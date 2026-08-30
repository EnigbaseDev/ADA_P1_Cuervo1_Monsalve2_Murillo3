#include "undoredo.hpp"
#include "ratelimiter.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>


// ESTRUCTURAS Y FUNCIONES DEL PROBLEMA 1

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
        std::cout << "Procesado P1: " << caso.archivoEntrada << std::endl;
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
        std::ifstream fcheck(rutaEntrada);
        if (!fcheck.is_open()) {
            std::cout << "Omitiendo medicion de P1 para n=" << n << " (archivo no generado en local)" << std::endl;
            continue;
        }
        fcheck.close();

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
        std::cout << "P1 n=" << n << " -> promedio=" << promedio << "ms, desv=" << desviacion << "ms" << std::endl;
    }
}


// ESTRUCTURAS Y FUNCIONES DEL PROBLEMA 2

struct CasoPruebaP2 {
    std::string archivoEntrada;
    std::string archivoSalida;
    size_t C;
    uint64_t T;
    size_t L;
};

void correrCasosDePruebaP2() {
    std::vector<CasoPruebaP2> casos = {
        {"tests/p2_caso1_flujo_normal.txt", "results/p2_caso1_salida.txt", 5, 1000, 3},
        {"tests/p2_caso2_buffer_vacio.txt", "results/p2_caso2_salida.txt", 5, 1000, 3},
        {"tests/p2_caso3_unico_paquete.txt", "results/p2_caso3_salida.txt", 5, 1000, 3},
        {"tests/p2_caso4_buffer_lleno.txt", "results/p2_caso4_salida.txt", 2, 5000, 10},
        {"tests/p2_caso5_rafaga_excede_l.txt", "results/p2_caso5_salida.txt", 10, 1000, 2},
        {"tests/p2_caso6_dequeue_vacio.txt", "results/p2_caso6_salida.txt", 5, 1000, 3},
        {"tests/p2_caso7_borde_tiempo.txt", "results/p2_caso7_salida.txt", 10, 1000, 1}
    };

    for (const auto& caso : casos) {
        std::ifstream fileIn(caso.archivoEntrada);
        std::ofstream fileOut(caso.archivoSalida);

        if (!fileIn.is_open() || !fileOut.is_open()) {
            std::cerr << "Error accediendo a archivos en: " << caso.archivoEntrada << std::endl;
            continue;
        }

        RateLimiter rateLimiter(caso.C, caso.T, caso.L);
        uint64_t timestamp;
        size_t bytes;

        fileOut << "=== Reporte de Salida: " << caso.archivoEntrada << " ===" << std::endl;
        while (fileIn >> timestamp >> bytes) {
            PacketResult res = rateLimiter.processPacket(timestamp, bytes);
            std::string estado = (res.decision == Decision::ACCEPTED) ? "ACEPTADO" :
                                 (res.decision == Decision::REJECTED_BUFFER_FULL) ? "RECHAZADO_BUFFER_FULL" : "RECHAZADO_RATE_LIMIT";
            fileOut << "Paquete t=" << timestamp << "ms, " << bytes << " bytes -> " << estado << std::endl;
        }

        fileOut << "\nEstadisticas Agregadas:" << std::endl;
        fileOut << "Total Aceptados: " << rateLimiter.getTotalAccepted() << std::endl;
        fileOut << "Total Rechazados Bufer Lleno: " << rateLimiter.getTotalRejectedBuffer() << std::endl;
        fileOut << "Total Rechazados Limite Tasa: " << rateLimiter.getTotalRejectedRate() << std::endl;
        fileOut << "Ocupacion Maxima del Bufer: " << rateLimiter.getMaxBufferOccupancy() << std::endl;

        std::cout << "Procesado P2: " << caso.archivoEntrada << std::endl;
    }
}


// ORQUESTADOR PRINCIPAL

int main() {
    std::cout << "=== Corriendo casos de prueba Problema 1 (Seccion 11) ===" << std::endl;
    correrCasosDePrueba();

    std::cout << "\n=== Corriendo experimentacion de tiempos Problema 1 (Seccion 9) ===" << std::endl;
    medirTiempos();

    std::cout << "\n=== Corriendo casos de prueba Problema 2 (Seccion 11) ===" << std::endl;
    correrCasosDePruebaP2();

    return 0;
}