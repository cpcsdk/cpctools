/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2010  cpcsdk crew
   Caprice32 - Amstrad CPC Emulator
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


#include "SDLVideo.h"


/* ------------------------------------------------------------------------------------ */
/* OpenGL video plugin ---------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */

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
	
//	_publicVideo=SDL_CreateRGBSurface(SDL_SWSURFACE,CPCVisibleSCRWidth,CPCVisibleSCRHeight,surface_bpp,0,0,0,0);
	return _publicVideo;
}

void OpenGLPlugin::SetPalette(ColorARGB8888* c)
{
	//SDL_SetPalette(_publicVideo, SDL_LOGPAL | SDL_PHYSPAL, c, 0, 32);
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
}

void OpenGLPlugin::Close()
{
	if (!_video)
		return;
	//SDL_FreeSurface(_publicVideo);
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
