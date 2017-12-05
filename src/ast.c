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

quad ast_codegen(ast *ast, symbol* symbol_table)
{
    symbol left;
    symbol right;
    symbol res;

    switch(ast->type)
    {
    case AST_NUMBER:
        // oui mais non
        //symbol tmp = symbol_new_temp(symbol_table, 0);
        //return quad_add(&quad_list, quad_gen(QUAD_OP_PLUS, tmp, left, right));
        break;
    case AST_ID:
        break;
    case AST_OP_PLUS:
        left = ast_codegen(ast->u.statement.left,symbol_table);
        right = ast_codegen(ast->u.statement.right,symbol_table);
        //res = symbol_new_temp(symbol_table, 0);
        quad_add(ql,quad_gen(QUAD_OP_PLUS,res,left,right));

        return res;
    break;
    default:
        break;
    }
}
