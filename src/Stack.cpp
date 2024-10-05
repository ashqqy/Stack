#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Stack.h"
#include "MyAllocation.h"

//-------------------------------------------------------

#ifdef CANARY_PROTECTION
#define $CANARY(param) + param

#else
#define $CANARY(param) + 0

#endif //CANARIES

//-------------------------------------------------------

#define STACK_ASSERT(stack) StackAssert (stack, __FILE__, __LINE__);

//-------------------------------------------------------

STACK_ERRORS StackInit (Stack_t* stack, size_t capacity)
    {
    if (stack == NULL)
        return STACK_BAD_STRUCT;

    stack->data = (StackElem_t*) MyCalloc (capacity + N_CANARIES, sizeof (StackElem_t), (void*) &STACK_POISON);
    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;

    stack->size = 0;
    stack->capacity = capacity;

    STACK_ASSERT (stack);

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackDestroy (Stack_t* stack)
    {
    if (stack == NULL)
        return STACK_BAD_STRUCT;
    if (stack->data == NULL)
        return STACK_BAD_DATA;

    memset (stack->data, 0, stack->capacity);
    free(stack->data); stack->data = NULL;

    stack->size = 0; stack->capacity = 0;

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackPush (Stack_t* stack, StackElem_t elem_push)
    {
    STACK_ASSERT (stack); // FIXME граничный элемент ядовитый

    if (elem_push == 7)
        printf ("%d\n", stack->data[stack->size]);

    if (stack->size == stack->capacity)
        {
        STACK_ERRORS resize_err = StackResize (stack, CAPACITY_GROWTH);
        if (resize_err != OK)
            return resize_err;
        }

    stack->data[stack->size++ + 1] = elem_push; // +1 из-за левой канарейки

    if (elem_push == 7)
        printf ("%d\n", stack->data[stack->size - 1]);

    if (elem_push == 6)
        printf ("%d\n", stack->data[stack->size]);

    STACK_ASSERT (stack);
    
    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackPop (Stack_t* stack, StackElem_t* elem_pop)
    {
    STACK_ASSERT (stack);

    if (stack->size <= stack->capacity * CAPACITY_DECREASE)
        {
        STACK_ERRORS resize_err = StackResize (stack, CAPACITY_DECREASE);
        if (resize_err != OK)
            return resize_err;
        }

    *elem_pop = stack->data[--stack->size + 1]; // +1 из-за левой канарейки
    stack->data[stack->size + 1] = STACK_POISON;

    STACK_ASSERT (stack);

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackResize (Stack_t* stack, const double new_size_coef)
    {
    stack->data = (StackElem_t*) MyRecalloc (stack->data, (int) stack->capacity * new_size_coef + N_CANARIES, 
                                                 sizeof (StackElem_t), stack->capacity, (void*) &STACK_POISON);
    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;           

    stack->capacity = (int) stack->capacity * new_size_coef;

    *stack->data = CANARY;
    stack->data[stack->capacity + 1] = CANARY; // +1 из-за левой канарейки

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackOk (Stack_t* stack)
    {
    if (stack == NULL)
        return STACK_BAD_STRUCT;
    if (stack->data == NULL)
        return STACK_BAD_DATA;
    if (stack->size < NULL)
        return STACK_NEGATIVE_SIZE;
    if (stack->capacity < NULL)
        return STACK_NEGATIVE_CAPACITY;
    if (stack->size > stack->capacity)
        return STACK_BAD_SIZE;
    return OK;
    }

//-------------------------------------------------------

void StackAssert (Stack_t* stack, const char* file, int line) // TODO __func__
    {
    STACK_ERRORS stack_error = StackOk (stack);
    // printf ("%d\n", stack_error);
    if (stack_error != OK)
        {
        printf ("%sERROR:%s %s %s \n", RED_COLOR, MAGENTA_COLOR, StackErrDescr (stack_error), DEFAULT_COLOR);
        StackDump (stack, file, line);
        assert ("StackOk" && !OK);
        }
    }

//-------------------------------------------------------

const char* StackErrDescr (STACK_ERRORS stack_error)
    {
    #define $DESCR(stack_error) case stack_error: return #stack_error

    switch (stack_error)
        {
        $DESCR(STACK_BAD_STRUCT);
        $DESCR(STACK_BAD_DATA);
        $DESCR(STACK_BAD_SIZE);
        $DESCR(STACK_NEGATIVE_SIZE);
        $DESCR(STACK_NEGATIVE_CAPACITY);
        $DESCR(CANNOT_ALLOCATE_MEMORY);
        default:
            return "u are fucking forgot some error (dumb)";
        }

    #undef $DESCR
    }

//-------------------------------------------------------

STACK_ERRORS StackDump (Stack_t* stack, const char* file, int n_line)
    {
    #define $PRINTERROR(error) printf ("%s" #error " (DUMP) %s \n", MAGENTA_COLOR, DEFAULT_COLOR); return error;

    if (stack == NULL)
        {
        $PRINTERROR (STACK_BAD_STRUCT);
        }

    printf ("Stack_t [0x%p] at %s:%d \n", stack, file, n_line);
    printf ("    { \n");
    printf ("    size = %d \n", stack->size);
    printf ("    capacity = %d \n", stack->capacity);

    if (stack->data == NULL)
        {
        $PRINTERROR (STACK_BAD_DATA);
        }

    printf ("    data[0x%p]: \n", stack->data);
    printf ("        { \n");

    printf ("        Left Canary = %d\n", stack->data[0]);

    for (int i = 0; i < stack->capacity; i++)
        {
        if (i < stack->size)
            printf ("        *[%d] = %d; \n", i, stack->data[i+1]);
        else 
            printf ("        [%d] = %d; \n", i, stack->data[i+1]);
        }

    printf ("        Right Canary = %d\n", stack->data[stack->capacity + 1]);

    printf ("        } \n");
    printf ("    } \n");

    #undef $PRINTERROR
    return OK;
    }

//-------------------------------------------------------
