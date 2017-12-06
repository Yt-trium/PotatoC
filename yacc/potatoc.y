%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"
  #include "symbol.h"
  #include "test.h"
  #include "quad.h"

  int yylex();
  void lex_free();
  void yyerror(char*);
  symbol st = NULL;
  quad_list qt = NULL;

  struct expr_node_ update_expr_node(struct expr_node_, symbol, quad_list);
%}

%union {
  char* string;
  int value;
  struct quad_list_* headQuadData; // The start of the block code of the statement
  struct bool_node_ {
      struct quad_list_* truelist;
      struct quad_list_*  falselist;
  } condData;
  struct expr_node_ {
     struct quad_list_* ql;
     struct symbol_* ptr;
  } exprData;
}

%token INT STENCIL
%token IF ELSE WHILE FOR RETURN
%token ASSIGN PLUS MINUS MULT DIVI INC DEC END
%token TRUE FALSE OR AND NOT

%token <value> CONSTANT RELOP
%token <string> IDENTIFIER

%type <condData> condition
%type <headQuadData> statement 
%type <exprData> expr assign

%left OR
%left AND
%left NOT
%right ASSIGN
%left PLUS MINUS
%left MULT DIVI
%left INC DEC

%start axiom

%%

axiom:
  statement_list END

statement_list:
  statement_list END statement {
        printf("New statement\n");
        if($3 != NULL)
            quad_list_complete(qt, $3->q);
    }
  | statement {
        printf("Statement\n");
        quad_list_complete(qt, $1->q);
    }

statement:
  assign {
        $$ = $1.ql;
    }
  | expr {
        $$ = $1.ql;
        if($$ == NULL)
        {
            fprintf(stderr, "ERROR: No instruction generated for the expression.\n");
            YYABORT;
        }
    }
  | IF '(' condition ')' statement {
        quad_list_complete($3.truelist, $5->q);
        // Need to find top quad for this block
        $$ = quad_list_find(qt, $3.truelist->q->id); 
        // We need the top quad list element, but from the global list
        // Free true list and false list but not the quads
        printf("End if\n");
  }


assign:
    IDENTIFIER ASSIGN expr    {
            $$ = $3;
            symbol s = symbol_new(&st, $1);
            quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, s, $3.ptr));
            $$ = update_expr_node($$, s, ql);
        }
  ;

expr:
    expr PLUS expr  { 
        $$ = $1;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_PLUS, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
        }
  | PLUS expr {
        $$ = $2;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_PLUS, s, $2.ptr));
        $$ = update_expr_node($$, s, ql);
      }
  | expr MINUS expr {
        $$ = $1;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_MINUS, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
      }
  | MINUS expr {
        $$ = $2;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_MINUS, s, $2.ptr));
        $$ = update_expr_node($$, s, ql);
      }
  | INC IDENTIFIER  {
        symbol s = symbol_find(st, $2);
        if(s == NULL)
        {
            YYABORT;
        }
        symbol one = symbol_new_const(&st, 1);
        symbol add = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_PLUS, add, s, one));
        quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, s, add));
        $$ = update_expr_node($$, s, ql);
      }
  | DEC IDENTIFIER  {
        symbol s = symbol_find(st, $2);
        if(s == NULL)
        {
            YYABORT;
        }
        symbol one = symbol_new_const(&st, 1);
        symbol sub = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_MINUS, sub, s, one));
        quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, s, sub));
        $$ = update_expr_node($$, s, ql);
      }
  | expr MULT expr  {
        $$ = $1;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_MULT, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
      }
  | expr DIVI expr  { 
        $$ = $1;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_DIVI, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
      }
  | '(' expr ')'    { 
        $$ = $2;
      }
  | IDENTIFIER      { 
        symbol s = symbol_find(st, $1);
        if(s == NULL)
        {
            YYABORT;
        }
        $$ = update_expr_node($$, s, NULL);
      }
  | CONSTANT        {
        $$ = update_expr_node($$, 
            symbol_new_const(&st, $1),
            NULL
            );
      }
      ;

condition:
    expr RELOP expr
    {
        quad qif = quad_ifgoto_gen($1.ptr, $2, $3.ptr);
        quad qgo = quad_goto_gen();
        quad_add(&qt, qif); 
        quad_add(&($$.truelist), qif);
        quad_add(&qt, qgo);
        quad_add(&($$.falselist), qgo);
    }
  | TRUE
    {
    }
  | FALSE
    {
    }
  | condition OR condition
    {
    }
  | condition AND condition
    {
    }
  | NOT condition
    {
    }
  | '(' condition ')'
    {
    }
;

%%
  
struct expr_node_ update_expr_node(struct expr_node_ node, symbol s, quad_list q)
{
    node.ptr = s;

    if(q == NULL)
        node.ql = 0;
    else if(node.ql == NULL && q!= NULL)
        node.ql = q;

    return node;
}

int main() {

  int status = yyparse();


  // Remove uncompleted branches
  int rmQuad = quad_list_clean_gotos(qt);
  symbol_list_print(st);
  quad_list_print(qt);

  printf("Removed %d quad(s) with undefined branch\n", rmQuad);
  printf("Cleaning...");
  quad_free_memory(qt);
  symbol_free_memory(st);
  lex_free();
  printf("OK\n");
  printf("YACC Exit code: %d\n", status);
  return status;
}
