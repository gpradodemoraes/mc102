#include "Joomba1.h"
#include <memory>
#include <fmt/core.h>
#include <fmt/ranges.h>

static uint8_t total_andares;
static uint8_t total_janelas;
static int32_t current_position = 0;
static std::unique_ptr<char[]> janelas_array;

static void print_predio() {
	for (int8_t a = total_andares - 1; a >= 0;--a) {
		int32_t posicao = 4 * total_janelas * a;
		for (int8_t j = 0; j < total_janelas; ++j)
			fmt::print("{} ", janelas_array[static_cast<int32_t>(posicao) + j]);
		fmt::println("");
	}
	for (int8_t j = 0; j < total_janelas; ++j)
		fmt::print("- ");

	fmt::println("");


}
static void move_direita(uint8_t posicoes) {
	janelas_array[current_position] = '.';
	for (int32_t i = 0; i < posicoes; ++i) {
		if (current_position + 1 % (total_janelas * 4) == 0) {
			current_position -= total_janelas * 4;
		}
		janelas_array[current_position] = '.';
		current_position++;
	}
}

bool processa_entrada_joomba_1(std::vector<instrucao>* instrucoes,
	const uint8_t ANDARES,
	const uint8_t JANELAS) {

	total_andares = ANDARES;
	total_janelas = JANELAS;
	int32_t array_size = static_cast<int32_t>(ANDARES)
		 * static_cast<int32_t>(JANELAS) * 4;

	janelas_array = std::make_unique<char[]>(array_size);
	//uint8_t* janelas_array = new uint8_t[array_size];
	memset(janelas_array.get(), '#', array_size);
	// B 1
	// C 1
	// D 2
	janelas_array[0] = 'R';
	move_direita(5);
	print_predio();
	fmt::println("\nARRAY {}", fmt::join(janelas_array.get(), janelas_array.get() + array_size, " "));
	return true;

}
