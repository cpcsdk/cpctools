/**
 *   $Id$
 *	 Reloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  

#ifndef __CAPASM__H__
#define __CAPASM__H__

#include "emulator.h"

/**
 * Wrapper between Reloaded and SjasmPLUS
 */
class CapASM
{
  private:
    /**
     * Reference to the emu
     */
    Emulator *emulator ;

  public:
    CapASM( Emulator *emulator)
    {
      this->emulator = emulator ;
    }

    /**
     * Compile the source file and inject result in emu memory
     * @param filename Name of the z80 source file
     */
    void Compile(const char * filename);

    /**
     * Launch the last compiled code
     */
    void Run();

};
#endif
