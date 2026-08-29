#include <iostream>
#include <fstream>
#include <random>
#include <string>

//Semilla Fija
void generarArchivo(const std::string& ruta, int n, unsigned int semilla = 42){
    std::mt19937 generador(semilla);
    std::uniform_int_distribution<int> distTipoEvento(1, 100);
    std::uniform_int_distribution<int> distPosicion(0, 50);

    std::ofstream salida(ruta);
    int documentoLen = 0; 

    for(int i = 0; i < n; i++){
        int dado = distTipoEvento(generador);

        if(dado <= 70) {
            int pos = documentoLen > 0 ? (generador() % documentoLen) : 0;
            salida << "EDIT INSERT" << pos << " x" << i <<"\n";
            documentoLen += 2;
        } else if (dado <= 85) {
            salida << "UNDO\n";
        } else {
            salida << "REDO\n";
            
        }
    }
}

int main (int argc, char* argv[]) {
    if(argc < 3){
        std:: cerr << "Uso: generador <n_eventos> <ruta_salida>" <<std::endl;
        return 1;
    }
    int n = std::stoi(argv[1]);
    std::string ruta = argv[2];
    generarArchivo(ruta, n);
    std::cout << "Generados " << n << " eventos en " << ruta << " (semilla=42)" << std::endl;
    return 0;
}