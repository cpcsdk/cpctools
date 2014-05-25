/**
* @class CCPCFile
* Classe permettant la gestion d'un fichier CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/
 

#include <iostream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "dllexport.h"
#include "libdsk.h"

#ifndef _CCPCFILE_HH_
#define _CCPCFILE_HH_

enum TFileType
{
	Ascii,
	Binary
};

class DLL_PUBLIC CCPCFile
{
public:
	/// Constructeur
	CCPCFile();
	/// Destructeur
	virtual ~CCPCFile();
	
	/// Renvoie le type de fichier
	virtual TFileType getType() const = 0;
	
	/// Renvoie la taille du fichier (en octet)
	virtual unsigned int getSize() const = 0;
	
	/// Creation d'un fichier a partir d'un buffer
	virtual void createFile(const void* i_buffer, const unsigned int i_isize, bool i_createHeader = false) = 0;
	/// Lecture et creation d'un fichier a partir d'un fichier
	virtual void openFile(const std::string &i_filename,bool i_createHeader = false) = 0;
	/// Ecriture d'un fichier dans une stream
	virtual void saveFile(const std::string &i_filename,bool i_saveHeader = true) const = 0;
	
	/// Renvoie la taille des donnees contenues dans le fichier (header eventuellement compris)
	virtual unsigned int getDatasSize() const = 0;
	/// Renvoie un pointeur sur les donnees contenues dans le fichier
	virtual unsigned char* getDatas() const = 0;
	
	/// Ouverture d'un fichier (lecture du header) a partir d'un buffer (le fichier contient un header !)
	static CCPCFile* openFile(const void *i_buffer, const unsigned int i_size);
	
	/// Calcul le checksum
	static unsigned int checkSum(const void *i_buffer);
	
	/// Affichage du header du fichier
	virtual std::ostream& printHeader(std::ostream& io_os) const = 0;
	/// Affichage de contenu du fichier
	virtual std::ostream& printContent(std::ostream& io_os) const = 0;
	/// Affiche les infos sur le ficher
	virtual std::ostream& printInfo(std::ostream &io_os) const = 0;
};

#endif
