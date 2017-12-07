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
  struct bool_node_ {
      struct quad_list_* truelist;
      struct quad_list_* falselist;
      struct quad_* top; // Usefull when lists are revert
  } condData;
  struct statement_node_ {
    struct quad_list_* head;
    struct quad_list_* next;
  } statementData;
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
%type <statementData>
  statement
  statement_list
  link
  expr_statement
  assign_statement
  declare_statement
%type <exprData> expr

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

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

    statement_list

statement_list:

    statement_list statement
    {
        /*
        printf("Concat statement.");
        if($$.next == NULL)
            printf("Next list empty.\n");
        if($2.head != NULL)
            printf(" New next statement is %d\n\n", $2.head->q->id);
        */
        if($2.head != NULL)
            quad_list_complete($$.next, $2.head->q);
        // free list
        //quad_list_free($$.next, false);
        $$.next = $2.next;
        $$.head = $2.head;
    }

    | statement 
    {
        $$.next = $1.next;
        $$.head = $1.head;
        /*
        printf("Alone statement. ");
        if($1.next == NULL)
            printf("Next list empty.\n");
        if($1.head != NULL)
            printf(" Statement is %d\n\n", $1.head->q->id);
        */

        //$$.next = $1.next;
        //quad_list_complete($3.next, $3->q);
        //quad_list_complete(qt, $1->q);
        // Free list
    }

statement:

    assign_statement
    {
        $$ = $1;
    }

    | declare_statement
    {
        $$ = $1;
    }
    
    | expr_statement
    {
        $$ = $1;
    }

    | IF '(' condition ')' statement %prec LOWER_THAN_ELSE
    {
        $$.next = NULL;
        quad_list_complete($3.truelist, $5.head->q);

        $$.next = quad_list_concat($3.falselist, $5.next);
        // We need the top quad list element, but from the global list
        $$.head = quad_list_find(qt, $3.top->id);
        // Free true list and false list but not the quads
        //quad_list_free($3.truelist, false);
        //quad_list_free($3.falselist, false);
    }

    | IF '(' condition ')' statement ELSE link statement
    {
        $$.next = NULL;
        quad_list_complete($3.truelist, $5.head->q);
        quad_list_complete($3.falselist, $8.head->q);

        $$.next = quad_list_concat($5.next, $8.next);
        $$.next = quad_list_concat($$.next, $8.next);
        $$.next = quad_list_append($$.next, $7.head->q);
        // We need the top quad list element, but from the global list
        $$.head = quad_list_find(qt, $3.top->id);
        // Free true list and false list but not the quads
        //quad_list_free($3.truelist, false);
        //quad_list_free($3.falselist, false);
    }


declare_statement:

    INT IDENTIFIER END
    {
        symbol s = symbol_find(st, $2);
        if(s != NULL)
        {
            fprintf(stderr, "The variable %s already exists.\n", $2);
            YYABORT;
        }
        s = symbol_new(&st, $2);
        $$.head = NULL;
        $$.next = NULL;
    }

    | INT IDENTIFIER ASSIGN expr END
    {
        symbol s = symbol_find(st, $2);
        if(s != NULL)
        {
            fprintf(stderr, "The variable %s already exists.\n", s->name);
            YYABORT;
        }
        s = symbol_new(&st, $2);

        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, s, $4.ptr));
        $4 = update_expr_node($4, s, ql);

        $$.next = NULL;
        $$.head = $4.ql;
    }

assign_statement:

    IDENTIFIER ASSIGN expr END
    {

        symbol s = symbol_find(st, $1);
        if(s == NULL){
            fprintf(stderr, "The variable %s isn't declared.\n", $1);
            YYABORT;
        }
        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_ASSIGN, s, $3.ptr));
        $3 = update_expr_node($3, s, ql);

        $$.next = NULL;
        $$.head = $3.ql;
    }

expr_statement:

    expr END
    {
        $$.next = NULL;
        if($1.ql == NULL)
        {
            fprintf(stderr, "ERROR: No instruction generated for the expression.\n");
            YYABORT;
        }
        $$.head = $1.ql;
    }

