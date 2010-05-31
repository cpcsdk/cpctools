//
// Caprice32 ZIP manager
//

#ifndef _ZIP_H_
#define _ZIP_H_
#include "cap32type.h"

typedef struct {
   char *pchZipFile;
   char *pchExtension;
   char *pchFileNames;
   char *pchSelection;
   int iFiles;
   unsigned int dwOffset;
} t_zip_info;

int zip_dir (t_zip_info *zi);
int zip_extract (char *pchZipFile, char *pchFileName, dword dwOffset);

#endif
