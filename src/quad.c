#include "quad.h"

#include <stdlib.h>
#include <stdio.h>

quad quad_alloc()
{
    static unsigned int countQuad = 0;
    quad q = malloc(sizeof(quad_));
    q->id = countQuad++;
    q->type = QUAD_OP_PLUS;
    q->res = NULL;
    q->left = NULL;
    q->right = NULL;
    q->dest = NULL;
    q->cmp = QUAD_RELOP_EQUAL;
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

quad quad_unary_gen(enum OpType type, symbol res, symbol right)
{
    quad q = quad_alloc();
    q->type = type;
    q->res = res;
    q->right = right;
    q->left = NULL;

    return q;

}

quad quad_ifgoto_gen(symbol left, enum RelOp op, symbol right)
{
    quad q = quad_alloc();
    q->type = QUAD_GOTO_IF;
    q->cmp = op;
    q->left = left;
    q->right = right;
    return q;
}

quad quad_goto_gen()
{
    quad q = quad_alloc();
    q->type = QUAD_GOTO;
    return q;

}

quad_list quad_add(quad_list *head, quad q)
{
    if(*head == NULL)
    {
        *head = quad_list_alloc();
        (*head)->q = q;
        return *head;
    }
    else
    {
        quad_list end = *head;
        while(end->next != NULL)
            end = end->next;
        end->next = quad_list_alloc();
        end->next->previous = end;
        end->next->q = q;
        return end->next;
    }
}

/*
void quad_append(quad listLeft, quad listRight)
{
    while(listLeft->next != NULL)
        listLeft = listLeft->next;

    listLeft->next = listRight;
}
*/

void quad_print(quad q)
{
    printf("id: %4d, ", q->id);

    if(q->type <= QUAD_UOP_PLUS)
    {
        printf("operator: ");
        switch (q->type)
        {
            case QUAD_OP_PLUS:
            case QUAD_UOP_PLUS:
                printf("+"); break;
            case QUAD_OP_MINUS:
            case QUAD_UOP_MINUS:
                printf("-"); break;
            case QUAD_OP_MULT:
                printf("*"); break;
            case QUAD_OP_DIVI:
                printf("/"); break;
            case QUAD_UOP_ASSIGN:
                printf("="); break;
            default:
                break;
        }
        // If unary
        if(q->type >= QUAD_UOP_ASSIGN)
        {
            printf(", res: %5s, right: %5s\n",
                   q->res->name,
                   q->right->name);
        }
        else // binary
        {
            printf(", res: %5s, left: %5s, right: %5s\n",
                   q->res->name,
                   q->left->name,
                   q->right->name);
        }
    }
    else // GOto
    {
        if(q->type == QUAD_GOTO_IF)
        {
            printf("IF %5s ", q->left->name);
            switch (q->cmp) {
            case QUAD_RELOP_EQUAL:
                printf("==");
                break;
            default:
                break;
            }
            printf(" %5s -> ", q->right->name);
        }
        printf("Goto -> ");
        if(q->dest == NULL){
            printf("NULL QUAD");
        }
        else
        {
            printf("QUAD %5d\n", q->dest->id);
        }
    }



}

quad_list quad_list_alloc()
{
    quad_list ql = malloc(sizeof(quad_list_));
    ql->previous = NULL;
    ql->next = NULL;
    ql->q = NULL;
}

void quad_list_print(quad_list head)
{
    printf("///////////////////\n");
    printf("// quad List\n");
    printf("///////////////////\n");
    while(head != NULL)
    {
        quad_print(head->q);
        head = head->next;
    }

}

void quad_free_memory(quad_list head)
{
    while(head != NULL)
    {
        quad_list next = head->next;

        if(head->q != NULL)
        {
            free(head->q);
        }
        free(head);

        head = next;
    }

}
