/**
* @class CCPCSystemDisc
* Classe permettant la gestion d'un disc CPC System (utilisation de la sous-lib DSK)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#pragma warning( disable : 4786 )

#include <iostream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "libdsk.h"
#include <errno.h>

#include <vector>

#include "CCPCDisc.h"

#ifndef _CCPCSYSTEMDISC_HH_
#define _CCPCSYSTEMDISC_HH_

class CCPCSystemDisc:public CCPCDisc
{
private:
protected:
	
	/// Indique que le bloc correspond au boot CPM
	static unsigned int boot;
	
	/// Renvoie la liste des block libres
	virtual void getEmptyBlock(std::vector<unsigned int> &o_block) const;
public:
	/// Lit un bloc sur le disc (buffer deja alloue !)
	virtual void ReadBlock(const unsigned char i_idBlock, void* i_buffer) const;
	/// Ecrit un secteur sur le disc
	virtual void WriteBlock(const unsigned char i_idBlock, const void* i_buffer);
	
};

#endif
