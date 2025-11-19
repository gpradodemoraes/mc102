// Joomba1.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>

struct instrucao {
	char direcao;
	int32_t distancia;
};

bool parse_entrada_joomba1(char* filepath, std::vector<instrucao>* instrucoes, int32_t *ANDARES, int32_t *JANELAS);
bool processa_entrada_joomba_1(std::vector<instrucao>* instrucoes, const int32_t ANDARES, const int32_t JANELAS);


// TODO: Reference additional headers your program requires here.
