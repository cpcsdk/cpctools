/**************************************************************************/
/* header.c : fonctions propres à la construction de l'entête             */
/**************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "messages.h"
#include "header.h"
#include "debug.h"
#include "aff.h"

/**************************************************************************/
/*  atoNb : petite fonction utilitaire qui convertit une chaine           */
/*  representant soit un hexadécimal (précédé d'un &) soit un décimal     */
/**************************************************************************/

unsigned int atoNb(char* val)
{
  unsigned int nb;
  if ((val[0]=='&') || (val[0]=='#') || (val[0]=='x'))
    sscanf(val+1,"%x",&nb);
  else
    nb=(unsigned int)atoi(val);
  DEBUG("atoNb chaine : %s.\n",val);
  DEBUG("atoNb conversion : %d.\n",nb);
  return nb;
}


/**************************************************************************/
/* getNomFichAms : permet d'obtenir à partir du chemin d'accès du fichier */
/* le nom sur 11 caractère du fichier amsdos                              */
/**************************************************************************/

void getNomFichAms(char *nomFichAms,char *pathFichAms)
{
  int a=10;
  char *fin=pathFichAms+strlen(pathFichAms)-1;
  
  while(a) nomFichAms[a--]=' '; /* nettoyage par l'espace */
  a=0;

  /* extraction du nom du fichier sans le chemin d'accès */

  while (fin>=pathFichAms  && *fin!='/' &&*fin!='\\') fin--;
  pathFichAms=fin+1;
  DEBUG("Nom du fichier extrait %s\n",pathFichAms);

  /* copie et vérification du nom du ficher amdos */
  if (strlen(pathFichAms)>12)
    {printf(ERR10);exit(1);}
  
  while(*pathFichAms && *pathFichAms!='.' && a<8) 
    *(nomFichAms+a++)=*pathFichAms++;
  
  if(a>=8 && *pathFichAms=='.') {printf(ERR11);exit(1);}

  /* copie de l'extension si elle existe*/
  if (*pathFichAms=='.')
    memcpy(nomFichAms+8,++pathFichAms,3);
  DEBUG("Extension du fichier extrait %s\n",pathFichAms);
  if (strlen(pathFichAms)>3) {printf(ERR12);exit(1);}
}


/**********************************************/
/* affDump : affiche la copie écran de        */
/* l'en-tête du fichier Amsdos                */
/**********************************************/

void affDump(unsigned char* buff)
{
  int i=0,t=0;
  unsigned short taille,adrLoad,adrExe,somme,somme2,somme3=0;
  
  taille=0;
  adrLoad=buff[ADRLOAD+1];
  adrLoad=buff[ADRLOAD]+adrLoad<<8;
  adrExe=buff[ADREXE+1];
  adrExe=buff[ADREXE]+adrLoad<<8;
  somme=buff[SOMME+1]*256 + buff[SOMME];
  
  CLS();
  LOCATE(1,1);
    while(t<HEADSIZE)
      {
	SETCOL(32);
	printf("%02x|",t);
	for(i=0;i<8;i++)
	  AFFHDMP((unsigned char)buff[i+t]);
	printf("%c[%d;%d;%dm | ",27,1,33,44);
	for(i=0;i<8;i++)
	  AFFCDMP (buff[i+t]);
	t+=8;
	RESTDEFAULT();
	putchar('\n');
      }
    LOCATE(1,1);
    printf(MESS1,(unsigned char) buff[USER]);
    printf(MESS2,buff+NOM); /* c'est bien une chaine */
    printf(MESS3,(unsigned char)buff[TYPE]);
    printf(MESS4,buff[TAILLE+1] *256 + buff[TAILLE]);
    printf(MESS5,buff[ADRLOAD+1]*256 + buff[ADRLOAD]);
    printf(MESS6,buff[ADREXE+1] *256 + buff[ADREXE]);
    printf(MESS7,buff[SOMME+1]  *256 + buff[SOMME]);
    LOCATE(21,1);
    
}


/**********************************************/
/* dos2Ams : ajoute un en-tête au fichier Dos */
/* et écrit le résultat dans le fichier Ams   */
/**********************************************/

