#include "sintetizadorDNA.h"
#include <iostream>
#include <filesystem>
#define FMT_HEADER_ONLY
#include <fstream>
#include <regex>
#include <fmt/core.h>

static int get_files_numbers(const char* dir, std::vector<std::string>* filesNumbers) {
    std::error_code ec;
    std::filesystem::directory_iterator testesDir(dir, ec);

    if (ec) {
        fmt::println("Não foi possível abrir o diretório {}", dir);
        return 1;
    }
    std::regex testFilesRegex(R"(arq(\d+)\.in$)");
    std::smatch matches;
    for (const auto & entry : testesDir) {
        if (!entry.is_regular_file()) continue;
        std::string text(entry.path().string());
        if (std::regex_search(text, matches, testFilesRegex)) {
            //fmt::println("{}", matches.str(1));
            filesNumbers->push_back(matches.str(1));
        }
        //std::cout << entry.path() << std::endl;
    }
    return 0;
}
int main(const int argc, char **argv) {
    if (argc < 2) {
        fmt::println("Usage: {} /path/to/tests/dir", argv[0]);
        return 1;
    }
    std::vector<std::string> filesNumbers{};
    if (get_files_numbers(argv[1], &filesNumbers) != 0) return 1;

    for (auto fileNumber: filesNumbers) {
        fmt::println("================{}================", fileNumber);
        std::string fileInput(fmt::format("{}/arq{}.in", argv[1], fileNumber));
        std::string fileOutput(fmt::format("{}/arq{}.out", argv[1], fileNumber));
        std::ifstream file(fileInput);
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
            fmt::println("não consegui abrir o arquivo {}", fileInput);
        }
        sequencia_log root{};
        root.parent = nullptr;
        root.operacao = std::string("INICIO");
        fmt::println("TAREFA: {}=>{} com custo máximo {}", seqInicial, seqFinal, MAX_CUSTO);
        if (sintetizadorDNA(&seqInicial, &seqFinal, 0, MAX_CUSTO, &root)) {
            fmt::println("=>É possível sintetizar essa sequência.");
        } else {
            fmt::println("=>Não é possível sintetizar essa sequência.");
        }
        std::ifstream fileOut(fileOutput);
        if (fileOut.good() && fileOut.is_open()) {
            std::string line;
            while (getline(fileOut, line)) {
                fmt::println("<={}", line);
            }
            file.close();
        }
    }
    return 0;
}