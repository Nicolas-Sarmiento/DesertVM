#include <string.h>
#include <stdlib.h>
#include "parser/parser.h"

const char* OP[] = { 
    "PUSH",
    "PUSH_STR",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "AND",
    "OR",
    "EQ",
    "NEQ",
    "GT",
    "NOT",
    "PRINT",
    "STORE",
    "LOAD",
    "LBL",
    "JMP",
    "JZ",
    "RET","INVALID"
};

const Opcode no_args[] = {
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
    OP_RETURN
};

void print_OP( Opcode code ){
    printf("< %s >\n", OP[code]);
}


Opcode str_to_opcode(const char *s){
    size_t ln = sizeof(OP)/sizeof(OP[0]);
    for( size_t i = 0; i < ln; i++){
        if( strcmp(s, OP[i]) == 0) {
            return (Opcode) i;
        }
    }
    return OP_INVALID;
}

int op_no_args ( Opcode code ){
    size_t l = sizeof(no_args)/sizeof(no_args[0]);
    for( size_t i = 0; i < l; i++){
        if( code == no_args[i]) return 1;
    }
    return 0;
}

Bytecode parse_bytecode( FILE* in){
    Bytecode bcode;
    bcode.instructions = malloc(sizeof(Instruction) * MAX_LINES);
    bcode.labels = malloc(sizeof(Label) * MAX_LABELS);
    bcode.count_lines = 0;
    bcode.count_labels = 0;

    char buff [MAX_LINE_LEN];

    while (fgets(buff, sizeof(buff), in)) {
        buff[strcspn(buff, "\r\n")] = 0;

        char op_str[10];
        char arg[90];
        int matched = sscanf(buff, "%10s %89s", op_str, arg);

        Opcode opcode = str_to_opcode(op_str); 

        Instruction inst = {0};
        inst.opcode = opcode;
        inst.int_arg = -1;

        if ( !op_no_args(inst.opcode) && matched != 2 ){
            printf("Error: %s TOKEN not mathed with the properly args.\n", op_str);
            bcode.valid = 0;
            return bcode;
        }

        switch (inst.opcode){
            case OP_PUSH:{
                inst.int_arg = atoi(arg);
                break;    
            }
            case OP_LABEL:
                Label lb;
                lb.label = strdup(arg);
                lb.instr_pos = bcode.count_lines;
                bcode.labels[bcode.count_labels++] = lb;
                break;
            case OP_PUSH_STR:
            case OP_LOAD:
            case OP_JMP:
            case OP_JZ:
            case OP_STORE:
                inst.str_arg = strdup(arg);
                break;
            default:
                break;
        }
       

        bcode.instructions[bcode.count_lines++] = inst;
    }
    bcode.valid = 1;
    return bcode;
}