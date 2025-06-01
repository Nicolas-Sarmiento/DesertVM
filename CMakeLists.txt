cmake_minimum_required(VERSION 3.10)
project(DessertVM)

set(CMAKE_C_STANDARD 99)

include_directories(include)
include_directories(src)


set(SOURCES
    src/main.c
    src/parser/parser.c
    src/stack/stack.c
    src/var_table/varTable.c 
    src/vm/vm.c
)


add_executable(dessert ${SOURCES})

