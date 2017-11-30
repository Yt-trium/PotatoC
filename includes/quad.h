#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"

enum OpType{
    QUAD_OP_PLUS,
    QUAD_OP_MINUS,
    QUAD_OP_MULT,
    QUAD_OP_DIVI
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
