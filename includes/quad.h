#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"
#include <stdbool.h>

/** @enum OpType
  * @brief The type that a quad can have
  */
enum OpType{
    QUAD_OP_PLUS,
    QUAD_OP_MINUS,
    QUAD_OP_MULT,
    QUAD_OP_DIVI,
    QUAD_OP_MODULO,
    QUAD_UOP_ASSIGN,
    QUAD_UOP_MINUS,
    QUAD_UOP_PLUS,
    QUAD_GOTO_IF,
    QUAD_GOTO_END, // Exit the program
    QUAD_GOTO,
    QUAD_PRINTI
};

/** @enum RelOp
  * @brief The type of comparaison in an if statement
  */
enum RelOp{
    QUAD_RELOP_EQ,  // ==
    QUAD_RELOP_NEQ, // !=
    QUAD_RELOP_GT,  // >
    QUAD_RELOP_GTE, // >=
    QUAD_RELOP_LT,  // <
    QUAD_RELOP_LTE  // <=

};

/** @struct quad_
  * @brief A quad, typically an instruction in the resulting program
  * @member type    The type of the instruction
  * @member cmp     The type of the comparaison if any (if statement)
  * @member id      The id of the instruction, starting at 0.
  * @member res     The resulting symbol if any
  * @member left    The left symbol of the operation if any
  * @member right   The right symbol of the operation if any
  * @member dest    The destination quad if any (goto)
  */
typedef struct quad_{
    enum OpType type;
    enum RelOp cmp;
    int id;
    symbol res;
    symbol left;
    symbol right;
    struct quad_* dest;
} quad_, *quad;

/** @struct quad_list_
  * @brief A double chained list of quad
  * @member q           The current quad
  * @member previous    The previous quad in the list
  * @member next        The next quad in the list
  */
typedef struct quad_list_
{
    struct quad_* q;
    struct quad_list_ *previous; // Not really usefull for the moment
    struct quad_list_ *next;
}quad_list_, *quad_list;

/**
  * @brief Allocate in memory a new quad
  * @return The allocated quad
  */
quad alloc_quad();

/**
  * @brief Generate a binary operation quad (3 adresses)
  * @param type         The type of the quad
  * @param res          The output symbol
  * @param left         The left operand symbol
  * @param right        The right operand symbol
  * @return             The generated quad
  */
quad quad_gen(enum OpType type, symbol res, symbol left, symbol right);

/**
  * @brief Generate a new quad for an unary operation (2 adresses)
  * @param      Type of the operation
  * @param      Output symbol
  * @param      Right symbol
  * @return     The generated quad
  */
quad quad_unary_gen(enum OpType type, symbol res, symbol right);

/**
 * @brief Generate a quad goto.
 * Two symbols and a comparaison are given.
 * The destination quad is not yet assigned, it has to be completed in the main program.
 * @param   The left symbol
 * @param   The comparaison type
 * @param   The right symbol
 * @return  The generated quad
 */
quad quad_ifgoto_gen(symbol, enum RelOp, symbol);

/**
  * @brief Generate a direct goto quad.
  * No checks, goto directly to the quad.
  * The destination quad is not yet assigned, it has to be completed in the main program.
  * @return The generated quad
  */
quad quad_goto_gen();

/**
  * @brief Create a printi instruction for a given symbol
  * @param  The symbol to print
  * @return The generated quad
  */
quad quad_printi_gen(symbol);

/**
  * @brief Add a quad in the given list
  * @param  The list of quad
  * @param  The quad to add
  * @return The quad list element containing the given quad
  */
quad_list quad_add(quad_list* head, quad q);

/**
  * @brief Allocate in memory a new quad list element
  * @return     The allocated struct
  */
quad_list quad_list_alloc();

/**
  * @brief  Completed all empty goto quads of the list with the given quad
  * @param  The quad list
  * @param  The quad used to complete
  */
void quad_list_complete(quad_list, quad);

/**
  * @biref Create a new list with the concatenation of two given list
  * Will create a new list in memory.
  * @param      First list
  * @param      Second list
  * @return     Concatened list
  */
quad_list quad_list_concat(quad_list, quad_list);

/**
 * @brief Append an element to the given list
 * @return The given list that will be created if is null or the given one
 */
quad_list quad_list_append(quad_list*, quad);

/**
  * @brief Detect unlinked quads and link them to the end (exit)
  * Usefull when the last instruction is an if statement to link the else to the end
  * @param  The quad list
  * @return Number of cleaned quad
  */
int quad_list_clean_gotos(quad_list);

/**
  * @brief Find the specified quad in the specified list
  * @param  The list
  * @param  The quad id
  * @return The quad list element containing the wanted quad or NULL
  */
quad_list quad_list_find(quad_list, int);

/**
  * @brief Print a given quad
  * @param  The quad
  */
void quad_print(quad);

/**
  * @brief Print all the elements of the given quad_list
  * @param      The list
  */
void quad_list_print (quad_list);

/**
 * @brief   Free the memory used by the given list
 * @param   The list of quad
 * @param   If true, will free quads inside the list
 */
void quad_list_free(quad_list, bool);

#endif
