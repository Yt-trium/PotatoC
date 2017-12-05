#include "test.h"
#include "ast.h"

int test_all()
{
    printf(".------------------.\n");
    printf("|     TEST ALL     |\n");
    printf(".------------------.\n");

    // gen an AST
    ast* n1 = ast_new_number(2);
    ast* n2 = ast_new_number(5);
    ast* n12 = ast_new_operation(AST_OP_PLUS, n1, n2);
    ast* n3 = ast_new_number(8);
    ast* n123 = ast_new_operation(AST_OP_PLUS, n12, n3);

    ast_print(n123,0);

    symbol  symbol_table = NULL;

    //symbol_new_temp(&symbol_table, 0);
    //symbol_new_temp(&symbol_table, 2);

    symbol_list_print(symbol_table);

    // gen code from AST
    quad    quad_list = NULL;

    ast_codegen(n123,&symbol_table);

    printf(".------- END ------.\n");
    return 0;
}
