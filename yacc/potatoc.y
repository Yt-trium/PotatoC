%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"

  int yylex();
  void yyerror(char*);
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


%type <ast> expr

%left PLUS
%left MINUS
%left MULT
%left DIVI

%%

axiom:
    | expr END { printf("Chaine reconnue !\n");
                 ast_print($1, 0);
                 exit(0);
               }
    | assign END {printf("Chaine reconnue !\n");}
  ;

assign:
    | IDENTIFIER ASSIGN expr    {}
    ;

expr:
    expr PLUS expr { $$ = ast_new_operation("+", $1, $3); }
  | expr MINUS expr { $$ = ast_new_operation("-", $1, $3); }
  | expr MULT expr { $$ = ast_new_operation("*", $1, $3); }
  | expr DIVI expr { $$ = ast_new_operation("/", $1, $3); }
  | '(' expr ')'  { $$ = $2; }
  | IDENTIFIER            { $$ = ast_new_id($1); }
  | CONSTANT        { $$ = ast_new_number($1); }
  ;

%%


int main() {
  printf("PotatoC 1.0\n");
  return yyparse();
}
