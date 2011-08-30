/**************************************************************************/
/* messages.h : définitions des erreurs, des messages en fonction de la   */
/* langue et de la platteforme.                                           */
/**************************************************************************/

#ifdef FRENCH

#ifdef DOS

#define USAGE "Hideur Maikeur 2.0\n"\
"Conversion : hideur fichierDos -o fichierAms [-u user] [-t type]\n"\
"                               [-x adrexe] [-l adrload] [-v]\n"\
"Info       : hideur -info fichier\n"\
"Aide       : hideur -h\n\nEnvoyez-moi les bugs relev‚s "\
"… downwater@free.fr\nSite : http://downwater.free.fr - http://"\
"www.cpcscene.com/downwater\n"
#define HELP  "\nSyntaxe :\n\n"\
"* fichierDos    : le fichier … convertir\n"\
"* -o fichierAms : le fichier amsdos … obtenir\n"\
"* -u user       : le r‚pertoire amsdos du fichier (0-255)\n"\
"* -t type       : le type de fichier 0 : Basic\n"\
"                                  1 : Basic Prot‚g‚\n"\
"                                  2 : Binaire\n"\
"                                  3 : Binaire prot‚g‚\n"\
"* -x adrexe     : adresse d'ex‚cution\n"\
"* -l adrload    : adresse de chargement\n"\
"* -v            : mode verbeux, affiche le r‚sultat\n"

#define ERR1  "ERREUR : un seul fichier amsdos doit ˆtre donné.\n"
#define ERR2  "ERREUR : nom du fichier amsdos manquant.\n"
#define ERR3  "ERREUR : un seul num‚ro de répertoire (user) doit ˆtre donn‚.\n"
#define ERR4  "ERREUR : numéro de r‚pertoire (user) manquant après un '-u'.\n"
#define ERR5  "ERREUR : une seule adresse d'ex‚cution doit ˆtre donn‚e.\n"
#define ERR6  "ERREUR : adresse d'ex‚cution manquante aprŠs un '-x'.\n"
#define ERR7  "ERREUR : une seule adresse de chargement doit ˆtre donn‚e.\n"
#define ERR8  "ERREUR : adresse de chargement manquante aprŠs un '-l'.\n"
#define ERR9  "ERREUR : un nombre entre 0 et 255 doit ˆtre donn‚ pour le r‚pertoire (user).\n" 
#define ERR10 "ERREUR : nom du fichier amsdos trop long.\n"
#define ERR11 "ERREUR : radical du nom du fichier amsdos sup‚rieur … huit caractŠres.\n"
#define ERR12 "ERREUR : extension du nom du fichier amsdos sup‚rieur … trois caractŠres.\n"
#define ERR13 "ERREUR : %s n'existe pas.\n"
#define ERR14 "ERREUR : fichier de taille supérieure … 65535 octets.\n"
#define ERR15 "ERREUR : un seul type de fichier doit ˆtre donn‚.\n"
#define ERR16 "ERREUR : type manquant.\n"
#define ERR17 "ERREUR : un nombre entre 0 et 255 doit ˆtre donn‚ pour le type de fichier.\n"
#define ERR18 "ERREUR : paramŠtre inconnu %s.\n"

#define MESS1 "R‚pertoire (user) : %d.\n"
#define MESS2 "Nom : %s.\n"
#define MESS3 "Type : %d.\n"
#define MESS4 "Taille : &%xo.\n"
#define MESS5 "Adresse de chargement : &%x.\n"
#define MESS6 "Adresse d'ex‚cution : &%x.\n"
#define MESS7 "Somme de v‚rification : &%x.\n"
#define MESS8 "%s : n'est pas un en-tˆte valide.\n"\
"La somme lu est &%x mais la somme r‚elle vaut &%x.\n"

#else

