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
        printf("id: %16s [", head->name);
        if(head->type == SYMBOL_CST)
            printf("[SYMBOL_CST] = %d", head->value);
        else if(head->type == SYMBOL_INT)
            printf("[SYMBOL_INT]");
        else if(head->type == SYMBOL_ID)
            printf("[SYMBOL_ID]");
        else
            printf("false, value: N/A");
        printf("\n");
        head = head->next;
    }
}

symbol symbol_new(symbol * head, char * name)
{
    symbol tmp = symbol_alloc();

    tmp->name = strdup(name);
    tmp->type = SYMBOL_ID;
    tmp->value = 0;

    symbol_list_add(head, tmp);
    return tmp;
}

symbol symbol_new_temp(symbol* head)
{
    static unsigned int cmpCount = 0;
    symbol tmp = symbol_alloc();

    tmp->name = malloc(SYMBOL_MAX_NAME_LENGTH);
    tmp->type = SYMBOL_INT;
    tmp->value = 0;

    snprintf(tmp->name, SYMBOL_MAX_NAME_LENGTH, "TEMP_%u", cmpCount++);

    symbol_list_add(head, tmp);
    return tmp;
}

symbol symbol_new_const(symbol* head, int v)
{
    symbol tmp = symbol_new_temp(head);
    tmp->value = v;
    tmp->type = SYMBOL_CST;
    return tmp;
}
