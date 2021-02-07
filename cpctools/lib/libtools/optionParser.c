/*
 * Some cmdOption to help cmdOption line parsing
 * (c) Ramlaid 2003
 */

#include "optionParser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int errorID;
static char errorString[1024];

#define RET_ERROR( e , s ) {errorID = e;strcpy(errorString,s);return e;}

int getError()
{
	return errorID;
}
char *getErrorString()
{
	return errorString;
}

void addArgv(int *argc, char ***argv, char *newArg)
{
	int i;
	int sizeH;
	int sizeS = 0;
	char *newArgv;
	char **newArgv2;
	char *ptr;

	for (i=0;i<(*argc);i++)
		sizeS += strlen((*argv)[i])+1;
	sizeH = ((*argc)+2)*sizeof(char*);

	newArgv = (char*)malloc(sizeH+sizeS+strlen(newArg)+1);

	if (*argv != NULL && (*argc != 0))
		memcpy(newArgv+sizeH, (*argv)[0], sizeS);
	memcpy(newArgv+sizeH+sizeS,newArg,strlen(newArg)+1);
	ptr = newArgv+sizeH;
	newArgv2 = (char**)newArgv;
	for (i=0;i<((*argc)+1);i++)
	{
		newArgv2[i]=ptr;
		if (i != (*argc))
			ptr += strlen((*argv)[i])+1;
	}
	newArgv2[(*argc)+1] = NULL;

	if (*argv != NULL && (*argc != 0))
		free(*argv);

	*argv = newArgv2;
	(*argc)++;
}

void removeArgv(int *argc, char ***argv, int idx)
{
	int i,j;
	int sizeH;
	int sizeS = 0;
	char *newArgv;
	char **newArgv2;
	char *ptr;

	if ((*argc) < idx)
		return;

	for (i=0;i<(*argc);i++)
	{
		if (i != idx)
			sizeS += strlen((*argv)[i])+1;
	}
	sizeH = ((*argc))*sizeof(char*);

	newArgv = (char*)malloc(sizeH+sizeS);
	if (!newArgv)
	{
		printf( "MALLOC IMPOSSIBLE ! %s : %d \n", __FILE__, __LINE__); ;
	}

	ptr = newArgv+sizeH;
	newArgv2 = (char**)newArgv;
	j = 0;
	for (i=0;i<(*argc);i++)
	{
		if (i != idx)
		{
			memcpy(ptr,(*argv)[i],strlen((*argv)[i])+1);
			newArgv2[j]=ptr;
			ptr += strlen((*argv)[i])+1;
			j++;
		}
	}
	newArgv2[(*argc)-1] = NULL;

//	if(*argv) free(*argv);

	*argv = newArgv2;
	(*argc)--;
}

int isOptionString(char *argv)
{
	return (argv[0] == '-');
}

int compareOptionString(char *argv, char shortName, const char *longName)
{
	unsigned int i;
	if (argv == NULL || longName == NULL)
		RET_ERROR(ERROR_UNKNOWN, "Internal error");

	if (argv[0] != '-')
		RET_ERROR( ERROR_UNKOPT, "Unknown option, not beginning with '-'");
	if (argv[1] != '-')
	{
		if (strlen(argv) != 2)
			RET_ERROR( ERROR_UNKOPT, "Unknown option, '-' with more than one caracter");
		if (toupper(argv[1]) == toupper(shortName))
			RET_ERROR( OPT_NO_ERROR, "No error");
		RET_ERROR( ERROR_UNKOPT, "Unknown option");
	}
	else
	{
		if ((strlen(argv)-2) != strlen(longName))
			RET_ERROR( ERROR_UNKOPT, "Unknown option");
		i = 0;
		while ((toupper(argv[i+2]) == toupper(longName[i])) && (i < strlen(longName)))
			i++;
		if (i == strlen(longName))
			RET_ERROR( OPT_NO_ERROR, "No error");
		RET_ERROR( ERROR_UNKOPT, "Unknown option");
	}
}

int findOption(char *argv, struct SOption *option)
{
	int i=0;
	if (option == NULL)
		RET_ERROR(ERROR_UNKNOWN, "Internal error");
	while (option[i].shortName != 0)
	{
		if (compareOptionString(argv,option[i].shortName,option[i].longName) == OPT_NO_ERROR)
			return i;
		i++;
	}
	RET_ERROR(ERROR_UNKOPT, "Unknown option");
}

