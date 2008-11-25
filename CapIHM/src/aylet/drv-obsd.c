/* aylet 0.4, a .AY music file player.
 * Copyright (C) 2001-2004 Russell Marks and Ian Collier. See main.c for licence.
 *
 * drv_obsd.c - (Rough) native audio driver for OpenBSD (by Chris Cox).
 */

#ifdef DRIVER_OPENBSD

#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/audioio.h>
#include <unistd.h>
#include <fcntl.h>
#include "sound.h"

#include "driver.h"


int driver_init (int *freqptr, int *stereoptr)
{
	audio_info_t ainfo;

	if (play_to_stdout)
	{
		soundfd = 1;
		return 1;
	}

	if ((soundfd = open ("/dev/sound", O_WRONLY, 0)) < 0)
		return 0;

	AUDIO_INITINFO (&ainfo);

	/* TODO: hackish. */
	ainfo.play.encoding = AUDIO_ENCODING_SLINEAR;
	ainfo.play.channels = (*stereoptr) ? 2 : 1;
	ainfo.play.precision = (sixteenbit ? 16 : 8);
	ainfo.play.sample_rate = (*freqptr); 

	if ((ioctl (soundfd, AUDIO_SETINFO, &ainfo)) < 0)
	{
		sixteenbit = 0;
		return 0;
	}
	else
		sixteenbit = 1;

	return 1;
}

void driver_end (void)
{
    if (soundfd > 1)
	close (soundfd);
}


void driver_frame(signed short *data,int len)
{
static unsigned char buf8[4096];
unsigned char *data8=(unsigned char *)data;
int ret=0,ofs=0;

len<<=1;	/* now in bytes */

if(!sixteenbit)
  {
  signed short *src;
  unsigned char *dst;
  int f;

  src=data; dst=buf8;
  len>>=1;
  for(f=0;f<len;f++)
    *dst++=128+(int)((*src++)/256);
  
  data8=buf8;
  }

while(len)
  {
  ret=write(soundfd,data8+ofs,len);
  if(ret>0)
    ofs+=ret,len-=ret;
  }
}

#endif	/* DRIVER_OPENBSD */
