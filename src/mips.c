#include "mips.h"

void toMips(symbol st, quad_list qt, FILE *os)
{
    fprintf(os, ".data\n");

    while (st != NULL) {
        // if(st->type == SYMBOL_CST)
        {
            fprintf(os, "PTC_%s_:\t.word %d\n", st->name, st->value);
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
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    add $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, PTC_%s_\n", qt->q->res->name);
            break;
        case QUAD_OP_MINUS:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    sub $t2, $t0, $t1\n");
            fprintf(os, "    sw $t2, PTC_%s_\n", qt->q->res->name);
            break;
        case QUAD_OP_MULT:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    mult $t0, $t1\n");
            fprintf(os, "    mflo $t2\n");
            fprintf(os, "    sw $t2, PTC_%s_\n", qt->q->res->name);
            break;
        case QUAD_OP_DIVI:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    div $t0, $t1\n");
            fprintf(os, "    mflo $t2\n");
            fprintf(os, "    sw $t2, PTC_%s_\n", qt->q->res->name);
            break;            
        case QUAD_OP_MODULO:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    div $t0, $t1\n");
            fprintf(os, "    mfhi $t2\n");
            fprintf(os, "    sw $t2, PTC_%s_\n", qt->q->res->name);
            break;
        // UOP -> RIGHT
        case QUAD_UOP_MINUS:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    sub $t0, $t0, 1\n");
            fprintf(os, "    sw $t0, PTC_%s_\n", qt->q->res->name);
            break;
        case QUAD_UOP_PLUS:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    add $t0, $t0, 1\n");
            fprintf(os, "    sw $t0, PTC_%s_\n", qt->q->res->name);
            break;
        case QUAD_PRINTI:
            fprintf(os, "    li $v0, 1\n");
            fprintf(os, "    lw $a0, PTC_%s_\n", qt->q->res->name);
            fprintf(os, "    syscall\n");
            fprintf(os, "    li $v0, 0xB\n");
            fprintf(os, "    li $a0, 0xA\n");
            fprintf(os, "    syscall\n");
            break;
        // ASSIGN
        case QUAD_UOP_ASSIGN:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->right->name);
            fprintf(os, "    sw $t0, PTC_%s_\n", qt->q->res->name);
            break;
        // GOTO
        case QUAD_GOTO_IF:
            fprintf(os, "    lw $t0, PTC_%s_\n", qt->q->left->name);
            fprintf(os, "    lw $t1, PTC_%s_\n", qt->q->right->name);
            switch (qt->q->cmp)
            {
            case QUAD_RELOP_EQ:
                fprintf(os, "    beq $t0, $t1, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            case QUAD_RELOP_NEQ:
                fprintf(os, "    bne $t0, $t1, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            case QUAD_RELOP_GT:
                fprintf(os, "    slt $t2, $t1, $t0\n");
                fprintf(os, "    beq $t2, 0, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            case QUAD_RELOP_GTE:
                fprintf(os, "    slt $t2, $t1, $t0\n");
                fprintf(os, "    beq $t2, 0, LABEL_Q_%d\n", qt->q->dest->id);
                fprintf(os, "    beq $t0, $t1, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            case QUAD_RELOP_LT:
                fprintf(os, "    slt $t2, $t0, $t1\n");
                fprintf(os, "    beq $t2, 0, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            case QUAD_RELOP_LTE:
                fprintf(os, "    slt $t2, $t0, $t1\n");
                fprintf(os, "    beq $t2, 0, LABEL_Q_%d\n", qt->q->dest->id);
                fprintf(os, "    beq $t0, $t1, LABEL_Q_%d\n", qt->q->dest->id);
                break;
            }
            break;
        case QUAD_GOTO:
            fprintf(os, "    j LABEL_Q_%d\n", qt->q->dest->id);
            break;
        case QUAD_GOTO_END:
            fprintf(os, "    j LABEL_END\n");
            break;
        }

        qt = qt->next;
    }
    fprintf(os, "LABEL_END:\n");
    fprintf(os, "    li $v0, 10\n");
    fprintf(os, "    syscall\n");

    // LOOOOOL return;
}
