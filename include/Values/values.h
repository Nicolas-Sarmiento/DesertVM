#ifndef VALUES_H
#define VALUES_H

typedef enum {
    INT,
    STR
} ValueType;

typedef struct value {
    ValueType type;
    union {
        int val;
        char* literal;
    };
    
} Value;

#endif //VALUES_H