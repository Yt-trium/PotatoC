%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"
  #include "symbol.h"
  #include "test.h"
  #include "quad.h"

  int yylex();
  void yyerror(char*);
  symbol st = NULL;
  quad_list qt = NULL;
%}

%union {
  char* string;
  int value;
  struct ast* ast;
  struct symbol_* sym;
}

%token <value> INT STENCIL
%token <value> IF ELSE WHILE FOR RETURN
%token <value> CONSTANT ASSIGN PLUS MINUS MULT DIVI
%token <value> END
%token <string> IDENTIFIER

%type <sym> expr instr assign id

%left PLUS
%left MINUS
%left MULT
%left DIVI

%start axiom

%%

axiom:
     instr END axiom
    | instr
  ;

instr:
     assign
     | expr

assign:
    id ASSIGN expr    { 
    $$ = symbol_new(&st, $1);
    quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, $$, $3));
    //$$ = ast_new_statement($1, $3);
    }
    ;

expr:
    expr PLUS expr  { 
    //$$ = ast_new_operation(AST_OP_PLUS, $1, $3);
    $$ = symbol_new_temp(&st);
    quad_add(&qt, quad_gen(QUAD_OP_PLUS, $$, $1, $3));
    }
  | expr MINUS expr { 
  //$$ = ast_new_operation(AST_OP_MINUS, $1, $3);
    $$ = symbol_new_temp(&st);
    quad_add(&qt, quad_gen(QUAD_OP_MINUS, $$, $1, $3));
  }
  | expr MULT expr  { 
  //$$ = ast_new_operation(AST_OP_MULT, $1, $3); 
    $$ = symbol_new_temp(&st);
    quad_add(&qt, quad_gen(QUAD_OP_MULT, $$, $1, $3));
  }
  | expr DIVI expr  { 
  //$$ = ast_new_operation(AST_OP_DIVI, $1, $3); 
    $$ = symbol_new_temp(&st);
    quad_add(&qt, quad_gen(QUAD_OP_DIVI, $$, $1, $3));
  }
  | '(' expr ')'    { 
    $$ = $2; 
  }
  | IDENTIFIER      { 
  //$$ = ast_new_id($1); 
  }
  | CONSTANT        { 
    //$$ = ast_new_number($1); 
    //$$ = add_symbol(&head_symbol, "Number");
    $$ = symbol_new_const(&st, $1);
  }
  ;

id: IDENTIFIER { 
  //$$ = ast_new_id($1); 
  };

%%

int main() {
  printf("PotatoC 1.0\n");
  yyparse();
  printf("Parsing over.\n");
  symbol_list_print(st);
  quad_list_print(qt);
  printf("Debug over.\n");
  return 0;
}
