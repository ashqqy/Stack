#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "MyAllocation.h"
#include "Stack.h"

//-------------------------------------------------------

int main ()
{
    stack_t stack = {};
    StackInit (&stack);

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

    stack_elem_t elem_pop = 0;
    StackPop (&stack, &elem_pop);

    STACKDUMP (&stack, stdout);

    StackDestroy (&stack);
}

//-------------------------------------------------------