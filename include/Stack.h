#ifndef MY_STACK
#define MY_STACK

#include <stdio.h>

//-------------------------------------------------------

typedef int StackElem_t;
typedef unsigned long long Canary_t;

//-------------------------------------------------------

const double CAPACITY_GROWTH = 2;
const double CAPACITY_DECREASE = 1 / (CAPACITY_GROWTH * 2);

const StackElem_t STACK_POISON = 666;

const int N_CANARIES = 2;
const Canary_t CANARY = 123456789;

//-------------------------------------------------------

struct Stack_t
    {
    Canary_t left_canary;
    ssize_t size;
    ssize_t capacity;
    size_t hash;
    StackElem_t* data;
    Canary_t right_canary;
    };

//-------------------------------------------------------

enum STACK_ERRORS
    {
    OK = 1,
    STACK_BAD_STRUCT = 101,
    STACK_BAD_DATA = 102, 
    STACK_BAD_SIZE = 103, 
    STACK_NEGATIVE_SIZE = 104,
    STACK_NEGATIVE_CAPACITY = 105,
    CANNOT_ALLOCATE_MEMORY = 106,
    STACK_DATA_BAD_LEFT_CANARY = 107,
    STACK_DATA_BAD_RIGHT_CANARY = 108,
    STACK_STRUCT_BAD_LEFT_CANARY = 109,
    STACK_STRUCT_BAD_RIGHT_CANARY = 110,
    STACK_BAD_HASH = 111,
    BAD_POPa = 112
    };

//-------------------------------------------------------

const char* const RED_COLOR     = "\033[1;31m";
const char* const MAGENTA_COLOR = "\033[1;35m";
const char* const DEFAULT_COLOR = "\033[1;0m";

//-------------------------------------------------------

STACK_ERRORS StackInit (Stack_t* stack, size_t capacity = 0);
STACK_ERRORS StackDestroy (Stack_t* stack);

STACK_ERRORS StackPush (Stack_t* stack, StackElem_t elem_push);
STACK_ERRORS StackPop (Stack_t* stack, StackElem_t* elem_pop);
STACK_ERRORS StackResize (Stack_t* stack, const double new_size_coef);

STACK_ERRORS StackOk (Stack_t* stack);
const char* StackErrDescr (STACK_ERRORS stack_error);
void StackAssert (Stack_t* stack, const char* file, int line);
STACK_ERRORS StackDump (Stack_t* stack, const char* file, int n_line);

size_t StackHash (Stack_t* stack);

//-------------------------------------------------------

#endif //MY_STACK