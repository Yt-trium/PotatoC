#include "symbol.h"

symbol * alloc_symbol()
{
    symbol * ptr = malloc(sizeof(symbol));
    ptr->name = NULL;
    ptr->value = 0;
    ptr->constant = false;
    ptr->next = NULL;
    return ptr;
}

symbol * add_symbol(symbol ** head, char* name)
{
    // If list is empty
    if(*head == NULL)
    {
        *head = alloc_symbol();
        (*head)->name = strdup(name);
        return *head;
    }
    else
    {
        symbol * current  = *head;
        // Looking for the end
        while(current->next != NULL)
            current = current->next;
        current->next = alloc_symbol();
        current->next->name = strdup(name);
        return current->next;
    }
}

symbol * find_symbol(symbol * head, char * name)
{
    while(head != NULL)
    {
        if(strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

void print_symbol(symbol * head)
{
    printf("///////////////////\n");
    printf("// Symbol Table\n");
    printf("///////////////////\n");

    while(head != NULL)
    {
        printf("id: %10s, is_constant : ", head->name);
        if(head->constant)
            printf("true, value: %d", head->value);
        else
            printf("false, value: N/A");
        printf("\n");
        head = head->next;
    }
}
