#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "Values/values.h"

#define STACK_SIZE 1024

typedef struct stack{
    Value vals [STACK_SIZE];
    size_t pointer;
} Stack;

int push( Stack* s, Value v );
int pop (Stack * s, Value* v);

void free_stack(Stack* s);

#endif //STACK_H