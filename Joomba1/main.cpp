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
	parse_entrada_joomba1(argv[1], &instrucoes, &ANDARES, &JANELAS);
	bool retval = processa_entrada_joomba_1(&instrucoes, ANDARES, JANELAS);
	return retval ? 0 : 1;
#elif defined JOOMBA_2
	print_hello_Joomba2();
	return 0;
#endif
}