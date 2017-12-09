#ifndef MIPS_H
#define MIPS_H

#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"
#include "quad.h"

void toMips(symbol, quad_list, FILE*);

#endif // MIPS_H
