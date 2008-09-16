/**
* @class CYm6
* Classe permettant la gestion d'un YM6
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#ifndef _CYM6_HH_
#define _CYM6_HH_

// Nombre de registres virtuels supplémentaires
#define NB_VIRTUAL_REG 2

#include "CYm.h"

class CYm6 : public CYm  
{
private:
	void allocateYmRegisters();
	void decodeYM6();
	const unsigned int getSE1Freq(const int i_vbl);
	const unsigned char getSE1Parameter(const int i_vbl);
	const unsigned char getSE1Voice(const int i_vbl);
	const unsigned char getSE1Type(const int i_vbl);
	const unsigned int getSE2Freq(const int i_vbl);
	const unsigned char getSE2Parameter(const int i_vbl);
	const unsigned char getSE2Voice(const int i_vbl);
	const unsigned char getSE2Type(const int i_vbl);
protected:
	// Members Attributes
	
	// YM Check ( = 'LeOnArD!')
	char m_Check[T_CHECK + 1];
	
	// Song Attributes :
	//		b0 = 1 -> Interlaced data block
	//		b1 = 1 -> DD Signed Data
	//		b2 = 1 -> DD in ST 4 bits format
	unsigned long m_SngAttrib;
	// Chip Frequency
	unsigned long m_ChipFreq;
	// Player Frequency
	unsigned short m_PlayFreq;
	// Additionnal Data
	unsigned short m_Add;
	// Song Name
	std::string m_Name;
	// Author Name
	std::string m_Author;
	// Comments
	std::string m_Comments;
	
	// Digit Drums
	// Number of DigitDrums
	unsigned short m_NbDD;
	// DD Datas
	unsigned char *m_DigitDrums[NB_MAX_DD];
	// DD Size
	unsigned long m_DDSize[NB_MAX_DD];
	// YM Virtual Registers
	unsigned char *m_VirtualRegPSG[NB_VIRTUAL_REG];
	// Special effect 1
	// Special effect 1 type (SID, DD, Sinus-SID, Sync Buzzer)
	unsigned char *m_SE1Type;
	// Special effect 1 voice (none, A, B, C)
	unsigned char *m_SE1Voice;
	// Special effect 1 parameter (N° DD, VMax for SID)
	unsigned char *m_SE1Parameter;
	// Special effect 1 frequency
	unsigned int *m_SE1Freq;
	// Special effect 2
	// Special effect 2 type (SID, DD, Sinus-SID, Sync Buzzer)
	unsigned char *m_SE2Type;
	// Special effect 2 voice (none, A, B, C)
	unsigned char *m_SE2Voice;
	// Special effect 2 parameter (N° DD, VMax for SID)
	unsigned char *m_SE2Parameter;
	// Special effect 2 frequency
	unsigned int *m_SE2Freq;
	
	// Check End (='End!')
	char m_CheckEnd[T_CHECKEND+1];
public:
	CYm6(const char *i_ymData, const int i_tYmData);
	virtual ~CYm6();

	virtual std::string getName() const;
	virtual std::string getAuthor() const;
	virtual std::string getComments() const;

	virtual const bool containsSID() const;
	virtual const bool containsDigitDrums() const;
	virtual const bool containsSinusSID() const;
	virtual const bool containsSyncBuzzer() const;
	virtual const bool is4BitsDigitDrums() const;
	virtual const bool isSignedDigitDrums() const;
	virtual const bool isInterlaced() const;
	virtual const unsigned int getYmFrequency() const;
	virtual const unsigned int getPlayFrequency() const;
	
	virtual const unsigned int getNbDigitDrums() const;
	virtual const unsigned int getDigitDrumsSize(unsigned int i_num) const;
	virtual const unsigned char* getDigitDrums(unsigned int i_num) const;
	
	virtual const unsigned char* getSpecialEffect1Type() const;
	virtual const unsigned char* getSpecialEffect1Voice() const;
	virtual const unsigned char* getSpecialEffect1Parameter() const;
	virtual const unsigned int* getSpecialEffect1Frequency() const;
	
	virtual const unsigned char* getSpecialEffect2Type() const ;
	virtual const unsigned char* getSpecialEffect2Voice() const ;
	virtual const unsigned char* getSpecialEffect2Parameter() const ;
	virtual const unsigned int* getSpecialEffect2Frequency() const ;

};

#endif
