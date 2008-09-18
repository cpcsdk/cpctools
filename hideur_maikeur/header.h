/**************************************************************************/
/* header.h : définitions propres à la construction de l'en-tête          */
/**************************************************************************/

#define HEADSIZE 0x80

/*Structure de l'entête Amsdos*/

#define USER 0x00
#define USERL 1
#define NOM 0x1
#define NOML 11
#define TYPE 0x12
#define TYPEL 1
#define TAILLE 0x18
#define TAILLEL 2
#define TAILLE2 0x40
#define TAILLE2L 2
#define ADRLOAD 0x15
#define ADRIMPL 2
#define ADREXE 0x1a
#define ADREXEL 2
#define SOMME 0x43
#define SOMMEL 2

/* signatures */
unsigned int  atoNb(char* val);
void getNomFichAms(char *nomFichAms,char *pathFichAms);
void affDump(unsigned char *buf);
void dos2Ams(char* pathFichDos,char* pathFichAms,
	     char* nomFichAms,char user, char type, 
	     unsigned short adrExe,unsigned short adrLoad,char verbose);
void info(char* buff);



