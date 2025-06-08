#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "var_table/varTable.h"

Value* get_variable ( VarTable* table, const char* name ){
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->table[i].name, name) == 0) {
            return &table->table[i].value;
        }
    }
    return NULL;
}

int put_variable ( VarTable* table, Variable v ){
    if( table->count >= MAX_VAR_TABLE_SIZE ){
        printf("Error: Variable table out of bounds!\n");
        return 1;
    }

    Value* v_old = get_variable(table, v.name );
    if( v_old ){
        *v_old = v.value;
        return 0;
    }

    table->table[table->count++] = v;
    return 0;
}

void free_table( VarTable* table ){
    for (int i = 0; i < table->count; i++) {
        free(table->table[i].name); 
        if( table->table[i].value.type == STR ){
            free(table->table[i].value.literal);
        }
    }
    table->count = 0;
}