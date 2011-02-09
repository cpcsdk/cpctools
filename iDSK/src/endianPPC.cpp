#include <iostream>
#include <cstdlib>
using namespace std;
#include "endianPPC.h"

#include <sys/param.h>

bool isBigEndian(void)
{
#ifdef BYTE_ORDER
    return BYTE_ORDER == BIG_ENDIAN;
#else
    return __BYTE_ORDER == __BIG_ENDIAN;
#endif
}
