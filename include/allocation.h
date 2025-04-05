#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string.h>

void* MyCalloc (size_t n_elems, size_t size_elems, const void* poison);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, const void* poison);

#endif // ALLOCATION_H
