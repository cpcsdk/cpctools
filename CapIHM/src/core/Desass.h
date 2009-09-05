#ifndef DESASS_H
#define DESASS_H

#include "memory.h"
#include <iostream>

/**
 * Desassemble memory
 * @param Prg Memory to disassm
 * @param Listing Output stream to populate
 * @param Start Start point
 * @param Size quantity of data to read
 */
void Desass( t_Memory mem, std::ostream &Listing, int Start=0x0000, int Size=0x10000 );


#endif
