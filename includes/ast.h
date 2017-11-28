#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "quad.h"

typedef struct ast {
    char* type;
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

ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_statement(ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
void ast_print(ast*, int);
struct quad* ast_codegen(ast*, struct symbol**);
