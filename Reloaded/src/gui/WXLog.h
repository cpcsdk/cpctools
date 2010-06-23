#ifndef _WXLOG_H_
#define _WXLOG_H_

#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "emulator.h"

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
#else
            char* buffer = new char[2048];
            int r = vsprintf(buffer, format, args);
#endif
            if(r != -1)
            {
                Emulator::getInstance()->logMessage(buffer);
            }
            delete buffer;
            buffer = NULL;
        }
        void infoLogMsg(const char *format, va_list args)
        {
            errLogMsg(format, args);
        }
};

#endif /* #ifndef _WXLOG_H_ */
