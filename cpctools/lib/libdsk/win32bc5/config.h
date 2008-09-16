/* Auto-configuration for 16-bit Windows */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#define HAVE_GETTEMPFILENAME

#define HAVE_LIMITS_H 1
#define HAVE_STRICMP 1
#define HAVE_WINDOWS_H 1
#define HAVE_WINIOCTL_H 1
#define HAVE_SYS_STAT_H 1

