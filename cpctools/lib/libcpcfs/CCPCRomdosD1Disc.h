/**
* @class CCPCRomdosD1Disc
* Classe permettant la gestion d'un disc CPC Romdos D1 (utilisation de la sous-lib DSK)
* @author PulkoMandy
* @version 1.0
* @date 19/09/08
*/

#pragma once

#include <iostream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "libdsk.h"
#include <errno.h>
#include <string.h>

#include <vector>

#include "CCPCDisc.h"


class CCPCRomdosD1Disc:public CCPCDisc
{
private:
protected:
	/// Renvoie la liste des block libres
	virtual void getEmptyBlock(std::vector<unsigned int> &o_block) const;
	
public:
	/// Lit un bloc sur le disc (buffer deja alloue !)
	virtual void ReadBlock(const unsigned char i_idBlock, void* i_buffer) const;
	/// Ecrit un secteur sur le disc
	virtual void WriteBlock(const unsigned char i_idBlock, const void* i_buffer);
	
};

