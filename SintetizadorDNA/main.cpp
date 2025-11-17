#include "sintetizadorDNA.h"
#include <iostream>
#include <filesystem>
#define FMT_HEADER_ONLY
#include <fstream>
#include <regex>
#include <fmt/core.h>

int main(const int argc, char **argv) {
    if (argc < 2) {
        fmt::println("Usage: {} /path/to/tests/dir/teste.txt", argv[0]);
        return 1;
    }

    std::ifstream file(argv[1]);
    unsigned int MAX_CUSTO = 0;
    std::string seqInicial{};
    std::string seqFinal{};
    if (file.good() && file.is_open()) {
        std::string line;
        int counter = 0;
        while (getline(file, line)) {
           switch (counter) {
               case 0:
                   MAX_CUSTO = std::stoi(line);
               break;
               case 1:
                   seqInicial = line;
               break;
               case 2:
                   seqFinal = line;
               break;
               default:
               continue;
           }
            counter++;
        }
        file.close();
    } else {
        fmt::println("não consegui abrir o arquivo {}", argv[1]);
        return 1;
    }
    sequencia_log root{};
    root.parent = nullptr;
    root.operacao = std::string("INICIO");
    //fmt::println("TAREFA: {}=>{} com custo máximo {}", seqInicial, seqFinal, MAX_CUSTO);
    if (sintetizadorDNA(&seqInicial, &seqFinal, 0, MAX_CUSTO, &root)) {
        fmt::println("É possível sintetizar essa sequência.");
    } else {
        fmt::println("Não é possível sintetizar essa sequência.");
    }
    return 0;
}