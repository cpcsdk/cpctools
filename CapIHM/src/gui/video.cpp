/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
   This file includes video filters from the SMS Plus/SDL 
   sega master system emulator :
   (c) Copyright Gregory Montoir
   http://membres.lycos.fr/cyxdown/smssdl/
*/

/*
   This file includes video filters from MAME
   (Multiple Arcade Machine Emulator) :
   (c) Copyright The MAME Team
   http://www.mame.net/
*/

#include "video.h"
#include <SDL_opengl.h>
#include "glfuncs.h"
#include <math.h>
#include <malloc.h>
#include <algorithm>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>
#include "CapriceApp.h"

#ifndef min
#define min(a,b) (a<b ? a : b)
#endif

#ifndef max
#define max(a,b) (a>b ? a : b)
#endif

/* ------------------------------------------------------------------------------------ */
/* Half size video plugin ------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class HalfSizePlugin : public VideoPlugin
{
public:
	HalfSizePlugin() : VideoPlugin("Half size", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
};

SDL_Surface* HalfSizePlugin::Init(int w,int h, int bpp,bool fs)
{
	if (!fs)
	{
		w=CPCVisibleSCRWidth;
		h=CPCVisibleSCRHeight;
	}
	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	if (!_video)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth,CPCVisibleSCRHeight,bpp,0,0,0,0);
	return _publicVideo;
}

void HalfSizePlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool HalfSizePlugin::Lock()
{
	return true;
}

void HalfSizePlugin::Unlock()
{
}

void HalfSizePlugin::Flip()
{
	SDL_Rect dr;
	dr.x=(_video->w-CPCVisibleSCRWidth)/2;
	dr.y=(_video->h-CPCVisibleSCRHeight)/2;
	dr.w=CPCVisibleSCRWidth;
	dr.h=CPCVisibleSCRHeight;
	SDL_BlitSurface(_publicVideo,NULL,_video,&dr);
	SDL_UpdateRects(_video,1,&dr);
}

void HalfSizePlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_publicVideo);
}

/* ------------------------------------------------------------------------------------ */
/* Half size with hardware flip video plugin ------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
class HalfSizeHardwarePlugin : public VideoPlugin
{
public:
	HalfSizeHardwarePlugin() : VideoPlugin("Half size with hardware flip", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
};

SDL_Surface* HalfSizeHardwarePlugin::Init(int w,int h, int bpp, bool fs)
{
	if (!fs)
	{
		w=CPCVisibleSCRWidth;
		h=CPCVisibleSCRHeight;
	}
	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	if (!_video)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	return _video;
}

void HalfSizeHardwarePlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool HalfSizeHardwarePlugin::Lock()
{
	if (SDL_MUSTLOCK(_video))
		return (SDL_LockSurface(_video)==0);
	return true;
}

void HalfSizeHardwarePlugin::Unlock()
{
	if (SDL_MUSTLOCK(_video))
		SDL_UnlockSurface(_video);
}

void HalfSizeHardwarePlugin::Flip()
{
	SDL_Flip(_video);
}

void HalfSizeHardwarePlugin::Close()
{
}

/* ------------------------------------------------------------------------------------ */
/* Double width video plugin ---------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class DoubleWidthPlugin : public VideoPlugin
{
public:
	DoubleWidthPlugin() : VideoPlugin("Double width", ALL, 0) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
};

SDL_Surface* DoubleWidthPlugin::Init(int w,int h, int bpp, bool fs)
{
	if (!fs)
	{
		w=CPCVisibleSCRWidth*2;
		h=CPCVisibleSCRHeight;
	}
	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	if (!_video)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth*2,CPCVisibleSCRHeight,bpp,0,0,0,0);
	return _publicVideo;
}

void DoubleWidthPlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool DoubleWidthPlugin::Lock()
{
	return true;
}

void DoubleWidthPlugin::Unlock()
{
}

void DoubleWidthPlugin::Flip()
{
	SDL_Rect dr;
	dr.x=(_video->w-CPCVisibleSCRWidth*2)/2;
	dr.y=(_video->h-CPCVisibleSCRHeight)/2;
	dr.w=CPCVisibleSCRWidth*2;
	dr.h=CPCVisibleSCRHeight;
	SDL_BlitSurface(_publicVideo,NULL,_video,&dr);
	SDL_UpdateRects(_video,1,&dr);
}

void DoubleWidthPlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_publicVideo);
}

/* ------------------------------------------------------------------------------------ */
/* Scanlines video plugin ------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class ScanlinePlugin : public VideoPlugin
{
public:
	ScanlinePlugin() : VideoPlugin("Scanlines", ALL, 0) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
};

SDL_Surface* ScanlinePlugin::Init(int w,int h, int bpp, bool fs)
{
	if (!fs)
	{
		w=CPCVisibleSCRWidth*2;
		h=CPCVisibleSCRHeight*2;
	}
	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	if (!_video)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth*2,CPCVisibleSCRHeight,bpp,0,0,0,0);
	return _publicVideo;
}

void ScanlinePlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool ScanlinePlugin::Lock()
{
	return true;
}

void ScanlinePlugin::Unlock()
{
}

void ScanlinePlugin::Flip()
{
	int line;
	SDL_Rect sr,dr;
	if (CPCVisibleSCRWidth*2>_video->w)
		sr.x=(CPCVisibleSCRWidth*2-_video->w)/2;
	else
		sr.x=0;
	if (CPCVisibleSCRHeight*2>_video->h)
		sr.y=(CPCVisibleSCRHeight*2-_video->h)/4;
	else
		sr.y=0;
	sr.w=_publicVideo->w;
	sr.h=1;
	dr.x=(_video->w-CPCVisibleSCRWidth*2)/2;
	dr.y=(_video->h-CPCVisibleSCRHeight*2)/2;
	dr.w=_publicVideo->w;
	dr.h=1;
	for(line=0;line<_publicVideo->h;line++)
	{
		SDL_BlitSurface(_publicVideo,&sr,_video,&dr);
		sr.y++;
		dr.y+=2;
	}
	dr.x=max((_video->w-CPCVisibleSCRWidth*2)/2,0);
	dr.y=max((_video->h-CPCVisibleSCRHeight*2)/2,0);
	dr.w=min(_publicVideo->w,_video->w);
	dr.h=min(_publicVideo->h*2,_video->h);
	SDL_UpdateRects(_video,1,&dr);
}

void ScanlinePlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_publicVideo);
}

/* ------------------------------------------------------------------------------------ */
/* Line doubling video plugin --------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class DoublePlugin : public VideoPlugin
{
public:
	DoublePlugin() : VideoPlugin("Double size", ALL, 0) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();
};

SDL_Surface* DoublePlugin::Init(int w,int h, int bpp, bool fs)
{
	if (!fs)
	{
		w=CPCVisibleSCRWidth*2;
		h=CPCVisibleSCRHeight*2;
	}
//	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	_video=SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,bpp,0x0000FF,0x00FF00,0xFF0000,0);
	if (!_video)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth*2,CPCVisibleSCRHeight,bpp,0x0000FF,0x00FF00,0xFF0000,0);
	return _publicVideo;
}

void DoublePlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool DoublePlugin::Lock()
{
	return true;
}

void DoublePlugin::Unlock()
{
}

void DoublePlugin::Flip()
{
	int line;
    // As we know both surface have the same format, use memcpy instead of sdl blit. Simpler and faster.
#ifdef USE_SDL_BLITS
	SDL_Rect sr,dr;
	if (CPCVisibleSCRWidth*2>_video->w)
		sr.x=(CPCVisibleSCRWidth*2-_video->w)/2;
	else
		sr.x=0;
	if (CPCVisibleSCRHeight*2>_video->h)
		sr.y=(CPCVisibleSCRHeight*2-_video->h)/4;
	else
		sr.y=0;
	sr.w=_publicVideo->w;
	sr.h=1;
	dr.x=(_video->w-CPCVisibleSCRWidth*2)/2;
	dr.y=(_video->h-CPCVisibleSCRHeight*2)/2;
	dr.w=_publicVideo->w;
	dr.h=1;
	for(line=0;line<_publicVideo->h;line++)
	{
		SDL_BlitSurface(_publicVideo,&sr,_video,&dr);
		dr.y++;
		SDL_BlitSurface(_publicVideo,&sr,_video,&dr);
		dr.y++;
		sr.y++;
	}
	dr.x=max((_video->w-CPCVisibleSCRWidth*2)/2,0);
	dr.y=max((_video->h-CPCVisibleSCRHeight*2)/2,0);
	dr.w=min(_publicVideo->w,_video->w);
	dr.h=min(_publicVideo->h*2,_video->h);
	SDL_UpdateRects(_video,1,&dr);
#else
	byte* src = (byte*)_publicVideo->pixels;
	byte* dest = (byte*)_video->pixels;
	int length = _publicVideo->w * 3;
	for(line=0;line<_publicVideo->h;line++)
	{
	    memcpy(dest,src,length);
	    dest+=length;
	    memcpy(dest,src,length);
	    dest+=length;
	    src+=length;
	}
	//SDL_UpdateRect(_video,0,0,0,0);
    wxBitmap bmp(wxImage(_video->w, _video->h,static_cast<unsigned char *>(_video->pixels), true));
    CapriceApp* MyApp =static_cast<CapriceApp*>(wxTheApp) ;
    wxBufferedPaintDC dc(MyApp->frame->getPanel(),bmp);
#endif
}

void DoublePlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_video);
	SDL_FreeSurface(_publicVideo);
}

/* ------------------------------------------------------------------------------------ */
/* OpenGL video plugin ---------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
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
	OpenGLPlugin(const string &name, const Uint32 &format, const Uint8 &halfPixels) :
	  VideoPlugin(name, format, halfPixels) {}

	virtual void SetPalette(SDL_Color* c);
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();

	virtual void SetOption(const string &optionName, bool val);
protected:
	SDL_Surface* OpenGLInit(int w,int h, int bpp, bool fs, int glScanline);
};

SDL_Surface* OpenGLPlugin::OpenGLInit(int w,int h, int bpp, bool fs, int glScanline)
{
	_GLScanlines = glScanline;

#ifdef _WIN32
	const char *gl_library = "OpenGL32.DLL";
#else
	const char *gl_library = "libGL.so.1";
#endif
	int surface_bpp;
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if (SDL_GL_LoadLibrary(gl_library)<0)
	{
		fprintf(stderr,"Unable to dynamically open GL lib : %s\n",SDL_GetError());
		return NULL;
	}
	
	if (!fs)
	{
		w=CPCVisibleSCRWidth*2;
		h=CPCVisibleSCRHeight*2;
	}
	_video=SDL_SetVideoMode(w,h,0,SDL_OPENGL | (fs?SDL_FULLSCREEN:0));
	if (!_video)
	{
		fprintf(stderr, "Could not set requested video mode: %s\n", SDL_GetError());
		return NULL;
	}
	if (init_glfuncs()!=0)
	{
		fprintf(stderr, "Cannot init OpenGL functions: %s\n", SDL_GetError());
		return NULL;
	}
	
	int major, minor;
	const char *version;
	version = (char *) eglGetString(GL_VERSION); 
	if (sscanf(version, "%d.%d", &major, &minor) != 2) {
		fprintf(stderr, "Unable to get OpenGL version\n");
		return NULL;
	}
	
	GLint max_texsize;
	eglGetIntegerv(GL_MAX_TEXTURE_SIZE,&max_texsize);
	if (max_texsize<512) {
		fprintf(stderr, "Your OpenGL implementation doesn't support 512x512 textures\n");
		return NULL;
	}
	
	// We have to react differently to the bpp parameter than with software rendering
	// Here are the rules :
	// for 8bpp OpenGL, we need the GL_EXT_paletted_texture extension
	// for 16bpp OpenGL, we need OpenGL 1.2+
	// for 24bpp reversed OpenGL, we need OpenGL 1.2+
	surface_bpp=0;
	switch(bpp)
	{
	case 8:
		surface_bpp = (HaveOpenGLExtension("GL_EXT_paletted_texture"))?8:0;
		break;
	case 15:
	case 16:
		surface_bpp = ((major>1)||(major == 1 && minor >= 2))?16:0;
		break;
	case 24:
		surface_bpp = ((major>1)||(major == 1 && minor >= 2))?24:0;
	case 32:
		surface_bpp = ((major>1)||(major == 1 && minor >= 2))?32:0;
	default:
		surface_bpp = ((major>1)||(major == 1 && minor >= 2))?32:0;
		break;
	}
	if (surface_bpp==0) {
		fprintf(stderr, "Your OpenGL implementation doesn't support %dbpp textures\n",surface_bpp);
		return NULL;
	}
	
	eglDisable(GL_FOG);
	eglDisable(GL_LIGHTING);
	eglDisable(GL_CULL_FACE);
	eglDisable(GL_DEPTH_TEST);
	eglDisable(GL_BLEND);
	eglDisable(GL_NORMALIZE);
	eglDisable(GL_ALPHA_TEST);
	eglEnable(GL_TEXTURE_2D);
	eglBlendFunc (GL_SRC_ALPHA, GL_ONE);
	
	eglGenTextures(1,&_screenTexnum);
	eglBindTexture(GL_TEXTURE_2D,_screenTexnum);
	eglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _openGLFilter?GL_LINEAR:GL_NEAREST);
	eglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _openGLFilter?GL_LINEAR:GL_NEAREST);
	_texWidth=512;
	_texHeight=512;
	
	switch(surface_bpp)
	{
	case 32:
		eglTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA,_texWidth,_texHeight, 0,
			GL_RGBA,
			GL_UNSIGNED_BYTE, NULL);
		break;
	case 24:
		eglTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,_texWidth,_texHeight, 0,
			GL_RGB,
			GL_UNSIGNED_BYTE, NULL);
		break;
	case 16:
		eglTexImage2D(GL_TEXTURE_2D, 0,GL_RGB5,_texWidth,_texHeight, 0,
			GL_RGB,
			GL_UNSIGNED_BYTE, NULL);
		break;
	case 8:
		eglTexImage2D(GL_TEXTURE_2D, 0,GL_COLOR_INDEX8_EXT,_texWidth,_texHeight, 0,
			GL_COLOR_INDEX,
			GL_UNSIGNED_BYTE, NULL);
		break;
	}
	
	if (_GLScanlines!=0)
	{
		Uint8 texmod;
		switch(_GLScanlines)
		{
		case 25:
			texmod=192; break;
		case 50:
			texmod=128; break;
		case 75:
			texmod=64; break;
		case 100:
			texmod=0; break;
		default:
			texmod=255;
		}
		eglGenTextures(1,&_modulateTexnum);
		eglBindTexture(GL_TEXTURE_2D,_modulateTexnum);
		eglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _openGLFilter?GL_LINEAR:GL_NEAREST);
		eglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _openGLFilter?GL_LINEAR:GL_NEAREST);
		
		Uint8 modulate_texture[]={
			255,255,255,
				0,0,0};
			modulate_texture[3]=texmod;
			modulate_texture[4]=texmod;
			modulate_texture[5]=texmod;
			eglTexImage2D(GL_TEXTURE_2D, 0,GL_RGB8,1,2, 0,GL_RGB,GL_UNSIGNED_BYTE, modulate_texture);
	}
	eglViewport(0,0,w,h);
	eglMatrixMode(GL_PROJECTION);
	eglLoadIdentity();
	eglOrtho(0,w,h,0,-1.0, 1.0);
	
	eglMatrixMode(GL_MODELVIEW);
	eglLoadIdentity();
	
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth,CPCVisibleSCRHeight,surface_bpp,0,0,0,0);
	return _publicVideo;
}

void OpenGLPlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32);
	if (_publicVideo->format->palette)
	{
		Uint8* pal=(Uint8*)malloc(sizeof(Uint8)*256*3);
		for(int i=0;i<256;i++)
		{
			pal[3*i  ] = _publicVideo->format->palette->colors[i].r;
			pal[3*i+1] = _publicVideo->format->palette->colors[i].g;
			pal[3*i+2] = _publicVideo->format->palette->colors[i].b;
		}
		eglBindTexture(GL_TEXTURE_2D,_screenTexnum);
		eglColorTableEXT(GL_TEXTURE_2D,GL_RGB8,256,GL_RGB,GL_UNSIGNED_BYTE,pal);
		free(pal);
	}
}

bool OpenGLPlugin::Lock()
{
	return true;
}

void OpenGLPlugin::Unlock()
{
}

void OpenGLPlugin::Flip()
{
	eglDisable(GL_BLEND);
	
	if (_GLScanlines!=0)
	{
		eglActiveTextureARB(GL_TEXTURE1_ARB);
		eglEnable(GL_TEXTURE_2D);
		eglBindTexture(GL_TEXTURE_2D,_modulateTexnum);
		eglTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		eglColor4f(1.0,1.0,1.0,1.0);
		eglActiveTextureARB(GL_TEXTURE0_ARB);
	}
	
	eglEnable(GL_TEXTURE_2D);
	eglBindTexture(GL_TEXTURE_2D,_screenTexnum);
	
	if (_remanency)
	{
		/* draw again using the old texture */
		eglBegin(GL_QUADS);
		eglColor4f(1.0,1.0,1.0,1.0);
		
		eglTexCoord2f(0.f, 0.f);
		if (_GLScanlines!=0)
			eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,0.f, 0.f);
		eglVertex2i(0, 0);
		
		eglTexCoord2f(0.f, (float)(_publicVideo->h)/_texHeight);
		if (_GLScanlines!=0)
			eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,0.f, (float)_video->h/2);
		eglVertex2i(0, _video->h);
		
		eglTexCoord2f((float)(_publicVideo->w)/_texWidth, (float)(_publicVideo->h)/_texHeight);
		if (_GLScanlines!=0)
			eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,(float)_video->w, (float)_video->h/2);
		eglVertex2i(_video->w, _video->h);
		
		eglTexCoord2f((float)(_publicVideo->w)/_texWidth, 0.f);
		if (_GLScanlines!=0)
			eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,(float)_video->w, 0);
		eglVertex2i(_video->w, 0);
		eglEnd();
		
		/* enable blending for the subsequent pass */
		eglEnable(GL_BLEND);
		eglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	/* upload the texture */
	switch(_publicVideo->format->BitsPerPixel)
	{
	case 32:
		eglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			_publicVideo->w, _publicVideo->h,
			GL_BGRA,GL_UNSIGNED_BYTE,
			_publicVideo->pixels);
		break;
	case 24:
		eglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			_publicVideo->w, _publicVideo->h,
			GL_BGR,GL_UNSIGNED_BYTE,
			_publicVideo->pixels);
		break;
	case 16:
		eglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			_publicVideo->w, _publicVideo->h,
			GL_RGB,GL_UNSIGNED_SHORT_5_6_5,
			_publicVideo->pixels);
		break;
	case 8:
		eglTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0,
			_publicVideo->w,_publicVideo->h, 
			GL_COLOR_INDEX, GL_UNSIGNED_BYTE, 
			_publicVideo->pixels);
		break;
	}
	
	/* draw ! */
	eglBegin(GL_QUADS);
	eglColor4f(1.0,1.0,1.0,0.5);
	
	eglTexCoord2f(0.f, 0.f);
	if (_GLScanlines!=0)
		eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,0.f, 0.f);
	eglVertex2i(0, 0);
	
	eglTexCoord2f(0.f, (float)(_publicVideo->h)/_texHeight);
	if (_GLScanlines!=0)
		eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,0.f, (float)_video->h/2);
	eglVertex2i(0, _video->h);
	
	eglTexCoord2f((float)(_publicVideo->w)/_texWidth, (float)(_publicVideo->h)/_texHeight);
	if (_GLScanlines!=0)
		eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,(float)_video->w, (float)_video->h/2);
	eglVertex2i(_video->w, _video->h);
	
	eglTexCoord2f((float)(_publicVideo->w)/_texWidth, 0.f);
	if (_GLScanlines!=0)
		eglMultiTexCoord2fARB(GL_TEXTURE1_ARB,(float)_video->w, 0);
	eglVertex2i(_video->w, 0);
	eglEnd();

	if (_postRenderCallBack != NULL)
		_postRenderCallBack();
	
	SDL_GL_SwapBuffers();
}

void OpenGLPlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_publicVideo);
	if (eglIsTexture(_screenTexnum))
		eglDeleteTextures(1,&_screenTexnum);
	if (eglIsTexture(_modulateTexnum))
		eglDeleteTextures(1,&_modulateTexnum);
}

void OpenGLPlugin::SetOption(const string &optionName, bool val)
{
	string optName = optionName;
	transform(optName.begin(), optName.end(), optName.begin(), (int(*)(int))std::tolower);

	if (optName == "openglfilter")
		_openGLFilter = val;
	else if (optName == "remanency")
		_remanency = val;
}

// Normal Open GL Scale plugin
class OpenGLScalePlugin : public OpenGLPlugin
{
public:
	OpenGLScalePlugin() : OpenGLPlugin("OpenGL scaling", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
};

SDL_Surface* OpenGLScalePlugin::Init(int w,int h, int bpp, bool fs)
{
	return OpenGLInit(w, h, bpp, fs, 0);
}

// 25 % Open GL Scale plugin
class OpenGLScale25Plugin : public OpenGLPlugin
{
public:
	OpenGLScale25Plugin() : OpenGLPlugin("OpenGL scaling, 25%% scanlines", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
};

SDL_Surface* OpenGLScale25Plugin::Init(int w,int h, int bpp, bool fs)
{
	return OpenGLInit(w,h,bpp,fs,25);
}

// 50 % Open GL Scale plugin
class OpenGLScale50Plugin : public OpenGLPlugin
{
public:
	OpenGLScale50Plugin() : OpenGLPlugin("OpenGL scaling, 50%% scanlines", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
};

SDL_Surface* OpenGLScale50Plugin::Init(int w,int h, int bpp, bool fs)
{
	return OpenGLInit(w,h,bpp,fs,50);
}

// 75 % Open GL Scale plugin
class OpenGLScale75Plugin : public OpenGLPlugin
{
public:
	OpenGLScale75Plugin() : OpenGLPlugin("OpenGL scaling, 75%% scanlines", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
};

SDL_Surface* OpenGLScale75Plugin::Init(int w,int h, int bpp, bool fs)
{
	return OpenGLInit(w,h,bpp,fs,75);
}

// 100 % Open GL Scale plugin
class OpenGLScale100Plugin : public OpenGLPlugin
{
public:
	OpenGLScale100Plugin() : OpenGLPlugin("OpenGL scaling, 100%% scanlines", ALL, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
};

SDL_Surface* OpenGLScale100Plugin::Init(int w,int h, int bpp, bool fs)
{
	return OpenGLInit(w,h,bpp,fs,100);
}

/* ------------------------------------------------------------------------------------ */
/* Generic filter video plugin -------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class FilterPlugin : public VideoPlugin
{
protected:
	static Uint32 colorMask;
	static Uint32 lowPixelMask;
	static Uint32 qcolorMask;
	static Uint32 qlowpixelMask;
	static Uint32 redblueMask;
	static Uint32 greenMask;
public:
	FilterPlugin(const string &name) : VideoPlugin(name, F16_BPP, 1) {}

	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs);
	virtual void SetPalette(SDL_Color* c);
	
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();

protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch,
				Uint8 *dstPtr, Uint32 dstPitch, int width, int height) = 0;

	inline int GetResult1(Uint32 A, Uint32 B, Uint32 C, Uint32 D/*, Uint32 E*/)
	{
		int x = 0;
		int y = 0;
		int r = 0;
		
		if (A == C)
			x += 1;
		else if (B == C)
			y += 1;
		if (A == D)
			x += 1;
		else if (B == D)
			y += 1;
		if (x <= 1)
			r += 1;
		if (y <= 1)
			r -= 1;
		return r;
	}

