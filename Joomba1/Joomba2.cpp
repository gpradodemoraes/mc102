#include "Joomba2.h"
#include <conio.h>
#include <fstream>
#include <sstream>
#include <fmt/core.h>

static char* predio_auxiliar;

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
			if (node->direcao != last->direcao) counter++;
			last = node;
			node = node->parent;
		}
		return counter;
	}
	return 0;
}

static int32_t procura_janela_mais_proxima(int32_t janela_corrente, predio* p) {
	double menor_distancia = std::numeric_limits<double>::max();
	int32_t retval = -1;
	if (p->janelas_array) {
		for (int32_t i = 0; i < 4 * p->ANDARES * p->JANELAS; ++i) {
			if (p->janelas_array[i] == '#') {
				// achamos uma janela suja vamos calcular sua distancia
				int32_t vertical = calcula_passos_verticais(janela_corrente, i, p);
				int32_t horizontal = calcula_passos_horizontais(janela_corrente, i, p);
				double distancia = sqrt(pow(vertical, 2) + pow(horizontal, 2));
				if (distancia < menor_distancia) {
					menor_distancia = distancia;
					retval = i;
				}
			}
		}
	}
	return retval;
}

void inicia_predio_auxiliar(predio* p) {
	predio_auxiliar = (char*)malloc(static_cast<size_t>(p->ANDARES) * p->JANELAS * 4);
	if (predio_auxiliar) memset(predio_auxiliar, 0, static_cast<size_t>(p->ANDARES) * p->JANELAS * 4);
}

void destroi_predio_auxiliar() { free(predio_auxiliar); }

