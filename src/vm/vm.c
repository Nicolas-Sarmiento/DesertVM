#include "vm/vm.h"


Value make_int_value ( int value ){
    Value v;
    v.type = INT;
    v.val = value;
    return v;
}

Value make_str_value ( const char* value ){
    Value v;
    v.type = STR;
    v.literal = strdup(value);
    return v;
}

Variable make_variable( const char* name, Value v){
    Variable var;
    var.name = strdup(name);
    var.value = v;
    return var;
}

size_t look_label ( Label* labels, size_t size, const char* name ){
    for( size_t i = 0; i < size; i++ ){
        if( strcmp(labels[i].label, name) == 0 ){
            return labels[i].instr_pos;
        }
    }
    return (size_t)-1; 
}


const char* error_msg[] = {"OK", "ZERO_DIV", "STACK_ERROR", "UNKNOW_VAR", "UNKNOWN_LABEL", "TYPES_ERROR",
     "SYMBOL_TABLE_ERROR", "UNKNOWN_INSTRUCTION"
};

void print_runtime_error( ErrorCode code ){
    printf("RUNTIME ERROR: %s !!\n", error_msg[code]);    
}


char* strip_quotes(const char* input) {
    size_t len = strlen(input);
    if (len < 2) return NULL;

    char* result = malloc(len - 1);  // len-2 (sin comillas) + 1 para '\0'
    strncpy(result, input + 1, len - 2);
    result[len - 2] = '\0';
    return result;
}

ErrorCode unexpected_return ( ErrorCode code, Stack* st, VarTable* table ){
    free_stack(st);
    free_table(table);
    return code;
}

ErrorCode run_vm ( Bytecode code, int* result ){
    Stack st;
    st.pointer = 0;

    VarTable table;
    table.count = 0;

    for( size_t ins = 0; ins < code.count_lines; ){
        Instruction instr = code.instructions[ins];

        switch ( instr.opcode )
        {
            case OP_PUSH: {
                Value v = make_int_value(instr.int_arg);
                if( push(&st, v) ) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_PUSH_STR: {
                Value v = make_str_value(instr.str_arg);
                if (push(&st, v) ) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_ADD : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val + b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_SUB : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val - b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_MUL : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val * b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_DIV : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return 1;
                if (b.val == 0) return unexpected_return(ZERO_DIV, &st, &table);
                Value res = make_int_value(a.val / b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_AND : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val && b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_OR : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val || b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_EQ : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val == b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_NEQ : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val != b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_GT : {
                Value a, b;
                if (pop(&st, &b) || pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(a.val > b.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_NOT : {
                Value a;
                if (pop(&st, &a)) return unexpected_return(STACK_ERROR, &st, &table);
                Value res = make_int_value(!a.val);
                if (push(&st, res)) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_PRINT : {
                Value val;
                if (pop(&st, &val)) return unexpected_return(STACK_ERROR, &st, &table);
                if (val.type == INT) {
                    printf("%d\n", val.val);
                } else if (val.type == STR) {
                    printf("%s\n", strip_quotes(val.literal
                    ));
                }
                ins++;
                break;
            }
            case OP_STORE : {
                Value val;
                if (pop(&st, &val)) return unexpected_return(STACK_ERROR, &st, &table);
                if (val.type == INT) {
                    Variable var = make_variable(instr.str_arg, val);
                    if(put_variable(&table,var )) return unexpected_return(SYMBOL_TABLE_ERROR, &st, &table);

                } else if (val.type == STR) {
                    return unexpected_return(TYPES_ERROR, &st, &table);
                }
                ins++;
                break;
            }
            case OP_LOAD : {
                Value* val = get_variable(&table, instr.str_arg);
                if ( !val ) return unexpected_return(UNKNOW_VAR, &st, &table);
                if ( push(&st, *val) ) return unexpected_return(STACK_ERROR, &st, &table);
                ins++;
                break;
            }
            case OP_JMP : {
                size_t new_pos = look_label(code.labels, code.count_labels, instr.str_arg);
                if( new_pos == __SIZE_MAX__ ) return unexpected_return(UNKNOWN_LABEL, &st, &table);
                ins = new_pos;
                break;
            }
            case OP_JZ : {
                Value a;
                if( pop(&st, &a) ) return unexpected_return(STACK_ERROR, &st, &table);
                if ( a.val != 0 ) { ins++; break; }
                size_t new_pos = look_label(code.labels, code.count_labels, instr.str_arg);
                if( new_pos == __SIZE_MAX__ ) return unexpected_return(UNKNOWN_LABEL, &st, &table);
                ins = new_pos;
                break;
            }
            case OP_RETURN : {
                Value a;
                if( pop(&st, &a) ) return unexpected_return(STACK_ERROR, &st, &table);
                *result = a.val;
                return OK;
            }
            case OP_LABEL : 
                ins++;
                break;
            case OP_INVALID:
            default:
                printf("Ivalid Instruction\n");
                return unexpected_return(UNKNOWN_INSTRUCTION, &st, &table);
                break;
        }
    }
    return OK;
}