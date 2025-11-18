#include "Joomba1.h"
#include <string>
#include <sstream>
#include <regex>
#include <fstream>
#include "predio.h"

void parse_entrada_joomba1(char* filepath, std::vector<instrucao>* instrucoes, int32_t *ANDARES_, int32_t *JANELAS_) {
	std::ifstream file(filepath);
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
					i.distancia = stoi(word);
					instrucoes->insert(instrucoes->begin(), i);
					continue;
				}
			}
		}
		file.close();
		*ANDARES_ = ANDARES;
		*JANELAS_ = JANELAS;
	}
}
bool processa_entrada_joomba_1(std::vector<instrucao>* instrucoes,
	const int32_t ANDARES,
	const int32_t JANELAS) {

	predio* p = constroi_predio(ANDARES, JANELAS);

	while (!instrucoes->empty())  {
		instrucao i = instrucoes->back();
		switch (i.direcao) {
		case 'D':
			move_direita(i.distancia, p);
			break;
		case 'E':
			move_esquerda(i.distancia, p);
			break;
		case 'C':
			move_para_cima(i.distancia, p);
			break;
		case 'B':
			move_para_baixo(i.distancia, p);
			break;
		default:
			break;
		}
		instrucoes->pop_back();
	}
	print_predio(p);
	//fmt::println("\nARRAY {}", fmt::join(janelas_array, janelas_array + array_size, ""));
	destroi_predio(p);
	return true;

}
