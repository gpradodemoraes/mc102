#include "Joomba2.h"

#include <fstream>
#include <sstream>
#include <fmt/core.h>

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

static void print_lista_instrucoes(instrucao_node* i) {
	instrucao_node* n = i;
	while (n != nullptr) {
		fmt::println("direcao {}; distancia {}", n->direcao, n->direcao);
		n = n->parent;
	}
}

int32_t conta_instrucoes(instrucao_node* i) {
	int32_t counter = 0;
	if (i) {
		if (i->direcao) counter = 1;
		instrucao_node* last = i;
		instrucao_node* node = i;
		while (node->parent != nullptr) {
			if(node->direcao != last->direcao) counter++;
			last = node;
			node = node->parent;
		}
		return counter;
	}
	return 0;
}

bool checar_limpeza(instrucao_node* list_instrucoes, int32_t max_instrucoes_permitido, predio* p) {
	// Verificar se o prédio está limpo
	if (!p) return false;
	bool predio_esta_limpo = is_predio_limpo(p);

	if (conta_instrucoes(list_instrucoes) > max_instrucoes_permitido) return false;
	if (predio_esta_limpo) {
		print_lista_instrucoes(list_instrucoes);
		return true;
	}
	// salva o estado atual da janela
	char current_janela_state = p->janelas_array[p->current_position];

	p->janelas_array[p->current_position] = '.'; // marquei a posição corrente como limpa

	if (pode_ir_para_baixo(p)) {
		instrucao_node nova{'B', 1, list_instrucoes};
		checar_limpeza(&nova, max_instrucoes_permitido, p);
	}
	
	return false;
}
