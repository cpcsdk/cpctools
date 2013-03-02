/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2010  cpcsdk crew

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include <pthread.h>

#define ErrorLogMessage Log::log

#define CriticalLogMessage Log::log

#define WarningLogMessage Log::log

#define InfoLogMessage Log::infoLog

#define DebugLogMessage Log::debugLog

class Log
{
    public:
        static inline void init()
        {
            if(!instance) instance = new Log();
        }
        
        static inline void log(const char *format, ...)
        {
            if(!instance) init();
            va_list args;
            va_start(args,format);
#if 1
            instance->errLogMsg(format,args);
#else
            fprintf(stderr, "Thread ID: %lu [LOG] ", pthread_self());
            vfprintf(stderr, format, args);
            fprintf(stderr, "\n");
#endif
            va_end(args);
        }
        
        static inline void debugLog(const char *formatIn, ...)
        {
#ifndef NDEBUG
            char* formatOut = new char[(strlen(formatIn)+1) + 8];

            if(!instance) init();
            va_list args;
            va_start(args,formatIn);
            sprintf(formatOut, "[DEBUG] ");
            strcat(formatOut, formatIn);
#if 1
            instance->errLogMsg(formatOut,args);
#else
            fprintf(stderr, "Thread ID: %lu [LOG] ", pthread_self());
            vfprintf(stderr, formatOut, args);
            fprintf(stderr, "\n");
#endif
            va_end(args);

            delete[] formatOut;
            formatOut = NULL;
#endif
        }
        
        static inline void infoLog(const char *format, ...)
        {
            if(!instance) init();
            va_list args;
            va_start(args,format);
#if 1
            instance->infoLogMsg(format,args);
#else
            fprintf(stderr, "Thread ID: %lu [LOG] ", pthread_self());
            vfprintf(stderr, format, args);
            fprintf(stderr, "\n");
#endif
            va_end(args);
        }
        
    protected:
        Log() {};
        virtual ~Log() {};
        static Log* instance;
    private:
        virtual void errLogMsg(const char *format, va_list args);
        virtual void infoLogMsg(const char *format, va_list args);
};

#endif /* #ifndef _LOG_H_ */
