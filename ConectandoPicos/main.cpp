#include <fmt/base.h>
#include <fmt/core.h>
#include <cstddef>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <sstream>

int get_picos(const char *inputfile, std::vector<int> *picos) {
	if (std::ifstream file(inputfile); file.good() && file.is_open()) {
		std::string line;
		if (getline(file, line)) {
			std::string token;
			std::stringstream ss(line);
			while (ss >> token) picos->push_back(std::stoi(token));
		} else
			return 1;
	} else
		return 1;
	return 0;
}

/**
 */

int calcula_numero_de_furos(std::vector<int> &picos, size_t inicio, size_t fim) {
	if (fim == inicio + 1) return 0;
	if (fim <= inicio) return 0;
	// fmt::println("iniciando: {}; {}", inicio, fim);
	double maior_coeficente_angular = std::numeric_limits<double>::lowest();

	size_t posicao = 0;
	for (size_t i = inicio; i <= fim; i++) {
		if (i > inicio) {
			auto coef_angular = static_cast<double>(picos[i] - picos[inicio]) / static_cast<double>(i - inicio);
			//		fmt::println("{}->{} {}->{} ({:.5f})", inicio, i, picos[inicio], picos[i], coef_angular);
			if (coef_angular > maior_coeficente_angular) {
				maior_coeficente_angular = coef_angular;
				posicao = i;
			}
		}
	}
	// fmt::println("posicao: {}", posicao);
	double coef_ang_total = std::numeric_limits<double>::infinity();
	if (fim > inicio)
		coef_ang_total = static_cast<double>(picos[fim] - picos[inicio]) / static_cast<double>(fim - inicio);
	else
		return 0;

	// fmt::print("{:.5f} > {:.5f} ? ", maior_coeficente_angular, coef_ang_total);
	if (maior_coeficente_angular > coef_ang_total) {
		// fmt::println("SIM!");
		return 1 + calcula_numero_de_furos(picos, inicio, posicao) + calcula_numero_de_furos(picos, posicao, fim);
	}
	// fmt::println("NÃO!");
	return 0;
}
int main(const int argc, char **argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/file/input.txt", argv[0]);
		return 1;
	}
	std::vector<int> picos{};
	if (get_picos(argv[1], &picos) != 0) return 1;

	// O número de retas é o número de furos + 1
	// fmt::println("{}", calcula_numero_de_furos(picos, 0, picos.size() - 1));
	fmt::println("{}", 1 + calcula_numero_de_furos(picos, 0, picos.size() - 1));
	return 0;
}
