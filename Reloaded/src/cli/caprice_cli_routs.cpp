/**
 *   $Id$
 *   CapriceReloded an Amstrad CPC emulator
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
#include "Desass.h"

#if WITH_ASM
#include "../asm/CapASM.h"
#endif

#include "caprice_cli_routs.h"

void caprice_cli_show_registers()
{
  {
    std::cout << "Z80 Registers :" << std::endl ;
    std::cout << "AF : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().AF.w.l 
      << "\t\tAF' : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().AFx.w.l 
      << std::endl ;
    std::cout << "BC : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().BC.w.l 
      << "\t\tBC' : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().BCx.w.l 
      << std::endl ;
    std::cout << "DE : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().DE.w.l 
      << "\t\tDE' : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().DEx.w.l 
      << std::endl ;
    std::cout << "HL : 0x" <<  uppercase << hex << Emulator::getInstance()->GetZ80().HL.w.l 
      << "\t\tHL' : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().HLx.w.l 
      << std::endl ;

    std::cout << "PC : 0x" <<  uppercase << hex << Emulator::getInstance()->GetZ80().PC.w.l 
      << "\t\tSP : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().SP.w.l 
      << std::endl ;

    std::cout << "IX  : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().IX.w.l 
      << "\t\tIY  : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().IY.w.l 
      << std::endl ;
 
    std::cout << "I  : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().I 
      << "\t\tR  : 0X" <<  uppercase << hex << Emulator::getInstance()->GetZ80().R 
      << std::endl ;

  }
}

void caprice_cli_emu_reset()
{
  Emulator::getInstance()->emulator_reset(true);
}

void caprice_cli_add_breakpoint(int adress)
{
  std::cout << "# Add breakpoint :" << adress << endl ;
  Emulator::getInstance()->GetZ80().add_break_point(adress);
}

void caprice_cli_remove_breakpoint(int adress)
{
  std::cout << "# Remove breakpoint :" << adress << endl ;
  Emulator::getInstance()->GetZ80().remove_break_point(adress);
}

void caprice_cli_show_breakpoints()
{
  std::cout << "List of breakpoints :" << std::endl ;
  std::set<dword>  breakpoints = Emulator::getInstance()->GetZ80().GetBreakpoints() ;

  std::set<dword>::const_iterator iter;
  int i = 0 ;
  for(iter = breakpoints.begin(); iter != breakpoints.end() ; iter++)
  {
    std::cout << i << " : " << *iter << endl ;
    i++;
  }
}

void caprice_cli_continue()
{
  if (Emulator::getInstance()->GetConfig()->breakpoint)
  {
    std::cout << "# Continue execution flow" << std::endl ;
    Emulator::getInstance()->Run();
  }
  else
  {
    std::cout << "# Error ! Emulator not in a breakpoint strop !" << std::endl ;
  }
}

void caprice_cli_step()
{
  if (Emulator::getInstance()->GetConfig()->breakpoint)
  {
    std::cout << "# Continue execution flow step by step" << std::endl ;
    Emulator::getInstance()->Step();
  }
  else
  {
    std::cout << "# Error ! Emulator not in a breakpoint strop !" << std::endl ;
  }

}

void caprice_cli_video_color(int mode)
{
  if (mode == 1)
  {
    std::cout << "# Set color screen" << endl;
    Emulator::getInstance()->GetRenderer().SetMonitorColorTube(Renderer::ColoursMode);
  }
  else if (mode == 2)
  {
    std::cout << "# Set grey screen" << endl;
    Emulator::getInstance()->GetRenderer().SetMonitorColorTube(Renderer::GreyMode);
  }
  else if (mode == 3)
  {
    std::cout << "# Set green screen" << endl;
    Emulator::getInstance()->GetRenderer().SetMonitorColorTube(Renderer::GreenMode);
  }
}

void caprice_cli_memory_peek(int address)
{
  byte * ram = Emulator::getInstance()->GetMemory().GetRAM();
  int value = ram[address];

  std::cout << std::hex << value << endl ;
}

//TODO verify in order to avoid memory leaks
void caprice_cli_memory_poke(int address, int value)
{
  byte * ram = Emulator::getInstance()->GetMemory().GetRAM();

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

void caprice_cli_memory_disasm_quantity(int address, int size)
{
   std::cout << address << "\t" << size << endl;
  Disassemble(
      Emulator::getInstance()->GetMemory(),
      (std::ostream &) std::cout,
      address,
      size);

}

void caprice_cli_memory_disasm(int address)
{
  int size ;

  size = 0x10000 - address  ;
  caprice_cli_memory_disasm_quantity( address, size) ;
}

void caprice_cli_memory_disasm_pc(int size)
{
  caprice_cli_memory_disasm_quantity( 
      Emulator::getInstance()->GetZ80().PC.w.l , 
      size) ;
}

//Todo destroy it at the end
CapASM * capASM = NULL ;

void caprice_cli_asm_compile_file(char *filename)
{
#if WITH_ASM
//  if (capASM != NULL)
//  {
//    delete capASM;
//    capASM = NULL;
//  }

  capASM = new CapASM(Emulator::getInstance());
  capASM->Compile(filename);
#else
  std::cout << "Assembler not enabled in this binary !" << endl;
#endif
}

void  caprice_cli_asm_launch(int address)
{
#if WITH_ASM
  if (address == -1)
  {
    if (capASM != NULL ){capASM->Run();}
  }
  else
  {
    Emulator::getInstance()->GoTo(address);
  }
#else
  std::cout << "Assembler not enabled in this binary !" << endl;
#endif
}

// TODO: Autoconf: Make conditional on libpng presence
void caprice_cli_screenshot_save()
{
  Emulator::getInstance()->SaveScreenshot("caprice.png");
}