	inline int GetResult2(Uint32 A, Uint32 B, Uint32 C, Uint32 D/*, Uint32 E*/)
	{
		int x = 0;
		int y = 0;
		int r = 0;
		
		if (A == C)
			x += 1;
		else if (B == C)
			y += 1;
		if (A == D)
			x += 1;
		else if (B == D)
			y += 1;
		if (x <= 1)
			r -= 1;
		if (y <= 1)
			r += 1;
		return r;
	}

	inline int GetResult(Uint32 A, Uint32 B, Uint32 C, Uint32 D)
	{
		int x = 0;
		int y = 0;
		int r = 0;
		
		if (A == C)
			x += 1;
		else if (B == C)
			y += 1;
		if (A == D)
			x += 1;
		else if (B == D)
			y += 1;
		if (x <= 1)
			r += 1;
		if (y <= 1)
			r -= 1;
		return r;
	}


	inline Uint32 Interpolate(Uint32 A, Uint32 B)
	{
		if (A != B)
		{
			return (((A & colorMask) >> 1) + ((B & colorMask) >> 1) +
				(A & B & lowPixelMask));
		}
		else
			return A;
	}

	inline Uint32 Q_Interpolate(Uint32 A, Uint32 B, Uint32 C, Uint32 D)
	{
		register Uint32 x = ((A & qcolorMask) >> 2) +
			((B & qcolorMask) >> 2) +
			((C & qcolorMask) >> 2) + ((D & qcolorMask) >> 2);
		register Uint32 y = (A & qlowpixelMask) +
			(B & qlowpixelMask) + (C & qlowpixelMask) + (D & qlowpixelMask);
		y = (y >> 2) & qlowpixelMask;
		return x + y;
	}
};

Uint32 FilterPlugin::colorMask = 0xF7DEF7DE;
Uint32 FilterPlugin::lowPixelMask = 0x08210821;
Uint32 FilterPlugin::qcolorMask = 0xE79CE79C;
Uint32 FilterPlugin::qlowpixelMask = 0x18631863;
Uint32 FilterPlugin::redblueMask = 0xF81F;
Uint32 FilterPlugin::greenMask = 0x7E0;

SDL_Surface* FilterPlugin::Init(int w,int h, int bpp, bool fs)
{
	if (bpp!=16)
		return NULL;
	if (!fs)
	{
		w=CPCVisibleSCRWidth*2;
		h=CPCVisibleSCRHeight*2;
	}
	_video=SDL_SetVideoMode(w,h,bpp,SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | (fs?SDL_FULLSCREEN:0));
	if (!_video)
		return NULL;
	if (_video->format->BitsPerPixel!=16)
		return NULL;
	SDL_FillRect(_video,NULL,SDL_MapRGB(_video->format,0,0,0));
	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth,CPCVisibleSCRHeight,bpp,0,0,0,0);
	return _publicVideo;
}

void FilterPlugin::SetPalette(SDL_Color* c)
{
	SDL_SetPalette(_video, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
	SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32); 
}

bool FilterPlugin::Lock()
{
	return true;
}

void FilterPlugin::Unlock()
{
}

void FilterPlugin::Flip()
{
	if (SDL_MUSTLOCK(_video))
		SDL_LockSurface(_video);
	SDL_Rect src;
	SDL_Rect dst;
	ComputeRects(&src,&dst);
	Filter((Uint8*)_publicVideo->pixels + (2*src.x+src.y*_publicVideo->pitch), _publicVideo->pitch,
		(Uint8*)_video->pixels + (2*dst.x+dst.y*_video->pitch), _video->pitch, src.w, src.h);
	if (SDL_MUSTLOCK(_video))
		SDL_UnlockSurface(_video);
	SDL_UpdateRects(_video,1,&dst);
}

void FilterPlugin::Close()
{
	if (!_video)
		return;
	SDL_FreeSurface(_publicVideo);
}

/* ------------------------------------------------------------------------------------ */
/* Super eagle video plugin ----------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */

class SuperEaglePlugin : public FilterPlugin
{
public:
	SuperEaglePlugin() : FilterPlugin("Super eagle") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch,
				Uint8 *dstPtr, Uint32 dstPitch, int width, int height);
};

void SuperEaglePlugin::Filter(	Uint8 *srcPtr, Uint32 srcPitch, /* byte *deltaPtr,  */
								Uint8 *dstPtr, Uint32 dstPitch, int width, int height)
{
    Uint8  *dP;
    Uint16 *bP;
    Uint32 inc_bP;
	
	Uint32 finish;
	Uint32 Nextline = srcPitch >> 1;
	
	inc_bP = 1;
	
	for (; height ; height--)
	{
		bP = (Uint16 *) srcPtr;
		dP = dstPtr;
		for (finish = width; finish; finish -= inc_bP)
		{
			Uint32 color4, color5, color6;
			Uint32 color1, color2, color3;
			Uint32 colorA1, colorA2, colorB1, colorB2, colorS1, colorS2;
			Uint32 product1a, product1b, product2a, product2b;
			colorB1 = *(bP - Nextline);
			colorB2 = *(bP - Nextline + 1);
			
			color4 = *(bP - 1);
			color5 = *(bP);
			color6 = *(bP + 1);
			colorS2 = *(bP + 2);
			
			color1 = *(bP + Nextline - 1);
			color2 = *(bP + Nextline);
			color3 = *(bP + Nextline + 1);
			colorS1 = *(bP + Nextline + 2);
			
			colorA1 = *(bP + Nextline + Nextline);
			colorA2 = *(bP + Nextline + Nextline + 1);
			// --------------------------------------
			if (color2 == color6 && color5 != color3)
			{
				product1b = product2a = color2;
				if ((color1 == color2) || (color6 == colorB2))
				{
					product1a = Interpolate(color2, color5);
					product1a = Interpolate(color2, product1a);
					//                       product1a = color2;
				}
				else
				{
					product1a = Interpolate(color5, color6);
				}
				
				if ((color6 == colorS2) || (color2 == colorA1))
				{
					product2b = Interpolate(color2, color3);
					product2b = Interpolate(color2, product2b);
					//                       product2b = color2;
				}
				else
				{
					product2b = Interpolate(color2, color3);
				}
			}
			else if (color5 == color3 && color2 != color6)
			{
				product2b = product1a = color5;
				
				if ((colorB1 == color5) || (color3 == colorS1))
				{
					product1b = Interpolate(color5, color6);
					product1b = Interpolate(color5, product1b);
					//                       product1b = color5;
				}
				else
				{
					product1b = Interpolate(color5, color6);
				}
				
				if ((color3 == colorA2) || (color4 == color5))
				{
					product2a = Interpolate(color5, color2);
					product2a = Interpolate(color5, product2a);
					//                       product2a = color5;
				}
				else
				{
					product2a = Interpolate(color2, color3);
				}
				
			}
			else if (color5 == color3 && color2 == color6)
			{
				register int r = 0;
				
				r += GetResult (color6, color5, color1, colorA1);
				r += GetResult (color6, color5, color4, colorB1);
				r += GetResult (color6, color5, colorA2, colorS1);
				r += GetResult (color6, color5, colorB2, colorS2);
				
				if (r > 0)
				{
					product1b = product2a = color2;
					product1a = product2b = Interpolate(color5, color6);
				}
				else if (r < 0)
				{
					product2b = product1a = color5;
					product1b = product2a = Interpolate(color5, color6);
				}
				else
				{
					product2b = product1a = color5;
					product1b = product2a = color2;
				}
			}
			else
			{
				product2b = product1a = Interpolate(color2, color6);
				product2b =
					Q_Interpolate(color3, color3, color3, product2b);
				product1a =
					Q_Interpolate(color5, color5, color5, product1a);
				
				product2a = product1b = Interpolate(color5, color3);
				product2a =
					Q_Interpolate(color2, color2, color2, product2a);
				product1b =
					Q_Interpolate(color6, color6, color6, product1b);
				
				//                    product1a = color5;
				//                    product1b = color6;
				//                    product2a = color2;
				//                    product2b = color3;
			}
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			product1a = product1a | (product1b << 16);
			product2a = product2a | (product2b << 16);
#else
			product1a = (product1a << 16) | product1b;
			product2a = (product2a << 16) | product2b;
#endif
			
			*((Uint32 *) dP) = product1a;
			*((Uint32 *) (dP + dstPitch)) = product2a;
			
			bP += inc_bP;
			dP += sizeof (Uint32);
		}			// end of for ( finish= width etc..)
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
	}			// endof: for (height; height; height--)
}

/* ------------------------------------------------------------------------------------ */
/* Scale2x video plugin --------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class Scale2XPlugin : public FilterPlugin
{
public:
	Scale2XPlugin() : FilterPlugin("Scale2x") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch, 
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);
};

void Scale2XPlugin::Filter(Uint8 *srcPtr, Uint32 srcPitch, 
					Uint8 *dstPtr, Uint32 dstPitch,
					int width, int height)
{
	unsigned int nextlineSrc = srcPitch / sizeof(short);
	short *p = (short *)srcPtr;
	
	unsigned int nextlineDst = dstPitch / sizeof(short);
	short *q = (short *)dstPtr;
	
	while(height--) {
		int i = 0, j = 0;
		for(i = 0; i < width; ++i, j += 2) {
			short B = *(p + i - nextlineSrc);
			short D = *(p + i - 1);
			short E = *(p + i);
			short F = *(p + i + 1);
			short H = *(p + i + nextlineSrc);
			
			*(q + j) = D == B && B != F && D != H ? D : E;
			*(q + j + 1) = B == F && B != D && F != H ? F : E;
			*(q + j + nextlineDst) = D == H && D != B && H != F ? D : E;
			*(q + j + nextlineDst + 1) = H == F && D != H && B != F ? F : E;
		}
		p += nextlineSrc;
		q += nextlineDst << 1;
	}
}

/* ------------------------------------------------------------------------------------ */
/* ascale2x video plugin --------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class AdvancedScale2XPlugin : public FilterPlugin
{
public:
	AdvancedScale2XPlugin() : FilterPlugin("Advanced Scale2x") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch, 
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);
};

void AdvancedScale2XPlugin::Filter(Uint8 *srcPtr, Uint32 srcPitch,
					  Uint8 *dstPtr, Uint32 dstPitch, int width, int height)
{
    Uint8  *dP;
    Uint16 *bP;
    Uint32 inc_bP;
	
	Uint32 finish;
	Uint32 Nextline = srcPitch >> 1;
	inc_bP = 1;
	
	
	for (; height; height--)
	{
		bP = (Uint16 *) srcPtr;
		dP = dstPtr;
		
		for (finish = width; finish; finish -= inc_bP)
		{
			
			register Uint32 colorA, colorB;
			Uint32 colorC, colorD,
				colorE, colorF, colorG, colorH,
				colorI, colorJ, colorK, colorL,
				colorM, colorN, colorO, colorP;
			Uint32 product, product1, product2;
			
			//---------------------------------------
			// Map of the pixels:                    I|E F|J
			//                                       G|A B|K
			//                                       H|C D|L
			//                                       M|N O|P
			colorI = *(bP - Nextline - 1);
			colorE = *(bP - Nextline);
			colorF = *(bP - Nextline + 1);
			colorJ = *(bP - Nextline + 2);
			
			colorG = *(bP - 1);
			colorA = *(bP);
			colorB = *(bP + 1);
			colorK = *(bP + 2);
			
			colorH = *(bP + Nextline - 1);
			colorC = *(bP + Nextline);
			colorD = *(bP + Nextline + 1);
			colorL = *(bP + Nextline + 2);
			
			colorM = *(bP + Nextline + Nextline - 1);
			colorN = *(bP + Nextline + Nextline);
			colorO = *(bP + Nextline + Nextline + 1);
			colorP = *(bP + Nextline + Nextline + 2);
			
			if ((colorA == colorD) && (colorB != colorC))
			{
				if (((colorA == colorE) && (colorB == colorL)) ||
					((colorA == colorC) && (colorA == colorF)
					&& (colorB != colorE) && (colorB == colorJ)))
				{
					product = colorA;
				}
				else
				{
					product = Interpolate(colorA, colorB);
				}
				
				if (((colorA == colorG) && (colorC == colorO)) ||
					((colorA == colorB) && (colorA == colorH)
					&& (colorG != colorC) && (colorC == colorM)))
				{
					product1 = colorA;
				}
				else
				{
					product1 = Interpolate(colorA, colorC);
				}
				product2 = colorA;
			}
			else if ((colorB == colorC) && (colorA != colorD))
			{
				if (((colorB == colorF) && (colorA == colorH)) ||
					((colorB == colorE) && (colorB == colorD)
					&& (colorA != colorF) && (colorA == colorI)))
				{
					product = colorB;
				}
				else
				{
					product = Interpolate(colorA, colorB);
				}
				
				if (((colorC == colorH) && (colorA == colorF)) ||
					((colorC == colorG) && (colorC == colorD)
					&& (colorA != colorH) && (colorA == colorI)))
				{
					product1 = colorC;
				}
				else
				{
					product1 = Interpolate(colorA, colorC);
				}
				product2 = colorB;
			}
			else if ((colorA == colorD) && (colorB == colorC))
			{
				if (colorA == colorB)
				{
					product = colorA;
					product1 = colorA;
					product2 = colorA;
				}
				else
				{
					register int r = 0;
					
					product1 = Interpolate(colorA, colorC);
					product = Interpolate(colorA, colorB);
					
					r +=
						GetResult1 (colorA, colorB, colorG, colorE /*, colorI*/);
					r +=
						GetResult2 (colorB, colorA, colorK, colorF /*, colorJ*/);
					r +=
						GetResult2 (colorB, colorA, colorH, colorN /*, colorM*/);
					r +=
						GetResult1 (colorA, colorB, colorL, colorO /*, colorP*/);
					
					if (r > 0)
						product2 = colorA;
					else if (r < 0)
						product2 = colorB;
					else
					{
						product2 =
							Q_Interpolate(colorA, colorB, colorC,
							colorD);
					}
				}
			}
			else
			{
				product2 = Q_Interpolate(colorA, colorB, colorC, colorD);
				
				if ((colorA == colorC) && (colorA == colorF)
					&& (colorB != colorE) && (colorB == colorJ))
				{
					product = colorA;
				}
				else
					if ((colorB == colorE) && (colorB == colorD)
						&& (colorA != colorF) && (colorA == colorI))
					{
						product = colorB;
					}
					else
					{
						product = Interpolate(colorA, colorB);
					}
					
					if ((colorA == colorB) && (colorA == colorH)
						&& (colorG != colorC) && (colorC == colorM))
					{
						product1 = colorA;
					}
					else
						if ((colorC == colorG) && (colorC == colorD)
							&& (colorA != colorH) && (colorA == colorI))
						{
							product1 = colorC;
						}
						else
						{
							product1 = Interpolate(colorA, colorC);
						}
			}
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			product = colorA | (product << 16);
			product1 = product1 | (product2 << 16);
