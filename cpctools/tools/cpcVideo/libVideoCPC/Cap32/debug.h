//
// Debug function for Caprice
//

#pragma once
#define _CAPDEBUG_H_

//#define DEBUG

#ifdef DEBUG

//#define DEBUG_CRTC
//#define DEBUG_FDC
//#define DEBUG_GA
//#define DEBUG_TAPE
//#define DEBUG_Z80

#else

#undef DEBUG_CRTC
#undef DEBUG_FDC
#undef DEBUG_GA
#undef DEBUG_TAPE
#undef DEBUG_Z80

#endif