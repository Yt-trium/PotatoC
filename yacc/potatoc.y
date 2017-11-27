%{

    #include <stdio.h>
    #include <stdlib.h>
    int yylex();
    void yyerror(void *);

%}

%token INT;
%token ASSIGN;
%token SYM;
%token EOFI;
%token EOFL;
%token CTYPE;
%token PREPROC;
%start ligne;

%%

ligne: CTYPE SYM ASSIGN INT EOFI EOFL { printf("Assign %d to symbole %d of type %d\n", $4, $2, $1); }
     | CTYPE { printf("Type %d\n", $1); }
     | CTYPE EOFL { printf("Type %d\n", $1); }
     | EOFL { printf("Empty line\n"); }

%%


int main(){

    return yyparse();

}
