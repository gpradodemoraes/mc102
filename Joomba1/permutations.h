#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
int32_t* prepare_all_permutation(int32_t* array_model, size_t size);
void destroy_all_permutations();
int32_t* get_next_permutation();
#ifdef __cplusplus
}
#endif // __cplusplus
