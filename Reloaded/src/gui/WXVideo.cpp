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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cassert>

#include "core/video.h"
#include "WXVideo.h"
#include "core/gatearray.h"
#include "core/glfuncs.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>
#include "CapriceApp.h"

#include "misc/log.h"

/* ----------------------------------------------------------------------- *
 * wxWidget Line doubling video plugin ----------------------------------- *
 * This plugin is for wxWidget windows output.                             *
 * Only supports DoubleLine Mode and 24bpp mode.                           *
 * ----------------------------------------------------------------------- */

void* WXDoubleLinePlugin::Init(int w,int h, int bpp, bool fs)
{
	assert((w > 0) && (h > 0) && (bpp > 0));

    publicSurfaceSync.lock();
	if(bpp != 24)
	{
		CriticalLogMessage("WXDoubleLinePlugin initialization error: "
			"Only supports 24bpp mode");
		return NULL;
	}

	if(!fs)
	{
		w = CPCVisibleSCRWidth*2;
		h = CPCVisibleSCRHeight*2;
	}

	_outputWidth = w;
	_outputHeight = h;

	//_outputSurface = (unsigned char*)malloc(h*w*3*sizeof(unsigned char));
	_outputSurface = new uint8_t[h*w*3];
	if(!_outputSurface)
	{
		CriticalLogMessage("Creating output surface failed");
        publicSurfaceSync.unlock();
		return NULL;
	}

	_publicVideo = _outputSurface;

	_publicWidth = CPCVisibleSCRWidth*2;
	_publicHeight = CPCVisibleSCRHeight;
	_publicBPP = 24;
	_publicPitch = CPCVisibleSCRWidth*4*_publicBPP/8;

	img = new wxImage(w,h,(uint8_t*)_outputSurface,true);
	//img = new wxImage(CPCVisibleSCRWidth*2,CPCVisibleSCRHeight,(unsigned char*)_publicVideo->pixels,true);

	assert(img);
    bmp = new wxBitmap(*img);

	assert(bmp);
	assert(wxTheApp);
	CapriceWindowImpl* f = static_cast<CapriceApp*>(wxTheApp)->frame;

	assert(f);
	f->SetEmuImage(img);

    publicSurfaceSync.unlock();

	_is_init = true;
	return (void*)_publicVideo;
}

void WXDoubleLinePlugin::SetPalette(ColorARGB8888* c)
{
}

bool WXDoubleLinePlugin::TryLock()
{
//    DebugLogMessage("WXDoubleLinePlugin::TryLock()");
    return publicSurfaceSync.tryLock();
//    return true;
}

void WXDoubleLinePlugin::Unlock()
{
//    DebugLogMessage("WXDoubleLinePlugin::Unlock()");
    publicSurfaceSync.unlock();
}

bool WXDoubleLinePlugin::TryLockOutput()
{
//    DebugLogMessage("WXDoubleLinePlugin::TryLockOutput()");
    return true; /*outputSurfaceSync.tryLock();*/
}

bool WXDoubleLinePlugin::LockOutput()
{
    /*outputSurfaceSync.lock();*/
    return true;
}

void WXDoubleLinePlugin::UnlockOutput()
{
//    DebugLogMessage("WXDoubleLinePlugin::UnlockOutput()");
}

bool WXDoubleLinePlugin::IsUpdate()
{
    return isUpdateSync.tryLock();
}

void WXDoubleLinePlugin::Flip()
{
    publicSurfaceSync.lock();
	for(int i = 0; i < 270 /* TODO: define ou variable */; i++) {
		memcpy((uint8_t*)_publicVideo + i * _publicPitch + _publicPitch/2,
			(uint8_t*)_publicVideo + i * _publicPitch, _publicWidth * 3);
	}
	/*
    DebugLogMessage("Flip()");
	byte* src = (byte*)_publicVideo;
	byte* dest = (byte*)_outputSurface;
	int length = _publicWidth * 3;
	for(int line=0;line<_publicHeight;line++)
	{
	    memcpy(dest,src,length);
	    dest+=length;
	    memcpy(dest,src,length);
	    dest+=length;
	    src+=length;
	}
*/
    publicSurfaceSync.unlock();

#if !USE_PTHREAD
	CapriceWindowImpl* f = static_cast<CapriceApp*>(wxTheApp)->frame;
	assert(f);

	if (f->IsShownOnScreen() && !f->IsIconized())
	{
    	wxBitmap bmp(*img);
		//wxBitmap bmp(img->Scale(_outputWidth, _outputHeight, wxIMAGE_QUALITY_NORMAL));
		//wxBitmap bmp(img->Scale(_outputWidth, _outputHeight, wxIMAGE_QUALITY_HIGH));
    	wxClientDC dc(f->getPanel());
		dc.DrawBitmap(bmp,0,0,false);
	}
#endif

    isUpdateSync.unlock(); // Indicate update of output
}

void WXDoubleLinePlugin::Screenshot(string filename)
{
	wxBitmap* b;
	if (Emulator::getInstance()->GetGateArray()->GetMode() != 2)
		b = new wxBitmap(img->Scale(384,272));
	else
		b = new wxBitmap(*img);
	b->SaveFile(wxString(filename.c_str(), wxConvUTF8), wxBITMAP_TYPE_PNG, NULL);
	delete b;
}

void WXDoubleLinePlugin::Close()
{
    publicSurfaceSync.lock();

	delete img;
	img = NULL;

	//free(_outputSurface);
	delete [] (uint8_t*)_outputSurface;
	_outputSurface = NULL;

    publicSurfaceSync.unlock();
}

