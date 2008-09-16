/* Auto-configuration for 16-bit DOS */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys.h>
#include <stat.h>

#define PATH_MAX 67	/* DOS path maximum */

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#define HAVE_DOS_H 1
#define HAVE_STRICMP 1
#define HAVE_TIME_H 1
#undef HAVE_WINDOWS_H 
#undef HAVE_WINIOCTL_H 
#define S_ISDIR(x) ((x) & S_IFDIR)
