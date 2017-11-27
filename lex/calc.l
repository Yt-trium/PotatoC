%{
    #include <stdio.h>
    #include "y.calc.h"
%}

%%

[0-9]+ {yylval.value=atoi(yytext); return ENTIER;}
"+" { return '+';}
"-" { return '-';}
"*" { return '*';}
"(" { return '(';}
")" { return ')';}
'\n' { yyterminate();}

%%

