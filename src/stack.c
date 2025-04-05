#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "allocation.h"

//-------------------------------------------------------

#define STACK_ASSERT(stack) StackAssert (stack, __FILE__, __LINE__, __func__);

//-------------------------------------------------------

stack_error_t StackInit (stack_t* stack, ssize_t capacity) 
{
    if (stack == NULL)
        return STACK_BAD_STRUCT;

    if (capacity < 0)
    {
        stack->capacity = capacity;
        return STACK_NEGATIVE_CAPACITY;
    }

    if (capacity < MIN_CAPACITY)
        capacity = MIN_CAPACITY;

    stack->data = (stack_elem_t*) MyCalloc ((size_t) capacity CANARY(+ N_CANARIES), sizeof (stack_elem_t), (const void*) &STACK_POISON);
    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;

    CANARY(stack->left_canary = STACK_CANARY;)
    stack->size = 0;
    stack->capacity = capacity;
    CANARY(stack->right_canary = STACK_CANARY;)

    CANARY(stack->data[0] = STACK_CANARY;)
    CANARY(stack->data[capacity + 1] = STACK_CANARY;)

    HASH(stack->hash = StackHash (stack);)

    STACK_ASSERT (stack);

    return STACK_OK;
}

//-------------------------------------------------------

stack_error_t StackDestroy (stack_t* stack)
{
    if (stack == NULL)
        return STACK_BAD_STRUCT;
    if (stack->data != NULL)
        memset (stack->data, 0, (size_t) stack->capacity);

    free(stack->data); stack->data = NULL;

    stack->size = 0; stack->capacity = 0;
    CANARY(stack->left_canary = 0; stack->right_canary = 0;)
    HASH(stack->hash = 0;)

    return STACK_OK;
}

//-------------------------------------------------------

stack_error_t StackPush (stack_t* stack, stack_elem_t elem_push)
{
    STACK_ASSERT (stack);

    if ( (stack->size == stack->capacity) && (stack->size >= MIN_CAPACITY) )
    {
        stack_error_t resize_err = StackResize (stack, stack->capacity * CAPACITY_GROWTH);
        if (resize_err != STACK_OK)
            return resize_err;
    }

    stack->data[stack->size++ CANARY(+ 1)] = elem_push; // +1 because of left canary

    HASH(stack->hash = StackHash (stack);)

    STACK_ASSERT (stack);
    
    return STACK_OK;
}

//-------------------------------------------------------

stack_error_t StackPop (stack_t* stack, stack_elem_t* elem_pop)
{
    STACK_ASSERT (stack);

    if (stack->size == 0)
        return BAD_POPa;

    if ( (stack->size <= stack->capacity / CAPACITY_DECREASE) && (stack->size >= MIN_CAPACITY) )
    {
        stack_error_t resize_err = StackResize (stack, stack->capacity / CAPACITY_DECREASE);
        if (resize_err != STACK_OK)
            return resize_err;
    }

    *elem_pop = stack->data[--stack->size CANARY(+ 1)]; // +1 because of left canary
    stack->data[stack->size CANARY(+ 1)] = STACK_POISON;

    HASH(stack->hash = StackHash (stack);)

    STACK_ASSERT (stack);

    return STACK_OK;
}

//-------------------------------------------------------

stack_error_t StackResize (stack_t* stack, ssize_t new_size)
{
    if (stack->capacity == 0)
    {
        stack->data = (stack_elem_t*) MyRecalloc (stack->data, 1 CANARY(+ N_CANARIES), sizeof (stack_elem_t), 
                                                 (size_t) stack->capacity CANARY(+ N_CANARIES - 1), (const void*) &STACK_POISON);
        stack->capacity = 1;
    }
    else
    {
        stack->data = (stack_elem_t*) MyRecalloc (stack->data, (size_t) new_size CANARY(+ N_CANARIES), 
                                                 sizeof (stack_elem_t), (size_t) stack->capacity CANARY(+ N_CANARIES - 1), 
                                                 (const void*) &STACK_POISON);
        stack->capacity = new_size;
    }

    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;           

    CANARY(stack->data[0] = STACK_CANARY;)
    CANARY(stack->data[stack->capacity + 1] = STACK_CANARY;) // +1 because of left canary

    return STACK_OK;
}

//-------------------------------------------------------

stack_error_t StackOk (stack_t* stack)
{
    if (stack == NULL)                                             return STACK_BAD_STRUCT;
    if (stack->size < 0)                                           return STACK_NEGATIVE_SIZE;
    if (stack->capacity < 0)                                       return STACK_NEGATIVE_CAPACITY;
    if (stack->data == NULL)                                       return STACK_BAD_DATA;
    if (stack->size > stack->capacity)                             return STACK_BAD_SIZE;
    CANARY(if (stack->left_canary != STACK_CANARY)                 return STACK_STRUCT_BAD_LEFT_CANARY;)
    CANARY(if (stack->right_canary != STACK_CANARY)                return STACK_STRUCT_BAD_RIGHT_CANARY;)
    CANARY(if (stack->data[0] != STACK_CANARY)                     return STACK_DATA_BAD_LEFT_CANARY;)
    CANARY(if (stack->data[stack->capacity + 1] != STACK_CANARY)   return STACK_DATA_BAD_RIGHT_CANARY;)
    HASH(if (stack->hash != StackHash (stack))                     return STACK_BAD_HASH;)

    return STACK_OK;
}

