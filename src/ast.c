#include "ast.h"

ast* ast_new_operation(char* op, ast* left, ast* right) {
  ast* new = malloc(sizeof(ast));
  new->type = strdup(op);
  new->u.operation.left = left;
  new->u.operation.right = right;
  return new;
}

ast* ast_new_number(int number) {
  ast* new = malloc(sizeof(ast));
  new->type = strdup("number");
  new->u.number = number;
  return new;
}

ast* ast_new_id(char* id) {
  ast* new = malloc(sizeof(ast));
  new->type = strdup("id");
  new->u.id = strdup(id);
  return new;
}

void ast_print(ast* ast, int indent) {
  int i;
  for (i = 0; i < indent; i++)
    printf("    ");
  printf("%s", ast->type);
  if (strcmp(ast->type, "number") == 0)
    printf(" (%d)\n", ast->u.number);
  else if (strcmp(ast->type, "id") == 0)
    printf(" (%s)\n", ast->u.id);
  else {
    printf("\n");
    ast_print(ast->u.operation.left, indent + 1);
    ast_print(ast->u.operation.right, indent + 1);
  }
}
