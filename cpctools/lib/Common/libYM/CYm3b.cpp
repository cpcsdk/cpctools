/**
* @class CYm3b
* Classe permettant la gestion d'un YM3b
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm3b.h"
#include <iostream>

#include "CError.h"

// Constructeur
CYm3b::CYm3b(const char *i_ymData,const int i_tYmData)
{
	const char *pYmData = i_ymData;
	
	memcpy(m_ID,pYmData,T_ID);
	m_ID[T_ID]='\0';
	pYmData += T_ID;
	
	m_ValidVBL = (i_tYmData-4)/14;
	
	allocateYmRegisters();
	
	int r;
	for (r=0;r<NB_REG_PSG;r++)
	{
		memcpy(m_RegPSG[r],pYmData,m_ValidVBL);
		pYmData += m_ValidVBL;
	}
	
	m_Loop = getLong(pYmData); 
	pYmData += T_DWORD;
	
	if (((int)pYmData - (int)i_ymData) != i_tYmData)
	{
		free((void *)i_ymData);
		TOOLS_ERRORMSG("File size incorrect !" << "Size Found " << ((int)pYmData - (int)i_ymData) << " Real Size " << i_tYmData);
	}
}

CYm3b::~CYm3b()
{
}

std::string CYm3b::getName() const {return std::string("Unknown name");}
std::string CYm3b::getAuthor() const {return std::string("Unknown author");}
std::string CYm3b::getComments() const {return std::string("");}

const bool CYm3b::containsSID() const {return false;}
const bool CYm3b::containsDigitDrums() const {return false;}
const bool CYm3b::containsSinusSID() const {return false;}
const bool CYm3b::containsSyncBuzzer() const {return false;}
const bool CYm3b::is4BitsDigitDrums() const {return false;}
const bool CYm3b::isSignedDigitDrums() const {return false;}
const bool CYm3b::isInterlaced() const {return false;}
const unsigned int CYm3b::getYmFrequency() const {return ST_YM_FREQUENCY;}
const unsigned int CYm3b::getPlayFrequency() const {return VBL_PLAY_FREQUENCY;};
	
const unsigned int CYm3b::getNbDigitDrums() const {return 0;}
const unsigned int CYm3b::getDigitDrumsSize(unsigned int i_num) const {return 0;}
const unsigned char* CYm3b::getDigitDrums(unsigned int i_num) const {return NULL;}
	
const unsigned char* CYm3b::getSpecialEffect1Type() const {return NULL;}
const unsigned char* CYm3b::getSpecialEffect1Voice() const {return NULL;}
const unsigned char* CYm3b::getSpecialEffect1Parameter() const {return NULL;}
const unsigned int* CYm3b::getSpecialEffect1Frequency() const {return NULL;}
	
const unsigned char* CYm3b::getSpecialEffect2Type() const {return NULL;}
const unsigned char* CYm3b::getSpecialEffect2Voice() const {return NULL;}
const unsigned char* CYm3b::getSpecialEffect2Parameter() const {return NULL;}
const unsigned int* CYm3b::getSpecialEffect2Frequency() const {return NULL;}

