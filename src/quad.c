#include "quad.h"

#include <stdlib.h>
#include <stdio.h>

quad quad_alloc()
{
    static unsigned int countQuad = 0;
    quad q = malloc(sizeof(quad_));
    q->id = countQuad++;
    q->type = 0;
    q->res = NULL;
    q->left = NULL;
    q->right = NULL;
    q->dest = NULL;
    q->cmp = 0;
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

quad quad_printi_gen(symbol s)
{
    quad q = quad_alloc();
    q->type = QUAD_PRINTI;
    q->res = s;
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
            case QUAD_OP_MODULO:
                printf("%%"); break;
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
    else if(q->type <= QUAD_GOTO)// GOto
    {
        if(q->type == QUAD_GOTO_IF)
        {
            printf("IF %5s ", q->left->name);
            switch (q->cmp) {
            case QUAD_RELOP_EQ:
                printf("==");
                break;
            case QUAD_RELOP_NEQ:
                printf("!=");
                break;
            case QUAD_RELOP_GT:
                printf(">");
                break;
            case QUAD_RELOP_GTE:
                printf(">=");
                break;
            case QUAD_RELOP_LT:
                printf("<");
                break;
            case QUAD_RELOP_LTE:
                printf("<=");
                break;
            default:
                break;
            }
            printf(" %5s -> ", q->right->name);
        }
        printf("Goto -> ");
        if(q->type == QUAD_GOTO_END)
            printf("END\n");
        else if(q->dest == NULL)
            printf("?\n");
        else
            printf("QUAD %5d\n", q->dest->id);
    }
    else if(q->type <= QUAD_PRINTI) // Printi
    {
        printf("PRINTI %5s\n", q->res->name);
    }
    else
    {
        printf("UNDEFINED\n");
    }



}

quad_list quad_list_alloc()
{
    quad_list ql = malloc(sizeof(quad_list_));
    ql->previous = NULL;
    ql->next = NULL;
    ql->q = NULL;
    return ql;
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
    printf("///////////////////\n");
    printf("// end quad List\n");
    printf("///////////////////\n");

}

void quad_list_free(quad_list head, bool cleanQuads)
{
    while(head != NULL)
    {
        quad_list next = head->next;

        if(head->q != NULL && cleanQuads)
        {
            free(head->q);
        }
        free(head);

        head = next;
    }

}

void        quad_list_complete(quad_list list, quad q)
{
    while(list != NULL)
    {
        if(list->q != NULL &&
                list->q->type >= QUAD_GOTO_IF &&
                list->q->dest == NULL)
        {
            list->q->dest = q;
        }

        list = list->next;
    }
}

quad_list quad_list_concat(quad_list la, quad_list lb)
{
    quad_list res = NULL;

    while(la != NULL)
    {
        if(la->q != NULL)
            quad_add(&res, la->q);

        la = la->next;
    }

    while(lb != NULL)
    {
        if(lb->q != NULL)
            quad_add(&res, lb->q);

        lb = lb->next;
    }

    return res;
}

quad_list quad_list_append(quad_list* ql, quad q)
{
    quad_add(ql, q);
    return *ql;
}

int quad_list_clean_gotos(quad_list head)
{
    int count = 0;

    while(head != NULL)
    {
        if(head->q->type >= QUAD_GOTO_IF &&
                head->q->type <= QUAD_GOTO &&
                head->q->dest == NULL)
        {
            head->q->type = QUAD_GOTO_END;
            count++;
        }
        head = head->next;

    }
    return count;
}

quad_list quad_list_find(quad_list head, int id)
{
    while(head != NULL)
    {
        if(head->q != NULL && head->q->id == id)
            return head;
        head = head->next;
    }
    fprintf(stderr, "WARNING: Cannot find quad with the id %d\n", id);
    return 0;
}
