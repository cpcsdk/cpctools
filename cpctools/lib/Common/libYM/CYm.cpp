/**
* @class CYm
* Classe permettant la gestion d'un YM
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

#include "CError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructeur
CYm::CYm()
:m_ValidVBL(0),
m_Loop(0)
{	
}

// Destructeur
CYm::~CYm()
{
	for (int r=0;r<NB_REG_PSG;r++)
	{
		delete[] m_RegPSG[r];
	}
}

// Allocation des registres Ym (14 registres !)
void CYm::allocateYmRegisters()
{
	for (int r=0;r<NB_REG_PSG;r++)
	{
		m_RegPSG[r] = new unsigned char[m_ValidVBL];
	}
}

// Calcul de la valeur unsigned long
const unsigned long CYm::getLong(const char *io_long)
{
	unsigned char o0 = *(io_long);
	unsigned char o1 = *(io_long+1);
	unsigned char o2 = *(io_long+2);
	unsigned char o3 = *(io_long+3);
	
	unsigned long val = o0*256*256*256 + o1*256*256 + o2*256 + o3;
	
	return val;
}

// Calcul de la valeur unsigned short
const unsigned short CYm::getShort(const char *io_short)
{
	unsigned char o0 = *(io_short);
	unsigned char o1 = *(io_short+1);
	
	unsigned short val = o0*256 + o1;
	
	return val;
}

// Calcul la frequence en fonction du Timer ST
const unsigned int CYm::getTimerFrequence(const unsigned char i_TP, const unsigned char i_TC)
{
	unsigned int val_prediv;
	
	switch(i_TP) 
    {
    case 0 : {val_prediv=0;break;}
    case 1 : {val_prediv=4;break;}
    case 2 : {val_prediv=10;break;}
    case 3 : {val_prediv=16;break;}
    case 4 : {val_prediv=50;break;}
    case 5 : {val_prediv=64;break;}
    case 6 : {val_prediv=100;break;}
    case 7 : {val_prediv=200;break;}
    default : {TOOLS_ERRORMSG("Valeur prédiviseur Timer inconnue");}
    }
	if (i_TC != 0)
    {
		if (val_prediv != 0)
			return ((TIMER_FREQUENCE / val_prediv) / i_TC);
		else
			return 0;
    } 
	else
    {
		if (val_prediv != 0)
			return (TIMER_FREQUENCE / val_prediv);
		else
			return 0;
    }
}

const char* CYm::getID() const
{
	return m_ID;
}
// Renvoie le nombre de VBL du YM
const unsigned int CYm::nbValidVBL() const
{
	return m_ValidVBL;
}

// Renvoie le loop du YM
const unsigned int CYm::loopVBL() const
{
	return m_Loop;
}

// Renvoie un pointeur sur les datas du registre i_numReg
const unsigned char * CYm::getRegister(const unsigned int i_numReg) const
{
	return m_RegPSG[i_numReg];
}

void CYm::afficheYM(std::ostream &io_stream) const
{
	float min,sec;
	io_stream << "YM version\t: " << getID() << std::endl;
	io_stream << "Name\t\t: " << getName() << std::endl;
	io_stream << "Author\t\t: " << getAuthor() << std::endl;
	io_stream << "Comments\t: " << getComments() << std::endl << std::endl;
	
	io_stream << "Total Time\t: ";
	min = floor(nbValidVBL()/(getPlayFrequency()*60.0f));
	sec = floor((nbValidVBL()-min*getPlayFrequency()*60.0f)/getPlayFrequency());
	io_stream << min << ":";
	io_stream.fill('0');io_stream.width(2);io_stream << sec;
	io_stream << " (" << nbValidVBL() << " frames)" << std::endl;
	
	io_stream << "Loop Time\t: ";
	min = floor(loopVBL()/(getPlayFrequency()*60.0f));
	sec = floor((loopVBL()-min*getPlayFrequency()*60.0f)/getPlayFrequency());
	io_stream << min << ":";
	io_stream.fill('0');io_stream.width(2);io_stream << sec;
	io_stream << " (frame " << loopVBL() << ")" << std::endl;

	io_stream << "YM Chip Freq\t: " << getYmFrequency()/1000000 << " Mhz" << std::endl;
	io_stream << "Replay Freq\t: " << getPlayFrequency() << " Hz" << std::endl << std::endl;
	if (isInterlaced())
		io_stream << "Interlaced Datas" << std::endl;
	else
		io_stream << "No interlaced Datas" << std::endl;
	if (containsDigitDrums())
	{
		std::string ddSize = (is4BitsDigitDrums()) ? "4 Bits" : "8 Bits";
		std::string ddSign = (isSignedDigitDrums()) ? " Signed" : "";
		io_stream << "Contains " << ddSize << ddSign << " Digit drums" << std::endl;
		for (int i=0;i<getNbDigitDrums();i++)
		{
			io_stream << "\tDigitDrum " << i << ":" << getDigitDrumsSize(i) << " bytes" << std::endl;
		}
	}
	if (containsSID())
	{
		io_stream << "Contains SID" << std::endl;
	}
	if (containsSinusSID())
	{
		io_stream << "Contains Sinus SID" << std::endl;
	}
	if (containsSyncBuzzer())
	{
		io_stream << "Contains Sync Buzzer" << std::endl;
	}
}