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

#ifndef _SDLPANEL_H_
#define _SDLPANEL_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dcbuffer.h>
#include <wx/image.h>

#include "SDL.h"
#include "emulator.h"

enum {
    IDF_FRAME = wxID_HIGHEST + 1,
    IDP_PANEL
};

class SDLPanel : public wxPanel {
    DECLARE_CLASS(SDLPanel)
    DECLARE_EVENT_TABLE()


private:
    Emulator *emulator;
	SDL_Surface *screen;

    /**
     * Called to paint the panel.
     */
    void onPaint(wxPaintEvent &);

    

    /**
     * Called to erase the background.
     */
    void onEraseBackground(wxEraseEvent &);

    

    /**
     * Called to update the panel in idle time.
     */
    void onIdle(wxIdleEvent &);

    

    /**
     * Creates the SDL_Surface used by this SDLPanel.
    */
	void createScreen();

    

public:
    /**
     * Creates a new SDLPanel.
     *
     * @param parent The wxWindow parent.
     */
    SDLPanel(wxWindow *parent, Emulator *emulator);

    

    /**
     * Destructor for the SDLPanel.
     */

    ~SDLPanel();

};


#endif _SDLPANEL_H_

