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
	// fmt::println("==>{}", argv[1]);
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
		// print_predio(p);
		inicia_predio_auxiliar(p);
		int32_t* janelas_sujas = (int32_t*)malloc(sizeof(int32_t) * quantidade_janelas_sujas);
		int32_t counter = 0;

		for (int32_t i = 0; i < array_size; ++i)
			if (p->janelas_array[i] == '#')
				if (janelas_sujas && counter < quantidade_janelas_sujas) janelas_sujas[counter++] = i;


		int32_t* inicio_ponteiro_com_todas_as_permutacoes =
		  prepare_all_permutation(janelas_sujas, quantidade_janelas_sujas);

		int32_t indice_janela_corrente = 0;
		instrucao_node root{ 'X', 0, 0, 0, nullptr };
		if (checar_limpeza(&root, COMANDOS, inicio_ponteiro_com_todas_as_permutacoes, &indice_janela_corrente, p)) {
			fmt::println("Comandos encontrados");
		} else {
			fmt::println("Comandos não encontrados");
		}
		destroi_predio(p);
		destroi_predio_auxiliar();
		destroy_all_permutations();
		free(janelas_sujas);
		return 0;
	}
	return 1;
#endif
}
