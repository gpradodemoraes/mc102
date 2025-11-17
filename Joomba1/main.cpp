// --- By Gabriel ---

#include "Joomba1.h"
#include <string>
#include <sstream>
#include <regex>
#include <fstream>
#include <fmt/core.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/tests/test_file.txt", argv[0]);
		return 1;
	}
	std::ifstream file(argv[1]);
	std::vector<instrucao> instrucoes{};
	bool retval = false;
	if (file.good() && file.is_open()) {
		int32_t JANELAS = 0;
		int32_t ANDARES = 0;
		std::string line;
		while (getline(file, line)) {
			//fmt::println("|=>{}", line);
			std::stringstream ss(line);
			std::string word;
			instrucao i{};
			while (ss >> word) {
				if(word.at(0) == 'F') break;
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
		file.close();
		retval = processa_entrada_joomba_1(&instrucoes, ANDARES, JANELAS);
	}

	return retval ? 0 : 1;
}