/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2009  cpcsdk crew
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

#include "video.h"
#include "glfuncs.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#ifndef min
#define min(a,b) (a<b ? a : b)
#endif

#ifndef max
#define max(a,b) (a>b ? a : b)
#endif

/* ------------------------------------------------------------------------------------ */
/* Generic video plugin --------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------ */
const int VideoPlugin::CPCVisibleSCRWidth = 384; // visible width: 4+40+4 * 8
const int VideoPlugin::CPCVisibleSCRHeight = 270;

VideoPlugin::VideoPlugin(const string name, const uint32_t format) :
	_name(name),
	_formats(format),
	_postRenderCallBack(NULL),
	_is_init(false)
{
}

VideoPlugin::~VideoPlugin()
{
}

// checks for an OpenGL extension
#if 0
bool VideoPlugin::HaveOpenGLExtension(const string &name_ext)
{
#ifdef USE_OPENGL
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
#endif
	return false;
}
#endif

void* VideoPlugin::Init(int w, int h, int bpp, bool fs)
{
	_publicBPP = bpp;
	
	_outputWidth = w;
	_outputHeight = h;
	_publicWidth = w;
	_publicHeight = h / 2; // we will double the scanlines ourselves.
	_publicPitch = w * bpp / 8;

	return _publicVideo;
}

void VideoPlugin::SetOption(const string &/*optionName*/, bool /*val*/)
{
}
