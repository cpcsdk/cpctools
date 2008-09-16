/* Auto-configuration for 16-bit DOS */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#define HAVE_LIBZ

#define HAVE_STRICMP 1
#undef HAVE_WINDOWS_H 
#undef HAVE_WINIOCTL_H 
#define HAVE_TIME_H
#define HAVE_SYS_STAT_H
#define HAVE_UNISTD_H
#define HAVE_LIMITS_H
#define HAVE_DIR_H
#define HAVE_DOS_H 1
#define HAVE_SYS_FARPTR_H 1
#define _NAIVE_DOS_REGS 1
