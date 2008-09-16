/**
* @class CYm2
* Classe permettant la gestion d'un YM2 (sans les digitDrums !)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm2.h"
#include <string>
#include <iostream>
#include "CError.h"

// Constructeur
CYm2::CYm2(const char *i_ymData,const int i_tYmData)
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
		TOOLS_ERRORMSG("File size incorrect !");
	}
	
	checkYm2Format();
	
}

CYm2::~CYm2()
{
}

// Verifie le format YM2 (pas d'extras bits !)
const void CYm2::checkYm2Format()
{
	unsigned int vbl;
	for (vbl=0;vbl<m_ValidVBL;vbl++)
	{
		if ((m_RegPSG[1][vbl] >> 4) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 1 ! " << "R1 : " << (unsigned int)m_RegPSG[1][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[3][vbl] >> 4) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 3 ! " << "R3 : " << (unsigned int)m_RegPSG[3][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[5][vbl] >> 4) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 5 ! "<< "R5 : " << (unsigned int)m_RegPSG[5][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[6][vbl] >> 5) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 6 ! " << "R6 : " << (unsigned int)m_RegPSG[6][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[8][vbl] >> 5) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 8 ! " << "R8 : " << (unsigned int)m_RegPSG[8][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[9][vbl] >> 5) != 0)
		{
			TOOLS_ERRORMSG("Unknow bits on register 9 ! " << "R9 : " << (unsigned int)m_RegPSG[9][vbl] << " VBL : " << vbl);
		}
		if ((m_RegPSG[10][vbl] >> 5) != 0)
		{      
			TOOLS_ERRORMSG("Unknow bits on register 10 ! " << "R10 : " << (unsigned int)m_RegPSG[10][vbl] << " VBL : " << vbl);
		}
		if (((m_RegPSG[13][vbl] >> 4) != 0) && (m_RegPSG[13][vbl] != 255))
		{
			TOOLS_ERRORMSG("Unknow bits on register 13 ! " << "R13 : " << (unsigned int)m_RegPSG[13][vbl] << " VBL : " << vbl);
		}
	}
}

std::string CYm2::getName() const {return std::string("Unknown name");}
std::string CYm2::getAuthor() const {return std::string("Unknown author");}
std::string CYm2::getComments() const {return std::string("");}

const bool CYm2::containsSID() const {return false;}
const bool CYm2::containsDigitDrums() const {return false;}
const bool CYm2::containsSinusSID() const {return false;}
const bool CYm2::containsSyncBuzzer() const {return false;}
const bool CYm2::is4BitsDigitDrums() const {return false;}
const bool CYm2::isSignedDigitDrums() const {return false;}
const bool CYm2::isInterlaced() const {return false;}
const unsigned int CYm2::getYmFrequency() const {return ST_YM_FREQUENCY;}
const unsigned int CYm2::getPlayFrequency() const {return VBL_PLAY_FREQUENCY;};
	
const unsigned int CYm2::getNbDigitDrums() const {return 0;}
const unsigned int CYm2::getDigitDrumsSize(unsigned int i_num) const {return 0;}
const unsigned char* CYm2::getDigitDrums(unsigned int i_num) const {return NULL;}
	
const unsigned char* CYm2::getSpecialEffect1Type() const {return NULL;}
const unsigned char* CYm2::getSpecialEffect1Voice() const {return NULL;}
const unsigned char* CYm2::getSpecialEffect1Parameter() const {return NULL;}
const unsigned int* CYm2::getSpecialEffect1Frequency() const {return NULL;}
	
const unsigned char* CYm2::getSpecialEffect2Type() const {return NULL;}
const unsigned char* CYm2::getSpecialEffect2Voice() const {return NULL;}
const unsigned char* CYm2::getSpecialEffect2Parameter() const {return NULL;}
const unsigned int* CYm2::getSpecialEffect2Frequency() const {return NULL;}
