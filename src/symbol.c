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

symbol_list     symbol_list_alloc()
{
    symbol_list head = NULL;
    (*head) = symbol_alloc();
    return head;
}

symbol          symbol_list_add(symbol *head, symbol s)
{
    symbol current = *head;
    if(current == NULL)
    {
        (*head) = s;
    }
    else
    {
        while(current->next != NULL)
            current = current->next;
        current->next = s;
        return current->next;
    }
    return s;
}

symbol          symbol_list_find(symbol head, symbol s)
{
    while(head != NULL)
    {
        if(strcmp(head->name, s->name) == 0)
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
        printf("id: %10s, is_constant : ", head->name);
        if(head->type == SYMBOL_CST)
            printf("true, value: %d", head->value);
        else
            printf("false, value: N/A");
        printf("\n");
        head = head->next;
    }
}

symbol symbol_new_temp(symbol* head, int v)
{
    static unsigned int cmpCount = 0;
    symbol tmp = symbol_alloc();

    tmp->name = malloc(SYMBOL_MAX_NAME_LENGTH);
    tmp->next = NULL;
    tmp->type = SYMBOL_INT;
    tmp->value = v;

    snprintf(tmp->name, SYMBOL_MAX_NAME_LENGTH, "TEMP[%ud]", cmpCount++);

    return symbol_list_add(head, tmp);
}
