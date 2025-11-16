#include "Joomba1.h"
#include <memory>
#include <fmt/core.h>
#include <fmt/ranges.h>

static int32_t total_andares;
static int32_t total_janelas;
static int32_t current_position = 0;
static char* janelas_array = nullptr;

static void print_predio() {
	const char* faces[] = {"NORTE", "OESTE", "SUL", "LESTE"};
	for (int32_t f = 0; f < 4; ++f) {
		fmt::println("Face {}", faces[f][0]);
		for (int32_t a = total_andares - 1; a >= 0;--a) {
			int32_t posicao = 4 * total_janelas * a + f * total_janelas;
			for (int32_t j = 0; j < total_janelas; ++j)
				fmt::print("{}", janelas_array[posicao + j]);
			fmt::println("");
		}
		for (int32_t j = 0; j < total_janelas; ++j)
			fmt::print("-");

		fmt::println("");
	}
}
static void move_direita(int32_t posicoes) {
	for (int32_t i = 0; i < posicoes; ++i) {
		janelas_array[current_position] = '.';
		if ((current_position + 1) % (total_janelas * 4) == 0) {
			current_position -= (total_janelas * 4 - 1);
		} else current_position++;
		janelas_array[current_position] = 'R';
	}
}
static void move_esquerda(int32_t posicoes) {
	for (int32_t i = 0; i < posicoes; ++i) {
		janelas_array[current_position] = '.';
		if (current_position % (total_janelas * 4) == 0) {
			current_position += (total_janelas * 4 - 1);
		} else current_position--;
		janelas_array[current_position] = 'R';
	}
}
static void move_para_cima(int32_t posicoes) {
	for (int32_t i = 0; i < posicoes; ++i)
		if (current_position < 4 * total_janelas * (total_andares - 1)) {
			janelas_array[current_position] = '.';
			current_position += 4 * total_janelas;
			janelas_array[current_position] = 'R';
		}
}
static void move_para_baixo(int32_t posicoes) {
	for (int32_t i = 0; i < posicoes; ++i)
		if (current_position > 4 * total_janelas - 1) {
			janelas_array[current_position] = '.';
			current_position -= 4 * total_janelas;
			janelas_array[current_position] = 'R';
		}
}


bool processa_entrada_joomba_1(std::vector<instrucao>* instrucoes,
	const int32_t ANDARES,
	const int32_t JANELAS) {

	total_andares = ANDARES;
	total_janelas = JANELAS;
	int32_t array_size = ANDARES * JANELAS * 4;

	janelas_array = new char[array_size];
	//int32_t* janelas_array = new int32_t[array_size];
	memset(janelas_array, '#', array_size);
	// B 1
	// C 1
	// D 2
	current_position = 0;
	janelas_array[current_position] = 'R';

	while (!instrucoes->empty())  {
		instrucao i = instrucoes->back();
		switch (i.direcao) {
		case 'D':
			move_direita(i.distancia);
			break;
		case 'E':
			move_esquerda(i.distancia);
			break;
		case 'C':
			move_para_cima(i.distancia);
			break;
		case 'B':
			move_para_baixo(i.distancia);
			break;
		default:
			break;
		}
		instrucoes->pop_back();
	}
	print_predio();
	//fmt::println("\nARRAY {}", fmt::join(janelas_array, janelas_array + array_size, ""));
	delete[] janelas_array;
	janelas_array = nullptr;
	return true;

}
