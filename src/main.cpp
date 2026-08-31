#include "undoredo.hpp"
#include "stack_list.hpp"
#include "ratelimiter.hpp"
#include "queue_circular.hpp"
#include "queue_list.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

// ==========================================
// ESTRUCTURAS Y FUNCIONES DEL PROBLEMA 1
// ==========================================

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
        GestorUndoRedo<StackArray> gestor(caso.capacidadInicial);
        gestor.procesarArchivo(caso.archivoEntrada, caso.archivoSalida);
        std::cout << "Procesado P1: " << caso.archivoEntrada << std::endl;
    }
}

// ==========================================
// DEMOSTRACION: PILA COMO TAD (Seccion 6)
// ==========================================
// Corre los mismos 7 casos de prueba del Problema 1 sobre las dos
// representaciones de la Pila (StackArray y StackList) usando exactamente
// la misma logica de GestorUndoRedo<PilaT>, y verifica que ambas producen
// una salida identica. Esto es evidencia concreta de que ambas satisfacen
// el mismo contrato de interfaz (mismo TAD, dos implementaciones distintas).
std::string leerArchivoComoTexto(const std::string& ruta) {
    std::ifstream f(ruta);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void demostrarPilaComoTAD() {
    std::vector<CasoPrueba> casos = {
        {"tests/caso1_secuencia_normal.txt", "results/tad_array_caso1.txt", 4},
        {"tests/caso2_undo_vacio.txt", "results/tad_array_caso2.txt", 4},
        {"tests/caso3_edicion_undo_redo.txt", "results/tad_array_caso3.txt", 4},
        {"tests/caso4_edicion_tras_undo.txt", "results/tad_array_caso4.txt", 4},
        {"tests/caso5_n_ediciones_n_deshacer.txt", "results/tad_array_caso5.txt", 4},
        {"tests/caso6_redo_exceso.txt", "results/tad_array_caso6.txt", 4},
        {"tests/caso7_crecimiento_capacidad.txt", "results/tad_array_caso7.txt", 2}
    };

    std::ofstream reporte("results/comparacion_pilas.txt");
    reporte << "Comparacion StackArray vs StackList sobre el mismo TAD Pila (Undo/Redo)\n";
    reporte << "=========================================================================\n\n";

    bool todoIdentico = true;

    for (const auto& caso : casos) {
        std::string salidaList = caso.archivoSalida;
        size_t posArray = salidaList.find("tad_array_");
        if (posArray != std::string::npos) {
            salidaList.replace(posArray, std::string("tad_array_").size(), "tad_list_");
        }

        GestorUndoRedo<StackArray> gestorArray(caso.capacidadInicial);
        gestorArray.procesarArchivo(caso.archivoEntrada, caso.archivoSalida);

        GestorUndoRedo<StackList> gestorList(caso.capacidadInicial);
        gestorList.procesarArchivo(caso.archivoEntrada, salidaList);

        std::string contenidoArray = leerArchivoComoTexto(caso.archivoSalida);
        std::string contenidoList = leerArchivoComoTexto(salidaList);

        bool iguales = (contenidoArray == contenidoList) && !contenidoArray.empty();
        todoIdentico = todoIdentico && iguales;

        reporte << "Caso: " << caso.archivoEntrada << " -> "
                << (iguales ? "IDENTICO en ambas representaciones" : "DIFERENTE (revisar)")
                << std::endl;
    }

    reporte << "\nConclusion: "
            << (todoIdentico
                    ? "Las dos representaciones del TAD Pila (arreglo dinamico y lista "
                      "enlazada) producen resultados identicos para los 7 casos de prueba "
                      "del Problema 1, confirmando que ambas satisfacen el mismo contrato "
                      "de interfaz (push/pop/peek/isEmpty/size) sin que quien las use "
                      "necesite conocer la representacion interna."
                    : "Se detectaron diferencias entre representaciones; revisar implementacion.")
            << std::endl;

    std::cout << "Comparacion TAD Pila (StackArray vs StackList) completada -> "
              << "results/comparacion_pilas.txt" << std::endl;
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

            GestorUndoRedo<StackArray> gestor(4);
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

// ==========================================
// ESTRUCTURAS Y FUNCIONES DEL PROBLEMA 2
// ==========================================

struct CasoPruebaP2 {
    std::string archivoEntrada;
    std::string archivoSalida;
    size_t C;
    uint64_t T;
    size_t L;
};

bool leerCabeceraP2(std::ifstream& fileIn, size_t& C, uint64_t& T, size_t& L) {
    std::string linea;
    if (!std::getline(fileIn, linea)) {
        return false;
    }

    std::istringstream iss(linea);
    size_t c = 0;
    uint64_t t = 0;
    size_t l = 0;

    if (!(iss >> c >> t >> l)) {
        fileIn.clear();
        fileIn.seekg(0, std::ios::beg);
        return false;
    }

    C = c;
    T = t;
    L = l;
    return true;
}

void probarDequeueSobreColaVacia(const std::string& archivoSalida) {
    std::ofstream out(archivoSalida, std::ios::app);
    out << "\n=== Prueba dedicada: dequeue/front sobre cola vacia ===\n";

    QueueCircular<size_t> colaCircularVacia(5);
    try {
        colaCircularVacia.dequeue();
        out << "QueueCircular.dequeue() en vacio: NO lanzo excepcion (INESPERADO)\n";
    } catch (const std::underflow_error& e) {
        out << "QueueCircular.dequeue() en vacio -> excepcion capturada: \""
            << e.what() << "\" (manejo correcto, sin caida del programa)\n";
    }
    try {
        colaCircularVacia.front();
        out << "QueueCircular.front() en vacio: NO lanzo excepcion (INESPERADO)\n";
    } catch (const std::underflow_error& e) {
        out << "QueueCircular.front() en vacio -> excepcion capturada: \""
            << e.what() << "\" (manejo correcto, sin caida del programa)\n";
    }

    QueueList<uint64_t> colaListaVacia;
    try {
        colaListaVacia.dequeue();
        out << "QueueList.dequeue() en vacio: NO lanzo excepcion (INESPERADO)\n";
    } catch (const std::underflow_error& e) {
        out << "QueueList.dequeue() en vacio -> excepcion capturada: \""
            << e.what() << "\" (manejo correcto, sin caida del programa)\n";
    }
    try {
        colaListaVacia.front();
        out << "QueueList.front() en vacio: NO lanzo excepcion (INESPERADO)\n";
    } catch (const std::underflow_error& e) {
        out << "QueueList.front() en vacio -> excepcion capturada: \""
            << e.what() << "\" (manejo correcto, sin caida del programa)\n";
    }
}

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

        size_t C = caso.C;
        uint64_t T = caso.T;
        size_t L = caso.L;

        if (!leerCabeceraP2(fileIn, C, T, L)) {
            fileIn.clear();
            fileIn.seekg(0, std::ios::beg);
            C = caso.C;
            T = caso.T;
            L = caso.L;
        }

        RateLimiter rateLimiter(C, T, L);
        uint64_t timestamp;
        size_t bytes;

        fileOut << "=== Reporte de Salida: " << caso.archivoEntrada << " ===" << std::endl;
        fileOut << "Parametros: C=" << C << ", T=" << T << "ms, L=" << L << std::endl;
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
        fileOut << "\nEstado Final de Colas:" << std::endl;
        fileOut << "- Buffer: " << (rateLimiter.isBufferEmpty() ? "VACIO" : (rateLimiter.isBufferFull() ? "LLENO" : "PARCIAL"))
                << ", ocupacion=" << rateLimiter.getBufferCount() << "/" << rateLimiter.getBufferCapacity() << std::endl;
        fileOut << "- Cola de marcas de tiempo: " << (rateLimiter.isTimestampQueueEmpty() ? "VACIA" : "CON DATOS")
                << ", elementos=" << rateLimiter.getTimestampCount() << std::endl;
        

        if (caso.archivoEntrada.find("p2_caso6") != std::string::npos) {
                probarDequeueSobreColaVacia(caso.archivoSalida);
        }
        std::cout << "Procesado P2: " << caso.archivoEntrada << std::endl;
    }
}

