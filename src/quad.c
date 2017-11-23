#include "quad.h"

quad * add_quad(quad ** head, 
                enum OpType type, 
                symbol * res, 
                symbol * left,
                symbol * right)
{
    static int maxId = 0;

    quad * new = *head; // Where to add the new quad

    // Head is not new
    if(*head != NULL)
    {
        // Find the last
        while(new->next != NULL)
            new = new->next;
        new->next = malloc(sizeof(quad));
        new = new->next;
    }
    else
    {
        new = malloc(sizeof(quad));
    }

    // Create
    new->id = maxId++;
    new->type = type;
    new->res = res;
    new->left = left;
    new->right = right;
    new->next = NULL;

    return new;
}

void append_quad(quad * listLeft, quad * listRight)
{
    while(listLeft->next != NULL)
        listLeft = listLeft->next;

    listLeft->next = listRight;
}

