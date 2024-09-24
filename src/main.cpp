#include <stdio.h>
#include <cstdlib>
#include <assert.h>

//-------------------------------------------------------

#define STACK_ASSERT(stack) ( \
    { \
    if (StackOk (stack) == 0) \
        { \
        StackDump (stack); \
        assert ("StackOk" && !OK); \
        } \
    })

//-------------------------------------------------------

typedef int StackElem_t;

const int OK = 1;

struct Stack_t
    {
    size_t size;
    size_t capacity;
    StackElem_t* data;
    };

int StackInit (Stack_t* stack, size_t capacity = 0);
int StackDestroy (Stack_t* stack);
int StackPush (Stack_t* stack, StackElem_t elem_push);
int StackPop (Stack_t* stack, StackElem_t* elem_pop);
int StackOk (Stack_t* stack);
void StackDump (Stack_t* stack);

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 10);

    StackElem_t elem_push = 0;
    scanf ("Enter the element for pushing: %d", &elem_push);
    StackPush (&stack, elem_push);

    StackElem_t elem_pop = 0;
    StackPop (&stack, &elem_pop);

    printf ("Popped element: %d", elem_pop);

    StackDestroy (&stack);
    }

//-------------------------------------------------------

int StackInit (Stack_t* stack, size_t capacity)
    {
    stack->data = (StackElem_t*) calloc (capacity, sizeof (StackElem_t));
    stack->size = 0;
    stack->capacity = capacity;

    STACK_ASSERT (stack);
    }

//-------------------------------------------------------

int StackPush (Stack_t* stack, StackElem_t elem_push)
    {

    }

//-------------------------------------------------------

int StackPop (Stack_t* stack, StackElem_t* elem_pop)
    {

    }

//-------------------------------------------------------

int StackDestroy (Stack_t* stack)
    {

    }

//-------------------------------------------------------