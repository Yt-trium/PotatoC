#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"

enum OpType{
    QUAD_OP_PLUS,
    QUAD_OP_MINUS,
    QUAD_OP_MULT,
    QUAD_OP_DIVI
};

typedef struct quad_{
    int id;
    enum OpType type;
    symbol res;
    symbol left;
    symbol right;
    struct quad_ * next;
} quad;


quad * alloc_quad();

quad * add_quad(quad ** head, 
                enum OpType type, 
                symbol res,
                symbol left,
                symbol right);

// Append listRight to listLeft
void append_quad(quad * listLeft, quad * listRight);

void quad_print(quad * head);


typedef struct quad_list
{
    quad *q;
    int count;
    struct quad_list *previous;
    struct quad_list *next;
}quad_list;

quad_list*  quad_list_new(quad*);
void        quad_list_free(quad_list*);
void        quad_list_complete(quad_list*, symbol);
void        quad_list_add(quad_list**, quad_list*);
void        quad_list_print (quad_list *);


#endif
