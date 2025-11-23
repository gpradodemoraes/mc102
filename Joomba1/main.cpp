// --- By Gabriel ---

#if defined JOOMBA_1
#include "Joomba1.h"
#elif defined JOOMBA_2
#include "Joomba2.h"
#include "permutations.h"
#endif

#include <fmt/core.h>

int main(const int argc, char** argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/tests/test_file.txt", argv[0]);
		return 1;
	}
#if defined JOOMBA_1
	std::vector<instrucao> instrucoes;
	int32_t ANDARES, JANELAS;
	if (!parse_entrada_joomba1(argv[1], &instrucoes, &ANDARES, &JANELAS)) return 1;
	bool retval = processa_entrada_joomba_1(&instrucoes, ANDARES, JANELAS);
	return retval ? 0 : 1;
#elif defined JOOMBA_2

	// int32_t arraymodel[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	// size_t originalsize = sizeof(arraymodel) / sizeof(int32_t);
	// prepare_all_permutation(arraymodel, originalsize);
	// for (int32_t i = 0; i < 120; ++i) {
	// 	int32_t* qwe = get_next_permutation();
	// 	for (int32_t j = 0; j < originalsize; ++j)
	// 		fmt::print("{} ", qwe[j]);
	// 	fmt::println("\n-------------");
	// }
	// destroy_all_permutations();
	// 
	// return 0;

	std::vector<posicao> posicoes;
	int32_t ANDARES, JANELAS, COMANDOS;
	fmt::println("==>{}", argv[1]);
	if (parse_entrada_joomba2(argv[1], &posicoes, &ANDARES, &JANELAS, &COMANDOS)) {
		predio* p = constroi_predio(ANDARES, JANELAS);
		if (p == NULL) return 1;
		// transforma as janelas de sujas em limpas
		int32_t array_size = ANDARES * JANELAS * 4;
		memset(p->janelas_array, '.', array_size);
		int32_t quantidade_janelas_sujas = 0;
		for (auto& pos : posicoes) {
			int32_t index = convert_posicao_to_index(pos.face, pos.andar, pos.janela, p);
			if (index > 0) {
				p->janelas_array[index] = '#';
				quantidade_janelas_sujas++;
			}
		}
		print_predio(p);
		inicia_predio_auxiliar(p);
		int32_t* janelas_sujas = (int32_t*)malloc(sizeof(int32_t) * quantidade_janelas_sujas);
		int32_t counter = 0;
		if (janelas_sujas)
			for (int32_t i =0; i< array_size; ++i) 
				if (p->janelas_array[i] == '#')
					janelas_sujas[counter++] = i;

		int32_t* backup = (int32_t*) malloc(sizeof(int32_t) * 4 * p->ANDARES * p->JANELAS);
		memcpy(backup, p->janelas_array, sizeof(int32_t) * 4 * p->ANDARES * p->JANELAS);
		int32_t* inicio_ponteiro_com_todas_as_permutacoes = prepare_all_permutation(janelas_sujas, quantidade_janelas_sujas);

		counter = 0;
		while (true) {
			if (inicio_ponteiro_com_todas_as_permutacoes[counter] == -2) {
				fmt::println("Comandos não encontrados");
				break;
			}
			memcpy(p->janelas_array, backup, sizeof(int32_t) * 4 * p->ANDARES * p->JANELAS);
			int32_t current_position = 0;
			p->janelas_array[p->current_position] = 'R';
			instrucao_node *latest_node = create_instrucao_node('X', 0, 0, 0, nullptr);
			while (true) {
				int32_t janela = inicio_ponteiro_com_todas_as_permutacoes[counter];
				if (janela == -1) break;
				int32_t passos_horizontais = calcula_passos_horizontais(p->current_position, janela, p);

				if (passos_horizontais > 0) {
					move_direita(passos_horizontais, p, true);
					latest_node = create_instrucao_node('D', passos_horizontais, latest_node->profundidade + 1,
						latest_node->direcao == 'D' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
				} else if (passos_horizontais < 0) {
					move_esquerda(-1 * passos_horizontais, p, true);
					latest_node = create_instrucao_node('E', -1 * passos_horizontais, latest_node->profundidade + 1,
					  latest_node->direcao == 'E' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
				}
				print_predio(p);
				fmt::println("\n=======================");
				p->janelas_array[p->current_position] = '.';
				int32_t passos_verticais = calcula_passos_verticais(p->current_position, janela, p);

				if (passos_verticais > 0) {
					if (pode_ir_para_cima(p)) {
						move_para_cima(passos_verticais, p, true);
						latest_node = create_instrucao_node('C', passos_verticais, latest_node->profundidade + 1,
						  latest_node->direcao == 'C' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
					}
				} else if (passos_verticais < 0) {
					if (pode_ir_para_baixo(p)) {
						move_para_baixo(-1 * passos_verticais, p, true);
						latest_node = create_instrucao_node('B', -1 * passos_verticais, latest_node->profundidade + 1,
						  latest_node->direcao == 'B' ? latest_node->comandos : latest_node->comandos + 1, latest_node);
					}
				}
				print_predio(p);
				fmt::println("\n=======================");
				p->janelas_array[p->current_position] = '.';

				if (is_predio_limpo(p) && conta_instrucoes(latest_node) <= COMANDOS) {
					print_predio(p);
					fmt::println("Comandos encontrados");
					break;
					AQUI PRECISA DAR UM BREAK MASTER
				}
				counter++;
			}

			//if (list_instrucoes->comandos > max_instrucoes_permitido) {
			while (inicio_ponteiro_com_todas_as_permutacoes[counter] != -1) counter++;
			counter++;
		}

		//instrucao_node root{'X', 0, 0, 0, nullptr};
		//if (checar_limpeza(&root, COMANDOS, inicio_ponteiro_com_todas_as_permutacoes, p)) {
		//	fmt::println("Comandos encontrados");
		//} else {
		//	fmt::println("Comandos não encontrados");
		//}
		destroi_predio(p);
		destroi_predio_auxiliar();
		destroy_all_permutations();
		free(janelas_sujas);
		return 0;
	}
	return 1;
#endif
}