instrucao_node* create_instrucao_node(char direcao, int32_t distancia, int32_t profundidade, int32_t comandos,
									  instrucao_node* parent) {
	instrucao_node* inst = (instrucao_node*)malloc(sizeof(instrucao_node));
	if (inst) {
		inst->direcao = direcao;
		inst->distancia = distancia;
		inst->profundidade = profundidade;
		inst->comandos = comandos;
		inst->parent = parent;
		return inst;
	}
	return nullptr;
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

static char alterar_as_direcoes_para_evitar_lock() {
	static int32_t index = 0;
	static char direcoes[] = { 'B', 'D', 'C', 'E' };
	return direcoes[index++ % 4];
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

bool checar_limpeza(instrucao_node* list_instrucoes, int32_t max_instrucoes_permitido, int32_t* permutacoes,
					int32_t* janela_suja_indice, predio* p) {
	if (!p) return false;

	int32_t janela = permutacoes[*janela_suja_indice];

	if (janela == -2) {
		fmt::println("Esgotamos todas as possibilidades!");
		return false;
	}
	char current_janela_state = p->janelas_array[p->current_position];
	int32_t current_position = p->current_position;
	p->janelas_array[p->current_position] = '.'; // marquei a posição corrente como limpa

	bool comandos_ok = list_instrucoes->comandos <= max_instrucoes_permitido;
	bool predio_esta_limpo = is_predio_limpo(p);
	// fmt::println("total instrucoes: {}; máximo permitido: {}", list_instrucoes->comandos, max_instrucoes_permitido);
	// print_caminho(list_instrucoes, p);
	if (predio_esta_limpo && comandos_ok) {
		// missão cumprida!
		// fmt::println("total instrucoes: {}; máximo permitido: {}", list_instrucoes->comandos,
		// max_instrucoes_permitido); print_caminho(list_instrucoes, p);
		return true;
	}

	if (!comandos_ok) {
		// esse não deu. Vamos adiantar o ponteiro até o início da
		// próxima permutacao
		// while (permutacoes[*janela_suja_indice] != -1) (*janela_suja_indice)++;
		//
		//(*janela_suja_indice)++;
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
		return false;
	}

	while (janela != -1 && p->janelas_array[janela] != '#') {
		// janela já está limpa. Vamos adiantar o ponteiro
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
		if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
	} else if (passos_horizontais < 0) {
		move_esquerda(-1 * passos_horizontais, p, false);
		instrucao_node nova{ 'E', -1 * passos_horizontais, list_instrucoes->profundidade + 1,
							 list_instrucoes->direcao == 'E' ? list_instrucoes->comandos
															 : list_instrucoes->comandos + 1,
							 list_instrucoes };

		if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
		p->current_position = current_position;
		p->janelas_array[p->current_position] = current_janela_state;
	}


	int32_t passos_verticais = calcula_passos_verticais(current_position, janela, p);

	if (passos_verticais > 0) {
		if (pode_ir_para_cima(p)) {
			move_para_cima(passos_verticais, p, false);
			instrucao_node nova{ 'C', passos_verticais, list_instrucoes->profundidade + 1,
								 list_instrucoes->direcao == 'C' ? list_instrucoes->comandos
																 : list_instrucoes->comandos + 1,
								 list_instrucoes };
			if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
			// quando chegar aqui significa que a recursão acima voltou falso.
			// Ela adiantou o ponteiro janela_suja_indice porém quando chega aqui, o janela_suja_indice volta a
			// ser o que era antes!
			p->current_position = current_position;
			p->janelas_array[p->current_position] = current_janela_state;
		}
	} else if (passos_verticais < 0) {
		if (pode_ir_para_baixo(p)) {
			move_para_baixo(-1 * passos_verticais, p, false);
			instrucao_node nova{ 'B', -1 * passos_verticais, list_instrucoes->profundidade + 1,
								 list_instrucoes->direcao == 'B' ? list_instrucoes->comandos
																 : list_instrucoes->comandos + 1,
								 list_instrucoes };
			if (checar_limpeza(&nova, max_instrucoes_permitido, permutacoes, janela_suja_indice, p)) return true;
			p->current_position = current_position;
			p->janelas_array[p->current_position] = current_janela_state;
		}
	}

	// e retornar falso, pois se todas as direçoes falharam então não tem solução
	// esse não deu. Vamos adiantar o ponteiro até o início da
	// próxima permutacao
	while (permutacoes[*janela_suja_indice] != -1) (*janela_suja_indice)++;

	(*janela_suja_indice)++;
	return false;
}

static void free_instrucoes_nodes(instrucao_node* latest_node) {
	instrucao_node* node = latest_node;
	instrucao_node* delete_node;
	while (node) {
		delete_node = node;
		node = node->parent;
		free(delete_node);
	}
}

bool procura_comandos_sem_recorrencia(int32_t* array_all_permutations, predio* p, const int32_t maximo_comandos) {
	char* backup = (char*)malloc(sizeof(char) * 4 * p->ANDARES * p->JANELAS);
	memcpy(backup, p->janelas_array, sizeof(char) * 4 * p->ANDARES * p->JANELAS);
	int32_t counter = 0;
	while (true) {
		if (array_all_permutations[counter] == -2) {
			// fmt::println("Comandos não encontrados");
			free(backup);
			return false;
		}
		memcpy(p->janelas_array, backup, sizeof(char) * 4 * p->ANDARES * p->JANELAS);
		// int32_t current_position = 0;
		p->current_position = 0;
		p->janelas_array[p->current_position] = 'R';
		instrucao_node* latest_node = create_instrucao_node('X', 0, 0, 0, nullptr);
		while (true) {
			int32_t janela = array_all_permutations[counter];
			if (janela == -1) {
				free_instrucoes_nodes(latest_node);
				counter++;
				break;
			}

			if (p->janelas_array[janela] == '.') {
				// se a janela corrente já está limpa, então pular para a próxima
				counter++;
				continue;
			}

			int32_t passos_horizontais = calcula_passos_horizontais(p->current_position, janela, p);

			if (passos_horizontais > 0) {
				move_direita(passos_horizontais, p, true);
				latest_node = create_instrucao_node(
				  'D', passos_horizontais, latest_node->profundidade + 1,
				  latest_node->direcao == 'D' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
			} else if (passos_horizontais < 0) {
				move_esquerda(-1 * passos_horizontais, p, true);
				latest_node = create_instrucao_node(
				  'E', -1 * passos_horizontais, latest_node->profundidade + 1,
				  latest_node->direcao == 'E' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
			}
			// print_predio(p);
			// fmt::println("\n=======================");
			p->janelas_array[p->current_position] = '.';
			int32_t passos_verticais = calcula_passos_verticais(p->current_position, janela, p);

			if (passos_verticais > 0) {
				if (pode_ir_para_cima(p)) {
					move_para_cima(passos_verticais, p, true);
					latest_node = create_instrucao_node(
					  'C', passos_verticais, latest_node->profundidade + 1,
					  latest_node->direcao == 'C' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
				}
			} else if (passos_verticais < 0) {
				if (pode_ir_para_baixo(p)) {
					move_para_baixo(-1 * passos_verticais, p, true);
					latest_node = create_instrucao_node(
					  'B', -1 * passos_verticais, latest_node->profundidade + 1,
					  latest_node->direcao == 'B' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
				}
			}
			// print_predio(p);
			// fmt::println("\n=======================");
			p->janelas_array[p->current_position] = '.';
			int32_t total_instrucoes = conta_instrucoes(latest_node);
			bool comandos_ok = total_instrucoes <= maximo_comandos;
			if (is_predio_limpo(p) && comandos_ok) {
				// print_predio(p);
				// print_caminho(latest_node, p);
				// fmt::println("Comandos encontrados");
				free_instrucoes_nodes(latest_node);
				free(backup);
				return true;
			}
			if (!comandos_ok) {
				// fmt::println("TOTAL INSTRUÇOES: {}", total_instrucoes);
				// print_caminho(latest_node, p);
				free_instrucoes_nodes(latest_node);
				while (array_all_permutations[counter] != -1) counter++;
				counter++;
				break;
			}
			counter++;
		}

		// if (list_instrucoes->comandos > max_instrucoes_permitido) {
		//  while (array_all_permutations[counter] != -1) counter++;
		//  counter++;
	}
}
