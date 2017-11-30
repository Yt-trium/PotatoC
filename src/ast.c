#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "symbol.h"
#include "ast.h"

ast* ast_new_operation(ast_type op, ast* left, ast* right) {
  ast* new = malloc(sizeof(ast));
  new->type = op;
  new->u.operation.left = left;
  new->u.operation.right = right;
  return new;
}

ast *ast_new_statement(ast* left, ast* right)
{
    ast* new = malloc(sizeof(ast));
    new->type = AST_STATEMENT;
    new->u.operation.left = left;
    new->u.operation.right = right;
    return new;
  }

ast* ast_new_number(int number) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_NUMBER;
  new->u.number = number;
  return new;
}

ast* ast_new_id(char* id) {
  ast* new = malloc(sizeof(ast));
  new->type = AST_ID;
  new->u.id = strdup(id);
  return new;
}

void ast_print(ast* ast, int indent) {
  int i;
  for (i = 0; i < indent; i++)
    printf("    ");
  printf("%d", ast->type);
  if (ast->type == AST_NUMBER)
    printf(" (%d)\n", ast->u.number);
  else if (ast->type == AST_ID)
    printf(" (%s)\n", ast->u.id);
  else {
    printf("\n");
    ast_print(ast->u.operation.left, indent + 1);
    ast_print(ast->u.operation.right, indent + 1);
  }
}

struct quad* ast_codegen(ast *ast, symbol_list symbol_table)
{
    quad* left;
    quad* right;
    quad* cg;

    switch(ast->type)
    {
    case AST_OP_PLUS:
        left = ast_codegen(ast->u.statement.left,symbol_table);
        right = ast_codegen(ast->u.statement.right,symbol_table);
        quad* new_code = add_quad(NULL,QUAD_OP_PLUS,cg->res, left->res , right->res);
        /*
        cg->result = symbol_new_temp(symbol_table);
        cg->code = left->code;
        quad_add(&cg->code, right->code);
        quad_add(&cg->code, new_code);
        */
    break;
    default:
        break;
    }
}
