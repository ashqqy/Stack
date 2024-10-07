#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define CANARY_PROTECTION

#include "MyAllocation.h"
#include "Stack.h"

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 3);
    StackPush (&stack, 1);
    StackPush (&stack, 2);

    StackPush (&stack, 0);

    StackElem_t elem_pop = 0;
    StackPop (&stack, &elem_pop);

    StackDump (&stack, __FILE__, __LINE__);

    StackDestroy (&stack);
    }

//-------------------------------------------------------