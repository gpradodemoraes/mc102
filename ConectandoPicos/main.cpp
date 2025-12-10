#include <fmt/core.h>
#include <fstream>
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


int main(const int argc, char **argv) {
	if (argc < 2) {
		fmt::println("Usage: {} /path/to/file/input.txt", argv[0]);
		return 1;
	}
	std::vector<int> picos{};
	if (get_picos(argv[1], &picos) != 0) return 1;
	return 0;
}
