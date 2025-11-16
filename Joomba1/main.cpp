// --- By Gabriel ---

#include "Joomba1.h"
#include <string>
#include <sstream>
#include <filesystem>
#include <regex>
#include <fstream>
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
	for (const auto& entry : testesDir) {
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

int main(int argc, char** argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/tests/dir", argv[0]);
		return 1;
	}
	std::vector<std::string> filesNumbers{};
	if (get_files_numbers(argv[1], &filesNumbers) != 0) return 1;
	for (auto fileNumber : filesNumbers) {
		fmt::println("================{}================", fileNumber);
		std::string fileInput(fmt::format("{}/arq{}.in", argv[1], fileNumber));
		std::string fileOutput(fmt::format("{}/arq{}.out", argv[1], fileNumber));
		std::ifstream file(fileInput);
		uint8_t ANDARES = 0;
		uint8_t JANELAS = 0;
		std::vector<instrucao> instrucoes;
		if (file.good() && file.is_open()) {
			std::string line;
			while (getline(file, line)) {
				fmt::println("|=>{}", line);
				std::stringstream ss(line);
				std::string word;
				instrucao i{};
				while (ss >> word) {
					if (ANDARES == 0) {
						ANDARES = stoi(word);
						continue;
					}
					if (JANELAS == 0) {
						JANELAS = stoi(word);
						continue;
					}
					if (i.direcao == 0) {
						i.direcao = word.at(0);
						continue;
					}
					if (i.distancia == 0) {
						i.distancia = word.at(0) - 48;
						instrucoes.insert(instrucoes.begin(), i);
						continue;
					}
				}
			}
		}
		processa_entrada_joomba_1(&instrucoes, ANDARES, JANELAS);
	}

	return 0;
}