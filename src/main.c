#include "stack.h"

//-------------------------------------------------------

int main ()
{
    stack_t stack = {};
    StackInit (&stack, 10);

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
