#include "MyAllocation.h"
#include "Constants.h"

#include <assert.h>
#include <cstring> //memset

//-------------------------------------------------------

void* MyCalloc (size_t n_elems, size_t size_elems, StackElem_t memset_elem)
    {
    void* mem_ptr = calloc (n_elems, size_elems);
    assert (mem_ptr != NULL);
    memset (mem_ptr, memset_elem, n_elems);
    return mem_ptr;
    }

//-------------------------------------------------------

void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, StackElem_t memset_elem)
    {
    memory = realloc (memory, n_elements * size_elements);
    assert (memory != NULL);

    if (previous_n_elements < n_elements)
        for (int i = 0; i < n_elements - previous_n_elements; i++)
            {
            *((StackElem_t*) memory + previous_n_elements + i) = memset_elem;
            }

    return memory;
    }

//-------------------------------------------------------