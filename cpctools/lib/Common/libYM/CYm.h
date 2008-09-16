/**
* @class CYm
* Classe permettant la gestion d'un YM
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#ifndef _CYM_HH_
#define _CYM_HH_

// Taille de la zone ID
#define T_ID 4
// Taille de la zone CHECK
#define T_CHECK 8
// Taille de la zone CHECKEND
#define T_CHECKEND 4
// Nombre maximum de registres
#define NB_REG_PSG 14
// Nombre maximum de DigitDrums
#define NB_MAX_DD 32

// Taille (en octet) d'un DWORD
#define T_DWORD 4
// Taille (en octet) d'un WORD
#define T_WORD 2

// Definition des différents effets possibles
#define NO_EFFECT 0
#define SID 1
#define DIGIT_DRUMS 2
#define SINUS_SID 3
#define SYNC_BUZZER 4
#define SID_RESTARTED 5

// Definition de la fréquence du Timer ST
#define TIMER_FREQUENCE 2457600

// Definition des frequences des differents ordinateurs
#define ST_YM_FREQUENCY 2000000
#define AMSTRAD_YM_FREQUENCY 1000000

// Definition de la frequence de replay 'VBL'
#define VBL_PLAY_FREQUENCY 50

#include <string>

class CYm  
{
private:
protected:
	// YM ID ( = 'YM2!' | 'YM3!' | 'YM3b' | 'YM4!' | 'YM5!' | 'YM6!')
	char m_ID[T_ID + 1];
	// YM Registers
	unsigned char *m_RegPSG[NB_REG_PSG];
	// Nb of valid VBL
	unsigned long  m_ValidVBL;
	// Loop VBL
	unsigned long m_Loop;
	
	// Methods
	CYm();
	
	void allocateYmRegisters();

	const static unsigned long getLong(const char *io_long);
	const static unsigned short getShort(const char *io_short);
	const static unsigned int getTimerFrequence(const unsigned char i_TP,const unsigned char i_TC);
public:
	virtual ~CYm();

	void afficheYM(std::ostream &io_stream) const;

	const char* getID() const;
	const unsigned int nbValidVBL() const;
	const unsigned int loopVBL() const;
	const unsigned char * getRegister(const unsigned int i_numReg) const;
	
	virtual std::string getName() const=0;
	virtual std::string getAuthor() const=0;
	virtual std::string getComments() const=0;

	virtual const bool containsSID() const=0;
	virtual const bool containsDigitDrums() const=0;
	virtual const bool containsSinusSID() const=0;
	virtual const bool containsSyncBuzzer() const=0;
	virtual const bool is4BitsDigitDrums() const=0;
	virtual const bool isSignedDigitDrums() const=0;
	virtual const bool isInterlaced() const=0;
	virtual const unsigned int getYmFrequency() const=0;
	virtual const unsigned int getPlayFrequency() const=0;
	
	virtual const unsigned int getNbDigitDrums() const=0;
	virtual const unsigned int getDigitDrumsSize(unsigned int i_num) const=0;
	virtual const unsigned char* getDigitDrums(unsigned int i_num) const=0;
	
	virtual const unsigned char* getSpecialEffect1Type() const=0;
	virtual const unsigned char* getSpecialEffect1Voice() const=0;
	virtual const unsigned char* getSpecialEffect1Parameter() const=0;
	virtual const unsigned int* getSpecialEffect1Frequency() const=0;
	
	virtual const unsigned char* getSpecialEffect2Type() const =0;
	virtual const unsigned char* getSpecialEffect2Voice() const =0;
	virtual const unsigned char* getSpecialEffect2Parameter() const =0;
	virtual const unsigned int* getSpecialEffect2Frequency() const =0;
};

#endif
