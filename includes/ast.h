#ifndef AST_H
#define AST_H

#include "quad.h"

quad *ql;

typedef enum ast_type
{
    AST_NUMBER,
    AST_ID,
    AST_STATEMENT,
    AST_OP_PLUS,
    AST_OP_MINUS,
    AST_OP_MULT,
    AST_OP_DIVI

} ast_type;

typedef struct ast {
    ast_type type;
    union {
        struct {
            struct ast* left;
            struct ast* right;
        } operation;
        struct {
            struct ast* left;
            struct ast* right;
        } statement;
        int number;
        char* id;
    } u;
} ast;

ast* ast_new_operation(ast_type, ast*, ast*);
ast* ast_new_statement(ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
void ast_print(ast*, int);
quad ast_codegen(ast*, symbol*);

#endif // AST_H