void dos2Ams(char* pathFichDos,char* pathFichAms,char* nomFichAms,
	     char user, char type, unsigned short adrExe,unsigned short adrLoad,char verbose)
{
  unsigned char *buff;
  unsigned short somme=0,i;
  unsigned long taille;
  FILE *fichDos=fopen(pathFichDos,"rb"),*fichAms;
  

  /* Le fichier source existe-t-il ? */

  if (fichDos==NULL) {printf (ERR13,pathFichDos);exit(1);}

  /* Test de la taille du fichier */

  fseek(fichDos, 0, SEEK_END);
  taille=ftell(fichDos);
  if (taille>65535) {printf(ERR14);exit(1);}
  DEBUG("Taille du fichier DOS : %do.\n", taille);
  
  fseek(fichDos, 0, SEEK_SET);    /* Pour lire, rembobinons. */

  
  /* copie du contenu du fichier en mémoire (64K max, bah...) */
  /* avec réservation de 128 octets pour l'en-tête */

  buff=calloc(taille+HEADSIZE,1);
  fread(buff+HEADSIZE,1,taille,fichDos);
  DEBUG("Elements lus : %d.\n",taille);

  /* remplissage de l'en-tête avec les informations données */
  
  /* user */
  buff[USER]= user;

  /* nom de fichier */
  memcpy(buff+NOM,nomFichAms,11);
  
  /* type de fichier */
  buff[TYPE]=type;

  /* taille  (en deux endroits différents) */
  buff[TAILLE]=taille&255;
  buff[TAILLE+1]=taille>>8;
  buff[TAILLE2]=taille&255;
  buff[TAILLE2+1]=taille>>8;
  
  /* adresse de chargement */
  buff[ADRLOAD]=(char)(adrLoad&255);
  buff[ADRLOAD+1]=(char)(adrLoad>>8);

  /* adresse d'exécution */
  buff[ADREXE]=(char)(adrExe&255);
  buff[ADREXE+1]=(char)(adrExe>>8);
  if (verbose) printf(MESS6,adrExe);

  /* somme de vérification */
  for(i=0;i<SOMME;i++) somme+=buff[i];
  DEBUG("Somme : %x\n",somme);
  buff[SOMME]=(char)(somme&255);
  buff[SOMME+1]=(char)(somme>>8);

  /* si le mode verbeux est mis */
  /* invoquer affDump est de mise */

  if(verbose) affDump(buff);
  
  /* sauvegarde (ouf !) */
  fichAms=fopen(pathFichAms,"wb");
  fwrite(buff,1,taille + HEADSIZE,fichAms);
  DEBUG("Elements ecrits : %d.\n",taille + HEADSIZE);
}


/**************************************************************************/
/* info : obtenir les propriétés d'un fichier                             */
/* est-il un fichier amsdos ?                                             */
/**************************************************************************/

void info(char* path)
{
  unsigned short sommeLu,sommeDmp=0,i;
  FILE* fichAms=fopen(path,"r");
  unsigned char buff[HEADSIZE];
  fread(buff,1,HEADSIZE,fichAms);  
 
  /* calcul de la somme de vérification */
  /* et comparaison avec la somme lue dans l'en-tête */
  for(i=0;i<SOMME;i++) sommeDmp+=buff[i];
  sommeLu=buff[SOMME] + buff[SOMME+1]*256;
  DEBUG("Somme Lu: %x.\n",sommeLu);
  DEBUG("Somme Dmp : %x.\n",sommeDmp);
  
  /* affichage de l'en-tête (dump) si le fichier est valide */
  if(sommeLu!=sommeDmp)
    {printf(MESS8,path,sommeLu,sommeDmp);return;}
  affDump(buff);
}



/***************************/
/* Petit programme de test */
/***************************/

#ifdef TESTHC

int main()
{
  char i[11],a;
  getNomFichAms(i,"c:\\hahahaha\\hohohoo\\uhu.txt");
  for(a=0;a<11;a++) putchar(i[a]);
  putchar('\n');  putchar('\n');
  getNomFichAms(i,"/mnt/zut/hopla/rahihi.tar");
  for(a=0;a<11;a++) putchar(i[a]);
  putchar('\n');
  getNomFichAms(i,"/mnt/zut/hopla/rahihih");
  for(a=0;a<11;a++) putchar(i[a]);
  putchar('\n');
  getNomFichAms(i,"/mnt/zut/hopla/r.scr");
  for(a=0;a<11;a++) putchar(i[a]);
  putchar('\n');
}
#endif
