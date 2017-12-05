#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"

enum OpType{
    QUAD_OP_PLUS,
    QUAD_OP_MINUS,
    QUAD_OP_MULT,
    QUAD_OP_DIVI,
    QUAD_UOP_ASSIGN,
    QUAD_UOP_MINUS
};

typedef struct quad_{
    enum OpType type;
    int id;
    symbol res;
    symbol left;
    symbol right;
} quad_, *quad;

typedef struct quad_list_
{
    quad q;
    struct quad_list_ *previous;
    struct quad_list_ *next;
}quad_list_, *quad_list;

/**
  * Allocate in memory a new quad
  * @return The allocated quad
  */
quad alloc_quad();

/**
  * Generate a new quad
  * @param type         The type of the quad
  * @param res          The output 
  * @param left         The left operand
  * @param right        The right operand
  * @return             The generated quad
  */
quad quad_gen(enum OpType type, symbol res, symbol left, symbol right);

/**
  * Generate a new quad for an unary operation
  * @param      Type of the operation
  * @param      Output symbol
  * @param      Right symbol
  * @return     The generated quad
  */
quad quad_unary_gen(enum OpType type, symbol res, symbol right);

/**
  * Add a quad in the list
  * @param  The list of quad
  * @param  The quad to add
  * @return The linked quad list element
  */
quad_list quad_add(quad_list* head, quad q);

// Append listRight to listLeft
//void append_quad(quad listLeft, quad listRight);

/**
  * Print a quad
  * @param q    The quad
  */
void quad_print(quad q);

/**
  * Allocate a new quad list
  * @return     The allocated struct
  */
quad_list quad_list_alloc();

/**
  * Print a list of quad
  * @param      The list
  */
void quad_list_print (quad_list);

/**
 * @brief quad_free_memory
 */
void quad_free_memory(quad_list);

quad_list*  quad_list_new(quad*);
void        quad_list_free(quad_list*);
void        quad_list_complete(quad_list*, symbol);
void        quad_list_add(quad_list**, quad_list*);


#endif
