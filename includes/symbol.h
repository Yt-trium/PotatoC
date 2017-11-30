#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

typedef enum{
    SYMBOL_INT,
    SYMBOL_FUNC,
    SYMBOL_CST
} symbol_type;

typedef struct symbol_{
    char*           name;
    symbol_type     type;
    int             value;
    struct symbol_* next;
} symbol_, *symbol;

typedef symbol* symbol_list;

/**
 * @brief symbol_alloc alloc a default symbol
 * @return
 */
symbol          symbol_alloc();
/**
 * @brief symbol_print print a symbol
 */
void            symbol_print(symbol);

/**
 * @brief symbol_list_alloc alloc a default list
 * @return
 */
symbol_list     symbol_list_alloc();
/**
 * @brief symbol_list_add
 * @return
 */
symbol          symbol_list_add(symbol_list, symbol);
/**
 * @brief symbol_list_find
 * @return
 */
symbol          symbol_list_find(symbol, symbol);
/**
 * @brief symbol_list_print
 */
void            symbol_list_print(symbol);

/**
 * @brief symbol_new_temp
 * @return
 */
symbol symbol_new_temp(symbol_list);

#endif // SYMBOL_H
