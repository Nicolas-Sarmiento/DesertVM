#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_LINES    1000
#define MAX_LABELS   1000
#define MAX_LINE_LEN 100


typedef enum {
    OP_PUSH,
    OP_PUSH_STR,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_AND,
    OP_OR,
    OP_EQ,
    OP_NEQ,
    OP_GT,
    OP_NOT,
    OP_PRINT,
    OP_STORE,
    OP_LOAD,
    OP_LABEL,
    OP_JMP,
    OP_JZ,
    OP_RETURN,
    OP_INVALID
} Opcode;

typedef struct instruction {
    Opcode opcode;
    int int_arg;             
    char *str_arg;     
} Instruction;

typedef struct label {
    char* label;
    size_t instr_pos;
} Label;

typedef struct  bytecode {
    Instruction* instructions;
    Label* labels;
    size_t count_lines;
    size_t count_labels;
    int valid;
} Bytecode;

Opcode str_to_opcode(const char *s);

Bytecode parse_bytecode( FILE* in);

void print_OP( Opcode code );

#endif //PARSER_H