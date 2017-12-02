%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"

  int yylex();
  void yyerror(char*);
  symbol *st = NULL;
%}

%union {
  char* string;
  int value;
  struct ast* ast;
}

%token <value> INT STENCIL
%token <value> IF ELSE WHILE FOR RETURN
%token <value> CONSTANT ASSIGN PLUS MINUS MULT DIVI
%token <value> END
%token <string> IDENTIFIER

%type <ast> expr assign id

%left PLUS
%left MINUS
%left MULT
%left DIVI

%%

axiom:
    | expr END {    printf("axiom -> expr ;\n");
                    ast_print($1, 0);
                    ast_codegen($1, st);}
    | assign END {  printf("axiom -> assign ;\n");
                    ast_print($1, 0);
                    ast_codegen($1, st);}
    // | axiom axiom
  ;

assign:
    id ASSIGN expr    { $$ = ast_new_statement($1, $3); }
    ;

expr:
    expr PLUS expr  { $$ = ast_new_operation(AST_OP_PLUS, $1, $3); }
  | expr MINUS expr { $$ = ast_new_operation(AST_OP_MINUS, $1, $3); }
  | expr MULT expr  { $$ = ast_new_operation(AST_OP_MULT, $1, $3); }
  | expr DIVI expr  { $$ = ast_new_operation(AST_OP_DIVI, $1, $3); }
  | '(' expr ')'    { $$ = $2; }
  | IDENTIFIER      { $$ = ast_new_id($1); }
  | CONSTANT        { $$ = ast_new_number($1); }
  ;

id: IDENTIFIER { $$ = ast_new_id($1); };


%%


int main() {
  symbol_new_temp(st, 0) ;
  printf("PotatoC 1.0\n");
  return yyparse();
}
