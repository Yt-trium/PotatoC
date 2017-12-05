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
  //quad * qt = NULL;
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

%type <ast> assign id
%type <sym> expr

%left PLUS
%left MINUS
%left MULT
%left DIVI

%%

axiom:
    | expr END {    printf("axiom -> expr ;\n");
                    //ast_print($1, 0);
                    //ast_codegen($1, &st);
                    }
    | assign END {  printf("axiom -> assign ;\n");
                    //ast_print($1, 0);
                    //ast_codegen($1, &st);
                    }
    // | axiom axiom
  ;

assign:
    id ASSIGN expr    { 
    //$$ = ast_new_statement($1, $3); 
    }
    ;

expr:
    expr PLUS expr  { 
    //$$ = ast_new_operation(AST_OP_PLUS, $1, $3);
    }
  | expr MINUS expr { 
  //$$ = ast_new_operation(AST_OP_MINUS, $1, $3);
  }
  | expr MULT expr  { 
  //$$ = ast_new_operation(AST_OP_MULT, $1, $3); 
  }
  | expr DIVI expr  { 
  //$$ = ast_new_operation(AST_OP_DIVI, $1, $3); 
  }
  | '(' expr ')'    { 
  //$$ = $2; 
  }
  | IDENTIFIER      { 
  //$$ = ast_new_id($1); 
  }
  | CONSTANT        { 
    //$$ = ast_new_number($1); 
    //$$ = add_symbol(&head_symbol, "Number");
    $$ = symbol_new_temp(&st, $1);
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
  //quad_print(qt);
  return 0;
}
