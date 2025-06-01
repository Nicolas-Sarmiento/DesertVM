#ifndef VARTABLE_H
#define VARTABLE_H

#include "Values/values.h"
#define MAX_VAR_TABLE_SIZE 1024

typedef struct variable{
    char* name;
    Value value;
} Variable;

typedef struct vartable {
    int count;
    Variable table[MAX_VAR_TABLE_SIZE];
}VarTable;

Value* get_variable ( VarTable* table, const char* name );
int put_variable ( VarTable* table, Variable v );
void free_table( VarTable* table ); 


#endif //VARTABLE_H