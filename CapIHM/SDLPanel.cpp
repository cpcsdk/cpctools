/*******************************************************************************
 * wx-sdl tutorial v1.1
 * Copyright (C) 2005,2007 John David Ratliff
 *
 * wx-sdl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wx-sdl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with wx-sdl; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*******************************************************************************/

#include "SDLPanel.h"
#include <iostream>


inline void SDLPanel::onEraseBackground(wxEraseEvent &) { /* do nothing */ }


IMPLEMENT_CLASS(SDLPanel, wxPanel)


BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT(SDLPanel::onPaint)
    EVT_ERASE_BACKGROUND(SDLPanel::onEraseBackground)
    EVT_IDLE(SDLPanel::onIdle)
END_EVENT_TABLE()


SDLPanel::SDLPanel(wxWindow *parent, Emulator *emulator) : wxPanel(parent, IDP_PANEL), screen(0) {
    // ensure the size of the wxPanel
    wxSize size(768, 270);

    SetMinSize(size);
    SetMaxSize(size);

    this->emulator = emulator ;
}


SDLPanel::~SDLPanel() {
    if (screen) {
        SDL_FreeSurface(screen);
    }
}



void SDLPanel::onPaint(wxPaintEvent &) {
    return;    
    int width, height;
        GetSize(&width, &height);
        
        SDL_Surface *surface ;
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 
                                      24, 0, 0, 0, 0);     

	if (!emulator)
	    return ;
	
	Renderer render = emulator->GetRenderer();
return;
	//surface = render.GetBackSurface();



    // can't draw if the screen doesn't exist yet
    if (!surface) {
        return;
    }
   
    // lock the surface if necessary
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0) {
            return;
        }
    }

    // create a bitmap from our pixel data
    wxBitmap bmp(wxImage(surface->w, surface->h, 
                    static_cast<unsigned char *>(surface->pixels), true));
    


    // unlock the screen
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
    // paint the screen
    wxBufferedPaintDC dc(this, bmp);

   	SDL_FreeSurface(surface);
}

void SDLPanel::onIdle(wxIdleEvent &) {
	return;
    // create the SDL_Surface
    createScreen();
    
    // Lock surface if needed
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }
    
    // Ask SDL for the time in milliseconds
    int tick = SDL_GetTicks();
    
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            wxUint32 color = (y * y) + (x * x) + tick;
            wxUint8 *pixels = static_cast<wxUint8 *>(screen->pixels) + 
                              (y * screen->pitch) +
                              (x * screen->format->BytesPerPixel);

            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixels[0] = color & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = (color >> 16) & 0xFF;
            #else
                pixels[0] = (color >> 16) & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = color & 0xFF;
            #endif
        }
    }
    
    // Unlock if needed
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
    
    // refresh the panel
    Refresh(false);
    
    // throttle to keep from flooding the event queue
    wxMilliSleep(33);
}


void SDLPanel::createScreen() {
    if (!screen) {
        int width, height;
        GetSize(&width, &height);
        
        screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 
                                      24, 0, 0, 0, 0);     
    }
}

