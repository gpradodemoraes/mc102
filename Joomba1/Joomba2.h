//
// Created by gabriel.moraes on 18/11/2025.
//
#pragma once
#include <cstdint>
#include <vector>
#include "predio.h"
struct posicao {
	char face;
	int32_t andar;	// de 0->(ANDARES - 1)
	int32_t janela; // de 1 a JANELAS
};

struct instrucao_node {
	char direcao;
	int32_t distancia;
	int32_t profundidade;
	int32_t comandos;
	instrucao_node* parent;
};

// 7 9 7 ANDARES, JANELAS, COMANDOS
// L 0 7 FACE, ANDAR, JANELA
// O 6 7
// L 1 3
// S 5 4
// O 0 7
// F 0 0

bool parse_entrada_joomba2(char* filepath, std::vector<posicao>* posicoes, int32_t* ANDARES_, int32_t* JANELAS,
  int32_t* COMANDOS);

bool checar_limpeza(instrucao_node* list_instrucoes, int32_t max_instrucoes_permitido, int32_t* janela_suja_corrente,
  predio* p);

int32_t conta_instrucoes(instrucao_node* i);

void inicia_predio_auxiliar(predio* p);

void destroi_predio_auxiliar();

instrucao_node* create_instrucao_node(char direcao, int32_t distancia, int32_t profundidade, int32_t comandos,
  instrucao_node* parent);

bool procura_comandos_sem_recorrencia(int32_t* array_all_permutations, predio* p, const int32_t maximo_comandos);
