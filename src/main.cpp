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

    StackElem_t elem_push = 0;
    StackPop (&stack, &elem_push);

    StackDump (&stack, __FILE__, __LINE__, __func__);

    StackDestroy (&stack);
    }

//-------------------------------------------------------