#else
			product = (colorA << 16) | product;
			product1 = (product1 << 16) | product2;
#endif
			*((Uint32 *) dP) = product;
			*((Uint32 *) (dP + dstPitch)) = product1;
			
			bP += inc_bP;
			dP += sizeof (Uint32);
		}			// end of for ( finish= width etc..)
		
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
	}			// endof: for (height; height; height--)
}

/* ------------------------------------------------------------------------------------ */
/* tv2x video plugin ------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
class TVScale2XPlugin : public FilterPlugin
{
public:
	TVScale2XPlugin() : FilterPlugin("TV 2x") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch, 
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);
};

void TVScale2XPlugin::Filter(	Uint8 *srcPtr, Uint32 srcPitch, 
								Uint8 *dstPtr, Uint32 dstPitch, 
								int width, int height)
{
	unsigned int nextlineSrc = srcPitch / sizeof(Uint16);
	Uint16 *p = (Uint16 *)srcPtr;
	
	unsigned int nextlineDst = dstPitch / sizeof(Uint16);
	Uint16 *q = (Uint16 *)dstPtr;
	
	while(height--) {
		int i = 0, j = 0;
		for(; i < width; ++i, j += 2) {
			Uint16 p1 = *(p + i);
			Uint32 pi;
			
			pi = (((p1 & redblueMask) * 7) >> 3) & redblueMask;
			pi |= (((p1 & greenMask) * 7) >> 3) & greenMask;
			
			*(q + j) = p1;
			*(q + j + 1) = p1;
			*(q + j + nextlineDst) = pi;
			*(q + j + nextlineDst + 1) = pi;
		}
		p += nextlineSrc;
		q += nextlineDst << 1;
	}
}

/* ------------------------------------------------------------------------------------ */
/* Software bilinear video plugin ----------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
class SoftwareBilinearPlugin : public FilterPlugin
{
public:
	SoftwareBilinearPlugin() : FilterPlugin("Software bilinear") {}

protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch, 
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);
};

void SoftwareBilinearPlugin::Filter(Uint8 *srcPtr, Uint32 srcPitch, 
					 Uint8 *dstPtr, Uint32 dstPitch, 
					 int width, int height)
{
	unsigned int nextlineSrc = srcPitch / sizeof(Uint16);
	Uint16 *p = (Uint16 *)srcPtr;
	unsigned int nextlineDst = dstPitch / sizeof(Uint16);
	Uint16 *q = (Uint16 *)dstPtr;
	
	while(height--) {
		int i, ii;
		for(i = 0, ii = 0; i < width; ++i, ii += 2) {
			Uint16 A = *(p + i);
			Uint16 B = *(p + i + 1);
			Uint16 C = *(p + i + nextlineSrc);
			Uint16 D = *(p + i + nextlineSrc + 1);
			*(q + ii) = A;
			*(q + ii + 1) = Interpolate(A, B);
			*(q + ii + nextlineDst) = Interpolate(A, C);
			*(q + ii + nextlineDst + 1) = Q_Interpolate(A, B, C, D);
		}
		p += nextlineSrc;
		q += nextlineDst << 1;
	}
}

/* ------------------------------------------------------------------------------------ */
/* Software bicubic video plugin ------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
class SoftwareBicubicPlugin : public FilterPlugin
{
private:
	static Uint32 BLUE_MASK565;
	static Uint32 RED_MASK565;
	static Uint32 GREEN_MASK565;

	static Uint32 BLUE_MASK555;
	static Uint32 RED_MASK555;
	static Uint32 GREEN_MASK555;
public:
	SoftwareBicubicPlugin() : FilterPlugin("Software bicubic") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch,
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);

private:
	inline void Mult(Uint16 c, float* r, float* g, float* b, float alpha) 
	{
		*r += alpha * ((c & RED_MASK565  ) >> 11);
		*g += alpha * ((c & GREEN_MASK565) >>  5);
		*b += alpha * ((c & BLUE_MASK565 ) >>  0);
	};

	inline Uint16 MakeRGB565(float r, float g, float b) 
	{
		return 
			((((Uint8)r) << 11) & RED_MASK565  ) |
			((((Uint8)g) <<  5) & GREEN_MASK565) |
			((((Uint8)b) <<  0) & BLUE_MASK565 );
	};

	inline float CubicWeight(float x) 
	{
		// P(x) = { x, x>0 | 0, x<=0 }
		// P(x + 2) ^ 3 - 4 * P(x + 1) ^ 3 + 6 * P(x) ^ 3 - 4 * P(x - 1) ^ 3
		double r = 0.;
		if(x + 2 > 0) r +=      pow(x + 2, 3);
		if(x + 1 > 0) r += -4 * pow(x + 1, 3);
		if(x     > 0) r +=  6 * pow(x    , 3);
		if(x - 1 > 0) r += -4 * pow(x - 1, 3);
		return (float)r / 6;
	}

};

Uint32 SoftwareBicubicPlugin::BLUE_MASK565 = 0x001F001F;
Uint32 SoftwareBicubicPlugin::RED_MASK565 = 0xF800F800;
Uint32 SoftwareBicubicPlugin::GREEN_MASK565 = 0x07E007E0;

Uint32 SoftwareBicubicPlugin::BLUE_MASK555 = 0x001F001F;
Uint32 SoftwareBicubicPlugin::RED_MASK555 = 0x7C007C00;
Uint32 SoftwareBicubicPlugin::GREEN_MASK555 = 0x03E003E0;


void SoftwareBicubicPlugin::Filter(Uint8 *srcPtr, Uint32 srcPitch, 
                    Uint8 *dstPtr, Uint32 dstPitch, 
                    int width, int height)
{
	unsigned int nextlineSrc = srcPitch / sizeof(Uint16);
	Uint16 *p = (Uint16 *)srcPtr;
	unsigned int nextlineDst = dstPitch / sizeof(Uint16);
	Uint16 *q = (Uint16 *)dstPtr;
	int dx = width << 1, dy = height << 1;
	float fsx = (float)width / dx;
	float fsy = (float)height / dy;
	float v = 0.0f;
	int j = 0;
	for(; j < dy; ++j) {
		float u = 0.0f;
		int iv = (int)v;
		float decy = v - iv;
		int i = 0;
		for(; i < dx; ++i) {
			int iu = (int)u;
			float decx = u - iu;
			float r, g, b;
			int m;
			r = g = b = 0.;
			for(m = -1; m <= 2; ++m) {
				float r1 = CubicWeight(decy - m);
				int n;
				for(n = -1; n <= 2; ++n) {
					float r2 = CubicWeight(n - decx);
					Uint16* pIn = p + (iu  + n) + (iv + m) * nextlineSrc;
					Mult(*pIn, &r, &g, &b, r1 * r2);
				}
			}
			*(q + i) = MakeRGB565(r, g, b);
			u += fsx;
		}
		q += nextlineDst;
		v += fsy;
	}
}

/* ------------------------------------------------------------------------------------ */
/* Dot matrix video plugin ------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
class DotMatrixPlugin : public FilterPlugin
{
private:
	static const Uint16 DotMatrix[16];
public:
	DotMatrixPlugin() : FilterPlugin("Dot matrix") {}
protected:
	virtual void Filter(Uint8 *srcPtr, Uint32 srcPitch, 
						Uint8 *dstPtr, Uint32 dstPitch,
						int width, int height);
private:
	inline Uint16 Dot16(Uint16 c, int j, int i) const
	{
		return c - ((c >> 2) & *(DotMatrix + ((j & 3) << 2) + (i & 3)));
	}
};

const Uint16 DotMatrixPlugin::DotMatrix[16] =
{
	0x01E0, 0x0007, 0x3800, 0x0000,
	0x39E7, 0x0000, 0x39E7, 0x0000,
	0x3800, 0x0000, 0x01E0, 0x0007,
	0x39E7, 0x0000, 0x39E7, 0x0000
};

void DotMatrixPlugin::Filter(Uint8 *srcPtr, Uint32 srcPitch, 
					  Uint8 *dstPtr, Uint32 dstPitch,
					  int width, int height)
{
	unsigned int nextlineSrc = srcPitch / sizeof(Uint16);
	Uint16 *p = (Uint16 *)srcPtr;
	
	unsigned int nextlineDst = dstPitch / sizeof(Uint16);
	Uint16 *q = (Uint16 *)dstPtr;
	
	int i, ii, j, jj;
	for(j = 0, jj = 0; j < height; ++j, jj += 2) {
		for(i = 0, ii = 0; i < width; ++i, ii += 2) {
			Uint16 c = *(p + i);
			*(q + ii) = Dot16(c, jj, ii);
			*(q + ii + 1) = Dot16(c, jj, ii + 1);
			*(q + ii + nextlineDst) = Dot16(c, jj + 1, ii);
			*(q + ii + nextlineDst + 1) = Dot16(c, jj + 1, ii + 1);
		}
		p += nextlineSrc;
		q += nextlineDst << 1;
	}
}

/* ------------------------------------------------------------------------------------ */
/* Generic video plugin --------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
int VideoPlugin::CPCVisibleSCRWidth = 384; // visible width: 4+40+4 * 8
int VideoPlugin::CPCVisibleSCRHeight = 270;

VideoPlugin::VideoPlugin(const string name, const Uint32 format, const Uint8 halfPixels) :
_name(name),
_formats(format),
_halfPixels(halfPixels),
_postRenderCallBack(NULL)
{
}

VideoPlugin::~VideoPlugin()
{
}

// computes the clipping of two rectangles and changes src and dst accordingly
// dst is the screen
// src is the internal window
void VideoPlugin::ComputeRects(SDL_Rect* src, SDL_Rect* dst)
{
	/* initialise the source rect to full source */
	src->x=0;
	src->y=0;
	src->w=_publicVideo->w;
	src->h=_publicVideo->h;
	
	dst->x=(_video->w-CPCVisibleSCRWidth*2)/2,
		dst->y=(_video->h-CPCVisibleSCRHeight*2)/2;
	dst->w=_video->w;
	dst->h=_video->h;
	
	int dw=src->w*2-dst->w;
	/* the src width is too big */
	if (dw>0)
	{
		src->w-=dw/2;
		src->x+=dw/4;
		
		dst->x=0;
		dst->w=_video->w;
	}
	else
	{
		dst->w=CPCVisibleSCRWidth*2;
	}
	int dh=src->h*2-dst->h;
	/* the src height is too big */
	if (dh>0)
	{
		src->h-=dh/2;
		src->y+=dh/4;
		
		dst->y=0;
		dst->h=_video->h;
	}
	else
	{
		src->h-=2*3;
		dst->h=CPCVisibleSCRHeight*2;
	}
}