int optionIndex(struct SOptionOut *cmdLine, char c)
{
	int i=0;
	c = toupper(c);
	while (cmdLine[i].shortName != 0 && toupper(cmdLine[i].shortName) != c)
		i++;
	if (cmdLine[i].shortName == 0)
		RET_ERROR(ERROR_UNKNOWN, "Unknown option")
	else
		return i;
}

void addOption(struct SOptionOut **cmdOption, char **argv, struct SOption *option)
{
	int nbCmdOption=0;
	int i;
	struct SOptionOut *newCmdOption;
	

	//compte le nombre d'option possibles
	if (*cmdOption != NULL)
	{
		while ((*cmdOption)[nbCmdOption].shortName != 0)
			nbCmdOption++;
	}

	newCmdOption = (struct SOptionOut*)malloc((nbCmdOption+2)*sizeof(struct SOptionOut));
	if (!newCmdOption)
	{
		printf( "MALLOC ERROR !  %s : %d \n", __FILE__, __LINE__) ;
		printf(" nb : %d \n size : %zu\ntot : %zu \n", nbCmdOption+2, sizeof(struct SOptionOut), (nbCmdOption+2)*sizeof(struct SOptionOut));
		exit (-1);
	}

	if (nbCmdOption != 0)
	{
		memcpy(newCmdOption,*cmdOption,(nbCmdOption)*sizeof(struct SOptionOut));
	}
	
	newCmdOption[nbCmdOption].shortName = option->shortName;
	newCmdOption[nbCmdOption].argc = 0;
	if (option->nbArgs != 0)
	{
		for (i=0;i<option->nbArgs;i++)
		{
			addArgv(&newCmdOption[nbCmdOption].argc,&newCmdOption[nbCmdOption].argv,argv[i]);
		}
	}
	else
	{
		newCmdOption[nbCmdOption].argv = NULL;
	}
	newCmdOption[nbCmdOption+1].shortName=0;
	newCmdOption[nbCmdOption+1].argc=0;
	newCmdOption[nbCmdOption+1].argv=NULL;

	if (*cmdOption != NULL)
		free(*cmdOption);
	*cmdOption = newCmdOption;
}

void freeOptionOut(struct SOptionOut *cmdOptionLine)
{
	int i=0;
	char shortName;

	if (cmdOptionLine != NULL)
	{
		do
		{
			shortName = cmdOptionLine[i].shortName;
			if (cmdOptionLine[i].argc != 0)
			{
				free(cmdOptionLine[i].argv);
			}
			i++;
		}
		while (shortName != 0);
		free(cmdOptionLine);
	}
}

int parseCommandLineOption(int *argc, char*** argv, struct SOption *option, struct SOptionOut **cmdOptionLine)
{


	int o,i;
	int nbOption=0;
	int *nbValOption;

	int nbArg = (*argc)-1;
	int iArgv = 1;

	//compte nb d'options
	while(option[nbOption].shortName != 0)
		nbOption++;

	//reserve memoire pour compter le nombre de valeur des options
	nbValOption = (int*)malloc(nbOption*sizeof(int));
	for (o=0;o<nbOption;o++)
	{
		nbValOption[o]=0;
	}

	//Securisation
	if ((*cmdOptionLine) == NULL)
	{
		*cmdOptionLine = (struct SOptionOut*)malloc(sizeof(struct SOptionOut));
		(*cmdOptionLine)[0].shortName=0;
		(*cmdOptionLine)[0].argc=0;
		(*cmdOptionLine)[0].argv=NULL;
	}


	//Lecture de la liste des arguments
	while (nbArg>0)
	{
		if (isOptionString((*argv)[iArgv]) != 0)
		{
			//verification si aide
			if (compareOptionString((*argv)[iArgv],'h',"help") == OPT_NO_ERROR)
			{
				free(nbValOption);
				RET_ERROR(DISPLAY_HELP, "Display help");
			}

			//recuperation option
			o = findOption((*argv)[iArgv],option);
			if (o == ERROR_UNKNOWN)
			{
				free(nbValOption);
				RET_ERROR( ERROR_UNKOPT, "Unknown option");
			}
			if (o == ERROR_UNKOPT)
			{
				// Begin like an option, but doesn't seems to be one
				nbArg--;
				iArgv++;
			}
			else
			{
				//option connue
				
				//verification nombre d'arguments minimum necessaires pour l'option
				if (nbArg <= option[o].nbArgs)
				{
					free(nbValOption);
					RET_ERROR( ERROR_OPTARG, "Wrong number of argument");
				}

				//ajout de l'option
				addOption(cmdOptionLine,&((*argv)[iArgv+1]),&option[o]);
	
				//consideration de la presence de cette option
				nbValOption[o]++;

				//suppression des arguments lus
				for (i=0;i<option[o].nbArgs + 1;i++)
				{
					removeArgv(argc,argv,iArgv);
					nbArg--;
				}
			}
		}
		else
		{
			//lecture d'une non option
			nbArg--;
			iArgv++;
		}
	}
	for (o=0;o<nbOption;o++)
	{
		if (!(option[o].nbMin == 0 && option[o].nbMax == 0))
		{
			if (option[o].nbMax != 0)
			{
				if (!((nbValOption[o] >= option[o].nbMin) && (nbValOption[o] <= option[o].nbMax)))
				{
					free(nbValOption);
					RET_ERROR( ERROR_OPTARG, "Wrong number of option");
				}
			}
			else
			{
				if (!(nbValOption[o] >= option[o].nbMin))
				{
					free(nbValOption);
					RET_ERROR( ERROR_OPTARG, "Wrong number of option");
				}
			}
		}
	}
	free(nbValOption);
	RET_ERROR(OPT_NO_ERROR, "No error");
}

