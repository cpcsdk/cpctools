/**
* @class CYm4
* Classe permettant la gestion d'un YM4
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm4.h"
#include <iostream>
#include "CError.h"

// Constructeur
CYm4::CYm4(const char *i_ymData,const int i_tYmData)
{
	const char *pYmData = i_ymData;
	
	memcpy(m_ID,pYmData,T_ID); 
	m_ID[T_ID]='\0';
	pYmData += T_ID;
	
	memcpy(m_Check,pYmData,T_CHECK); 
	m_Check[T_CHECK]='\0';
	pYmData += T_CHECK;
	
	if (strcmp(m_Check,"LeOnArD!")!=0)
	{
		free((void *)i_ymData);
		TOOLS_ERRORMSG("Check 'LeOnArD' incorrect");
	}
	
	m_ValidVBL = getLong(pYmData); 
	pYmData += T_DWORD;
	
	m_SngAttrib = getLong(pYmData);
	pYmData += T_DWORD;
	
	m_NbDD = getShort(pYmData);
	pYmData += T_WORD;
	
	m_Loop = getLong(pYmData); 
	pYmData += T_DWORD;
	
	if (m_NbDD != 0) 
	{
		for (int i=0;i<m_NbDD;i++) 
		{
			m_DDSize[i] = getLong(pYmData);
			pYmData += T_DWORD;
			
			m_DigitDrums[i] = new unsigned char[m_DDSize[i]];
			memcpy(m_DigitDrums[i],pYmData,m_DDSize[i]);
			pYmData += m_DDSize[i];
		}
	}
	
	m_Name = pYmData;
	pYmData += m_Name.length() + 1;
	m_Author = pYmData;
	pYmData += m_Author.length() + 1;
	m_Comments = pYmData;
	pYmData += m_Comments.length() + 1;
	
	allocateYmRegisters();
	
	if (isInterlaced())
	{
		int r;
		for (r=0;r<NB_REG_PSG;r++)
		{
			memcpy(m_RegPSG[r],pYmData,m_ValidVBL);
			pYmData += m_ValidVBL;
		}
		for (r=0;r<NB_VIRTUAL_REG;r++)
		{
			memcpy(m_VirtualRegPSG[r],pYmData,m_ValidVBL);
			pYmData += m_ValidVBL;
		}
	} else
	{
		for (unsigned int vbl=0;vbl<m_ValidVBL;vbl++)
		{
			int r;
			for (r=0;r<NB_REG_PSG;r++)
			{
				m_RegPSG[r][vbl] = *pYmData;
				pYmData ++;
			}
			for (r=0;r<NB_VIRTUAL_REG;r++)
			{
				m_VirtualRegPSG[r][vbl] = *pYmData;
				pYmData ++;
			}
		}
	}
	
	memcpy(m_CheckEnd,pYmData,T_CHECKEND); 
	m_CheckEnd[T_CHECKEND]='\0';
	pYmData += T_CHECKEND;
	
	if (strcmp(m_CheckEnd,"End!")!=0)
	{
		std::cerr << "Check 'End!' incorrect... Continue\n";
		std::cerr << "Check End : " << m_CheckEnd << "\n";
		pYmData = i_ymData+i_tYmData;
	}
	
	if (((int)pYmData - (int)i_ymData) != i_tYmData)
	{
		free((void *)i_ymData);
		TOOLS_ERRORMSG("File size incorrect !" << "Size Found " << ((int)pYmData - (int)i_ymData) << " Real Size " << i_tYmData);
	}
	
	decodeYM4();
	
}

// Destructeur
CYm4::~CYm4()
{
	for (int i=0;i<m_NbDD;i++)
	{
		delete[] m_DigitDrums[i];
	}
	for (int r=0;r<NB_VIRTUAL_REG;r++)
	{
		delete[] m_VirtualRegPSG[r];
	}
	delete[] m_SE1Type;
	delete[] m_SE1Voice;
	delete[] m_SE1Parameter;
	delete[] m_SE1Freq;
}

// Alloue les registres YM
void CYm4::allocateYmRegisters()
{
	CYm::allocateYmRegisters();
	for (int r=0;r<NB_VIRTUAL_REG;r++)
	{
		m_VirtualRegPSG[r] = new unsigned char[m_ValidVBL];
	}
	m_SE1Type = new unsigned char[m_ValidVBL];
	m_SE1Voice = new unsigned char[m_ValidVBL];
	m_SE1Parameter = new unsigned char[m_ValidVBL];
	m_SE1Freq = new unsigned int[m_ValidVBL];
}

// Special effect 1
// Renvoie la frequence du special effect 1
const unsigned int CYm4::getSE1Freq(const int i_vbl)
{
	if ((getSE1Type(i_vbl) == SID) || (getSE1Type(i_vbl) == SID_RESTARTED))
	{
		unsigned char tp = (m_RegPSG[6][i_vbl] & (32 + 64 + 128)) >> 5;
		unsigned char tc = m_VirtualRegPSG[0][i_vbl];
		return getTimerFrequence(tp,tc);
	}
	if (getSE1Type(i_vbl) == DIGIT_DRUMS)
	{
		unsigned char tp = (m_RegPSG[8][i_vbl] & (32 + 64 + 128)) >> 5;
		unsigned char tc = m_VirtualRegPSG[1][i_vbl];
		return getTimerFrequence(tp,tc);
	}
	return 0;
}

// Renvoie le parametre du special effect 1
const unsigned char CYm4::getSE1Parameter(const int i_vbl)
{
	if ((getSE1Type(i_vbl) == SID) || (getSE1Type(i_vbl) == SID_RESTARTED))
	{
		return ((m_RegPSG[5][i_vbl] & (16 + 32 + 64 + 128)) >> 4);
	}
	if (getSE1Type(i_vbl) == DIGIT_DRUMS)
	{
		return (m_RegPSG[getSE1Voice(i_vbl)][i_vbl]);
	}
	return 0;
}

// Renvoie la voie du special effect 1
const unsigned char CYm4::getSE1Voice(const int i_vbl)
{
	if ((getSE1Type(i_vbl) == SID) || (getSE1Type(i_vbl) == SID_RESTARTED))
	{
		return (((m_RegPSG[1][i_vbl] & (16 + 32)) >> 4) + 7);
	}
	if (getSE1Type(i_vbl) == DIGIT_DRUMS)
	{
		return (((m_RegPSG[3][i_vbl] & (16 + 32)) >> 4) + 7);
	}
	return 0;
}

// Renvoie le type du special effect 1
const unsigned char CYm4::getSE1Type(const int i_vbl)
{
	if ((m_RegPSG[1][i_vbl] & (16+32)) != 0)
	{
		if ((m_RegPSG[1][i_vbl] & (64)) != 0)
		{
			return SID_RESTARTED;
		} else
		{
			return SID;
		}
	}
	if ((m_RegPSG[3][i_vbl] & (16+32)) != 0)
	{
		return DIGIT_DRUMS;
	}
	
	return NO_EFFECT;
}

// Decode le format YM4
void CYm4::decodeYM4()
{
	for (unsigned int i=0;i<m_ValidVBL;i++)
	{
		m_SE1Type[i] = getSE1Type(i);
		if (m_SE1Type[i] != NO_EFFECT)
		{
			m_SE1Parameter[i] = getSE1Parameter(i);
			m_SE1Voice[i] = getSE1Voice(i);
			m_SE1Freq[i] = getSE1Freq(i);
		}
	}
}

std::string CYm4::getName() const {return m_Name;}
std::string CYm4::getAuthor() const {return m_Author;}
std::string CYm4::getComments() const {return m_Comments;}

const bool CYm4::containsDigitDrums() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if (m_SE1Type[i] == DIGIT_DRUMS)
		{
			return true;
		}
		i++;
	}
	return false;
}
const bool CYm4::containsSID() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if ((m_SE1Type[i] == SID) || (m_SE1Type[i] == SID_RESTARTED))
		{
			return true;
		}
		i++;
	}
	return false;
}
const bool CYm4::containsSinusSID() const {return false;}
const bool CYm4::containsSyncBuzzer() const {return false;}
const bool CYm4::isInterlaced() const {return ((m_SngAttrib & 1) != 0);}
const bool CYm4::isSignedDigitDrums() const {return ((m_SngAttrib & 2) != 0);}
const bool CYm4::is4BitsDigitDrums() const {return ((m_SngAttrib & 4) != 0);}

const unsigned int CYm4::getYmFrequency() const {return ST_YM_FREQUENCY;}
const unsigned int CYm4::getPlayFrequency() const {return VBL_PLAY_FREQUENCY;}

const unsigned int CYm4::getNbDigitDrums() const {return m_NbDD;}
const unsigned int CYm4::getDigitDrumsSize(unsigned int i_num) const {return m_DDSize[i_num];}
const unsigned char* CYm4::getDigitDrums(unsigned int i_num) const {return m_DigitDrums[i_num];}
	
const unsigned char* CYm4::getSpecialEffect1Type() const {return m_SE1Type;}
const unsigned char* CYm4::getSpecialEffect1Voice() const {return m_SE1Voice;}
const unsigned char* CYm4::getSpecialEffect1Parameter() const {return m_SE1Parameter;}
const unsigned int* CYm4::getSpecialEffect1Frequency() const {return m_SE1Freq;}
	
const unsigned char* CYm4::getSpecialEffect2Type() const {return NULL;}
const unsigned char* CYm4::getSpecialEffect2Voice() const {return NULL;}
const unsigned char* CYm4::getSpecialEffect2Parameter() const {return NULL;}
const unsigned int* CYm4::getSpecialEffect2Frequency() const {return NULL;}

