/**
* @class CCPCDataDisc
* Classe permettant la gestion d'un disc CPC Data (utilisation de la sous-lib DSK)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#ifndef _CCPCDATADISC_HH_
#define _CCPCDATADISC_HH_

#include <iostream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "libdsk.h"
#include <errno.h>
#include <string.h>

#include <vector>

#include "CCPCDisc.h"

class CCPCDataDisc:public CCPCDisc
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

#endif