//-------------------------------------------------------

void StackAssert (stack_t* stack, const char* file, int line, const char* func)
{
    stack_error_t stack_error = StackOk (stack);
    if (stack_error != STACK_OK)
    {
        printf ("%sERROR:%s %s %s \n", RED_COLOR, MAGENTA_COLOR, StackErrDescr (stack_error), DEFAULT_COLOR);
        StackDump (stack, stdout, file, line, func);
        assert ("StackOk" && !STACK_OK);
    }
}

//-------------------------------------------------------

const char* StackErrDescr (stack_error_t stack_error)
{
    #define $DESCR(stack_error)  \
        case stack_error:        \
            return #stack_error; \

    switch (stack_error)
    {
        $DESCR(STACK_OK);
        $DESCR(STACK_BAD_STRUCT);
        $DESCR(STACK_BAD_DATA);
        $DESCR(STACK_BAD_SIZE);
        $DESCR(STACK_NEGATIVE_SIZE);
        $DESCR(STACK_NEGATIVE_CAPACITY);
        $DESCR(CANNOT_ALLOCATE_MEMORY);
        $DESCR(STACK_DATA_BAD_LEFT_CANARY);
        $DESCR(STACK_DATA_BAD_RIGHT_CANARY);
        $DESCR(STACK_STRUCT_BAD_LEFT_CANARY);
        $DESCR(STACK_STRUCT_BAD_RIGHT_CANARY);
        $DESCR(STACK_BAD_HASH);
        $DESCR(STACK_CANNOT_CREATE_HASH);
        $DESCR(BAD_POPa);
        default:
            return "u are fucking forgot some error (dumb)";
    }

    #undef $DESCR
}

//-------------------------------------------------------

stack_error_t StackDump (stack_t* stack, FILE* dump_file, const char* file, int n_line, const char* func)
{
    #define $PRINTERROR(error, dump_file)                                               \
        fprintf (dump_file, "%s" #error " (DUMP) %s \n", MAGENTA_COLOR, DEFAULT_COLOR); \
        return error;                                                                   \

    if (stack == NULL)
    {
        $PRINTERROR (STACK_BAD_STRUCT, dump_file);
    }

    fprintf (dump_file, "stack_t [%p] at %s:%d (%s) \n", stack, file, n_line, func);
    fprintf (dump_file, "{ \n");
    CANARY(fprintf (dump_file, "\t left_canary (struct) = %X \n", (unsigned int) stack->left_canary);)
    fprintf (dump_file, "\t size = %ld \n", stack->size);
    fprintf (dump_file, "\t capacity = %ld \n", stack->capacity);
    HASH(fprintf (dump_file, "\t hash = %lX \n", stack->hash);)

    if (stack->data == NULL)
    {
        $PRINTERROR (STACK_BAD_DATA, dump_file);
    }

    fprintf (dump_file, "\t data[%p]: \n", stack->data);
    fprintf (dump_file, "\t { \n");

    CANARY(fprintf (dump_file, "\t\t left_canary (data) = %X \n", (unsigned int) stack->data[0]);)

    for (int i = 0; i < stack->capacity; i++)
    {
        if (i < stack->size)
            fprintf (dump_file, "\t\t *[%d] = %d; \n", i, stack->data[i CANARY(+ 1)]);
        else 
            fprintf (dump_file, "\t\t [%d] = %d; \n", i, stack->data[i CANARY(+ 1)]);
    }

    CANARY(fprintf (dump_file, "\t\t right_canary (data) = 0x%X \n", (unsigned int) stack->data[stack->capacity + 1]);)

    fprintf (dump_file, "\t } \n");
    CANARY(fprintf (dump_file, "\t right_canary (struct) = 0x%X \n", (unsigned int) stack->right_canary);)
    fprintf (dump_file, "} \n");

    #undef $PRINTERROR
    return STACK_OK;
}

//-------------------------------------------------------

size_t StackHash (stack_t* stack)
{
    size_t hash = 5381;

    if (stack == NULL)
        return hash;

    // don't hash the old hash
    HASH(size_t old_hash = stack->hash;)
    HASH(stack->hash = 0;)

    // struct hashing
    HASH(for (size_t i = 0; i < sizeof (stack_t); i++))
        HASH({)
        HASH(char hash_element = (char) *((char*) stack + i);)
        HASH(hash = ((hash << 5) + hash) + (size_t) hash_element;)
        HASH(})
    
    HASH(stack->hash = old_hash;)

    HASH(if (stack->data == NULL))
        HASH(return hash;)

    // buffer hashing
    HASH(for (int i = 0; i < stack->capacity + N_CANARIES; i++))
        HASH({)
        HASH(size_t hash_element = (size_t) stack->data[i];)
        HASH(hash = ((hash << 5) + hash) + hash_element;)
        HASH(})

    return hash;
}

//-------------------------------------------------------
