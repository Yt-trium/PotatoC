#ifndef __QUAD__
#define __QUAD__

#include "symbol.h"

enum OpType{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQ,
    NE
};

typedef struct quad{
    int id;
    enum OpType type;
    symbol * res;
    symbol * left;
    symbol * right;
    struct quad * next;
} quad;

quad * add_quad(quad ** head, 
                enum OpType type, 
                symbol * res, 
                symbol * left,
                symbol * right);

// Append listRight to listLeft
void append_quad(quad * listLeft, quad * listRight);

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


#endif
