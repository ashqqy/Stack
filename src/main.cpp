#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <cstring> //memset

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
const int CAPACITY_GROWTH = 2;
const int CAPACITY_DECREASE = CAPACITY_GROWTH * 2;

enum ERRORS
    {
    BAD_DATA = 100, 
    BAD_SIZE = 200, 
    NEGATIVE_SIZE = 300
    };

struct Stack_t
    {
    size_t size;
    size_t capacity;
    StackElem_t* data;
    };

int StackInit (Stack_t* stack, size_t capacity = 0);
int StackPush (Stack_t* stack, StackElem_t elem_push);
int StackPop (Stack_t* stack, StackElem_t* elem_pop);
int StackOk (Stack_t* stack);
void StackDump (Stack_t* stack);
int StackDestroy (Stack_t* stack);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements);

//-------------------------------------------------------

int main ()
    {
    Stack_t stack = {};
    StackInit (&stack, 10);

    StackElem_t elem_push = 0;
    printf ("Enter the element for pushing (main): ");
    scanf ("%d", &elem_push);
    StackPush (&stack, elem_push);

    StackElem_t elem_pop = 0;
    StackPop (&stack, &elem_pop);

    printf ("Popped element (main): %d \n", elem_pop);

    StackDump (&stack);
    StackDestroy (&stack);
    }

//-------------------------------------------------------

int StackInit (Stack_t* stack, size_t capacity)
    {
    (*stack).data = (StackElem_t*) calloc (capacity, sizeof (StackElem_t));
    assert (stack->data != NULL);
    stack->size = 0;
    stack->capacity = capacity;

    STACK_ASSERT (stack);

    return 0; //исправить
    }

//-------------------------------------------------------

int StackPush (Stack_t* stack, StackElem_t elem_push)
    {
    STACK_ASSERT (stack);

    if (stack->size == stack->capacity)
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, CAPACITY_GROWTH * stack->capacity, sizeof (StackElem_t));
        assert (stack->data != NULL);
        stack->capacity *= 2;
        }

    stack->data[stack->size++] = elem_push;

    STACK_ASSERT (stack);
    
    printf ("Pushed element (StackPush): %d \n", stack->data[stack->size - 1]);

    return 0; //исправить
    }

//-------------------------------------------------------

int StackPop (Stack_t* stack, StackElem_t* elem_pop)
    {
    STACK_ASSERT (stack);

    if (stack->size == stack->capacity / 4)
        {
        stack->data = (StackElem_t*) MyRecalloc (stack->data, stack->capacity / CAPACITY_DECREASE, sizeof (StackElem_t));
        assert (stack->data != NULL);
        stack->capacity /= 4;
        }

    *elem_pop = stack->data[stack->size - 1];
    stack->size -= 1;

    STACK_ASSERT (stack);

    printf ("Popped element (StackPop): %d \n", *elem_pop);

    return 0; //исправить
    }
//-------------------------------------------------------

int StackOk (Stack_t* stack)
    {
    if (stack->data == NULL)
        return BAD_DATA;
    if (stack->size > stack->capacity)
        return BAD_SIZE;
    if (stack->size < 0)
        return NEGATIVE_SIZE;
    return OK;
    }

//-------------------------------------------------------

void StackDump (Stack_t* stack)
    {
    printf ("Stack_t [%p] \n", stack);
    printf ("\t { \n");
    printf ("\t size = %d \n", stack->size);
    printf ("\t capacity = %d \n", stack->capacity);
    printf ("\t data[%p]: \n", stack->data);
    printf ("\t\t { \n");
    for (int i = 0; i < stack->capacity; i++)
        {
        printf ("\t\t [%d] = %d; \n", i, stack->data[i]);
        }
    printf ("\t\t } \n");
    printf ("\t } \n");
    }

//-------------------------------------------------------

int StackDestroy (Stack_t* stack)
    {
    memset (stack->data, 0, stack->capacity);
    free(stack->data); stack->data = NULL;
    
    return 0; //исправить
    }

//-------------------------------------------------------

void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements)
    {
    memory = realloc (memory, n_elements * size_elements);
    memset (memory, 0, n_elements);
    return memory;
    }

//-------------------------------------------------------