//
// Caprice general file tools
//

#include "filetools.h"
#include <sys/stat.h>
#include <string.h>

int file_size (int file_num)
{
	struct stat s;
	
	if (!fstat(file_num, &s)) {
		return s.st_size;
	} else {
		return 0;
	}
}

void splitPathFileName(char *pchCombined, char *pchPath, char *pchFile)
{
	char *pchPtr;
	
	pchPtr = strrchr(pchCombined, '/'); // start from the end and find the first path delimiter
	if (!pchPtr) {
		pchPtr = strrchr(pchCombined, '\\'); // try again with the alternate form
	}
	if (pchPtr) {
		pchPtr++; // advance the pointer to the next character
		if (pchFile) {
			strcpy(pchFile, pchPtr); // copy the filename
		}
		char chOld = *pchPtr;
		*pchPtr = 0; // cut off the filename part
		if (pchPath != pchCombined) {
			if (pchPath) {
				strcpy(pchPath, pchCombined); // copy the path
			}
			*pchPtr = chOld; // restore original string
		}
	} else {
		if (pchFile) {
			*pchFile = 0; // no filename found
		}
		if (pchPath != pchCombined) {
			if (pchPath) {
				strcpy(pchPath, pchCombined); // copy the path
			}
		}
	}
}
