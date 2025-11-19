//
// Created by gabriel.moraes on 18/11/2025.
//
#pragma once
#include <cstdint>
#include <vector>
#include "predio.h"
struct posicao {
	char face;
	int32_t andar; // de 0->(ANDARES - 1)
	int32_t janela; // de 1 a JANELAS
};

struct instrucao_node {
	char direcao;
	int32_t distancia;
	instrucao_node* parent;
};

// 7 9 7 ANDARES, JANELAS, COMANDOS
// L 0 7 FACE, ANDAR, JANELA
// O 6 7
// L 1 3
// S 5 4
// O 0 7
// F 0 0

bool parse_entrada_joomba2(char* filepath, std::vector<posicao>* posicoes, int32_t *ANDARES_, int32_t *JANELAS, int32_t *COMANDOS);

bool checar_limpeza(instrucao_node* list_instrucoes, int32_t max_instrucoes_permitido, predio* p);

int32_t conta_instrucoes(instrucao_node* i);
// 
// 1) verificar se o predio todo está limpo
// 2) se limpo e comandos maior ou igual a zero então retornar verdadeiro
// 3) se não limpo e comandos menor doque zero então retornar falso
// 4) Seguir adiante
// 
// -- salvar o estado atual (ou seja, se a janela está limpa ou suja)
// 5) Se possível ir para baixo então chamar a função recursivamente indo para baixo
// 	(se o comando anterior foi "para baixo" então manter o valor de comandos_restantes. Caso contrário, diminuir 1)
// 6) Se possível ir para cima então chamar a função recursivamente indo para cima
// 	(se o comando anterior foi "para cima" então manter o valor de comandos_restantes. Caso contrário, diminuir 1)
// 7) Verificar se ele já andou para a esquerda metade de 4 * janelas ou mais. Caso contrário chamar a função
// 	recursivamente para a esquerda
// 7) Verificar se ele já andou para a direita metade de 4 * janelas ou mais. Caso contrário chamar a função
// 	recursivamente para a direita
// 
// 
// -- se o comando anterior retornou falso, então restaurar o valor da janela que havia sido salvo antes.
