#include "ptcVideo.h"

#include <tinyptc.h>

ptcVideo::ptcVideo()
	: VideoPlugin("cpcVideo", ALL, 0)
{
}

VideoPlugin* ptcVideo::create()
{
	return new ptcVideo();
}

void* ptcVideo::Init(int w, int h, int bpp, bool fs)
{
	ptc_open("cpcVideo", w, h);

	_publicVideo = malloc(w*h*bpp/8);

	VideoPlugin::Init(w,h,bpp,fs);

	_publicPitch *= 2;
	return _publicVideo;
}

void ptcVideo::Flip()
{
	for(int y = 0; y < _publicHeight; y++)
	{
		uint8_t* start = (uint8_t*)_publicVideo + y * _publicPitch;
		memcpy(start + _publicPitch / 2, start, _publicPitch / 2);
	}
	ptc_update(_publicVideo);
}

void ptcVideo::Close()
{
	ptc_close();
}

bool ptcVideo::IsUpdate()
{
	// TODO
	return true;
}

void ptcVideo::Screenshot(string)
{
	// ignored
}

void ptcVideo::SetPalette(ColorARGB8888*)
{
	// ignored
}

void ptcVideo::UnlockOutput()
{
}

bool ptcVideo::LockOutput()
{
	return true;
}

bool ptcVideo::TryLockOutput()
{
	return true;
}

void ptcVideo::Unlock()
{
}

bool ptcVideo::TryLock()
{
	return true;
}
