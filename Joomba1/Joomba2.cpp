#include "Joomba2.h"
#include <conio.h>
#include <fstream>
#include <sstream>
#include <fmt/core.h>

bool parse_entrada_joomba2(char* filepath, std::vector<posicao>* posicoes, int32_t* ANDARES_, int32_t* JANELAS_,
						   int32_t* COMANDOS_) {
	std::ifstream file(filepath);
	if (file.good() && file.is_open()) {
		int32_t JANELAS = -1;
		int32_t ANDARES = -1;
		int32_t COMANDOS = -1;
		std::string line;
		while (getline(file, line)) {
			// fmt::println("|=>{}", line);
			std::stringstream ss(line);
			std::string word;
			posicao i{ 'X', -1, -1 };
			while (ss >> word) {
				if (word.at(0) == 'F') break;
				if (ANDARES == -1) {
					ANDARES = stoi(word);
					continue;
				}
				if (JANELAS == -1) {
					JANELAS = stoi(word);
					continue;
				}
				if (COMANDOS == -1) {
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
		fmt::println("direcao {}; distancia {}; comandos {}", n->direcao, n->distancia, n->comandos);
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
			if (node->direcao != last->direcao) counter++;
			last = node;
			node = node->parent;
		}
		return counter;
	}
	return 0;
}

/********
COMBINAR O PRINT_PREDIO COM AS INSTRUCOES
PARA DESENHAR O CAMINHO QUE ELE SUPOSTAMENTE TENTOU FAZER
*********/
static void print_caminho(instrucao_node* n, predio* p) {
	if (!p) return;

	predio* q = constroi_predio(p->ANDARES, p->JANELAS);
	instrucao_node* node = n;
	std::vector<instrucao_node*> instrucoes;
	std::vector<int32_t> posicoes;
	while (node) {
		instrucoes.push_back(node);
		node = node->parent;
	}
	// memset(q->janelas_array, '.', q->ANDARES * q->JANELAS * 4);
	memcpy(q->janelas_array, p->janelas_array, static_cast<size_t>(q->ANDARES) * q->JANELAS * 4);
	posicoes.push_back(q->current_position);
	while (!instrucoes.empty()) {
		node = instrucoes.back();
		fmt::println("DIRECAO: {}, DISTANCIA: {}, PROFUNDIDADE: {}, COMANDOS: {}", node->direcao, node->distancia,
					 node->profundidade, node->comandos);
		switch (node->direcao) {
			case 'E': move_esquerda(node->distancia, q); break;
			case 'D': move_direita(node->distancia, q); break;
			case 'C': move_para_cima(node->distancia, q); break;
			case 'B': move_para_baixo(node->distancia, q); break;
			default: break;
		}
		posicoes.push_back(q->current_position);
		instrucoes.pop_back();
	}
	for (int32_t i : posicoes) q->janelas_array[i] = 'X';
	q->janelas_array[posicoes.back()] = 'R';
	fmt::println("Total de comandos: {}", conta_instrucoes(n));
	print_predio(q);
	destroi_predio(q);
}

bool checar_limpeza(instrucao_node* list_instrucoes, int32_t max_instrucoes_permitido, int32_t* permutacoes,
					int32_t* janela_suja_indice, predio* p) {
	if (!p) return false;

	/** pegar a próxima janela suja dentro da nossa lista de permutaçoes atual */
	int32_t janela = permutacoes[*janela_suja_indice];

	/**
	 * colocamos um marcador janela = -2 no final do array que contém todas as permutações
	 * se chegarmos no valor -2 significa que não existe mais nenhuma permutação a tentar
	 */
	if (janela == -2) {
		// fmt::println("Esgotamos todas as possibilidades!");
		return false;
	}

	/**
	 * vamos salvar o estado corrente do prédio e da janela atual
	 * para que a gente possa retornar nele caso estejamos indo pelo caminho errado
	 * assim podemos voltar a subir na pilha testando novos caminhos	 *
	 */
	char current_janela_state = p->janelas_array[p->current_position];
	int32_t current_position = p->current_position;
	p->janelas_array[p->current_position] = '.'; // marquei a posição corrente como limpa

	/**
	 * Agora verificamos se ainda estamos dentro do valor máximo de comandos permitidos
	 * e se o prédio já está totalmente limpo
	 */
	bool comandos_ok = list_instrucoes->comandos <= max_instrucoes_permitido;
	bool predio_esta_limpo = is_predio_limpo(p);

	// fmt::println("total instrucoes: {}; máximo permitido: {}", list_instrucoes->comandos, max_instrucoes_permitido);
	// print_caminho(list_instrucoes, p);
	if (predio_esta_limpo && comandos_ok) {
		/**
		 * missão cumprida! O prédio está totalmente limpo
		 * e fizemos isso dentro do número máximo de comandos
		 * permitidos. Agora é só retornar true que o true vai se propagar
		 * na pilha até retornar na primeira chamada dentro da função main
		 */
		// fmt::println("total instrucoes: {}; máximo permitido: {}", list_instrucoes->comandos,
		// max_instrucoes_permitido); print_caminho(list_instrucoes, p);
		return true;
	}

	if (!comandos_ok) {
		/**
		 * esse não deu. Vamos restaurar o valor da janela dentro do prédio
		 * e retornar false. Vamos voltar um estágio na pilha e continuar de
		 * onde parou
		 */
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
		return false;
	}

	while (janela != -1 && p->janelas_array[janela] != '#') {
		/**
		 * A janela correnta já estava limpa.
		 * Provavelmente o robozinho passou por cima dela
		 * enquanto se deslocava para outra janela.
		 * Vamos avançar dentro dessa permutação corrente até
		 * uma janela suja ou chegar no -1 (fim da permutação)
		 */
		(*janela_suja_indice)++;
		janela = permutacoes[*janela_suja_indice];
	}

	int32_t passos_horizontais = calcula_passos_horizontais(current_position, janela, p);

	if (passos_horizontais > 0) {
		move_direita(passos_horizontais, p, false);
		instrucao_node nova{ 'D', passos_horizontais, list_instrucoes->profundidade + 1,
							 list_instrucoes->direcao == 'D' ? list_instrucoes->comandos
															 : list_instrucoes->comandos + 1,
							 list_instrucoes };
		int32_t salva_indice = *janela_suja_indice;
		if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
		*janela_suja_indice = salva_indice;
	} else if (passos_horizontais < 0) {
		move_esquerda(-1 * passos_horizontais, p, false);
		instrucao_node nova{ 'E', -1 * passos_horizontais, list_instrucoes->profundidade + 1,
							 list_instrucoes->direcao == 'E' ? list_instrucoes->comandos
															 : list_instrucoes->comandos + 1,
							 list_instrucoes };
		int32_t salva_indice = *janela_suja_indice;
		if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
		*janela_suja_indice = salva_indice;
	}


	int32_t passos_verticais = calcula_passos_verticais(current_position, janela, p);

	if (passos_verticais > 0) {
		if (pode_ir_para_cima(p)) {
			move_para_cima(passos_verticais, p, false);
			instrucao_node nova{ 'C', passos_verticais, list_instrucoes->profundidade + 1,
								 list_instrucoes->direcao == 'C' ? list_instrucoes->comandos
																 : list_instrucoes->comandos + 1,
								 list_instrucoes };
			int32_t salva_indice = *janela_suja_indice;
			if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
			p->current_position = current_position;
			p->janelas_array[p->current_position] = current_janela_state;
			*janela_suja_indice = salva_indice;
		}
	} else if (passos_verticais < 0) {
		if (pode_ir_para_baixo(p)) {
			move_para_baixo(-1 * passos_verticais, p, false);
			instrucao_node nova{ 'B', -1 * passos_verticais, list_instrucoes->profundidade + 1,
								 list_instrucoes->direcao == 'B' ? list_instrucoes->comandos
																 : list_instrucoes->comandos + 1,
								 list_instrucoes };
			int32_t salva_indice = *janela_suja_indice;
			if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
			p->current_position = current_position;
			p->janelas_array[p->current_position] = current_janela_state;
			*janela_suja_indice = salva_indice;
		}
	}

	if (list_instrucoes->direcao == 'X') {
		/**
		 * Voltamos ao stack mais baixo dessa permutação.
		 * Significa que essa permutação não resultou num resultado válido
		 * Então vamos dar início a uma nova permutação
		 *
		 * Primeiramente a gente adianta o ponteiro até a próxima permutação
		 */
		while (permutacoes[*janela_suja_indice] != -1) (*janela_suja_indice)++;
		(*janela_suja_indice)++;

		/**
		 * Agora vamos chamar a próxima permutação
		 */
		return checar_limpeza(list_instrucoes, max_instrucoes_permitido, permutacoes, janela_suja_indice, p);
	}

	return false;
}
