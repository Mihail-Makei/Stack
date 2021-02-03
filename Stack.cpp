#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

///----------------------------------------------------------------------------///

#undef struct
#undef stack_t
#undef my_stack
#undef verificator
#undef StackPush
#undef StackCreate
#undef StackTop
#undef StackPop
#undef StackResizeUp
#undef StackResizeDown
#undef StackHash
#undef StackDestruct
#undef StackDump
#undef stk
#undef Err

///----------------------------------------------------------------------------///


int Err = 0; //ERRCODE


const int MAXSIZE = 100;
const int MULTIPLIER = 2;
const int HYSTERESIS = 10;
const int INIT_SIZE = 10;
const int KAN = 2910202;
const int POISON = 2905205;
const int ERR_MAGIC1 = 101;
const int ERR_HASH = 102;
const int ERR_EMPTY = 103;
const int ERR_MAGIC2 = 104;
const int ERR_SIZE = 105;
const int ERR_SIZE_EXCEED = 106;
const int ERR_NULLPOINTER = 107;
const int ERR_REALLOC = 108;

///----------------------------------------------------------------------------///

typedef struct my_stack
{
    int kan1;                               //Magic number 1
    int *data;                              //Data
    int size;                               //Stack size
    int capacity;                           // Capacity
    double hash;                            //Stack hash
    int kan2;                               //Magic number 2
} stack_t;

///----------------------------------------------------------------------------///

void StackCreate (stack_t* stk);            //Stack constructor

double StackHash (stack_t* stk);            //Hash function

int StackPush (stack_t* stk, int value);   //Push element

int StackPop (stack_t* stk);                //Pop element

bool verificator(stack_t *stk);             //Verificator

void StackDump(stack_t* stk);               //Dump

int StackTop(stack_t* stk);                 //See the last element

int StackResizeUp(stack_t* stk);           //Capacity increase

int StackResizeDown(stack_t* stk);         //Capacity decrease

void StackDestruct(stack_t* stk);           //Stack destructor

void ErrPrint(stack_t stk);                 //Print error message

void test();                                //Unitest

///----------------------------------------------------------------------------///

int main() {
    test();
    return 0;
}

///----------------------------------------------------------------------------///

void StackCreate(stack_t* stk)
{
    stk->kan1 = KAN;
    stk->kan2 = KAN;
    stk->data = (int*)malloc(INIT_SIZE*sizeof(int));
    stk->size = 0;
    stk->capacity = 10;
    for (int i = 0; i < stk->capacity; i++)
        stk->data[i] = POISON;
    stk->hash = StackHash(stk);
}

///----------------------------------------------------------------------------///

int StackResizeUp(stack_t* stk)
{
    assert(verificator(stk));

    stk->capacity = stk->capacity * MULTIPLIER;
    int* data = (int* )realloc(stk->data, stk->capacity * sizeof(int));

    if (!data)
    {
        Err = ERR_REALLOC;
        assert(stk->data);
        return 0;
    }

    stk->data = data;
    return 1;
}

///----------------------------------------------------------------------------///

int StackResizeDown(stack_t* stk)
{
    assert(verificator(stk));

    stk->capacity = stk->capacity - HYSTERESIS;
    int* data = (int* )realloc(stk->data, stk->capacity);

    if (!data)
    {
        Err = ERR_REALLOC;
        assert(stk->data);
        return -1;
    }

    stk->data = data;
    return 1;
}

///----------------------------------------------------------------------------///

void StackDestruct(stack_t* stk)
{
    assert(verificator(stk));

    free(stk->data);
    stk->data = NULL;
}

///----------------------------------------------------------------------------///

double StackHash(stack_t* stk)
{
    double hash = 0;
    for (int i = 0; i < stk->capacity; i++)
        hash = hash * 2 + stk->data[i];
    return hash;
}

///----------------------------------------------------------------------------///

int StackPush(stack_t* stk, int value)
{
    assert (verificator(stk));

    if (stk->size == stk->capacity)
        if (!StackResizeUp(stk))
            return 0;

    stk->data[stk->size] = value;
    stk->size++;
    stk->hash = StackHash(stk);
    return 1;
}

///----------------------------------------------------------------------------///

