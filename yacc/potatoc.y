%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"
  #include "symbol.h"
  #include "test.h"
  #include "quad.h"
  #include "mips.h"

  void lex_free();
  void yyerror(char*);
  symbol st = NULL;
  quad_list qt = NULL;

  extern FILE *yyin;
  extern int yylex();
  extern int yyparse();
  
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
%token IF ELSE WHILE FOR RETURN PRINTI
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
  braced_statement
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
    {
        quad_list_free($1.next, false);
    }

statement_list:

    statement_list statement
    {
        if($2.head != NULL)
            quad_list_complete($$.next, $2.head->q);
        // free list
        quad_list_free($$.next, false);
        $$.next = $2.next;
    }

    | statement 
    {
        $$.next = $1.next;
        $$.head = $1.head;
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

    | braced_statement
    {
        $$ = $1;
    }

    | PRINTI '(' expr ')' END
    {
        $$.next = NULL;
        $$.head = quad_add(&qt, quad_printi_gen($3.ptr));

    }

    | IF '(' condition ')' statement %prec LOWER_THAN_ELSE
    {
        $$.next = NULL;
        if($5.head == NULL || $5.head->q == NULL)
        {
            fprintf(stderr, "ERROR: Empty statement is not allowed inside an if block.\n");
            YYABORT;
        }
        quad_list_complete($3.truelist, $5.head->q);

        $$.next = quad_list_concat($3.falselist, $5.next);
        // We need the top quad list element, but from the global list
        $$.head = quad_list_find(qt, $3.top->id);
        // Free true list and false list but not the quads
        quad_list_free($3.truelist, false);
        quad_list_free($3.falselist, false);
        quad_list_free($5.next, false);
    }

    | IF '(' condition ')' statement ELSE link statement
    {
        $$.next = NULL;
        if($5.head == NULL || $5.head->q == NULL)
        {
            fprintf(stderr, "ERROR: Empty statement is not allowed inside an if block.\n");
            YYABORT;
        }
        if($8.head == NULL || $8.head->q == NULL)
        {
            fprintf(stderr, "ERROR: Empty statement is not allowed inside an else block.\n");
            YYABORT;
        }
        quad_list_complete($3.truelist, $5.head->q);
        quad_list_complete($3.falselist, $8.head->q);

        quad_list qll = NULL;
        qll = quad_list_concat($5.next, $8.next);
        $$.next = quad_list_concat(qll, $8.next);
        $$.next = quad_list_append(&($$.next), $7.head->q);
        quad_list_free(qll, false);
        // We need the top quad list element, but from the global list
        $$.head = quad_list_find(qt, $3.top->id);
        // Free true list and false list but not the quads
        quad_list_free($3.truelist, false);
        quad_list_free($3.falselist, false);
        quad_list_free($5.next, false);
        quad_list_free($8.next, false);
    }

    | WHILE '(' condition ')' statement
    {
        $$.next = NULL;
        if($5.head == NULL || $5.head->q == NULL)
        {
            fprintf(stderr, "ERROR: Empty statement is not allowed inside a while block.\n");
            YYABORT;
        }
        quad_list_complete($3.truelist, $5.head->q);
        $$.next = $3.falselist;

        // End while
        quad qgo = quad_goto_gen();
        qgo->dest = $3.top;
        quad_add(&qt, qgo);

        // Top while
        $$.head = quad_list_find(qt, $3.top->id);
        quad_list_complete($5.next, $3.top);
    }

braced_statement:

    '{' '}'
    {
        $$.head = NULL;
        $$.next = NULL;
    }

    | '{' statement_list '}'
    {
        $$.head = $2.head;
        $$.next = $2.next;
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
        free($2);
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
        free($2);
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
        free($1);
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
        free($2);
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
        free($2);
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
        free($1);
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
        if($1.ql != NULL && $1.ql->q != NULL)
            $$.top = $1.ql->q;
        else if($3.ql != NULL && $3.ql->q != NULL)
            $$.top = $3.ql->q;
        else
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
        quad_list_free($1.falselist, false);
        quad_list_free($1.truelist, false);
        quad_list_free($3.truelist, false);
    }

    | condition AND condition
    {
        $$.truelist = NULL;
        $$.falselist = NULL;
        $$.top = $1.top;

        quad_list_complete($1.truelist, $3.top);

        $$.falselist = quad_list_concat($1.falselist, $3.falselist);
        $$.truelist = $3.truelist;

        quad_list_free($1.falselist, false);
        quad_list_free($1.truelist, false);
        quad_list_free($3.falselist, false);
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

int main(int argc, const char** argv) {
    int status = 0;

    if(argc == 1)
    {
        fprintf(stdout, "Usage: %s [input [output]]\n", argv[0]);
        fprintf(stdout, "Reading from standard input.\n");
        status = yyparse();
    }
    else if(argc >= 2)
    {
        fprintf(stdout, "Reading from %s.\n", argv[1]);
        if(argc == 3)
        {
            fprintf(stdout, "Output will be saved to %s.\n", argv[2]);
            // FILE *fo = fopen(argv[2], "w");
        }
        FILE *fi = fopen(argv[1], "r");
        if (!fi) {
            fprintf(stderr, "ERROR: Unable to input the given file %s.\n", argv[1]);
            return -1;
        }
        yyin = fi;
        do
        {
            status = yyparse();
        } while(!feof(yyin) && status == 0);
        fclose(fi);
    }

    // Set uncompleted branches to end
    int rmQuad = 0;
    rmQuad = quad_list_clean_gotos(qt);
    symbol_list_print(st);
    quad_list_print(qt);
    toMips(st,qt);

    printf("Cleaned %d quad(s) with undefined branch\n", rmQuad);
    printf("Cleaning...");
    quad_list_free(qt, true);
    symbol_free_memory(st);
    lex_free();
    printf("OK\n");
    printf("YACC Exit code: %d\n", status);

    return status;
}
