#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

symbol          symbol_alloc()
{
    symbol s = malloc(sizeof(symbol_));

    s->name = NULL;
    s->next = NULL;
    s->type = SYMBOL_INT;
    s->value = 0;

    return s;
}

void            symbol_print(symbol s)
{
    if(s == NULL)
        printf("ERROR: TRYING TO PRINT NULL SYMBOL");
    else
        printf("%s %d %d", s->name, s->type, s->value);
}

symbol          symbol_list_add(symbol *head, symbol s)
{
    if((*head) == NULL)
    {
        (*head) = s;
    }
    else
    {
        symbol current = (*head);
        while(current->next != NULL)
            current = current->next;
        current->next = s;
    }
    return s;
}

symbol symbol_must_find(symbol head, char * name)
{
    symbol s = symbol_find(head, name);
    if(s == NULL)
        fprintf(stderr, "ERROR: Trying to access undeclared variable: %s\n", name);
    return s;
}

symbol symbol_find(symbol head, char * name)
{
    while(head != NULL)
    {
        if(head->type == SYMBOL_INT && strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

void            symbol_list_print(symbol head)
{
    printf("///////////////////\n");
    printf("// Symbol Table\n");
    printf("///////////////////\n");

    while(head != NULL)
    {
        printf("%8s   |   ", head->name);
        if(head->type == SYMBOL_CST)
            printf("SYMBOL_CST = %d", head->value);
        else if(head->type == SYMBOL_INT)
            printf("SYMBOL_INT");
        else
            printf("UNDEFINED %d", head->type);
        printf("\n");
        head = head->next;
    }
}

symbol symbol_new(symbol * head, char * name)
{
    symbol tmp = symbol_alloc();

    tmp->name = strdup(name);
    tmp->type = SYMBOL_INT;
    tmp->value = 0;

    symbol_list_add(head, tmp);
    return tmp;
}

symbol symbol_new_temp(symbol* head)
{
    static unsigned int cmpCount = 0;
    symbol tmp = symbol_alloc();

    size_t needed = snprintf(NULL, 0, "TEMP_%u", cmpCount++) + 1; // \0
    tmp->name = malloc(needed);
    snprintf(tmp->name, needed, "TEMP_%u", cmpCount++);

    tmp->type = SYMBOL_INT;
    tmp->value = 0;


    symbol_list_add(head, tmp);
    return tmp;
}

symbol symbol_new_const(symbol* head, int v)
{
    static unsigned int constCount = 0;
    symbol tmp = symbol_alloc();

    size_t needed = snprintf(NULL, 0, "CONST_%u", constCount++) + 1; // \0
    tmp->name = malloc(needed);
    snprintf(tmp->name, needed, "CONST_%u", constCount++);

    tmp->type = SYMBOL_CST;
    tmp->value = v;

    symbol_list_add(head, tmp);
    return tmp;
}


void symbol_free_memory(symbol head)
{
    // Clean from start to end
    while(head != NULL)
    {
        symbol next = head->next;

        if(head->name != NULL)
            free(head->name);

        free(head);

        head = next;
    }
}