VideoPlugin* VideoPlugin::Create(VideoType type)
{
	switch (type)
	{
	case Scanlines			: { return new ScanlinePlugin();		}
	case DoubleSize			: { return new DoublePlugin();			}
	case DoubleWidth		: { return new DoubleWidthPlugin();		}
	case HalfSize			: { return new HalfSizePlugin();		}
	case HardwareHalfSize	: { return new HalfSizeHardwarePlugin();}
	// Filtered video
	case SuperEagle			: { return new SuperEaglePlugin();		}
	case Scale2x			: { return new Scale2XPlugin();			}
	case AdvancedScale2x	: { return new AdvancedScale2XPlugin();	}
	case TVScale2x			: { return new TVScale2XPlugin();		}
	case SoftwareBilinear	: { return new SoftwareBilinearPlugin();}
	case SoftwareBicubic	: { return new SoftwareBicubicPlugin();	}
	case DotMatrix			: { return new DotMatrixPlugin();		}
	// OpenGL video
#ifdef USE_OPENGL
	case OpenGLScale		: { return new OpenGLScalePlugin();		}
	case OpenGLScale25		: { return new OpenGLScale25Plugin();	}
	case OpenGLScale50		: { return new OpenGLScale50Plugin();	}
	case OpenGLScale75		: { return new OpenGLScale75Plugin();	}
	case OpenGLScale100		: { return new OpenGLScale100Plugin();	}
#else
	case OpenGLScale		: { return new ScanlinePlugin();		}
	case OpenGLScale25		: { return new ScanlinePlugin();	}
	case OpenGLScale50		: { return new ScanlinePlugin();	}
	case OpenGLScale75		: { return new ScanlinePlugin();	}
	case OpenGLScale100		: { return new ScanlinePlugin();	}
#endif
	}
	return new ScanlinePlugin();
}


// checks for an OpenGL extension
bool VideoPlugin::HaveOpenGLExtension(const string &name_ext)
{
	const char *ext;
	ext = (const char *) (eglGetString (GL_EXTENSIONS));
	const char *f;
	if (ext == NULL)
		return false;
	f = ext + strlen (ext);
	while (ext < f)
	{
		unsigned int n = strcspn (ext, " ");
		if ((name_ext.size() == n) && (name_ext == string(ext)))
			return true;
		ext += (n + 1);
	}
	return false;
}

void VideoPlugin::SetOption(const string &/*optionName*/, bool /*val*/)
{
}
