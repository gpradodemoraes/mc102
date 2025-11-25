// C program to print all permutations
// with duplicates allowed
#include "permutations.h"
#include <string.h>
#include <stdlib.h>

static int32_t* all_permutations;
static int32_t full_permutation_index = 0;
static int32_t current_index = 0;
static int32_t original_array_size;



/* Function to swap values at
   two pointers */
static void swap(int32_t* x, int32_t* y) {
	int32_t temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

/* Function to print permutations
   of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
static void permute(int32_t* a, int32_t l, int32_t r) {
	int32_t i;
	if (l == r) {
		int32_t k = 0;
		for (int32_t j = 0; j < original_array_size; ++j) {
			all_permutations[full_permutation_index] = a[j];
			full_permutation_index++;
		}
		all_permutations[full_permutation_index] = -1;
		full_permutation_index++;
	} else {
		for (i = l; i <= r; i++) {
			swap((a + l), (a + i));
			permute(a, l + 1, r);

			// backtrack
			swap((a + l), (a + i));
		}
	}
}

/**
 * essa função calcula todas as permutações possíveis
 * ela aloca um vetor que vai caber todas as permutações possíveis
 * (usando o cálculo do fatorial abaixo) e deixa espaço entre as permutações
 * para marcar onde uma termina e outra começa com -1 e ao fim de todas
 * as permutações, depois do último -1, ele vai marcar o final absoluto com -2
 */
int32_t* prepare_all_permutation(int32_t* array_model, size_t size) {
	original_array_size = size;
	uint32_t amount_of_permutations = 1;

	/**
	 * a linha abaixo é equivalente a fatorial(size)
	 */
	for (int32_t i = 1; i <= size; ++i) amount_of_permutations *= i;

	all_permutations = (int32_t*)malloc(sizeof(int32_t) * (amount_of_permutations * (size + 1) + 1));

	permute(array_model, 0, size - 1);
	if (all_permutations) all_permutations[amount_of_permutations * (size + 1)] = -2;

	return all_permutations;
}

void destroy_all_permutations() { free(all_permutations); }

int32_t* get_next_permutation() { return all_permutations + current_index++ * original_array_size; }
