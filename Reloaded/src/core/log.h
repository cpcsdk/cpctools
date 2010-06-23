#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

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
            instance->errLogMsg(format,args);
            va_end(args);
        }
        static inline void debugLog(const char *formatIn, ...)
        {
#if DEBUG
            char* formatOut = new char[(strlen(formatIn)+1) + 8];

            if(!instance) init();
            va_list args;
            va_start(args,formatIn);
            sprintf(formatOut, "[DEBUG] ");
            strcat(formatOut, formatIn);
            instance->errLogMsg(formatOut,args);
            va_end(args);

            delete formatOut;
            formatOut = NULL;
#endif
        }
        static inline void infoLog(const char *format, ...)
        {
            if(!instance) init();
            va_list args;
            va_start(args,format);
            instance->infoLogMsg(format,args);
            va_end(args);
        }
    protected:
        Log() {};
        virtual ~Log() {};
        static Log* instance;
    private:
        virtual void errLogMsg(const char *format, va_list args)
        {
            fprintf(stderr,"[LOG] ");
            vfprintf(stderr,format,args);
            fprintf(stderr,"\n");
            fflush(stderr);
        }
        virtual void infoLogMsg(const char *format, va_list args)
        {
            fprintf(stdout,"[INFO LOG] ");
            vfprintf(stdout,format,args);
            fprintf(stdout,"\n");
            fflush(stdout);
        }
};

#endif /* #ifndef _LOG_H_ */
