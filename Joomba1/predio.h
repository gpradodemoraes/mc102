#pragma once
#include <cstdint>

struct predio {
	int32_t ANDARES;
	int32_t JANELAS;
	int32_t current_position;
	char* janelas_array;
};

void print_predio(predio *p);

void move_direita(int32_t posicoes, predio* p);
void move_esquerda(int32_t posicoes, predio* p);
void move_para_cima(int32_t posicoes, predio* p);
void move_para_baixo(int32_t posicoes, predio* p);
predio* constroi_predio(int32_t ANDARES, int32_t JANELAS);
void destroi_predio(predio* p);