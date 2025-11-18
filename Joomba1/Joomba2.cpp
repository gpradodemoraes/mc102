#include "Joomba2.h"

#include <fstream>
#include <sstream>
#include <fmt/core.h>

struct instrucao_node {
    char direcao;
	int32_t distancia;
    instrucao_node* parent;
};

bool parse_entrada_joomba2(char* filepath,
	std::vector<posicao>* posicoes,
	int32_t *ANDARES_,
	int32_t *JANELAS_,
	int32_t *COMANDOS_) {
	std::ifstream file(filepath);
	if (file.good() && file.is_open()) {
		int32_t JANELAS = 0;
		int32_t ANDARES = 0;
		int32_t COMANDOS = 0;
		std::string line;
		while (getline(file, line)) {
			fmt::println("|=>{}", line);
			std::stringstream ss(line);
			std::string word;
			posicao i{'X', -1, -1};
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
				if (COMANDOS == 0) {
					COMANDOS = stoi(word);
					continue;
				}
				if (i.face == 'X') {
					i.face = word.at(0);
					continue;
				}
				if (i.andar == -1) {
					i.andar = stoi(word);
					continue;
				}
				if (i.janela == -1) {
					i.janela = stoi(word);
					posicoes->insert(posicoes->begin(), i);
					continue;
				}
				fmt::println("NAO DEVERIA ESTAR AQUI!");
			}
		}
		file.close();
		*ANDARES_ = ANDARES;
		*JANELAS_ = JANELAS;
		*COMANDOS_ = COMANDOS;
		return true;
	}
	return false;
}
