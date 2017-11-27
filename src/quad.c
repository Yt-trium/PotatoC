#include "quad.h"

quad * alloc_quad()
{
    quad * res = malloc(sizeof(quad));
    res->id = 0;
    res->type = PLUS;
    res->res = NULL;
    res->left = NULL;
    res->right = NULL;
    res->next = NULL;
    return res;
}

quad * add_quad(quad ** head, 
                enum OpType type, 
                symbol * res, 
                symbol * left,
                symbol * right)
{
    static int maxId = 0;

    // List is empty
    if(*head == NULL)
    {
        *head = alloc_quad();
        (*head)->id = maxId++;
        (*head)->res = res;
        (*head)->left = left;
        (*head)->right = right;
        (*head)->type = type;
        return *head;
    }
    else
    {
        quad * new = *head;
        while(new->next != NULL)
            new = new->next;
        new->next = alloc_quad();
        new->next->id = maxId++;
        new->next->res = res;
        new->next->left = left;
        new->next->right = right;
        new->next->type = type;
        return new->next;
    }
}

void append_quad(quad * listLeft, quad * listRight)
{
    while(listLeft->next != NULL)
        listLeft = listLeft->next;

    listLeft->next = listRight;
}

void print_quad(quad * head)
{
    printf("///////////////////\n");
    printf("// Quad List\n");
    printf("///////////////////\n");
    while(head != NULL)
    {
        printf("id: %4d, operator: ", head->id);

        switch (head->type)
        {
            case PLUS:
                printf("+"); break;
            case MINUS:
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
