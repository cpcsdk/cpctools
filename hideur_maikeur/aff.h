/**************************************************************************/
/* aff.h : macros utilitaires pour l'affichage formattée et en couleurs   */
/* Sous dos, il faut charger le fichier système ansi.sys                  */
/* (dans le C:\config.sys, mettre a ligne DEVICEHIGH=[chemin]\ansi.sys).  */
/**************************************************************************/

#define SAUVCURSOR() printf("%c[s",27)
#define RESTCURSOR() printf("%c[u",27)
#define CLS() printf("%c[2J",27)
#define LOCATE(x,y) printf("%c[%d;%dH",27,x,y)
#define SETCOL(x) printf("%c[%dC",27,x)
#define RESTDEFAULT() printf("%c[%d;m",27,0)
#define AFFHDMP(x) printf("%c[%d;%d;%dm%02x ",27,1,33,44,x)
#define AFFCDMP(x) if (isprint(x))\
                     printf("%c[%d;%d;%dm%c ",27,1,33,44,x);\
                   else\
                     printf("%c[%d;%d;%dm  ",27,1,33,44);



