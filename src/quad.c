#include "quad.h"

#include <stdlib.h>

quad * alloc_quad()
{
    quad * res = malloc(sizeof(quad));
    res->id = 0;
    res->type = QUAD_OP_PLUS;
    res->res = NULL;
    res->left = NULL;
    res->right = NULL;
    res->next = NULL;
    return res;
}

quad * add_quad(quad ** head,
                enum OpType type, 
                symbol res,
                symbol left,
                symbol right)
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

void quad_print(quad * head)
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


void quad_list_print (quad_list *ql)
{
    puts((ql != NULL?"List of quads:":"quad list empty."));
    while(ql != NULL){
        quad_print(ql->q);
        ql = ql->next;
    }
}

void quad_list_complete (quad_list* list, symbol label)
{
    while(list != NULL) {
        list->q->res = label;
        list = list->next;
    }
}

void quad_list_add(quad_list** dest, quad_list* src) {
    if(*dest == NULL) {
        *dest = src;
    }
    else {
        quad_list* scan = *dest;
        while(scan->next != NULL)
            scan = scan->next;
        scan->next = src;
    }
}

quad_list* quad_list_new  (quad *q)
{
    quad_list *ql = (quad_list*)malloc(sizeof(quad_list));
    ql->q = q;
    ql->previous = NULL;
    ql->next = NULL;
    return ql;
}

void quad_list_free (quad_list *ql){
    free(ql);
}