struct PaqueteMemoria {
    uint64_t timestamp;
    size_t bytes;
};

void medirTiemposP2() {
    // Se corrigieron los nombres para usar los de P2: "p2_n*.txt"
    std::vector<std::pair<int, std::string>> tamanos = {
        {1000, "data/generados/p2_n1000.txt"},
        {10000, "data/generados/p2_n10000.txt"},
        {100000, "data/generados/p2_n100000.txt"},
        {1000000, "data/generados/p2_n1000000.txt"}
    };

    std::ofstream reporte("results/experimentacion_p2.txt");
    reporte << "n_paquetes,tiempo_ms_promedio,desviacion_estandar_ms" << std::endl;

    for (const auto& [n, rutaEntrada] : tamanos) {
        std::ifstream fileIn(rutaEntrada);
        if (!fileIn.is_open()) {
            std::cout << "Omitiendo medicion de P2 para n=" << n << " (archivo no encontrado)" << std::endl;
            continue;
        }

        size_t C = 10;
        uint64_t T = 1000;
        size_t L = 5;
        leerCabeceraP2(fileIn, C, T, L);

        // Cargar datos a memoria previo a la medicion
        std::vector<PaqueteMemoria> paquetes;
        paquetes.reserve(n);

        uint64_t ts;
        size_t b;
        while (fileIn >> ts >> b) {
            paquetes.push_back({ts, b});
        }
        fileIn.close();

        if (paquetes.empty()) {
            std::cout << "ERROR: El archivo " << rutaEntrada << " no contiene datos validos de P2." << std::endl;
            continue;
        }

        std::vector<double> tiempos;

        for (int repeticion = 0; repeticion < 5; repeticion++) {
            RateLimiter rateLimiter(C, T, L);

            auto inicio = std::chrono::high_resolution_clock::now();

            for (const auto& pkt : paquetes) {
                rateLimiter.processPacket(pkt.timestamp, pkt.bytes);
            }

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
        std::cout << "P2 n=" << n << " (" << paquetes.size() << " paquetes) -> promedio=" << promedio << "ms, desv=" << desviacion << "ms" << std::endl;
    }
}

// ==========================================
// ORQUESTADOR PRINCIPAL
// ==========================================

int main() {
    std::cout << "=== Corriendo casos de prueba Problema 1 (Seccion 11) ===" << std::endl;
    correrCasosDePrueba();

    std::cout << "\n=== Demostrando TAD Pila: StackArray vs StackList (Seccion 6) ===" << std::endl;
    demostrarPilaComoTAD();

    std::cout << "\n=== Corriendo experimentacion de tiempos Problema 1 (Seccion 9) ===" << std::endl;
    medirTiempos();

    std::cout << "\n=== Corriendo casos de prueba Problema 2 (Seccion 11) ===" << std::endl;
    correrCasosDePruebaP2();

    std::cout << "\n=== Corriendo experimentacion de tiempos Problema 2 (Seccion 9) ===" << std::endl;
    medirTiemposP2();

    return 0;
}