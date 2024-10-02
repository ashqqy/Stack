#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "Stack.h"
#include "Constants.h"
#include "MyAllocation.h"

//-------------------------------------------------------

#define STACK_ASSERT(stack) StackAssert (stack, __FILE__, __LINE__);

//-------------------------------------------------------

int StackInit (Stack_t* stack, size_t capacity)
    {
    // size_t n_blocks = capacity / sizeof (Canary_t);

    // stack->data = (StackElem_t*) MyCalloc (n_blocks + N_CANARIES, sizeof (Canary_t), STACK_POISON);

    stack->data = (StackElem_t*) MyCalloc (capacity, sizeof (StackElem_t), STACK_POISON);
    assert (stack->data != NULL);

    * (Canary_t*) stack->data = CANARY;

    stack->size = 0;
    stack->capacity = capacity;

    STACK_ASSERT (stack);

    return 0; //исправить
    }

//-------------------------------------------------------

int StackDestroy (Stack_t* stack)
    {
    memset (stack->data, 0, stack->capacity);
    free(stack->data); stack->data = NULL;
    
    return 0; //исправить
    }

//-------------------------------------------------------

int StackPush (Stack_t* stack, StackElem_t elem_push)
    {
    STACK_ASSERT (stack);

    if (stack->size == stack->capacity) // содержимое ифа в функцию
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, stack->capacity * CAPACITY_GROWTH,
                                                 sizeof (StackElem_t), stack->capacity, STACK_POISON);
        assert (stack->data != NULL);
        stack->capacity *= CAPACITY_GROWTH;
        }

    stack->data[stack->size++] = elem_push;

    STACK_ASSERT (stack);
    
    // printf ("Pushed element (StackPush): %d \n", stack->data[stack->size - 1]);

    return 0; //исправить
    }

//-------------------------------------------------------

int StackPop (Stack_t* stack, StackElem_t* elem_pop)
    {
    STACK_ASSERT (stack);

    if (stack->size == stack->capacity / CAPACITY_DECREASE) // в функцию
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, stack->capacity / CAPACITY_DECREASE, 
                                                 sizeof (StackElem_t), stack->capacity, STACK_POISON);
        assert (stack->data != NULL);
        stack->capacity /= CAPACITY_DECREASE;
        }

    *elem_pop = stack->data[stack->size - 1];
    stack->size -= 1;
    stack->data[stack->size] = STACK_POISON;

    STACK_ASSERT (stack);

    // printf ("Popped element (StackPop): %d \n", *elem_pop);

    return 0; //исправить
    }

//-------------------------------------------------------

int StackOk (Stack_t* stack)
    {
    if (stack == NULL)
        return STACK_BAD_STRUCT;
    if (stack->data == NULL)
        return STACK_BAD_DATA;
    if (stack->size > stack->capacity)
        return STACK_BAD_SIZE;
    if (stack->size < 0)
        return STACK_NEGATIVE_SIZE;
    return OK;
    }

//-------------------------------------------------------

void StackAssert (Stack_t* stack, const char* file, int line)
    {
    if (StackOk (stack) != OK)
        {
        StackDump (stack, __FILE__, __LINE__);
        assert ("StackOk" && !OK);
        }
    }

//-------------------------------------------------------

void StackDump (Stack_t* stack, const char* file, int n_line)
    {
    printf ("Stack_t [0x%p] at %s:%d \n", stack, file, n_line);
    printf ("    { \n");
    printf ("    size = %d \n", stack->size);
    printf ("    capacity = %d \n", stack->capacity);
    printf ("    data[0x%p]: \n", stack->data);
    printf ("        { \n");
    for (int i = 0; i < stack->capacity; i++)
        {
        if (i < stack->size)
            printf ("        *[%d] = %d; \n", i, stack->data[i]);
        else 
            printf ("        [%d] = %d; \n", i, stack->data[i]);
        }
    printf ("        } \n");
    printf ("    } \n");
    }

//-------------------------------------------------------