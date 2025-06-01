#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/stack.h"
#include "var_table/varTable.h"
#include "Values/values.h"
#include "parser/parser.h"


typedef enum err_code{
    OK, ZERO_DIV, STACK_ERROR, UNKNOW_VAR, UNKNOWN_LABEL, TYPES_ERROR, SYMBOL_TABLE_ERROR, UNKNOWN_INSTRUCTION
}ErrorCode;

ErrorCode run_vm ( Bytecode code, int* result );
void print_runtime_error( ErrorCode code );

#endif //VM_H