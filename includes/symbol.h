#ifndef __SYMBOLE_TABLE__
#define __SYMBOLE_TABLE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
  * Symbol
  */
typedef struct symbol{
    char * name;
    bool constant;
    int value;
    struct symbol * next;
} symbol;

symbol * alloc_symbol();

symbol * add_symbol(symbol ** head, char* name);

symbol * find_symbol(symbol * head, char * name);

void print_symbol(symbol * head);

#endif
