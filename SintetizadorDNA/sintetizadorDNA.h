//
// Created by gabriel.moraes on 11/11/2025.
//
#pragma once
#include <string>

struct sequencia_log {
    std::string operacao;
    sequencia_log* parent;
};
struct sequencia {
    std::string sequenciaInicial;
    std::string sequenciaFinal;
    unsigned int custo;
};

void print_memorizacao();
bool sintetizadorDNA(const std::string* sequencia_atual,
    const std::string* sequencia_final,
    const unsigned int custo_atual,
    const unsigned int MAX_CUSTO,
    sequencia_log *root);
