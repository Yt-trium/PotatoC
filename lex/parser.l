%{
    #include <stdio.h>
    #include <string.h>
    #include "y.parser.h"

    #define CTYPE_INT 1

    // Symboles table
    typedef struct idstack idstack;

    struct idstack{
        char * name;
        struct idstack * next;
        size_t id;
    };

    idstack * stack = 0;

    size_t addIdentifier(const char * name)
    {
        idstack * previous = stack;
        idstack * cur = stack;
        while(cur)
        {
            if(strcmp(cur->name, name) == 0)
                return cur->id;

            previous = cur;
            cur = cur->next;
        }

        cur =  malloc(sizeof(idstack));
        cur->name = malloc(strlen(name) + 1);
        strcpy(cur->name, name);
        if (previous)  cur->id = previous->id + 1;
        else cur->id = 0;
        cur->next = 0;
        if(previous) previous->next = cur;
        if(!stack) stack = cur;
        return cur->id;

    }
%}

ID [a-zA-Z_][a-zA-Z0-9_]*
INTEGER "0"|[1-9][0-9]*

%%

"int" { yylval=CTYPE_INT; return CTYPE; }
"=" { return ASSIGN; }
"\n" { return EOFL; }
";" { return EOFI; }
{ID} { yylval=addIdentifier(yytext); return SYM; }
{INTEGER} { yylval=atoi(yytext); return INT; }

%%

