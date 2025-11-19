#include "predio.h"
#include <memory>
#include <fmt/core.h>

void print_predio(predio *p) {
	const char* faces[] = {"NORTE", "OESTE", "SUL", "LESTE"};
	for (int32_t f = 0; f < 4; ++f) {
		fmt::println("Face {}", faces[f][0]);
		for (int32_t a = p->ANDARES - 1; a >= 0;--a) {
			int32_t posicao = 4 * p->JANELAS * a + f * p->JANELAS;
			for (int32_t j = 0; j < p->JANELAS; ++j)
				fmt::print("{}", p->janelas_array[posicao + j]);
			fmt::println("");
		}
		for (int32_t j = 0; j < p->JANELAS; ++j)
			fmt::print("-");

		fmt::println("");
	}
}
void move_direita(int32_t posicoes, predio* p) {
	for (int32_t i = 0; i < posicoes; ++i) {
		p->janelas_array[p->current_position] = '.';
		if ((p->current_position + 1) % (p->JANELAS * 4) == 0) {
			p->current_position -= (p->JANELAS * 4 - 1);
		} else p->current_position++;
		p->janelas_array[p->current_position] = 'R';
	}
}
void move_esquerda(int32_t posicoes, predio* p) {
	for (int32_t i = 0; i < posicoes; ++i) {
		p->janelas_array[p->current_position] = '.';
		if (p->current_position % (p->JANELAS * 4) == 0) {
			p->current_position += (p->JANELAS * 4 - 1);
		} else p->current_position--;
		p->janelas_array[p->current_position] = 'R';
	}
}
void move_para_cima(int32_t posicoes, predio* p) {
	for (int32_t i = 0; i < posicoes; ++i)
		if (pode_ir_para_cima(p)) {
			p->janelas_array[p->current_position] = '.';
			p->current_position += 4 * p->JANELAS;
			p->janelas_array[p->current_position] = 'R';
		}
}
void move_para_baixo(int32_t posicoes, predio* p) {
	for (int32_t i = 0; i < posicoes; ++i)
		if (pode_ir_para_baixo(p)) {
			p->janelas_array[p->current_position] = '.';
			p->current_position -= 4 * p->JANELAS;
			p->janelas_array[p->current_position] = 'R';
		}
}

predio* constroi_predio(const int32_t ANDARES_, const int32_t JANELAS_) {
	int32_t array_size = ANDARES_ * JANELAS_ * 4;
	predio* p = (predio*)malloc(sizeof(predio));
	if (p) {
		p->ANDARES = ANDARES_;
		p->JANELAS = JANELAS_;
		p->current_position = 0;
		p->janelas_array = (char*) malloc(sizeof(char) * array_size);
		if (p->janelas_array) {
			memset(p->janelas_array, '#', array_size);
			p->janelas_array[p->current_position] = 'R';
		}
		return p;
	}
	return NULL;

}
void destroi_predio(predio* p) {
	free(p->janelas_array);
	free(p);
}

int32_t convert_posicao_to_index(char face, int32_t andar, int32_t janela, predio* p) {
	int32_t fator_multiplicador;
	switch (face) {
		case 'N': fator_multiplicador = 0; break;
		case 'O': fator_multiplicador = 1; break;
		case 'S': fator_multiplicador = 2; break;
		case 'L': fator_multiplicador = 3; break;
		default: fator_multiplicador = -1; break;
	};
	if (fator_multiplicador == -1) return -1;
	return (fator_multiplicador * p->JANELAS + janela - 1) + (4 * p->JANELAS * andar);
}

bool is_predio_limpo(predio* p) {
	if (p->janelas_array) {
		for (int32_t i = 0;i < 4 * p->ANDARES * p->JANELAS;++i) {
			if (p->janelas_array[i] == '#') return false;
		}
		return true;
	}
	return false;
 }