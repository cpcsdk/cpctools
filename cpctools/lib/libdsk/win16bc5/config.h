/* Auto-configuration for 16-bit Windows */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <io.h>	/* for mktemp */

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#define HAVE_TIME_H
#define HAVE_STRICMP 1
#define HAVE_LIMITS_H 1
#define HAVE_WINDOWS_H 1
#undef HAVE_WINIOCTL_H
#define HAVE_SYS_STAT_H
#define S_ISDIR(x) ((x) & S_IFDIR)

