/**
* @class CYm3
* Classe permettant la gestion d'un YM3
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm3.h"
#include <iostream>

#include "CError.h"

// Constructeur
CYm3::CYm3(const char *i_ymData,const int i_tYmData)
{
	const char *pYmData = i_ymData;
	
	memcpy(m_ID,pYmData,T_ID);
	m_ID[T_ID]='\0';
	pYmData += T_ID;
	
	m_ValidVBL = (i_tYmData-4)/14;
	m_Loop = 0;
	
	allocateYmRegisters();
	
	int r;
	for (r=0;r<NB_REG_PSG;r++)
	{
		memcpy(m_RegPSG[r],pYmData,m_ValidVBL);
		pYmData += m_ValidVBL;
	}
	
	if (((int)pYmData - (int)i_ymData) != i_tYmData)
	{
		free((void *)i_ymData);
		TOOLS_ERRORMSG("File size incorrect !" << "Size Found " << ((int)pYmData - (int)i_ymData) << " Real Size " << i_tYmData);
	}
}

CYm3::~CYm3()
{
}

std::string CYm3::getName() const {return std::string("Unknown name");}
std::string CYm3::getAuthor() const {return std::string("Unknown author");}
std::string CYm3::getComments() const {return std::string("");}

const bool CYm3::containsSID() const {return false;}
const bool CYm3::containsDigitDrums() const {return false;}
const bool CYm3::containsSinusSID() const {return false;}
const bool CYm3::containsSyncBuzzer() const {return false;}
const bool CYm3::is4BitsDigitDrums() const {return false;}
const bool CYm3::isSignedDigitDrums() const {return false;}
const bool CYm3::isInterlaced() const {return false;}
const unsigned int CYm3::getYmFrequency() const {return ST_YM_FREQUENCY;}
const unsigned int CYm3::getPlayFrequency() const {return VBL_PLAY_FREQUENCY;};
	
const unsigned int CYm3::getNbDigitDrums() const {return 0;}
const unsigned int CYm3::getDigitDrumsSize(unsigned int i_num) const {return 0;}
const unsigned char* CYm3::getDigitDrums(unsigned int i_num) const {return NULL;}
	
const unsigned char* CYm3::getSpecialEffect1Type() const {return NULL;}
const unsigned char* CYm3::getSpecialEffect1Voice() const {return NULL;}
const unsigned char* CYm3::getSpecialEffect1Parameter() const {return NULL;}
const unsigned int* CYm3::getSpecialEffect1Frequency() const {return NULL;}
	
const unsigned char* CYm3::getSpecialEffect2Type() const {return NULL;}
const unsigned char* CYm3::getSpecialEffect2Voice() const {return NULL;}
const unsigned char* CYm3::getSpecialEffect2Parameter() const {return NULL;}
const unsigned int* CYm3::getSpecialEffect2Frequency() const {return NULL;}
