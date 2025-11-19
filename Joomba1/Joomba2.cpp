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
		fmt::println("direcao {}; distancia {}", n->direcao, n->distancia);
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

static int32_t conta_lista_consecutiva_de_direcao(const char direcao, instrucao_node* lista) {
	int32_t counter = 0;
	instrucao_node* node = lista;
	while (node) {
		if (node->direcao == direcao) {
			counter++;
			node = node->parent;
			continue;
		}
		break;		
	}
	return counter;
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
	int32_t current_position = p->current_position;
	p->janelas_array[p->current_position] = '.'; // marquei a posição corrente como limpa

	if (pode_ir_para_baixo(p)) {
		move_para_baixo(1, p);
		instrucao_node nova{'B', 1, list_instrucoes};
		if (checar_limpeza(&nova, max_instrucoes_permitido, p)) return true;
		p->current_position = current_position;
	}
	if (pode_ir_para_cima(p)) {
		move_para_cima(1, p);
		instrucao_node nova{ 'C', 1, list_instrucoes };
		if (checar_limpeza(&nova, max_instrucoes_permitido, p)) return true;
		p->current_position = current_position;
	}
	// como as quatro faces do prédio formam um círculo
	// é preciso andar no máximo metade das janelas para a esquerda ou direita
	// para se chegar a qualquer lugar

	if (conta_lista_consecutiva_de_direcao('E', list_instrucoes) < (4 * p->JANELAS / 2)) {
		move_esquerda(1, p);
		instrucao_node nova{ 'E', 1, list_instrucoes };
		if (checar_limpeza(&nova, max_instrucoes_permitido, p)) return true;
		p->current_position = current_position;
	}
	if (conta_lista_consecutiva_de_direcao('D', list_instrucoes) < (4 * p->JANELAS / 2)) {
		move_direita(1, p);
		instrucao_node nova{ 'D', 1, list_instrucoes };
		if (checar_limpeza(&nova, max_instrucoes_permitido, p)) return true;
		p->current_position = current_position;
	}

	// ele já tentou todas as direções possíveis. Então restaurar o valor da janela corrente
	p->janelas_array[current_position] = current_janela_state;

	// e retornar falso, pois se todas as direçoes falharam então não tem solução
	return false;
}
