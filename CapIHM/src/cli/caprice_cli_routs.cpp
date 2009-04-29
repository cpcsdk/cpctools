/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
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


#include <iostream>
#include "emulator.h"
#include "z80.h"
#include "render.h"

/**
 * Reference to the Emulator
 */
extern Emulator *emulatorClone;

extern "C" void caprice_cli_show_registers()
{
  std::cout << "Z80 Registers :" << std::endl ;
}

extern "C" void caprice_cli_emu_reset()
{
  emulatorClone->emulator_reset(true);
}

extern "C" void caprice_cli_add_breakpoint(int adress)
{
  std::cout << "# Add breakpoint :" << adress << endl ;
  emulatorClone->GetZ80().add_break_point(adress);
}

extern "C" void caprice_cli_remove_breakpoint(int adress)
{
  std::cout << "# Remove breakpoint :" << adress << endl ;
  emulatorClone->GetZ80().remove_break_point(adress);
}


extern "C" void caprice_cli_show_breakpoints()
{
  std::cout << "List of breakpoints :" << std::endl ;
  std::set<dword>  breakpoints = emulatorClone->GetZ80().GetBreakpoints() ;

  std::set<dword>::const_iterator iter;
  int i = 0 ;
  for(iter = breakpoints.begin(); iter != breakpoints.end() ; iter++)
  {
    std::cout << i << " : " << *iter << endl ;
    i++;
  }
}

extern "C" 
void caprice_cli_video_color(int mode)
{
  if (mode == 1)
  {
    std::cout << "# Set color screen" << endl;
    emulatorClone->GetRenderer().SetMonitorColorTube(Renderer::ColoursMode);
  }
  else if (mode == 2)
  {
    std::cout << "# Set grey screen" << endl;
    emulatorClone->GetRenderer().SetMonitorColorTube(Renderer::GreyMode);
  }
  else if (mode == 3)
  {
    std::cout << "# Set green screen" << endl;
    emulatorClone->GetRenderer().SetMonitorColorTube(Renderer::GreenMode);
  }
}

extern "C"
void caprice_cli_memory_peek(int address)
{
  byte * ram = emulatorClone->GetMemory().GetRAM();
  int value = ram[address];

  std::cout << std::hex << value << endl ;
}


//TODO verify in order to avoid memory leaks
extern "C"
void caprice_cli_memory_poke(int address, int value)
{
  byte * ram = emulatorClone->GetMemory().GetRAM();

  if (value>255)
  {
    std::cout << "# Put byte 0x" << std::hex << value << " in 0x" << std::hex << address << endl ;
    ram[address] = value ;
  }
  else
  {
    std::cout << "# Put word " << value << " in " << address << endl ;
    byte high  = value/256 ;
    byte low   = value%256 ;

    ram[address] = low ;
    ram[ (address+1) & 0xffff ] = high ;
  }
  
}
