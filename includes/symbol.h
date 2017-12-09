#ifndef SYMBOL_H
#define SYMBOL_H

/** @enum symbol_typ
  * @brief Type of a symbol.
  */
typedef enum{
    SYMBOL_INT,
    SYMBOL_FUNC,
    SYMBOL_CST
} symbol_type;

/** @struct symbol_
  * @brief A symbol
  * @member name    The variable's name
  * @member type    The symbol's type
  * @member value   The symbol's value if any
  * @member next    The next symbol after this one in the chained list
  */
typedef struct symbol_{
    char*           name;
    symbol_type     type;
    int             value;
    struct symbol_* next;
} symbol_, *symbol;

/**
 * @brief Alloc in memory a new symbol
 * @return The allocated symbol
 */
symbol symbol_alloc();

/**
  * Create a new ID symbol with a given name
  * @param  The head of the symbol list
  * @param  The name of the symbol
  * @return The added symbol
  */
symbol symbol_new(symbol *, char *);

/**
 * @brief Create a new TEMPORARY symbol
 * @param   The symbol list
 * @return  The added symbol
 */
symbol symbol_new_temp(symbol *);

/**
 * @brief Create a new CONST symbol
 * @param   The symbol list
 * @param   The const value
 * @return  The added symbol
 */
symbol symbol_new_const(symbol *, int);

/**
 * @brief Add a given symbol in a given list
 * @param       A pointer to the pointer of the list
 * @param       The symbol's pointer
 * @return      The added symbol
 */
symbol symbol_list_add(symbol *, symbol);

/**
  * @brief Find a symbol with the given name in a given list and yell if nothing is found.
  * @param      The symbol list
  * @param      The symbol name
  * @return     The symbol or NULL
  */
symbol symbol_must_find(symbol, char *);

/**
 * @brief Same as symbol_must_find but doesn't yell.
 * @param      The symbol list
 * @param      The symbol name
 * @return      The symbol or NULL
 */
symbol symbol_find(symbol, char*);

/**
 * @brief Print a given symbol
 * @param       The symbol
 */
void symbol_print(symbol);

/**
 * @brief Prints all the symbols of the given list.
 * @param       The symbol list
 */
void symbol_list_print(symbol);

/**
 * @brief Free the memory used by the given symbol list
 * @param   The symbol list
 */
void symbol_free_memory(symbol);

#endif // SYMBOL_H