expr:

    expr PLUS expr  
    { 
        $$ = $3;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_PLUS, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
        
    }

    | PLUS expr 
    {
        $$ = $2;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_PLUS, s, $2.ptr));
        $$ = update_expr_node($$, s, ql);
    }
    
    | expr MINUS expr 
    {
        $$ = $1;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_MINUS, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
    }

    | MINUS expr 
    {
        $$ = $2;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_unary_gen(QUAD_UOP_MINUS, s, $2.ptr));
        $$ = update_expr_node($$, s, ql);
    }

    | INC IDENTIFIER  
    {
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

    | DEC IDENTIFIER  
    {
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
  
    | expr MULT expr  
    {
        $$ = $3;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_MULT, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
    }
    
    | expr DIVI expr  
    { 
        $$ = $3;
        symbol s = symbol_new_temp(&st);
        quad_list ql = quad_add(&qt, quad_gen(QUAD_OP_DIVI, s, $1.ptr, $3.ptr));
        $$ = update_expr_node($$, s, ql);
    }

    | '(' expr ')'    
    {
        $$ = $2;
    }
    
    | IDENTIFIER      
    { 
        symbol s = symbol_must_find(st, $1);
        if(s == NULL)
        {
            YYABORT;
        }
        $$ = update_expr_node($$, s, NULL);
    }
    
    | CONSTANT        
    {
        $$ = update_expr_node($$, 
                        symbol_new_const(&st, $1),
                        NULL
                        );
    }

link:
  {
        quad qgo = quad_goto_gen();
        $$.head = quad_add(&qt, qgo);
        $$.next = NULL;
  }

condition:

    expr RELOP expr
    {
        $$.truelist = NULL;
        $$.falselist= NULL;
        quad qif = quad_ifgoto_gen($1.ptr, $2, $3.ptr);
        quad qgo = quad_goto_gen();
        $$.top = qif;
        quad_add(&qt, qif); 
        quad_add(&($$.truelist), qif);
        quad_add(&qt, qgo);
        quad_add(&($$.falselist), qgo);
    }

    | TRUE
    {
        $$.truelist = NULL;
        $$.falselist= NULL;
        quad qgo = quad_goto_gen();
        $$.top = qgo;
        quad_add(&qt, qgo);
        quad_add(&($$.truelist), qgo);
    }

    | FALSE
    {
        $$.truelist = NULL;
        $$.falselist= NULL;
        quad qgo = quad_goto_gen();
        $$.top = qgo;
        quad_add(&qt, qgo); 
        quad_add(&($$.falselist), qgo);
    }

    | condition OR condition
    {
        $$.truelist = NULL;
        $$.falselist = NULL;
        $$.top = $1.top;

        quad_list_complete($1.falselist, $3.top);

        $$.falselist = $3.falselist;
        $$.truelist = quad_list_concat($1.truelist, $3.truelist);
    }

    | condition AND condition
    {
        $$.truelist = NULL;
        $$.falselist = NULL;
        $$.top = $1.top;

        quad_list_complete($1.truelist, $3.top);

        $$.falselist = quad_list_concat($1.falselist, $3.falselist);
        $$.truelist = $3.truelist;
    }

    | NOT condition
    {
      $$.top = $2.top;
      $$.truelist = $2.falselist;
      $$.falselist = $2.truelist;
    }

    | '(' condition ')'
    {
      $$.truelist = $2.truelist;
      $$.falselist = $2.falselist;
      $$.top = $2.top;
    }

%%
  
void yyerror (char *s) {
    fprintf(stderr, "[Yacc] error: %s\n", s);
}

struct expr_node_ update_expr_node(struct expr_node_ node, symbol s, quad_list q)
{
    node.ptr = s;

    if(q == NULL)
        node.ql = 0; // First init
    else if(node.ql == NULL && q!= NULL)
        node.ql = q;

    return node;
}

int main() {

  int status = yyparse();

  // Set uncompleted branches to end
  int rmQuad = 0;
  rmQuad = quad_list_clean_gotos(qt);
  symbol_list_print(st);
  quad_list_print(qt);

  printf("Cleaned %d quad(s) with undefined branch\n", rmQuad);
  printf("Cleaning...");
  quad_list_free(qt, true);
  symbol_free_memory(st);
  lex_free();
  printf("OK\n");
  printf("YACC Exit code: %d\n", status);
  return status;
}
