//
// Created by gabriel.moraes on 11/11/2025.
//
#include "sintetizadorDNA.h"
#include <map>
#include <vector>
#define FMT_HEADER_ONLY
#include <fmt/core.h>

static std::map<std::string, sequencia> mutationsMap = {
    { "A", sequencia{"A","AT", 2} },
    { "G", sequencia{"G", "GC", 2} },
    { "TT", sequencia{"TT", "A", 5} },
    { "CG", sequencia{"CG", "G", 1} },
    { "AT", sequencia{"AT", "TA", 1} },
    { "C", sequencia{"C", "G", 3} },
    { "T", sequencia{"T", "A", 3} },
    { "AGA", sequencia{"AGA", "C", 8} },
    { "CC", sequencia{"CC", "G", 4} },
    { "TATA", sequencia{"TATA", "GC", 10} },
};

static void print_operacao(sequencia_log *l) {
    std::vector<std::string> sequencia;
    if (l == nullptr) return;

    for (sequencia_log* log = l; log != nullptr; log = log->parent) {
        sequencia.push_back(log->operacao);
    }
    int counter = 0;
    for (auto it = sequencia.rbegin(); it != sequencia.rend(); ++it)
        fmt::println("{}) {}", ++counter, *it);
}
void print_memorizacao() {
    for (auto i : mutationsMap) {
        fmt::println("{} => [{}=>{} ({})]", i.first,
            i.second.sequenciaInicial,
            i.second.sequenciaFinal,
            i.second.custo);
    }
}

bool sintetizadorDNA(const std::string* sequencia_atual,
    const std::string* sequencia_final,
    const unsigned int custo_atual,
    const unsigned int MAX_CUSTO,
    sequencia_log* parent) {
    const auto length = sequencia_atual->length();

    if (*sequencia_atual == *sequencia_final) {
        print_operacao(parent);
        fmt::println("Conseguimos metabolizar {} ao custo de {}", *sequencia_final, custo_atual);
        if (custo_atual > MAX_CUSTO) {
            fmt::println("porém o custo atual {} ultrapassou {}", custo_atual, MAX_CUSTO);
            return false;
        }
        return true;
    }
    // SEQUENCIA ATUAL: ATTT
    // SEQUENCIA FINAL: TATA
    // custo_atual: 2+2+2
    for (int i = 0; i < length; i++)
        for (int k = 1; k < length - i + 1; k++) {
            std::string original_mutacao = sequencia_atual->substr(i, k);
            // fmt::println("{}/{}={}", i,k,original_mutacao);
            try {
                sequencia* pSeq = &mutationsMap.at(original_mutacao);
                // existe uma mutação original_mutacao = TT
                // temos que fazer a substituição de TT na sequencia_atual A(TT)T onde i =1 e k = 2
                // o resultado final deve ser A(TT)T => A(A)T
                std::string seq_depois_mutacao;
                for (int j = 0; j < i; j++)
                    seq_depois_mutacao.push_back((*sequencia_atual)[j]);
                seq_depois_mutacao.append(pSeq->sequenciaFinal);
                for (int j = i + k; j < length; j++)
                    seq_depois_mutacao.push_back((*sequencia_atual)[j]);
                std::string operacao = fmt::format("Sequencia Atual:{}; Mutacao: {}=>{}; Depois: {}({})", *sequencia_atual, original_mutacao, pSeq->sequenciaFinal, seq_depois_mutacao, custo_atual + pSeq->custo);
                // fmt::println("{}", operacao);
                if (custo_atual + pSeq->custo > MAX_CUSTO) {
                    return false;
                }
                sequencia_log log{};
                log.parent = parent;
                log.operacao = operacao;

                if (sintetizadorDNA(&seq_depois_mutacao,sequencia_final,custo_atual + pSeq->custo, MAX_CUSTO, &log)) {
                    // só vai retornar se for true, ou seja, achou uma solução
                    // se for false deixa ele continuar tentando as outras opções
                    return true;
                }

            } catch (std::out_of_range& e) {

            }

        }
    return false;
}
