#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "MyAllocation.h"
#include "Stack.h"

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 1);

    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);
    StackPush (&stack, 133);

    StackElem_t elem_pop = StackPop (&stack);

    STACKDUMP (&stack, stdout);

    StackDestroy (&stack);
    }

//-------------------------------------------------------