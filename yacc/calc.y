%{

    #include <stdio.h>
    #include <stdlib.h>
    #include "symbol.h"
    int yylex();
    void yyerror(void *);

    symbol * head_symbol = NULL;

%}

%token ENTIER
%right '='
%left '+' '-'
%left '*'
%start ligne

%%

ligne: operation { printf("Résultat: %d\n", $1); fflush(stdout); }

operation: operation '+' operation { $$ = $1 + $3; }

         | '-' operation { $$ = -$2; }

         | operation '*' operation { $$ = $1 * $3; }

         | '(' operation ')' { $$ = $2;}

         | ENTIER { 
         $$ = $1; 
         symbol * s = add_symbol(&head_symbol, "Coucou");
         s->value = $1;
         s->constant = true;
         }

         ;

%%


int main(){

    printf("Entrez une operation: \n");
    fflush(stdout);
    yyparse();
    print_symbol(head_symbol);
    return 0;

}
