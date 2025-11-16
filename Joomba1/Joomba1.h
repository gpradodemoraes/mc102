// Joomba1.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <vector>

struct instrucao {
	int8_t direcao;
	int8_t distancia;
};

bool processa_entrada_joomba_1(std::vector<instrucao>* instrucoes, const int8_t ANDARES, const int8_t JANELAS);


// TODO: Reference additional headers your program requires here.
