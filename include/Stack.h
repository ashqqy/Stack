#ifndef MY_STACK
#define MY_STACK

#include <stdio.h>

//-------------------------------------------------------

#define STACKDUMP(stack, dump_file) StackDump (stack, dump_file, __FILE__, __LINE__, __func__);

//-------------------------------------------------------

typedef int StackElem_t;

//-------------------------------------------------------

const int MIN_CAPACITY = 10;

const int CAPACITY_GROWTH   = 2;
const int CAPACITY_DECREASE = 2 * CAPACITY_GROWTH;

const StackElem_t STACK_POISON = 666;

const int N_CANARIES = 2;
const StackElem_t CANARY = 0xBEB1A;

//-------------------------------------------------------

struct Stack_t
    {
    StackElem_t left_canary;
    ssize_t size;
    ssize_t capacity;
    size_t hash;
    StackElem_t* data;
    StackElem_t right_canary;
    };

//-------------------------------------------------------

enum STACK_ERRORS
    {
    OK =                            1,
    STACK_BAD_STRUCT =              101,
    STACK_BAD_DATA =                102, 
    STACK_BAD_SIZE =                103, 
    STACK_NEGATIVE_SIZE =           104,
    STACK_NEGATIVE_CAPACITY =       105,
    CANNOT_ALLOCATE_MEMORY =        106,
    STACK_DATA_BAD_LEFT_CANARY =    107,
    STACK_DATA_BAD_RIGHT_CANARY =   108,
    STACK_STRUCT_BAD_LEFT_CANARY =  109,
    STACK_STRUCT_BAD_RIGHT_CANARY = 110,
    STACK_CANNOT_CREATE_HASH =      111,
    STACK_BAD_HASH =                112,
    BAD_POPa =                      113
    };

//-------------------------------------------------------

const char* const RED_COLOR     = "\033[1;31m";
const char* const MAGENTA_COLOR = "\033[1;35m";
const char* const DEFAULT_COLOR = "\033[1;0m";

//-------------------------------------------------------

STACK_ERRORS StackInit (Stack_t* stack, ssize_t capacity = 10);
STACK_ERRORS StackDestroy (Stack_t* stack);

STACK_ERRORS StackPush (Stack_t* stack, StackElem_t elem_push);
StackElem_t StackPop (Stack_t* stack);
STACK_ERRORS StackResize (Stack_t* stack, ssize_t new_size);

STACK_ERRORS StackOk (Stack_t* stack);
const char* StackErrDescr (STACK_ERRORS stack_error);
void StackAssert (Stack_t* stack, const char* file, int line, const char* func);
STACK_ERRORS StackDump (Stack_t* stack, FILE* dump_file, const char* file, int n_line, const char* func);

size_t StackHash (Stack_t* stack);

//-------------------------------------------------------

#endif //MY_STACK