/**************************************************************************/
/* Hideur Maikeur 2.0, plus beau, plus confortable, plus cpc              */
/* (c) Downwater, le Gardien des Basses Eaux                              */
/* downwater@free.fr                                                      */
/* http://downwater.free.fr                                               */
/**************************************************************************/

/**************************************************************************/
/* Syntaxe d'appel                                                        */
/* Conversion                                                             */
/* hideur fichierDos -o fichierAms [-u user]                              */
/*  [-t type] [-x adrexe] [-l adrload] [-v]                               */
/* Info                                                                   */
/* hideur -info fichierDos                                                */
/* hideur -h                                                              */
/**************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include<stdlib.h>
#include "header.h"
#include "messages.h"
#include "debug.h"

int main(int argc,char* argv[])
{
  switch (argc)
    {
    case 1:
      printf(USAGE);break;
    case 2 :
      if (!strcmp("-h",argv[1])) {printf(USAGE HELP);return 0;}
    case 3 :
      if (!strcmp("-info",argv[1])) {info(argv[2]);return 0;}
    default :
      {
	char *pathFichDos,*pathFichAms,nomFichAms[11],verbose=0;
	unsigned char user=0,type=0; /* valeurs par défaut */
	unsigned short adrExe=0,adrLoad=0; /* valeurs par défaut */
	char setNomFichAms=0,setUser=0,setAdrExe=0,setAdrLoad=0,setType=0;
	int i=1;
	  
	/* récupération et test des paramètres */
	/* navré, mais le nom du fichier DOS doit être donné en premier */
	pathFichDos=argv[i++];
	DEBUG("Fichier Dos : %s\n",argv[i-1]);
	while (i<argc)
	  {
	    if (!strcmp(argv[i],"-o"))
	      {
		if (setNomFichAms) {printf(ERR1);exit(1);}
		i++;
		if (i>=argc) {printf(ERR2);exit(1);}
		DEBUG("Fichier Amsdos : %s\n",argv[i]);
		pathFichAms=argv[i];
		getNomFichAms(nomFichAms,argv[i++]);
		setNomFichAms=1;continue;
	      
	      }
	    if (!strcmp(argv[i],"-u"))
	      {
		if (setUser) {printf(ERR3);exit(1);}
		i++;
		if (i>=argc) {printf(ERR4);exit(1);}
		user=atoNb(argv[i++]);
		if (user>255)
		  {printf(ERR9);exit(1);}
		setUser=1;continue;
	      }

	     if (!strcmp(argv[i],"-t"))
	      {
		if (setType) {printf(ERR15);exit(1);}
		i++;
		if (i>=argc) {printf(ERR16);exit(1);}
		type=atoNb(argv[i++]);
		if (type>3)
		  {printf(ERR17);exit(1);}
		setType=1;continue;
	      }

	    if (!strcmp(argv[i],"-x"))
	      {
		if (setAdrExe)  {printf(ERR5);exit(1);}
		i++;
		if (i>=argc) {printf(ERR6);exit(1);}
		adrExe=atoNb(argv[i++]);
		setAdrExe=1;continue;
	      }
	    if (!strcmp(argv[i],"-l"))
	      {
		if (setAdrLoad)  {printf(ERR7);exit(1);}
		i++;
		if (i>=argc) {printf(ERR8);exit(1);}
		adrLoad=atoNb(argv[i++]);
		setAdrLoad=1;continue;
	      }
	    if (!strcmp(argv[i],"-v"))
	      {verbose=1;i++;continue;}
	    
	    printf(ERR18,argv[i]);return 1;
	  }
      
	/* les noms du fichier Amsdos doit être donné */
	if (!setNomFichAms) {printf(ERR2);exit(1);} 
	
	/* traitement de certaines variables et conversion du fichier */
	dos2Ams(pathFichDos,pathFichAms,nomFichAms,user,type,adrExe,adrLoad,
		verbose);
      }
    }
  return 0;
}
