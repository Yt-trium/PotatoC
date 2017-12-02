#include "quad.h"

#include <stdlib.h>

quad quad_alloc()
{
    static unsigned int countQuad = 0;
    quad q = malloc(sizeof(quad_));
    q->id = countQuad++;
    q->type = QUAD_OP_PLUS;
    q->res = NULL;
    q->left = NULL;
    q->right = NULL;
    q->next = NULL;
    return q;
}

quad quad_gen(enum OpType type, symbol res, symbol left, symbol right)
{
    quad q = quad_alloc();
    q->type = type;
    q->res = res;
    q->left = left;
    q->right = right;

    return q;
}

quad quad_add(quad *head, quad q)
{
    if(*head == NULL)
    {
        *head = q;
        return *head;
    }
    else
    {
        quad new = *head;
        while(new->next != NULL)
            new = new->next;
        new->next = q;

        return q;
    }
}

void quad_append(quad listLeft, quad listRight)
{
    while(listLeft->next != NULL)
        listLeft = listLeft->next;

    listLeft->next = listRight;
}

void quad_print(quad head)
{
    printf("///////////////////\n");
    printf("// quad List\n");
    printf("///////////////////\n");
    while(head != NULL)
    {
        printf("id: %4d, operator: ", head->id);

        switch (head->type)
        {
            case QUAD_OP_PLUS:
                printf("+"); break;
            case QUAD_OP_MINUS:
                printf("+"); break;
            default:
                break;
        }

        printf(", res: %5s, left: %5s, right: %5s\n",
                head->res->name,
                head->left->name,
                head->right->name);

        head = head->next;
    }
}
