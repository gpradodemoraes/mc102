#include <cfloat>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <limits.h>
#include <fmt/base.h>
#include <fmt/core.h>

int get_input(const char *inputfile, std::vector<double> *amplitudes, int *janela, double *valor_maximo) {
	if (std::ifstream file(inputfile); file.good() && file.is_open()) {
		std::string line;
		int counter{ 0 };
		while (std::getline(file, line)) {
			if (counter == 0) {
				//  2.1, 3.5, 1.8, 4.2, 2.9, 5.1, 1.0, 6.3
				std::stringstream ss(line);
				std::string token;
				const char delimiter = ',';
				while (std::getline(ss, token, delimiter)) {
					amplitudes->push_back(std::stod(token));
				}
			}
			if (counter == 1) {
				*janela = std::stoi(line);
			}
			if (counter == 2) {
				*valor_maximo = std::stod(line);
			}
			counter++;
		}

		file.close();
	}

	return 0;
}

int main(const int argc, char **argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/file/input.txt", argv[0]);
		return 1;
	}

	// lista de amplitudes em ordem
	std::vector<double> amplitudes;

	// tamanho da janela
	int janela{ 0 };

	// valor máximo permitido de oscilação (diferença entre a maior e a menor amplitude)
	// antes de gerar um alerta
	double valor_maximo{ 0.0 };

	get_input(argv[1], &amplitudes, &janela, &valor_maximo);

	for (int inicio = 0, fim = janela; fim < amplitudes.size() + 1; inicio++, fim++) {
		double max = DBL_MIN;
		double min = DBL_MAX;
		for (int i = inicio; i < fim; i++) {
			if (amplitudes[i] > max) max = amplitudes[i];
			if (amplitudes[i] < min) min = amplitudes[i];
		}
		if (max - min > valor_maximo)
			fmt::println("Janela {}: emitir alerta", inicio + 1);
		else
			fmt::println("Janela {}: normal", inicio + 1);
	}

	return 0;
}
