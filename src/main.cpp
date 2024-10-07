#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// TODO #define CANARY_PROTECTION and #define HASH_PROTECTION

#include "MyAllocation.h"
#include "Stack.h"

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 3);
    StackPush (&stack, 0);
    StackPush (&stack, 1);
    StackPush (&stack, 2);
    StackPush (&stack, 3);
    StackPush (&stack, 4);
    StackPush (&stack, 5);
    StackPush (&stack, 6);
    StackPush (&stack, 7);
    StackPush (&stack, 8);
    StackPush (&stack, 9);
    StackPush (&stack, 10);
    StackPush (&stack, 11);
    StackPush (&stack, 12);
    StackPush (&stack, 13);

    StackPush (&stack, 0);

    StackElem_t elem_pop = 0;
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);
    StackPop (&stack, &elem_pop);

    StackDump (&stack, __FILE__, __LINE__, __func__);

    StackDestroy (&stack);
    }

//-------------------------------------------------------