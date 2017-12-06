#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdbool.h>

#define SYMBOL_MAX_NAME_LENGTH 128

typedef enum{
    SYMBOL_INT,
    SYMBOL_FUNC,
    SYMBOL_CST,
    SYMBOL_ID
} symbol_type;

typedef struct symbol_{
    char*           name;
    symbol_type     type;
    int             value;
    struct symbol_* next;
} symbol_, *symbol;

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
 * @brief symbol_list_add
 * @return
 */
symbol          symbol_list_add(symbol *, symbol);

/**
  * @brief Check if the given symbols exists and yell if not
  */
symbol symbol_must_find(symbol, char *);

/**
 * @brief Find a symbol with the specified name in the list
 * @return      The symbol or NULL
 */
symbol symbol_find(symbol, char*);

/**
 * @brief symbol_list_print
 */
void            symbol_list_print(symbol);

/**
  * Create a new symbol with a given name
  * @param  The head of the symbol list
  * @param  The name of the symbol
  */
symbol symbol_new(symbol *, char *);

/**
 * @brief symbol_new_temp
 * @return
 */
symbol symbol_new_temp(symbol *);

/**
 * @brief Create a new symbol for a const
 * @return
 */
symbol symbol_new_const(symbol *, int);

/**
 * @brief clean_symbol_list
 * @param head
 */
void symbol_free_memory(symbol head);

#endif // SYMBOL_H
