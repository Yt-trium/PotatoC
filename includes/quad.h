#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"

enum OpType{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQ,
    NE
};

typedef struct quad{
    int id;
    enum OpType type;
    symbol * res;
    symbol * left;
    symbol * right;
    struct quad * next;
} quad;

quad * alloc_quad();

quad * add_quad(quad ** head, 
                enum OpType type, 
                symbol * res, 
                symbol * left,
                symbol * right);

// Append listRight to listLeft
void append_quad(quad * listLeft, quad * listRight);

void print_quad(quad * head);


#endif
