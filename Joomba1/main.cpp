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
	std::vector<posicao> posicoes;
	int32_t ANDARES, JANELAS, COMANDOS;
	// fmt::println("==>{}", argv[1]);
	/**
	 * Lê a entrada do arquivo texto
	 * e salva no vetor posições
	 */
	if (parse_entrada_joomba2(argv[1], &posicoes, &ANDARES, &JANELAS, &COMANDOS)) {
		predio* p = constroi_predio(ANDARES, JANELAS);
		if (p == NULL) return 1;
		// transforma as janelas de sujas em limpas
		/**
		 * Como no Joomba1 ele construía o prédio com todas
		 * as janelas sujas (#) de início, vamos ter que transformar
		 * todas as janelas em limpas (.)
		 */
		int32_t array_size = ANDARES * JANELAS * 4;
		memset(p->janelas_array, '.', array_size);
		int32_t quantidade_janelas_sujas = 0;

		/**
		 * Vamos pegar as entradas do arquivo e transformar
		 * em índice do nosso array uni-dimensional e marcar
		 * essas janelas como sujas.
		 * Vamos aproveitar para contar o número de
		 * janelas sujas (quantidade_janelas_sujas)
		 */
		for (auto& pos : posicoes) {
			int32_t index = convert_posicao_to_index(pos.face, pos.andar, pos.janela, p);
			if (index > 0) {
				p->janelas_array[index] = '#';
				quantidade_janelas_sujas++;
			}
		}
		// print_predio(p);
		if(quantidade_janelas_sujas == 0) return 0;
		int32_t* janelas_sujas = (int32_t*)malloc(sizeof(int32_t) * quantidade_janelas_sujas);
		int32_t counter = 0;

		/**
		 * Criando um array auxiliar de janelas sujas que será usado 
		 * para calcular todas as permutações de índices de janelas sujas
		 * Vamos rodar recursivamente todas as permutações de janelas sujas até encontrar
		 * uma solução ou até esgotar todas as possibilidades
		 */
		for (int32_t i = 0; i < array_size; ++i)
			if (p->janelas_array[i] == '#')
				if (janelas_sujas && counter < quantidade_janelas_sujas) janelas_sujas[counter++] = i;


		int32_t* inicio_ponteiro_com_todas_as_permutacoes =
		  prepare_all_permutation(janelas_sujas, quantidade_janelas_sujas);

		/**
		 * indice_janela_corrente é um ponteiro que vai ser usado para guardar a posição
		 * em que estamos no array que contém todas as permutações
		 */
		int32_t indice_janela_corrente = 0;

		/**
		 * instrucao_node serve para guardar os dados
		 * dos comandos dados ao robô e ver se já chegamos no limite e 
		 * também para poder printar o caminho que fizemos
		 * para fins de debugar
		 */
		instrucao_node root{ 'X', 0, 0, 0, nullptr };
		if (checar_limpeza(&root, COMANDOS, inicio_ponteiro_com_todas_as_permutacoes, &indice_janela_corrente, p)) {
			fmt::println("Comandos encontrados");
		} else {
			fmt::println("Comandos não encontrados");
		}
		destroi_predio(p);
		destroy_all_permutations();
		free(janelas_sujas);
		return 0;
	}
	return 1;
#endif
}
