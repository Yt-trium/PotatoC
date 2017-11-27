%{
  #include <stdio.h>
  #include <stdlib.h>

  void yyerror(char*);
  int yylex();
  void lex_free();
%}

%token ASSIGN WHILE DO DONE IF THEN ELSE ENDIF
%token EQUAL TRUE FALSE OR AND NOT
%token ID
%token NUM

%left OR
%left AND
%left NOT

%%

axiom:
  statement_list '\n'
    {
      printf("Match !!!\n");
      return 0;
    }

statement_list:
    statement_list statement
    {
    }
  | statement
    {
    }
  ;

statement:
    ID ASSIGN expression
    {
    }
  | WHILE condition DO statement_list DONE
    {
    }
  | IF condition THEN statement_list ENDIF
    {
    }
  | IF condition THEN statement_list ELSE statement_list ENDIF
    {
    }
  ;

condition:
    ID EQUAL NUM
    {
    }
  | TRUE
    {
    }
  | FALSE
    {
    }
  | condition OR tag condition
    {
    }
  | condition AND tag condition
    {
    }
  | NOT condition
    {
    }
  | '(' condition ')'
    { 
    }
  ;

expression:
    ID
    {
    }
  | NUM
    {
    }
  ;

tag:
    {
    }
  ;


%%

void yyerror (char *s) {
    fprintf(stderr, "[Yacc] error: %s\n", s);
}

int main() {
  printf("Enter your code:\n");
  yyparse();
  printf("-----------------\nSymbol table:\n");
  printf("-----------------\nQuad list:\n");

  // Be clean.
  lex_free();
  return 0;
}
