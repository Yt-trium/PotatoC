#ifndef MIPS_H
#define MIPS_H

#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"
#include "quad.h"

/**
 * @brief   Transform the symbol table and the quad list to an assembly source file
 * @param   symbol table
 * @param   quad list
 * @param   file pointer
 * @return  nothing
 */
void toMips(symbol, quad_list, FILE*);

#endif // MIPS_H
