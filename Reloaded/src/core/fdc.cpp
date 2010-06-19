/*
 *   $Id: fdc.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 Reloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
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



//
// Caprice32 FDC emulator
//


#include "config.h"


#ifdef HAVE_LIB765_H
#include "fdc_765.cpp"
#else
#include "fdc_old.cpp"
#endif

