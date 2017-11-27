%{

    #include <stdio.h>
    #include <stdlib.h>
    #include "symbol.h"
    #include "quad.h"
    int yylex();
    void yyerror(void *);

    symbol * head_symbol = NULL;
    quad * head_quad = NULL;

%}

%union{
    struct symbol * sym;
    int value;
}

%token <value> ENTIER
%type <sym> operation;
%type <sym> ligne;

%right '='
%left '+' '-'
%left '*'
%start ligne

%%

ligne: operation { 
     printf("Fin\n");
     //printf("Fin: %d\n", $1); fflush(stdout); 
     }

operation: operation '+' operation { 
         symbol * s = add_symbol(&head_symbol, "PLUSTEMP");
         $$ = s;
         add_quad(&head_quad, PLUS, s, $1, $3);
         }

         | operation '*' operation { 
         symbol * s = add_symbol(&head_symbol, "TIMESTEMP");
         $$ = s;
         add_quad(&head_quad, MULTIPLY, s, $1, $3);
         }

         | '(' operation ')' { $$ = $2;}

         | ENTIER { 
         $$ = add_symbol(&head_symbol, "Number");
         $$->value = $1;
         $$->constant = true;
         }

         ;

%%


int main(){

    printf("Entrez une operation: \n");
    fflush(stdout);
    yyparse();
    print_symbol(head_symbol);
    fflush(stdout);
    print_quad(head_quad);
    return 0;

}
