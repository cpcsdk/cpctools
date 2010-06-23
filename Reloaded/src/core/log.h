#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/*
#define ErrorLogMessage(msg) Emulator::getInstance()->logMessage(msg)

#define CriticalLogMessage(msg) Emulator::getInstance()->logMessage(msg)

#define WarningLogMessage(msg) Emulator::getInstance()->logMessage(msg)

#define InfoLogMessage(msg) Emulator::getInstance()->logMessage(msg)

#define DebugLogMessage(msg) Emulator::getInstance()->logMessage(msg)
*/

#define ErrorLogMessage StderrLogMessage

#define CriticalLogMessage StderrLogMessage

#define WarningLogMessage StderrLogMessage

#define InfoLogMessage StdoutLogMessage

//#define DebugLogMessage StderrLogMessage
inline void DebugLogMessage(const char *format, ...)
{
    va_list args;
    va_start(args,format);
    fprintf(stderr,"[DEBUG] ");
    vfprintf(stderr,format,args);
    fprintf(stderr,"\n");
    fflush(stderr);
    va_end(args);
}

//TODO: See for use of plugins (ex: WX) for message
inline void StderrLogMessage(const char *format, ...)
{
    va_list args;
    va_start(args,format);
    vfprintf(stderr,format,args);
    fprintf(stderr,"\n");
    fflush(stderr);
    va_end(args);
}

inline void StdoutLogMessage(const char *format, ...)
{
    va_list args;
    va_start(args,format);
    vfprintf(stdout,format,args);
    fprintf(stdout,"\n");
    //fflush(stdout);
    va_end(args);
}

#endif /* #ifndef _LOG_H_ */
