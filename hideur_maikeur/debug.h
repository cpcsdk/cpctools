/**************************************************************************/
/* debug.h : macros pour faciliter l'écriture du code                     */
/**************************************************************************/

#ifdef DBG
#define DEBUG(s,x) fprintf(stderr,"***DEBUG*** "s,x)
#else
#define DEBUG(s,x)
#endif
