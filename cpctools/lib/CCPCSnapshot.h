/**
* @class CCPCSnapshot
* Classe permettant la gestion d'un snapshot CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#pragma once
#define _CCPCSNAPSHOT_HH_

#include <iostream>
#include <fstream>
#include "CError.h"
#include <vector>

#if ! _WINDOWS
#include <errno.h>
#endif

using namespace std;

#define NB_SNAPSHOT_TOKEN 67
#define NB_SNAPSHOT_TOKEN_SECTION 10

class CCPCSnapshot
{
private:
	struct TTokenID
	{
		unsigned int Offset;
		unsigned int Size;
		const char *ID;
		const char *Description;
	};
	static const struct TTokenID TokenID[NB_SNAPSHOT_TOKEN];
	static const char *TokenSection[NB_SNAPSHOT_TOKEN_SECTION];
	static unsigned char DefaultHeader[0x100];

	/// Header du snapshot
	unsigned char _header[0x100];
	/// Dump de la memoire
	unsigned char *_memoryDump;
	
	/// Nombre de donnees additionnelles
	unsigned int _nbChunk;
	/// Donnees additionnels (noms)
	unsigned char **_chunkName;
	/// Donnees additionnels (size)
	unsigned int *_chunkSize;
	/// Donnees additionnels (data)
	unsigned char **_chunkData;

protected:
	// Return token struct for token, with appropriate index in data array if any
	static TTokenID GetToken(const char *i_dataID, int &o_index);
	// Get token struct from token name
	static int GetTokenIndex(const string &tokenName);
	// Renvoie l'offset du token
	static int GetTokenOffset(const char *i_dataID);
	// Renvoie la taille du token
	static int GetTokenSize(const char *i_dataID);

public:
	/// Constructeur par defaut
	CCPCSnapshot();
	/// Destructeur
	~CCPCSnapshot();
	
	static std::ostream& displayTokenHelp(std::ostream &io_os);

	/// Load token list using an .ini file
	void loadSnapshotIni(const string &filename);
	//! Save token list in an .ini file
	void saveSnapshotIni(const string &filename) const;

	/// Renvoie la version du snapshot
	unsigned int getSnapshotVersion() const;
	
	// Renvoie la byte valeur data
	unsigned char tokenByteValue(const char *i_dataID) const;
	// Renvoie la short valeur data
	unsigned short tokenShortValue(const char *i_dataID) const;

	// Met a jour la valeur
	void setTokenValue(const char *i_dataID,int);

	/// Efface la memoire du snapshot
	void clearMemory();
	/// Deconnecte toutes les ROMS
	void romDeconnect();
	/// EI
	void enableInterrupt();
	// DI
	void disableInterrupt();
	
	/// Put data
	void putData(unsigned int adr,unsigned char val);
	/// Fill memory with datas
	void fillData(unsigned int adr, unsigned int size, const vector<unsigned char> &datas);

	/// Charge un snapshot depuis un fichier
	void loadFromFile(const std::string &i_filename);
	/// Sauve un snapshot dans un fichier
	void saveToFile(const std::string &i_filename) const;
	
	/// Ajoute des donnees dans le snapshot a partir d'un fichier, et d'une adresse
	void loadDataFromFile(const std::string &i_filename,const int i_adress);
	/// Sauve des donnees dans le fichier a partir du snapshot d'une adresse et d'une taille
	void saveDataToFile(const std::string &i_filename,const int i_adress, const int i_length) const;
	
	/// Affiche les infos sur le snapshot
	std::ostream& printSnapshotInfo(std::ostream &io_os) const;
	/// Affiche les infos sur le Z80
	std::ostream& printZ80Info(std::ostream &io_os) const;
	/// Affiche les infos sur le GateArray
	std::ostream& printGateArrayInfo(std::ostream &io_os) const;
	/// Affiche les infos sur le CRTC
	std::ostream& printCRTCInfo(std::ostream &io_os) const;
	/// Affiche les infos sur le PPI
	std::ostream& printPPIInfo(std::ostream &io_os) const;
	/// Affiche les infos sur le PSG
	std::ostream& printPSGInfo(std::ostream &io_os) const;
	/// Affiche les infos sur la RAM/ROM
	std::ostream& printRAMROMInfo(std::ostream &io_os) const;
	/// Affiche les infos Chunk
	std::ostream& printChunkInfo(std::ostream &io_os) const;
	
	/// Affiche les infos sur le snapshot
	friend std::ostream& operator<<(std::ostream &io_os,const CCPCSnapshot &i_snap);
	
};
