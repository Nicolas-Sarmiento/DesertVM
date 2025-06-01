#include "stack/stack.h"
int push( Stack* s, Value v ){
    if( s->pointer >= STACK_SIZE ){
        printf("Error: Stack Overflow!!\n");
        return 1;
    }

    s->vals[s->pointer++] = v;
    return 0;
}

int pop (Stack * s, Value* v){
    if ( s->pointer == 0 ){
        printf("Error: Trying to pop in an empty stack\n");
        return 1;
    }
    *v = s->vals[--(s->pointer)];
    return 0;
}

void free_stack(Stack* s){
    for(size_t i = 0; i < s->pointer; i++ ){
        if( s->vals[i].type == STR ){
            free( s->vals[i].literal);
        }
    }
}