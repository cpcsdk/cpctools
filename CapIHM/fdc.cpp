//
// Caprice32 FDC emulator
//


#include "configBis.h"


#ifdef HAVE_LIB765_H
#include "fdc_765.cpp"
#else
#include "fdc_old.cpp"
#endif

