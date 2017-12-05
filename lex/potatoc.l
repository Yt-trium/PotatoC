%{
  #include <stdlib.h>
  #include <string.h>
  #include "y.potatoc.h"
%}

blank      [\t\b\v\n\f ]*
identifier [a-zA-Z_]+[0-9a-zA-Z_]*
number     [1-9][0-9]*

%%

{blank}                 {}

"if"                    { return(IF); }
"else"                  { return(ELSE); }

"for"                   { return(FOR); }
"while"                 { return(WHILE); }

"int"                   { return(INT); }
"stencil"               { return(STENCIL); }

"return"                { return(RETURN); }

"="                     { return(ASSIGN); }
"+"                     { return(PLUS); }
"-"                     { return(MINUS); }
"*"                     { return(MULT); }
"/"                     { return(DIVI); }
";"                     { return(END); }
[()]                    { return yytext[0]; }

{number}                { yylval.value = atoi(yytext); return CONSTANT; }
{identifier}            { yylval.string = strdup(yytext); return IDENTIFIER; }


.                       { printf("Caractere inconnu: %s", yytext); }

%%
