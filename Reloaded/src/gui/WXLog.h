#ifndef _WXLOG_H_
#define _WXLOG_H_

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "core/emulator.h"
#include "misc/log.h"

class WXLog: public Log
{
    public:
        static inline void init()
        {
            if(!instance) instance = new WXLog();
        }
    private:
        void errLogMsg(const char *format, va_list args)
        {
#if _GNU_SOURCE //TODO: BSD have the same fonctionality
            char* buffer;
            int r = vasprintf(&buffer, format, args);
            if(r != -1)
            {
                Emulator::getInstance()->logMessage(buffer);
            }
            else
            {
                buffer = NULL; // glibc vasprintf set buffer in an indefined stat if error, force to NULL pointer for not made a corrupt freeing.
            }
#else
            char* buffer = NULL;
            int bufferSize = 256 < strlen(format) ? 256 + strlen(format) : 256;
            int r = 0;
            do {
                delete[] buffer;
                buffer = new char[bufferSize];
                r = vsnprintf(buffer, bufferSize, format, args);
                if(r != -1 && r < bufferSize)
                {
                    Emulator::getInstance()->logMessage(buffer);
                }
                bufferSize *= 2;
            } while((r >= bufferSize) && (bufferSize <= 16384));
#endif
            delete[] buffer;
            buffer = NULL;
        }
        void infoLogMsg(const char *format, va_list args)
        {
            errLogMsg(format, args);
        }
};

#endif /* #ifndef _WXLOG_H_ */