#define USAGE "Hideur Maikeur 2.0\n"\
"Conversion : hideur fichierDos -o fichierAms [-u user] [-t type]\n"\
"                               [-x adrexe] [-l adrload] [-v]\n"\
"Info :       hideur -info fichier\n"\
"Aide :       hideur -h\n\nEnvoyez-moi les bugs relevés à downwater@free.fr\n"\
"Site : http://downwater.free.fr - http://www.cpcscene.com/downwater\n"
#define HELP  "\nSyntaxe :\n\n"\
"* fichierDos    : le fichier à convertir\n"\
"* -o fichierAms : le fichier amsdos à obtenir\n"\
"* -u user       : le répertoire amsdos du fichier (0-255)\n"\
"* -t type       : le type de fichier 0 : Basic\n"\
"                                  1 : Basic Protégé\n"\
"                                  2 : Binaire\n"\
"                                  3 : Binaire protégé\n"\
"* -x adrexe     : adresse d'exécution\n"\
"* -l adrload    : adresse de chargement\n"\
"* -v            : mode verbeux, affiche le résultat\n"
#define ERR1  "ERREUR : un seul fichier amsdos doit être donné.\n"
#define ERR2  "ERREUR : nom du fichier amsdos manquant.\n"
#define ERR3  "ERREUR : un seul numéro de répertoire (user) doit être donné.\n"
#define ERR4  "ERREUR : numéro de répertoire (user) manquant après un '-u'.\n"
#define ERR5  "ERREUR : une seule adresse d'exécution doit être donnée.\n"
#define ERR6  "ERREUR : adresse d'exécution manquante après un '-x'.\n"
#define ERR7  "ERREUR : une seule adresse de chargement doit étre donnée.\n"
#define ERR8  "ERREUR : adresse de chargement manquante après un '-l'.\n"
#define ERR9  "ERREUR : un nombre entre 0 et 255 doit être donné pour le répertoire (user).\n" 
#define ERR10 "ERREUR : nom du fichier amsdos trop long.\n"
#define ERR11 "ERREUR : radical du nom du fichier amsdos supérieur à huit caractères.\n"
#define ERR12 "ERREUR : extension du nom du fichier amsdos supérieur à trois caractères.\n"
#define ERR13 "ERREUR : %s n'existe pas.\n"
#define ERR14 "ERREUR : fichier de taille supérieure à 65535 octets.\n"
#define ERR15 "ERREUR : un seul type de fichier doit être donné.\n"
#define ERR16 "ERREUR : type manquant.\n"
#define ERR17 "ERREUR : un nombre entre 0 et 255 doit être donné pour le type de fichier.\n"
#define ERR18 "ERREUR : paramètre inconnu %s.\n"

#define MESS1 "Répertoire (user) : %d.\n"
#define MESS2 "Nom : %s.\n"
#define MESS3 "Type : %d.\n"
#define MESS4 "Taille : &%xo.\n"
#define MESS5 "Adresse de chargement : &%x.\n"
#define MESS6 "Adresse d'exécution : &%x.\n"
#define MESS7 "Somme de vérification : &%x.\n"
#define MESS8 "%s : n'est pas un en-tête valide.\n"\
"La somme lu est &%x mais la somme réelle vaut &%x.\n"

#endif

#else 

#define USAGE "Hideur Maikeur 2.0\n"\
"Conversion : hideur DosFile -o AmsFile [-u user] [-t type] \n"\
"                    [-x adrexe] [-l adrload] [-v]\n"\
"Info       :       hideur -info fichierDos\n"\
"Help       :       hideur -h\n\n"\
"Send bugs reports at downwater@free.fr\n"\
"Website : http://downwater.free.fr - http://www.cpcscene.com/downwater\n"
#define HELP  "\nSyntaxe :\n\n"\
"* DosFile    : file to convert\n"\
"* -o AmsFile : output file\n"\
"* -u user    : user of the amsdos file(0-255)\n"\
"* -t type    : file type 0 : Basic\n"\
"                         1 : Protected Basic\n"\
"                         2 : Binary\n"\
"                         3 : Protected Binary\n"\
"* -x adrexe  : address of execution\n"\
"* -l adrload : address of loading\n"\
"* -v         : verbose mode, affiche le r‚sultat\n"
#define ERR1  "ERROR : only one amsdos output file is needed. Please.\n"
#define ERR2  "ERROR : amsdos filename missing.\n"
#define ERR3  "ERROR : only one user number is needed. Please.\n"
#define ERR4  "ERROR : user number missing after '-u'.\n"
#define ERR5  "ERROR : only one address of execution is needed. Please.\n"
#define ERR6  "ERROR : adress of execution missing after '-x'.\n"
#define ERR7  "ERROR : only one address of loading is needed. Please.\n"
#define ERR8  "ERROR : adress of loading missing after '-l'.\n"
#define ERR9  "ERROR : a number between 0 and 255 must be given for the user id.\n"
#define ERR10 "ERROR : amsdos filename too long.\n"
#define ERR11 "ERROR : first part of amsdos filename longer than eight characteres.\n"
#define ERR12 "ERROR : extension of amsdos filename longer than three characteres.\n"
#define ERR13 "ERROR : %s doesn't exist.\n"
#define ERR14 "ERROR : file size greater than 65535 bytes.\n"
#define ERR15 "ERROR : only one type is needed. Please.\n"
#define ERR16 "ERROR : Type missing.\n"
#define ERR17 "ERROR : a number between 0 and 3 must be given for the type id.\n"
#define ERR18 "ERREUR : unknown parameter %s.\n"

#define MESS1 "User : %d.\n"
#define MESS2 "Name : %s.\n"
#define MESS3 "Type : %d.\n"
#define MESS4 "Size : &%xb.\n"
#define MESS5 "Address of loading : &%x.\n"
#define MESS6 "Address of execution : &%x.\n"  
#define MESS7 "Checksum : &%x.\n"
#define MESS8 "%s : invalid amsdos file.\n"\
"Read checksum is &%x but the real checksum is &%x.\n"

#endif


