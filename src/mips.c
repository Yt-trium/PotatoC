#include "mips.h"

void toMips(symbol st, quad_list qt)
{
    // out stream
    // FILE* os = fopen("out.asm", "w");
    FILE* os = stdout;

    fprintf(os, ".data\n");

    while (st != NULL) {
        if(st->type == SYMBOL_CST)
        {
            fprintf(os, "%s:\t.word %d\n", st->name, st->value);
        }
        st = st->next;
    }

    fprintf(os, ".text\n");
    fprintf(os, "main:\n");

    while (qt != NULL)
    {
        fprintf(os, "LABEL_Q_%d:\n", qt->q->id);
        switch (qt->q->type)
        {
        case QUAD_OP_PLUS:
            fprintf(os, "    lw $t0, %s\n", qt->q->left->name);
            fprintf(os, "    lw $t1, %s\n", qt->q->right->name);
            fprintf(os, "    add $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, %s\n", qt->q->res->name);
            break;
        case QUAD_OP_MINUS:
            fprintf(os, "    lw $t0, %s\n", qt->q->left->name);
            fprintf(os, "    lw $t1, %s\n", qt->q->right->name);
            fprintf(os, "    sub $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, %s\n", qt->q->res->name);
            break;
        case QUAD_OP_MULT:
            fprintf(os, "    lw $t0, %s\n", qt->q->left->name);
            fprintf(os, "    lw $t1, %s\n", qt->q->right->name);
            fprintf(os, "    mult $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, %s\n", qt->q->res->name);
            break;
        case QUAD_OP_DIVI:
            fprintf(os, "    lw $t0, %s\n", qt->q->left->name);
            fprintf(os, "    lw $t1, %s\n", qt->q->right->name);
            fprintf(os, "    div $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, %s\n", qt->q->res->name);
            break;
        // UOP -> RIGHT
        // ASSIGN
        case QUAD_UOP_ASSIGN:
            fprintf(os, "    lw $t0, %s\n", qt->q->right->name);
            fprintf(os, "    sw $t0, %s\n", qt->q->res->name);
            break;
        // GOTO
        case QUAD_GOTO_IF:
            fprintf(os, "    lw $t0, %s\n", qt->q->left->name);
            fprintf(os, "    lw $t1, %s\n", qt->q->right->name);
            switch (qt->q->cmp)
            {
            case QUAD_RELOP_EQ:
                fprintf(os, "    beq $t0, $t1, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            default:
                break;
            }
            break;
        case QUAD_GOTO:
            fprintf(os, "    b LABEL_Q_%d\n", qt->q->dest->id);
            break;
        case QUAD_GOTO_END:
            fprintf(os, "    b LABEL_END\n");
            break;
        default:
            break;
        }

        qt = qt->next;
    }
    fprintf(os, "LABEL_END:\n");




    // LOOOOOL return;
}
