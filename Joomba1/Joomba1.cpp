#include "Joomba1.h"
#include "predio.h"

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
