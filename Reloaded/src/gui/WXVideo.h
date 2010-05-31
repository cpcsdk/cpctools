#ifndef _WXVIDEO_H_
#define _WXVIDEO_H_

#include <wx/image.h>

class WXDoubleLinePlugin : public VideoPlugin
{
public:
	WXDoubleLinePlugin() : VideoPlugin("Double size", ALL, 0) {}

	virtual void* Init(int w,int h,int bpp,bool fs);
	//virtual void SetPalette(SDL_Color* c);
	virtual void SetPalette(ColorARGB8888* c);

	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
	
	static inline VideoPlugin* Create()
	{
		return (VideoPlugin*)(new WXDoubleLinePlugin());
	}

	wxImage* img;
};

#if 0
#include "glfuncs.h"

class OpenGLPlugin : public VideoPlugin
{
protected:
	int		_texWidth;
	int		_texHeight;
	GLuint	_screenTexnum;
	GLuint	_modulateTexnum;
	int		_GLScanlines;

	bool	_openGLFilter;
	bool	_remanency;
public:
/*	OpenGLPlugin(const string &name, const Uint32 &format, const Uint8 &halfPixels) :
	  VideoPlugin(name, format, halfPixels) {}*/

	OpenGLPlugin() : VideoPlugin("OpenGL scaling", ALL, 1) {}

	virtual void SetPalette(ColorARGB8888* c);
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
	
	static inline VideoPlugin* Create()
	{
		return (VideoPlugin*)(new OpenGLPlugin());
	}

	virtual void SetOption(const string &optionName, bool val);
protected:
	SDL_Surface* OpenGLInit(int w,int h, int bpp, bool fs, int glScanline);
	inline void* Init(int w,int h, int bpp, bool fs)
	{
		return OpenGLInit(w,h,bpp,fs,0);
	}
};
#endif // #if 0
#endif
