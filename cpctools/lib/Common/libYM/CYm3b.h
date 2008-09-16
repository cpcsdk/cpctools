/**
* @class CYm3b
* Classe permettant la gestion d'un YM3b
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#ifndef _CYM3B_HH_
#define _CYM3B_HH_

#include "CYm.h"

class CYm3b : public CYm  
{
public:
	CYm3b(const char *i_ymData, const int i_tYmData);
	virtual ~CYm3b();

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
