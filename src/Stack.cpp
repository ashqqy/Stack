#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Stack.h"
#include "MyAllocation.h"

//-------------------------------------------------------

// TODO #define CANARY_PROTECTION and #define HASH_PROTECTION (условная компиляция)

//-------------------------------------------------------

#define STACK_ASSERT(stack) StackAssert (stack, __FILE__, __LINE__, __func__);

//-------------------------------------------------------

STACK_ERRORS StackInit (Stack_t* stack, ssize_t capacity) 
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

    stack->data = (StackElem_t*) MyCalloc (capacity + N_CANARIES, sizeof (StackElem_t), (const void*) &STACK_POISON);
    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;

    stack->left_canary = CANARY;
    stack->size = 0;
    stack->capacity = capacity;
    stack->right_canary = CANARY;

    stack->data[0] = CANARY;
    stack->data[capacity + 1] = CANARY;

    stack->hash = StackHash (stack);

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
    stack->left_canary = 0; stack->right_canary = 0;
    stack->hash = 0;

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackPush (Stack_t* stack, StackElem_t elem_push)
    {
    STACK_ASSERT (stack);

    if ( (stack->size == stack->capacity) && (stack->size >= MIN_CAPACITY) )
        {
        STACK_ERRORS resize_err = StackResize (stack, stack->capacity * CAPACITY_GROWTH);
        if (resize_err != OK)
            return resize_err;
        }

    stack->data[stack->size++ + 1] = elem_push; // +1 из-за левой канарейки

    stack->hash = StackHash (stack);

    STACK_ASSERT (stack);
    
    return OK;
    }

//-------------------------------------------------------

// Нет проверки на ошибки для удобства использования
// Если ошибка, то возвращается ядовитое значение
StackElem_t StackPop (Stack_t* stack)
    {
    STACK_ASSERT (stack);

    if (stack->size == 0)
        return STACK_POISON;

    if ( (stack->size <= stack->capacity / CAPACITY_DECREASE) && (stack->size >= MIN_CAPACITY) )
        {
        STACK_ERRORS resize_err = StackResize (stack, stack->capacity / CAPACITY_DECREASE);
        if (resize_err != OK)
            return STACK_POISON;
        }

    StackElem_t elem_pop = stack->data[--stack->size + 1]; // +1 из-за левой канарейки
    stack->data[stack->size + 1] = STACK_POISON;

    stack->hash = StackHash (stack);

    STACK_ASSERT (stack);

    return elem_pop;
    }

//-------------------------------------------------------

STACK_ERRORS StackResize (Stack_t* stack, ssize_t new_size)
    {
    if (stack->capacity == 0)
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, 1 + N_CANARIES, sizeof (StackElem_t), 
                                                 stack->capacity + N_CANARIES - 1, (const void*) &STACK_POISON);
        stack->capacity = 1;
        }
    else
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, new_size + N_CANARIES, 
                                                 sizeof (StackElem_t), stack->capacity + N_CANARIES - 1, (const void*) &STACK_POISON);
        stack->capacity = new_size;
        }

    if (stack->data == NULL)
        return CANNOT_ALLOCATE_MEMORY;           

    stack->data[0] = CANARY;
    stack->data[stack->capacity + 1] = CANARY; // +1 из-за левой канарейки

    return OK;
    }

//-------------------------------------------------------

STACK_ERRORS StackOk (Stack_t* stack)
    {
    if (stack == NULL)                                return STACK_BAD_STRUCT;
    if (stack->size < 0)                              return STACK_NEGATIVE_SIZE;
    if (stack->capacity < 0)                          return STACK_NEGATIVE_CAPACITY;
    if (stack->data == NULL)                          return STACK_BAD_DATA;
    if (stack->size > stack->capacity)                return STACK_BAD_SIZE;
    if (stack->left_canary != CANARY)                 return STACK_STRUCT_BAD_LEFT_CANARY;
    if (stack->right_canary != CANARY)                return STACK_STRUCT_BAD_RIGHT_CANARY;  
    if (stack->data[0] != CANARY)                     return STACK_DATA_BAD_LEFT_CANARY;
    if (stack->data[stack->capacity + 1] != CANARY)   return STACK_DATA_BAD_RIGHT_CANARY;
    if (stack->hash != StackHash (stack))             return STACK_BAD_HASH;

    return OK;
    }

