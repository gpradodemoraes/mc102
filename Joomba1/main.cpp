// --- By Gabriel ---

#if defined JOOMBA_1
#include "Joomba1.h"
#elif defined JOOMBA_2
#include "Joomba2.h"
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
	fmt::println("==>{}", argv[1]);
	if (parse_entrada_joomba2(argv[1], &posicoes, &ANDARES, &JANELAS, &COMANDOS)) {
		predio* p = constroi_predio(ANDARES, JANELAS);
		if (p == NULL) return 1;
		// transforma as janelas de sujas em limpas
		int32_t array_size = ANDARES * JANELAS * 4;
		memset(p->janelas_array, '.', array_size);
		for (auto pos : posicoes) {
			int32_t index = convert_posicao_to_index(pos.face, pos.andar, pos.janela, p);
			if (index > 0)
				p->janelas_array[index] = '#';
		}
		print_predio(p);
		inicia_predio_auxiliar(p);
		instrucao_node root{'X', 0, 0, 0, nullptr};
		if (checar_limpeza(&root, COMANDOS, p)) {
			fmt::println("Comandos encontrados");
		} else {
			fmt::println("Comandos não encontrados");
		}
		destroi_predio(p);
		destroi_predio_auxiliar();
		return 0;
	}
	return 1;
#endif
}