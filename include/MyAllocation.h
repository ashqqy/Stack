#ifndef MY_ALLOCATION
#define MY_ALLOCATION

#include <string.h>

void* MyCalloc (size_t n_elems, size_t size_elems, void* poison);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, void* poison);

#endif //MY_ALLOCATION