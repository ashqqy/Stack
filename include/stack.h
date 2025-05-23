#ifndef MY_STACK
#define MY_STACK

#include <stdio.h>

//-------------------------------------------------------

#define STACKDUMP(stack, dump_file) StackDump (stack, dump_file, __FILE__, __LINE__, __func__);

#ifdef CANARY_PROTECTION
#define CANARY(...) __VA_ARGS__
#else
#define CANARY(...)
#endif // CANARY_PROTECTION

#ifdef HASH_PROTECTION
#define HASH(...) __VA_ARGS__
#else
#define HASH(...)
#endif // HASH_PPROTECTION

#ifdef POISON_PROTECTION
#define POISON(...) __VA_ARGS__
#else
#define POISON(...) 0
#endif // POISON_PROTECTION
//-------------------------------------------------------

typedef int stack_elem_t;

//-------------------------------------------------------

static const int MIN_CAPACITY = 10;

static const int CAPACITY_GROWTH   = 2;
static const int CAPACITY_DECREASE = 2 * CAPACITY_GROWTH;

static const stack_elem_t STACK_POISON = POISON(0xDEFACED);

CANARY(static const int N_CANARIES = 2;)
CANARY(static const stack_elem_t STACK_CANARY = 0xBADF00D;)

static const char* const RED_COLOR     = "\033[1;31m";
static const char* const MAGENTA_COLOR = "\033[1;35m";
static const char* const DEFAULT_COLOR = "\033[1;0m";

//-------------------------------------------------------

typedef struct
{
    CANARY(stack_elem_t left_canary;)
    ssize_t size;
    ssize_t capacity;
    HASH(size_t hash;)
    stack_elem_t* data;
    CANARY(stack_elem_t right_canary;)
} stack_t;

//-------------------------------------------------------

typedef enum
{
    STACK_OK =                      1,
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
} stack_error_t;

//-------------------------------------------------------

stack_error_t StackInit (stack_t* stack, ssize_t capacity);
stack_error_t StackDestroy (stack_t* stack);

stack_error_t StackPush (stack_t* stack, stack_elem_t elem_push);
stack_error_t StackPop (stack_t* stack, stack_elem_t* elem_pop);
stack_error_t StackResize (stack_t* stack, ssize_t new_size);

stack_error_t StackOk (stack_t* stack);
const char* StackErrDescr (stack_error_t stack_error);
void StackAssert (stack_t* stack, const char* file, int line, const char* func);
stack_error_t StackDump (stack_t* stack, FILE* dump_file, const char* file, int n_line, const char* func);

size_t StackHash (stack_t* stack);

//-------------------------------------------------------

#endif //MY_STACK