int parseConfigFileOption(char* configFile, int *argc, char*** argv, struct SOption *option, struct SOptionOut **cmdOptionLine)
{
	FILE *f=fopen(configFile,"r");
	char arg[256];
	int i;

	if (f == NULL)
		RET_ERROR(ERROR_UNKNOWN, "Unable to open file");

	while(!feof(f))
	{
		char c;
		c=fgetc(f);
		while ((c==' ' || c=='\t' || c=='\n') && !feof(f))
			c=fgetc(f);
		if (feof(f))
			continue;
		i=0;
		if (c == ';')
		{
			char comment[256];
			fgets(comment,256,f);
		}
		else
		{
			while ((!(c==' ' || c=='\t' || c=='\n')) && !feof(f))
			{
				arg[i++] = c;
				c=fgetc(f);
			}
			arg[i]=0;
			addArgv(argc,argv,arg);
		}
	}
	fclose(f);

	return parseCommandLineOption(argc,argv,option,cmdOptionLine);
}

void printUsage(char * appliName, char* cmdOptionLine, char* description, struct SOption *option)
{
	int i=0;
	int j=0;
	printf("Usage : %s %s ",appliName, cmdOptionLine);
	while (option[i].shortName !=0)
	{
		if (option[i].nbMin == 0)
			printf("[");
		else
			printf("<");
		printf("-%c",option[i].shortName);
		for (j=0;j<option[i].nbArgs;j++)
			printf(" $%d",j+1);
		if (option[i].nbMin == 0)
		{
			printf("]");
			if (option[i].nbMax == 0)
				printf("*");
		}
		else
		{
			printf(">");
			if (option[i].nbMax == 0)
				printf("+");
		}
		printf(" ");
		i++;
	}
	printf("[-h]");
	printf("\n\n%s %s\n\n",appliName, description);
	i = 0;
	while (option[i].shortName !=0)
	{
		printf("\t-%c",option[i].shortName);
		for (j=0;j<option[i].nbArgs;j++)
			printf(" <$%d>",j+1);
		printf(" | --%s",option[i].longName);
		for (j=0;j<option[i].nbArgs;j++)
			printf(" <$%d>",j+1);
		printf("\n\t\t%s\n",option[i].description);
		i++;
	}
	printf("\t-h | --help\n\t\tDisplay help\n\n");
}

int parseCommandLine(int *argc, char*** argv,
					 struct SOption *option, struct SOptionOut **commandLine,
					 int nbMinArg, int noConfig)
{
	if (parseCommandLineOption(argc,argv,option,commandLine) != OPT_NO_ERROR)
	{
		return getError();
	}

	if (!noConfig)
	{
		int o;
		if ((o=optionIndex(*commandLine,'c')) != ERROR_UNKNOWN)
		{
			if (parseConfigFileOption((*commandLine)[o].argv[0],argc,argv,option,commandLine) != OPT_NO_ERROR)
			{
				return getError();
			}
		}
	}

	if ((*argc) <= nbMinArg && nbMinArg >= 0)
	{
		char errorStr[1024];
		sprintf(errorStr, "Wrong number of arg, %d expected\n\n", nbMinArg);
		RET_ERROR(ERROR_NBARG, errorStr);
	}
	return OPT_NO_ERROR;
}
