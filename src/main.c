#include <stdio.h>
#include "vm/vm.h"

int main( int argc, char * argv[] ){
    if (argc != 2 ){
        printf("Dessert VM for CactusC bytecode. Usage: dessert <bytecode_file_name.bcode>\n");
        return 1;
    }
    
    char* bytecode =  argv[1];
    FILE* b_file = fopen(bytecode, "r");
    if ( ! b_file ){
        printf("Error opening the bytecode file!\n");
        return 1;
    }
    
    Bytecode byte_code = parse_bytecode(b_file);

    if ( !byte_code.valid ){
        printf("Error at reading the bytecode\n");
        return 1;
    }

    //printf("inst : %zu . Labels: %zu\n", byte_code.count_lines, byte_code.count_labels);
    // for( size_t i = 0; i < byte_code.count_lines; i++){
    //     print_OP(byte_code.instructions[i].opcode);
    // }
    // for ( size_t i = 0; i < byte_code.count_labels; i++ ){
    //     printf("%s, %zu \n", byte_code.labels[i].label, byte_code.labels[i].instr_pos );
    // }
    int result;
    ErrorCode code_response = run_vm(byte_code, &result);
    if( code_response != OK) {
        print_runtime_error(code_response);
        result = -1;
    }
    
    return result;
}