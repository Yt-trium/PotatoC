#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"
#include <stdbool.h>

enum OpType{
    QUAD_OP_PLUS,
    QUAD_OP_MINUS,
    QUAD_OP_MULT,
    QUAD_OP_DIVI,
    QUAD_UOP_ASSIGN,
    QUAD_UOP_MINUS,
    QUAD_UOP_PLUS,
    QUAD_GOTO_IF,
    QUAD_GOTO
};

enum RelOp{
    QUAD_RELOP_EQ,
    QUAD_RELOP_NEQ,
    QUAD_RELOP_GT,
    QUAD_RELOP_GTE,
    QUAD_RELOP_LT,
    QUAD_RELOP_LTE

};

typedef struct quad_{
    enum OpType type;
    enum RelOp cmp;
    int id;
    symbol res;
    symbol left;
    symbol right;
    struct quad_* dest;
} quad_, *quad;

typedef struct quad_list_
{
    struct quad_* q;
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
 * @brief quad_goto_gen
 * @return
 */
quad quad_ifgoto_gen(symbol, enum RelOp, symbol);

quad quad_goto_gen();

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
 * @param   The list of quad
 * @param   If true, will free quads inside the list
 */
void quad_list_free(quad_list, bool);

quad_list*  quad_list_new(quad*);
void        quad_list_add(quad_list**, quad_list*);

void        quad_list_complete(quad_list, quad);

/**
  * @biref Create a new list with the concatenation of two given list
  * @param      First list
  * @param      Second list
  * @return     Concatened list
  */
quad_list quad_list_concat(quad_list, quad_list);

/**
  * @brief Remove unlinked goto quads from the given list
  * @return Number of removed quad
  */
int quad_list_clean_gotos(quad_list);

/**
  * Find the specified quad in the specified list
  * @param  The list
  * @param  The quad id
  * @return The quad list element containing the wanted quad
  */
quad_list quad_list_find(quad_list, int);


#endif
