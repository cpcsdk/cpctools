/*
 * Some command to help command line parsing
 * (c) Ramlaid 2003
 */

#define NO_ERROR		0
#define DISPLAY_HELP	1
#define ERROR_UNKNOWN	-1
#define ERROR_UNKOPT	-2
#define ERROR_OPTARG	-3
#define ERROR_CMDARG	-4
#define ERROR_NBARG		-5

#ifdef  __cplusplus
extern "C" {
#endif

struct SOption
{
	const char shortName;
	const char *longName;
	int nbMin;
	int nbMax;
	int nbArgs;
	const char *description;
};

struct SOptionOut
{
	char shortName;
	int argc;
	char **argv;
};

int getError();
char *getErrorString();

int parseCommandLine(int *argc, char*** argv,
					 struct SOption *option, struct SOptionOut **commandLine,
					 int nbMinArg, int noConfig);

int parseCommandLineOption(int *argc, char*** argv, struct SOption *option, struct SOptionOut **commandLine);
int parseConfigFileOption(char* configFile, int *argc, char*** argv, struct SOption *option, struct SOptionOut **commandLine);

int optionIndex(struct SOptionOut *commandLine, char c);
void freeOptionOut(struct SOptionOut *commandLine);

void printUsage(char *appliName, char* commandLine, char* description, struct SOption *option);

#ifdef  __cplusplus
}
#endif
