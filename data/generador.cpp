#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cstdint>
#include <vector>

// Generador para el Problema 1 (Undo/Redo)
void generarArchivoP1(const std::string& ruta, int n, unsigned int semilla = 42) {
    std::mt19937 generador(semilla);
    std::uniform_int_distribution<int> distTipoEvento(1, 100);

    std::ofstream salida(ruta);
    int documentoLen = 0; 

    for (int i = 0; i < n; i++) {
        int dado = distTipoEvento(generador);

        if (dado <= 70) {
            // Se asegura de que pos nunca exceda documentoLen
            int pos = (documentoLen > 0) ? (generador() % (documentoLen + 1)) : 0;
            salida << "EDIT INSERT " << pos << " x" << i << "\n";
            documentoLen += 2; // "x" + dígito/carácter
        } else if (dado <= 85) {
            salida << "UNDO\n";
            if (documentoLen >= 2) documentoLen -= 2;
        } else {
            salida << "REDO\n";
        }
    }
}

// Generador para el Problema 2 (Rate Limiter)
void generarArchivoP2(const std::string& ruta,
                     int n,
                     unsigned int semilla = 42,
                     size_t C = 10,
                     uint64_t T = 1000,
                     size_t L = 5) {
    std::mt19937_64 generador(semilla);
    std::uniform_int_distribution<uint64_t> distTiempo(1, 15);     // Delays entre 1 y 15 ms
    std::uniform_int_distribution<size_t> distBytes(64, 1500);      // Tamaños típicos de paquetes

    std::ofstream salida(ruta);
    if (!salida.is_open()) {
        std::cerr << "Error al abrir el archivo: " << ruta << std::endl;
        return;
    }

    salida << C << " " << T << " " << L << "\n";

    uint64_t timestamp = 1000;
    for (int i = 0; i < n; i++) {
        timestamp += distTiempo(generador);
        size_t bytes = distBytes(generador);
        salida << timestamp << " " << bytes << "\n";
    }
}

int main() {
    std::vector<int> tamanos = {1000, 10000, 100000, 1000000};

    std::cout << "--- Generando datasets de prueba ---" << std::endl;

    for (int n : tamanos) {
        // Rutas para Problema 1
        std::string rutaP1 = "data/generados/n" + std::to_string(n) + ".txt";
        generarArchivoP1(rutaP1, n);
        std::cout << "[P1] Creado: " << rutaP1 << std::endl;

        // Rutas para Problema 2
        std::string rutaP2 = "data/generados/p2_n" + std::to_string(n) + ".txt";
        generarArchivoP2(rutaP2, n, 42, 10, 1000, 5);
        std::cout << "[P2] Creado: " << rutaP2 << std::endl;
    }

    std::cout << "¡Todos los archivos fueron generados con exito!" << std::endl;
    return 0;
}