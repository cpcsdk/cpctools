/**
* @class CYmFactory
* Classe permettant de charge les differents types de YM
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CYm.h"
#include "CYm2.h"
#include "CYm3.h"
#include "CYm3b.h"
#include "CYm4.h"
#include "CYm5.h"
#include "CYm6.h"
#include "CYmFactory.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "CError.h"

CYmFactory::CYmFactory()
{
}

CYmFactory::~CYmFactory()
{
}

// Chargement general d'un fichier YM
CYm* CYmFactory::createYm(const std::string i_YmFileName)
{
	char *ymData;
	int tYmData=0,size=0;
	char ID[T_ID+1];
	
	std::ifstream ymfile(i_YmFileName.c_str(),std::ios::binary);
	TOOLS_ASSERTMSG(ymfile.good(), "Unable to open " << i_YmFileName);

	ymfile.seekg(0,std::ios::end);
	tYmData = ymfile.tellg();
	ymfile.seekg(0,std::ios::beg);

	ymData = new char[tYmData];
	ymfile.read(ymData,tYmData);

	memcpy(ID,ymData,T_ID);
	ID[T_ID]='\0';
	
	if (strcmp(ID,"YM2!")==0)
	{
		CYm *loadedYM = new CYm2(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}  
	if (strcmp(ID,"YM3!")==0) 
	{
		CYm *loadedYM = new CYm3(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}
	if (strcmp(ID,"YM3b")==0) 
	{
		CYm *loadedYM = new CYm3b(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}
	if (strcmp(ID,"YM4!")==0) 
	{
		CYm *loadedYM = new CYm4(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}
	if (strcmp(ID,"YM5!")==0) 
	{
		CYm *loadedYM = new CYm5(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}
	if (strcmp(ID,"YM6!")==0) 
	{
		CYm *loadedYM = new CYm6(ymData,tYmData);
		free(ymData);
		return loadedYM;
	}
	TOOLS_ERRORMSG("Unknown YM file format !");
}