int StackTop(stack_t* stk)
{
    assert(verificator(stk));

    if (stk->size == 0)
    {
        Err = ERR_EMPTY;
        return -1;
    }

    return stk->data[stk->size - 1];
}

///----------------------------------------------------------------------------///

int StackPop(stack_t* stk)
{
    assert(verificator(stk));
    if (stk->size == 0)
    {
        Err = ERR_EMPTY;
        return -1;
    }

    stk->size--;
    int a = stk->data[stk->size];
    stk->data[stk->size] = POISON;

    if (stk->size < (stk->capacity - HYSTERESIS))
        StackResizeDown(stk);

    stk->hash = StackHash(stk);
    return a;
}

///----------------------------------------------------------------------------///

bool verificator(stack_t *stk)
{
    if (!stk)
    {
        Err = ERR_NULLPOINTER;
        return false;
    }

    if (stk->data == NULL)
    {
        Err = ERR_NULLPOINTER;
        return false;
    }

    if (stk->kan1 != KAN)
    {
        Err = ERR_MAGIC1;
        return false;
    }

    if (stk->kan2 != KAN)
    {
        Err = ERR_MAGIC2;
        return false;
    }

    if (stk->hash != StackHash(stk))
    {
        Err = ERR_HASH;
        return false;
    }

    if (stk->size > stk->capacity)
    {
        Err = ERR_SIZE_EXCEED;
        return false;
    }

    if (stk->size < 0)
    {
        Err = ERR_SIZE;
        return false;
    }

    return true;
}

///----------------------------------------------------------------------------///

void ErrPrint(stack_t stk)
{
    int dump = 0;
    scanf("%d", &dump);
    switch (Err)
    {
        case ERR_NULLPOINTER:
            printf("Error %d. Stack not available", ERR_NULLPOINTER);
            break;

        case ERR_MAGIC1:
            printf("Error %d. Stack is spoilt\n", ERR_MAGIC1);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_EMPTY:
            printf("Error %d. Stack is empty\n", ERR_EMPTY);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_HASH:
            printf("Error %d. Hash error\n", ERR_HASH);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_MAGIC2:
            printf("Error %d. Stack is spoilt\n", ERR_MAGIC2);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_SIZE:
            printf("Error %d. Stack size is below zero\n", ERR_SIZE);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_SIZE_EXCEED:
            printf("Error %d. Stack size exceeded\n", ERR_SIZE_EXCEED);
            if (dump)
                StackDump(&stk);
            break;

        case ERR_REALLOC:
            printf("Error %d. Reallocation error");
            break;

    }
}

///----------------------------------------------------------------------------///

void StackDump(stack_t* stk)
{
    printf("Size of stack %d\n", stk->size);
    int i = stk->capacity;
    while (i > 0)
    {
        i--;
        int num = stk->data[i];
        printf("ELEMENT %d VALUE %d\n", i, num);
    }
}

///----------------------------------------------------------------------------///

void test() {
    {
        stack_t stk = { 0,  0, 0, 0, 0, 0};
        StackCreate(&stk);
        for (int i = 0; i < 10; i++)
            StackPush(&stk, i);
        int n[10] = { 0 };
        for (int i = 0; i < 10; i++)
            n[i] = StackPop(&stk);

        assert(stk.size == 0);
        for (int i = 9; i >= 0; i--)
            assert(n[i] == (9 - i));

        if (Err)
            ErrPrint(stk);
        else
        {
            printf("TEST 1 OK\n");
            StackDestruct(&stk);
        }
    }

    {
        stack_t stk = { 0,  0, 0, 0, 0, 0};
        StackCreate(&stk);
        for (int i = 0; i < 10; i++)
            StackPush(&stk, i);

        assert(stk.size == 10);

        if (Err)
            ErrPrint(stk);
        else
        {
            printf("TEST 2 OK\n");
            StackDestruct(&stk);
        }
    }

    {
        stack_t stk = { 0,  0, 0, 0, 0, 0};
        StackCreate(&stk);
        for (int i = 0; i <= MAXSIZE*MAXSIZE; i++)
            StackPush(&stk, 10);

        if (Err)
            ErrPrint(stk);
        else
        {
            printf("TEST 3 OK\n");
            StackDestruct(&stk);
        }
    }
}
