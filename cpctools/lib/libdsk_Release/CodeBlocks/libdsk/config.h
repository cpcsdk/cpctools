#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#define HAVE_GETTEMPFILENAME 1
#define HAVE_LIMITS_H 1
#define HAVE_STRCMPI 1
#define HAVE_WINDOWS_H 1
#define HAVE_WINIOCTL_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_DIRECT_H 1
#define HAVE_SYS_UTIME_H 1
#define HAVE_TIME_H 1

#define S_ISDIR(x)  ((x) & _S_IFDIR)
#define S_ISREG(x)  ((x) & _S_IFREG)

