#pragma once
#include <cstdint>

struct predio {
	int32_t ANDARES;
	int32_t JANELAS;
	int32_t current_position;
	char* janelas_array;
};

void print_predio(predio* p);

void move_direita(int32_t posicoes, predio* p, bool marca_posicao_final = true);
void move_esquerda(int32_t posicoes, predio* p, bool marca_posicao_final = true);
void move_para_cima(int32_t posicoes, predio* p, bool marca_posicao_final = true);
void move_para_baixo(int32_t posicoes, predio* p, bool marca_posicao_final = true);
predio* constroi_predio(int32_t ANDARES, int32_t JANELAS);
void destroi_predio(predio* p);

inline bool pode_ir_para_baixo(predio* p) { return p->current_position > 4 * p->JANELAS - 1; }
inline bool pode_ir_para_cima(predio* p) { return p->current_position < 4 * p->JANELAS * (p->ANDARES - 1); }

// andar = 0..ANDARES-1 ; janela = 1..JANELAS
int32_t convert_posicao_to_index(char face, int32_t andar, int32_t janela, predio* p);

bool is_predio_limpo(predio* p);

int32_t calcula_passos_verticais(int32_t inicio, int32_t fim, predio* p);
int32_t calcula_passos_horizontais(int32_t inicio, int32_t fim, predio* p);
