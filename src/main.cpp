#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <cstring> //memset

#include "Stack.h"
#include "Constants.h"
#include "MyAllocation.h"

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 10);

    // StackElem_t elem_push = 0;
    // printf ("Enter the element for pushing (main): ");
    // scanf ("%d", &elem_push);
    // StackPush (&stack, elem_push);

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

    // StackElem_t elem_pop = 0;
    // StackPop (&stack, &elem_pop);

    // printf ("Popped element (main): %d \n", elem_pop);

    StackDump (&stack, __FILE__, __LINE__);
    StackDestroy (&stack);
    }

//-------------------------------------------------------

// typedef unsigned long long canary_t