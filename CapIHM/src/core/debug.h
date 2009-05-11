//
// Debug function for Caprice
//

#ifndef _CAPDEBUG_H_
#define _CAPDEBUG_H_

#ifdef USE_DEBUGGER

//#define USE_DEBUGGER_CRTC
//#define USE_DEBUGGER_FDC
//#define USE_DEBUGGER_GA
//#define USE_DEBUGGER_TAPE
#define USE_DEBUGGER_Z80

#else

#undef USE_DEBUGGER_CRTC
#undef USE_DEBUGGER_FDC
#undef USE_DEBUGGER_GA
#undef USE_DEBUGGER_TAPE
#undef USE_DEBUGGER_Z80

#endif

#endif