//-------------------------------------------------------

void StackAssert (Stack_t* stack, const char* file, int line, const char* func)
    {
    STACK_ERRORS stack_error = StackOk (stack);
    if (stack_error != OK)
        {
        printf ("%sERROR:%s %s %s \n", RED_COLOR, MAGENTA_COLOR, StackErrDescr (stack_error), DEFAULT_COLOR);
        StackDump (stack, stdout, file, line, func);
        assert ("StackOk" && !OK);
        }
    }

//-------------------------------------------------------

const char* StackErrDescr (STACK_ERRORS stack_error)
    {
    #define $DESCR(stack_error) case stack_error: return #stack_error

    switch (stack_error)
        {
        $DESCR(OK);
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

STACK_ERRORS StackDump (Stack_t* stack, FILE* dump_file, const char* file, int n_line, const char* func)
    {
    #define $PRINTERROR(error, dump_file) fprintf (dump_file, "%s" #error " (DUMP) %s \n", MAGENTA_COLOR, DEFAULT_COLOR); return error;

    if (stack == NULL)
        {
        $PRINTERROR (STACK_BAD_STRUCT, dump_file);
        }

    fprintf (dump_file, "Stack_t [0x%p] at %s:%d (%s)\n", stack, file, n_line, func);
    fprintf (dump_file, "    { \n");
    fprintf (dump_file, "    left_canary (struct) = 0x%X \n", stack->left_canary);
    fprintf (dump_file, "    size = %lld \n", stack->size);
    fprintf (dump_file, "    capacity = %lld \n", stack->capacity);
    fprintf (dump_file, "    hash = 0x%llX \n", stack->hash);

    if (stack->data == NULL)
        {
        $PRINTERROR (STACK_BAD_DATA, dump_file);
        }

    fprintf (dump_file, "    data[0x%p]: \n", stack->data);
    fprintf (dump_file, "        { \n");

    fprintf (dump_file, "        left_canary (data) = 0x%X\n", stack->data[0]);

    for (int i = 0; i < stack->capacity; i++)
        {
        if (i < stack->size)
            fprintf (dump_file, "        *[%d] = %d; \n", i, stack->data[i+1]);
        else 
            fprintf (dump_file, "        [%d] = %d; \n", i, stack->data[i+1]);
        }

    fprintf (dump_file, "        right_canary (data) = 0x%X\n", stack->data[stack->capacity + 1]);

    fprintf (dump_file, "        } \n");
    fprintf (dump_file, "    right_canary (struct) = 0x%X \n", stack->right_canary);
    fprintf (dump_file, "    } \n");

    #undef $PRINTERROR
    return OK;
    }

//-------------------------------------------------------

size_t StackHash (Stack_t* stack)
    {
    size_t hash = 5381;

    if (stack == NULL)
        return hash;

    // обнуляем хеш в структуре, чтобы он не хешировался
    size_t old_hash = stack->hash;
    stack->hash = 0;

    // считаем хеш для структуры
    for (size_t i = 0; i < sizeof (Stack_t); i++)
        {
        size_t hash_element = (char) *((char*) stack + i);
        hash = ((hash << 5) + hash) + hash_element;
        }
    
    stack->hash = old_hash;

    if (stack->data == NULL)
        return hash;

    // считаем хеш для буфера
    for (int i = 0; i < stack->capacity + N_CANARIES; i++)
        {
        size_t hash_element = stack->data[i];
        hash = ((hash << 5) + hash) + hash_element;
        }

    return hash;
    }

//-------------------------------------------------------