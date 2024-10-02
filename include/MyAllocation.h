#ifndef MY_ALLOCATION
#define MY_ALLOCATION

#include <cstring>

void* MyCalloc (size_t n_elems, size_t size_elems, int memset_elem);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements = 0, int memset_elem = 0);

#endif //MY_ALLOCATION