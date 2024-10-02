#ifndef STACK
#define STACK

#include <stdio.h>

//-------------------------------------------------------

typedef int StackElem_t;
typedef unsigned long long Canary_t;

struct Stack_t
    {
    size_t size;
    size_t capacity;
    StackElem_t* data;
    };

//-------------------------------------------------------

int StackInit (Stack_t* stack, size_t capacity = 0);
int StackDestroy (Stack_t* stack);

int StackPush (Stack_t* stack, StackElem_t elem_push);
int StackPop (Stack_t* stack, StackElem_t* elem_pop);

int StackOk (Stack_t* stack);
void StackAssert (Stack_t* stack, const char* file, int line);
void StackDump (Stack_t* stack, const char* file, int n_line);

//-------------------------------------------------------

#endif //STACK