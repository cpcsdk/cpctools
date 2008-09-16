/* Auto-configuration for 16-bit Windows */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <io.h> /* for mktemp */
#include <sys/stat.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#define HAVE_TIME_H 1
#define HAVE_LIMITS_H 1
#define HAVE_STRCMPI 1
#define HAVE_WINDOWS_H 1
#undef HAVE_WINIOCTL_H  

/* VC++ 1.5 hides stat() behind underscores */
#define stat _stat  
#define S_ISDIR(x)  ((x) & _S_IFDIR)

