/**
* @class CYm6
* Classe permettant la gestion d'un YM6
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm6.h"
#include <iostream>
#include "CError.h"

// Constructeur
CYm6::CYm6(const char *i_ymData,const int i_tYmData)
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
	
	m_ChipFreq = getLong(pYmData); 
	pYmData += T_DWORD;
	
	m_PlayFreq = getShort(pYmData); 
	pYmData += T_WORD;
	
	m_Loop = getLong(pYmData); 
	pYmData += T_DWORD;
	
	m_Add = getShort(pYmData);
	pYmData += T_WORD;
	
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
	
	decodeYM6();
	
}

// Destructeur
CYm6::~CYm6()
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
	delete[] m_SE2Type;
	delete[] m_SE2Voice;
	delete[] m_SE2Parameter;
	delete[] m_SE2Freq;
}

// Allocation des registres YM
void CYm6::allocateYmRegisters()
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
	m_SE2Type = new unsigned char[m_ValidVBL];
	m_SE2Voice = new unsigned char[m_ValidVBL];
	m_SE2Parameter = new unsigned char[m_ValidVBL];
	m_SE2Freq = new unsigned int[m_ValidVBL];
}

// Special Effect 1
// Recupere le type du special effect 1
const unsigned char CYm6::getSE1Type(const int i_vbl)
{
	if ((m_RegPSG[1][i_vbl] & (16+32)) != 0)
	{
		switch (m_RegPSG[1][i_vbl] >> 6)
		{
		case 0 : {return SID;}
		case 1 : {return DIGIT_DRUMS;}
		case 2 : {return SINUS_SID;}
		case 3 : {return SYNC_BUZZER;}
		default : {TOOLS_ERRORMSG("Special effect 1 inconnu");}
		}
	} else {
		return NO_EFFECT;
	}
}

// Recupere la voie du special effect 1
const unsigned char CYm6::getSE1Voice(const int i_vbl)
{
	if (getSE1Type(i_vbl) != NO_EFFECT)
	{
		return (((m_RegPSG[1][i_vbl] & (16 + 32)) >> 4) + 7);
	} else
	{
		return 0;
	}
}

// Recupere la valeur de parametre du special effect 1
const unsigned char CYm6::getSE1Parameter(const int i_vbl)
{
	if (getSE1Type(i_vbl) == SID)
	{
		return (m_RegPSG[getSE1Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8));
	}
	if (getSE1Type(i_vbl) == DIGIT_DRUMS)
	{
		return (m_RegPSG[getSE1Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8 + 16));
	}
	if (getSE1Type(i_vbl) == SYNC_BUZZER)
	{
		return (m_RegPSG[getSE1Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8));
	}
	return 0;
}

// Recupere la frequence du special effect 1
const unsigned int CYm6::getSE1Freq(const int i_vbl)
{
	if (getSE1Type(i_vbl) != NO_EFFECT)
	{
		unsigned char tp = (m_RegPSG[6][i_vbl] & (32 + 64 + 128)) >> 5;
		unsigned char tc = m_VirtualRegPSG[0][i_vbl];
		return getTimerFrequence(tp,tc);
	}
	return 0;
}

// Special Effect 2
// Recupere le type du special effect 2
const unsigned char CYm6::getSE2Type(const int i_vbl)
{
	if ((m_RegPSG[3][i_vbl] & (16+32)) != 0)
	{
		switch (m_RegPSG[3][i_vbl] >> 6)
		{
		case 0 : {return SID;}
		case 1 : {return DIGIT_DRUMS;}
		case 2 : {return SINUS_SID;}
		case 3 : {return SYNC_BUZZER;}
		default : {TOOLS_ERRORMSG("Special effect 2 inconnu");}
		}
	} else {
		return NO_EFFECT;
	}
}

// Recupere la voie du special effect 2
const unsigned char CYm6::getSE2Voice(const int i_vbl)
{
	if (getSE2Type(i_vbl) != NO_EFFECT)
	{
		return (((m_RegPSG[3][i_vbl] & (16 + 32)) >> 4) + 7);
	} else
	{
		return 0;
	}
}

// Recupere la valeur de parametre du special effect 2
const unsigned char CYm6::getSE2Parameter(const int i_vbl)
{
	if (getSE2Type(i_vbl) == SID)
	{
		return (m_RegPSG[getSE2Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8));
	}
	if (getSE2Type(i_vbl) == DIGIT_DRUMS)
	{
		return (m_RegPSG[getSE2Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8 + 16));
	}
	if (getSE2Type(i_vbl) == SYNC_BUZZER)
	{
		return (m_RegPSG[getSE2Voice(i_vbl)][i_vbl] & (1 + 2 + 4 + 8));
	}
	return 0;
}

// Recupere la frequence du special effect 2
const unsigned int CYm6::getSE2Freq(const int i_vbl)
{
	if (getSE2Type(i_vbl) != NO_EFFECT)
	{
		unsigned char tp = (m_RegPSG[8][i_vbl] & (32 + 64 + 128)) >> 5;
		unsigned char tc = m_VirtualRegPSG[1][i_vbl];
		return getTimerFrequence(tp,tc);
	}
	return 0;
}

// Decodage des extras bits YM6
void CYm6::decodeYM6()
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
		else
		{
			m_SE1Parameter[i] = (unsigned char) 0;
			m_SE1Voice[i] = (unsigned char) 0;
			m_SE1Freq[i] = (unsigned int) 0;
		}
		
		m_SE2Type[i] = getSE2Type(i);
		if (m_SE2Type[i] != NO_EFFECT)
		{
			m_SE2Parameter[i] = getSE2Parameter(i);
			m_SE2Voice[i] = getSE2Voice(i);
			m_SE2Freq[i] = getSE2Freq(i);
		} 
		else
		{
			m_SE2Parameter[i] = (unsigned char) 0;
			m_SE2Voice[i] = (unsigned char) 0;
			m_SE2Freq[i] = (unsigned int) 0;
		}
    }
}

std::string CYm6::getName() const {return m_Name;}
std::string CYm6::getAuthor() const {return m_Author;}
std::string CYm6::getComments() const {return m_Comments;}

const bool CYm6::containsDigitDrums() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if ((m_SE1Type[i] == DIGIT_DRUMS) || (m_SE2Type[i] == DIGIT_DRUMS))
		{
			return true;
		}
		i++;
	}
	return false;
}
const bool CYm6::containsSID() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if ((m_SE1Type[i] == SID) || (m_SE2Type[i] == SID))
		{
			return true;
		}
		i++;
	}
	return false;
}
const bool CYm6::containsSinusSID() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if ((m_SE1Type[i] == SINUS_SID) || (m_SE2Type[i] == SINUS_SID))
		{
			return true;
		}
		i++;
	}
	return false;
}
const bool CYm6::containsSyncBuzzer() const
{
	unsigned int i=0;
	while (i<m_ValidVBL)
	{
		if ((m_SE1Type[i] == SYNC_BUZZER) || (m_SE2Type[i] == SYNC_BUZZER))
		{
			return true;
		}
		i++;
	}
	return false;
}

const bool CYm6::isInterlaced() const {return ((m_SngAttrib & 1) != 0);}
const bool CYm6::isSignedDigitDrums() const {return ((m_SngAttrib & 2) != 0);}
const bool CYm6::is4BitsDigitDrums() const {return ((m_SngAttrib & 4) != 0);}

const unsigned int CYm6::getYmFrequency() const {return (unsigned int) m_ChipFreq;}
const unsigned int CYm6::getPlayFrequency() const {return (unsigned int) m_PlayFreq;}

const unsigned int CYm6::getNbDigitDrums() const {return m_NbDD;}
const unsigned int CYm6::getDigitDrumsSize(unsigned int i_num) const {return m_DDSize[i_num];}
const unsigned char* CYm6::getDigitDrums(unsigned int i_num) const {return m_DigitDrums[i_num];}
	
const unsigned char* CYm6::getSpecialEffect1Type() const {return m_SE1Type;}
const unsigned char* CYm6::getSpecialEffect1Voice() const {return m_SE1Voice;}
const unsigned char* CYm6::getSpecialEffect1Parameter() const {return m_SE1Parameter;}
const unsigned int* CYm6::getSpecialEffect1Frequency() const {return m_SE1Freq;}
	
const unsigned char* CYm6::getSpecialEffect2Type() const {return m_SE2Type;}
const unsigned char* CYm6::getSpecialEffect2Voice() const {return m_SE2Voice;}
const unsigned char* CYm6::getSpecialEffect2Parameter() const {return m_SE2Parameter;}
const unsigned int* CYm6::getSpecialEffect2Frequency() const {return m_SE2Freq;}