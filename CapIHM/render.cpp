/*
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  



//
// Render function used by CRTC & GateArray
//

#include "render.h"
#include "video.h"

#define FNT_CHAR_WIDTH 8
#define FNT_CHAR_HEIGHT 8
#define FNT_CHARS 96
#define FNT_MIN_CHAR 32
#define FNT_MAX_CHAR FNT_MIN_CHAR+FNT_CHARS
#define FNT_BAD_CHAR 95

#include <SDL_video.h>

#include <iostream>

// SSE Intrinsics
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif


Renderer::Renderer() :
_currentFlagConfig(0),
_renderFunc(NULL),
_preRenderFunc(NULL),
_preRenderSyncFunc(NULL),
_preRenderBorderFunc(NULL),
_preRenderNormalFunc(NULL),
_videoPlugin(NULL),
_colorMonitor(true),
_monitorIntensity(10),
_monitorRemanency(false)
{
	for( int i = 0; i < 20; i++)
    {
        _palette[i] = 0;
    }
	for( int i = 0; i < 32; i++)
    {
        _colours[i].r = 0;
        _colours[i].g = 0;
        _colours[i].b = 0;
    }
	for( int i = 0; i < 800; i++)
    {
        _renderBuffer[i] = 0;
    }
}

Renderer::~Renderer()
{
	Shutdown();
	delete _renderFunc;
	_renderFunc = NULL;
	delete _preRenderBorderFunc;
	_preRenderBorderFunc = NULL;
	delete _preRenderSyncFunc;
	_preRenderSyncFunc = NULL;
	delete _preRenderNormalFunc;
	_preRenderNormalFunc = NULL;
	delete _videoPlugin;
	_videoPlugin = NULL;
}

bool Renderer::BeginDisplay(int screenLine)
{
	if (!_videoPlugin->Lock()) { // lock the video buffer
		return false; // skip the emulation if we can't get a lock
	}
	_scrPos = _renderFunc->GetScreenPosition();
	dword dwOffset = _scrPos - _scrBase; // offset in current surface row
	if (screenLine > 0) {
		_scrBase = screenLine * _scrLineOffset; // determine current position
	} else {
		_scrBase = 0; // reset to surface start
	}
	_scrPos = _scrBase + dwOffset; // update current rendering position
	_renderFunc->SetScreenPosition(_scrPos);
	return true;
}

void Renderer::EndDisplay(bool frameCompleted)
{
	if (frameCompleted)
	{
		// Display texts
		for (unsigned int t=0 ; t < _textArray.size() ; t++)
		{
			Print(_textArray[t].PosX, _textArray[t].PosY, _textArray[t].Text, _textArray[t].Shadow);
		}
		_textArray.clear();

		_videoPlugin->Unlock();
		_videoPlugin->Flip(); // update PC display
	}
	else
	{
		_videoPlugin->Unlock();
	}
}


SDL_Surface *Renderer::GetBackSurface()
{
	return _renderFunc->GetBackSurface();
}

bool Renderer::SetFullScreen(bool fs)
{
	if (fs == _scrFullScreen)
		return false;

	return ToggleFullScreen();
}

bool Renderer::ToggleFullScreen()
{
	Shutdown();
	_scrFullScreen = !_scrFullScreen;
	
	return Init();
}

void Renderer::AddTextLocate(int x, int y, const string &text, bool shadow)
{
	_textArray.push_back(TextDisplay(x * FNT_CHAR_WIDTH, (y * FNT_CHAR_HEIGHT) + 1, text, shadow));
}
void Renderer::AddText(int x, int y, const string &text, bool shadow)
{
	_textArray.push_back(TextDisplay(x, y, text, shadow));
}

void Renderer::Print(int x, int y, const string &text, bool shadow)
{
	SDL_Color white;
	white.r = white.g = white.b = 255;
	SDL_Color black;
	black.r = black.g = black.b = 0;

	if (x >= _renderFunc->GetBackSurface()->w || y >= _renderFunc->GetBackSurface()->h)
	{
		return;
	}
	
	int lineXPos = x;
	int lineYPos = y;

	for (unsigned int n = 0; n < text.size(); n++)
	{
		int iIdx = (int)text[n]; // get the ASCII value
		
		// limit it to the range of chars in the font
		if ((iIdx < FNT_MIN_CHAR) || (iIdx > FNT_MAX_CHAR)) 
		{
			iIdx = FNT_BAD_CHAR;
		}
		
		iIdx -= FNT_MIN_CHAR; // zero base the index
		
		int y = lineYPos;
		// loop for all rows in the font character
		for (int iRow = 0; iRow < FNT_CHAR_HEIGHT; iRow++) 
		{
			// get the bitmap information for one row
			byte bRow = Font[iIdx];
			
			int x = lineXPos;

			// loop for all columns in the font character
			for (int iCol = 0; iCol < FNT_CHAR_WIDTH; iCol++)
			{ 
				// is the bit set?
				if (bRow & 0x80) 
				{
					_renderFunc->PlotPixel(x, y, white);
					if (shadow)
					{
						_renderFunc->PlotPixel(x+1, y, black);
						_renderFunc->PlotPixel(x, y+1, black);
					}
				}
				bRow <<= 1; // advance to the next bit

				x++;
			}
			y++;
			iIdx += FNT_CHARS; // advance to next row in font data
		}
		lineXPos += FNT_CHAR_WIDTH; // set screen address to next character position
		if (lineXPos >= _renderFunc->GetBackSurface()->w)
		{
			lineXPos = 0;
			lineYPos += FNT_CHAR_HEIGHT;
			
			if (y >= _renderFunc->GetBackSurface()->h)
				return;
		}
	}
}

void Renderer::InitPalette()
{
	if (_colorMonitor) 
	{
		int n;
		for (n = 0; n < 32; n++) 
		{
			dword red = (dword)(ColoursRGB[n][0] * (_monitorIntensity / 10.0) * 255);
			// limit to the maximum
			if (red > 255) 
			{ 
				red = 255;
			}
			dword green = (dword)(ColoursRGB[n][1] * (_monitorIntensity / 10.0) * 255);
			if (green > 255) 
			{
				green = 255;
			}
			dword blue = (dword)(ColoursRGB[n][2] * (_monitorIntensity / 10.0) * 255);
			if (blue > 255) 
			{
				blue = 255;
			}
			_colours[n].r = red;
			_colours[n].g = green;
			_colours[n].b = blue;
		}
	} 
	else 
	{
		int n;
		for (n = 0; n < 32; n++) 
		{
			dword green = (dword)(ColoursGreen[n] * (_monitorIntensity / 10.0) * 255);
			if (green > 255) 
			{
				green = 255;
			}
			_colours[n].r = 0;
			_colours[n].g = green;
			_colours[n].b = 0;
		}
	}
	
	_videoPlugin->SetPalette(_colours);

	if (_renderFunc != NULL)
	{
		for (int i=0 ; i<17 ; i++)
		{
			_renderFunc->SetPalette(i, _colours[_palette[i]]);
		}
		
		SDL_Color colour;
		colour.r = ((dword)_colours[_palette[18]].r + (dword)_colours[_palette[19]].r) >> 1;
		colour.g = ((dword)_colours[_palette[18]].g + (dword)_colours[_palette[19]].g) >> 1;
		colour.b = ((dword)_colours[_palette[18]].b + (dword)_colours[_palette[19]].b) >> 1;
		
		_renderFunc->SetAntiAliasingColour(colour);
	}
}



void Renderer::SetMemory(byte *memory)
{
	_preRenderNormalFunc->SetMemory(memory);
}

void Renderer::SetVideoMode(VideoPlugin::VideoType videoPlugType, unsigned int fsWidth, unsigned int fsHeight, unsigned int fsBPP, bool fullScreen)
{
	_videoPluginType = videoPlugType;
	_scrFullScreenWidth = fsWidth;
	_scrFullScreenHeight = fsHeight;
	_scrFullScreenBPP = fsBPP;
	_scrFullScreen = fullScreen;
}

void Renderer::SetOpenGLFilter(bool val)
{ 
	_videoPluginOpenGLFilter = val; 
}

void Renderer::SetMonitor(bool color, unsigned int intensity, bool remanency)
{
	_colorMonitor = color;
	_monitorIntensity = intensity;
	_monitorRemanency = remanency;
}

bool Renderer::Init()
{
	byte *oldMemory = NULL;
	if (_preRenderNormalFunc != NULL)
	{
		oldMemory = _preRenderNormalFunc->GetMemory();
	}
	dword *oldRendPos = NULL;
	unsigned int oldMode = 0;
	if (_preRenderFunc != NULL)
	{
		oldRendPos = _preRenderFunc->GetRenderPos();
		oldMode = _preRenderFunc->GetMode();
	}

	delete _renderFunc;
	_renderFunc = NULL;
	delete _preRenderBorderFunc;
	_preRenderBorderFunc = NULL;
	delete _preRenderSyncFunc;
	_preRenderSyncFunc = NULL;
	delete _preRenderNormalFunc;
	_preRenderNormalFunc = NULL;
	delete _videoPlugin;
	_videoPlugin = NULL;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) { // initialize the video subsystem
		return false;
	}
	
	_videoPlugin = VideoPlugin::Create( _videoPluginType );
	_videoPlugin->SetOption("OpenGLFilter", _videoPluginOpenGLFilter);
	_videoPlugin->SetOption("Remanency", _monitorRemanency);
	
	// attempt to set the required video mode
	SDL_Surface *backSurface=_videoPlugin->Init(_scrFullScreenWidth, _scrFullScreenHeight, _scrFullScreenBPP, _scrFullScreen);
	if (backSurface == NULL)
	{ 
		return false;
	}
	
	Uint8 scrBPP = backSurface->format->BitsPerPixel; // bit depth of the surface
	_renderHalf = _videoPlugin->IsHalfSize();

	if (_renderHalf)
	{
		_preRenderSyncFunc = new PreRenderSyncHalfFunction;
		_preRenderBorderFunc = new PreRenderBorderHalfFunction;
		_preRenderNormalFunc = new PreRenderNormalHalfFunction;
		_preRenderFunc = _preRenderNormalFunc;
	}
	else
	{
		_preRenderSyncFunc = new PreRenderSyncFunction;
		_preRenderBorderFunc = new PreRenderBorderFunction;
		_preRenderNormalFunc = new PreRenderNormalFunction;
		_preRenderFunc = _preRenderNormalFunc;
	}
	
	switch(scrBPP)
	{
	case 32:
		_renderFunc = new Render32BppFunction;
		break;
		
	case 24:
		_renderFunc = new Render24BppFunction;
		break;
		
	case 16:
	case 15:
		_renderFunc = new Render16BppFunction;
		break;
		
	case 8:
		_renderFunc = new Render8BppFunction;
		break;
	}
	
	_preRenderNormalFunc->SetMemory(oldMemory);
	_preRenderFunc->SetMode(oldMode);
	_preRenderFunc->SetRenderPos(oldRendPos);

	_videoPlugin->Lock();
	
	_scrLineOffset = backSurface->pitch / 4; // rendered screen line length (changing bytes to dwords)
	_scrPos = _scrBase = 0; // memory address of back buffer
	
	_renderFunc->SetBackSurface(backSurface);
	_renderFunc->SetScreenPosition(_scrPos);

	_videoPlugin->Unlock();

	InitPalette(); // init CPC colours
	
	return true;
}

void Renderer::Shutdown ()
{
	if (_renderFunc->GetBackSurface() != NULL) 
	{
		_videoPlugin->Unlock();
	}
	_videoPlugin->Close();

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Renderer::SetCaption(const char *title, const char *icon)
{
	SDL_WM_SetCaption(title, icon);
}

void Renderer::Reset()
{
	int pixelWidth;

	if (_renderHalf) 
	{
		pixelWidth = 8;
		_pos2CharShift = 5;
	} 
	else 
	{
		pixelWidth = 16;
		_pos2CharShift = 4;
	}
	for (int i = 0; i < 48; i++) {
		_horizontalPixelWidth[i] = pixelWidth;
	}
	_horizontalPixelWidth[48] = 0;
	_renderStart = (dword *)&_renderBuffer[pixelWidth];
	_preRenderFunc->SetRenderPos( (dword *)&_renderBuffer[0] );
	_renderFunc->SetRenderData( (byte *)_renderStart );
	_renderFunc->SetRenderWidth( &_horizontalPixelWidth[0] );
	
	_horizontalPosition = 0x500;
	_horizontalCurrentChar = 0x04;
	_horizontalCharMax = 48;
}

void Renderer::SetMode(unsigned int mode)
{
	_preRenderFunc->SetMode(mode);
}

void Renderer::SetPalette(unsigned int pen, unsigned int colour)
{
	_renderFunc->SetPalette(pen, _colours[colour]);
	_palette[pen] = colour;
}

void Renderer::SetAntiAliasingColour(unsigned int col0, unsigned int col1)
{
    if (col0<32 && col1<32)
    {
	SDL_Color colour;
	colour.r = ((dword)_colours[col0].r + (dword)_colours[col1].r) >> 1;
	colour.g = ((dword)_colours[col0].g + (dword)_colours[col1].g) >> 1;
	colour.b = ((dword)_colours[col0].b + (dword)_colours[col1].b) >> 1;
	
	_renderFunc->SetAntiAliasingColour(colour);
}
    else
    {
        std::cerr << "SetAntiAliasingColour : color index overflow !" << std::endl;
    }
}

void Renderer::Render(unsigned int memAddr, dword flags)
{
	if (_horizontalCurrentChar < _horizontalCharMax) 
	{ 
		// below horizontal cut-off?

		SetPreRender(flags); // change pre-renderer if necessary
		_preRenderFunc->PreRender(memAddr); // translate CPC video memory bytes to entries referencing the palette
		_renderFunc->Render();
	}
}

void Renderer::HSyncCycle(int horzPos, unsigned int flag_drawing)
{
	if (flag_drawing)
	{
		// advance surface pointer to next row
		_scrBase += _scrLineOffset;
	}

	_horizontalPosition = horzPos;

	_horizontalCurrentChar = _horizontalPosition >> 8;
	dword val = (_horizontalPosition & 0xf0) >> _pos2CharShift;
	if (!val) 
	{
		_horizontalCharMax = 48;
		_horizontalPixelWidth[0] = _horizontalPixelWidth[1];
		_preRenderFunc->SetRenderPos( _renderStart );
		_horizontalCurrentChar--;
	} 
	else 
	{
		// When have some deformation due to monitor
		// Adapt renderer
		_preRenderFunc->SetRenderPos( (dword *)&_renderBuffer[val] );
		int tmp = (byte *)_renderStart - (byte *)_preRenderFunc->GetRenderPos();
		_horizontalPixelWidth[48] = (byte)tmp;
		_horizontalPixelWidth[0] = _horizontalPixelWidth[1] - (byte)tmp;
		_horizontalCharMax = 49;
	}
	// Line finish to be displayed
	// Reset render function pointer
	_renderFunc->SetRenderData( (byte *)_renderStart ) ;
	_renderFunc->SetRenderWidth( &_horizontalPixelWidth[0] );

	_scrPos = _scrBase;

	_renderFunc->SetScreenPosition(_scrPos);
}

void Renderer::SetPreRender(dword flags)
{
	if (flags == _currentFlagConfig) 
		return;

	dword *oldRendPos = _preRenderFunc->GetRenderPos();
	unsigned int oldMode = _preRenderFunc->GetMode();
	
	_currentFlagConfig = flags;
	if (_currentFlagConfig == 0x03ff0000) 
	{
		_preRenderFunc = _preRenderNormalFunc;
	}
	else 
	{
		if (!(word)_currentFlagConfig) 
		{
			_preRenderFunc = _preRenderBorderFunc;
		}
		else 
		{
			_preRenderFunc = _preRenderSyncFunc;
		}
	}

	_preRenderFunc->SetRenderPos( oldRendPos );
	_preRenderFunc->SetMode( oldMode );
}

void Renderer::PreRenderStandardFunction::UpdateMode()
{
	switch (_mode)
	{
	case 0: _modeMap = M0Map; break;
	case 1: _modeMap = M1Map; break;
	case 2: _modeMap = M2Map; break;
	case 3: _modeMap = M3Map; break;
	}
}

void Renderer::PreRenderHalfFunction::UpdateMode()
{
	switch (_mode)
	{
	case 0: _modeMap = M0hMap; break;
	case 1: _modeMap = M1hMap; break;
	case 2: _modeMap = M2hMap; break;
	case 3: _modeMap = M3hMap; break;
	}
}

void Renderer::PreRenderBorderFunction::PreRender(unsigned int /*memAddr*/)
{
	register dword dwVal = 0x10101010;
	*_renderPos = dwVal;
	*(_renderPos + 1) = dwVal;
	*(_renderPos + 2) = dwVal;
	*(_renderPos + 3) = dwVal;
	_renderPos += 4;
}



void Renderer::PreRenderBorderHalfFunction::PreRender(unsigned int /*memAddr*/)
{
	register dword dwVal = 0x10101010;
	*_renderPos = dwVal;
	*(_renderPos + 1) = dwVal;
	_renderPos += 2;
}



void Renderer::PreRenderSyncFunction::PreRender(unsigned int /*memAddr*/)
{
	register dword dwVal = 0x11111111;
	*_renderPos = dwVal;
	*(_renderPos + 1) = dwVal;
	*(_renderPos + 2) = dwVal;
	*(_renderPos + 3) = dwVal;
	_renderPos += 4;
}



void Renderer::PreRenderSyncHalfFunction::PreRender(unsigned int /*memAddr*/)
{
	register dword dwVal = 0x11111111;
	*_renderPos = dwVal;
	*(_renderPos + 1) = dwVal;
	_renderPos += 2;
}



void Renderer::PreRenderNormalFunction::PreRender(unsigned int memAddr)
{
	/*register byte bVidMem = *(_memory + memAddr);
	*_renderPos = *(_modeMap + (bVidMem * 2));
	*(_renderPos + 1) = *(_modeMap + (bVidMem * 2) + 1);
	bVidMem = *(_memory + memAddr + 1);
	*(_renderPos + 2) = *(_modeMap + (bVidMem * 2));
	*(_renderPos + 3) = *(_modeMap + (bVidMem * 2) + 1);
	_renderPos += 4;*/
	register byte bVidMem = *(_memory + memAddr);
	register byte bVidMem2 = *(_memory + memAddr + 1);
#ifdef __SSE__
	_mm_prefetch(_modeMap + (bVidMem * 2), _MM_HINT_NTA);
	_mm_prefetch(_modeMap + (bVidMem2 * 2), _MM_HINT_NTA);
#endif
	*_renderPos = *(_modeMap + (bVidMem * 2));
	*(_renderPos + 1) = *(_modeMap + (bVidMem * 2) + 1);
	*(_renderPos + 2) = *(_modeMap + (bVidMem2 * 2));
	*(_renderPos + 3) = *(_modeMap + (bVidMem2 * 2) + 1);
	_renderPos += 4;
}



void Renderer::PreRenderNormalHalfFunction::PreRender(unsigned int memAddr)
{
	register byte bVidMem = *(_memory + memAddr);
	*_renderPos = *(_modeMap + bVidMem);
	bVidMem = *(_memory + memAddr + 1);
	*(_renderPos + 1) = *(_modeMap + bVidMem);
	_renderPos += 2;
}

void Renderer::RenderFunction::SetPalette(unsigned int pen, const SDL_Color &colour)
{
	unsigned int palColour = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	_palette[pen] = palColour;
}

void Renderer::RenderFunction::SetAntiAliasingColour(const SDL_Color &colour)
{
	unsigned int palColour = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	_palette[18] = palColour;
}

void Renderer::Render8BppFunction::Render(void)
{
	register byte *pbPos = (byte *)_scrPos;
	register byte bCount = *_renderWidth++;
	while (bCount--) {
		*pbPos++ = _palette[*_renderData++];
	}
	_scrPos = (dword *)pbPos;
}

void Renderer::Render8BppFunction::PlotPixel(int x, int y, const SDL_Color &colour)
{
	if (x<0 || x>=_backSurface->w || y<0 || y>=_backSurface->h)
		return;

	byte* pos = (byte*)_backSurface->pixels + (_backSurface->pitch * y) + x;

	dword val = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	*pos = (byte)val;
}

void Renderer::Render16BppFunction::Render(void)
{
	register word *pwPos = (word *)_scrPos;
	register byte bCount = *_renderWidth++;
	while (bCount--) {
		*pwPos++ = _palette[*_renderData++];
	}
	_scrPos = (dword *)pwPos;
}

void Renderer::Render16BppFunction::PlotPixel(int x, int y, const SDL_Color &colour)
{
	if (x<0 || x>=_backSurface->w || y<0 || y>=_backSurface->h)
		return;

	byte* pos = (byte*)_backSurface->pixels + (_backSurface->pitch * y) + (x * 2);

	dword val = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	*(word *)pos = (word)val;
}

void Renderer::Render24BppFunction::Render(void)
{
	register byte *pbPos = (byte *)_scrPos;
	register byte bCount = *_renderWidth++;
#ifdef __SSE__
	_mm_prefetch(_palette, _MM_HINT_NTA);
	//_mm_prefetch(_renderData, _MM_HINT_NTA);
#endif
	while (bCount--) {
		register dword val = _palette[*_renderData++];
//		*(word *)pbPos = (word)val;
//		*(pbPos + 2) = (byte)(val >> 16);
		*(dword*)pbPos=val;
		pbPos += 3;
	}
	_scrPos = (dword *)pbPos;
}

void Renderer::Render24BppFunction::PlotPixel(int x, int y, const SDL_Color &colour)
{
	if (x<0 || x>=_backSurface->w || y<0 || y>=_backSurface->h)
		return;

	byte* pos = (byte*)_backSurface->pixels + (_backSurface->pitch * y) + (x * 3);

	dword val = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	*(word *)pos = (word)val;
	*(pos + 2) = (byte)(val >> 16);
}

void Renderer::Render32BppFunction::Render(void)
{
	register byte bCount = *_renderWidth++;
#ifdef __SSE__
	_mm_prefetch(_palette, _MM_HINT_NTA);
	//_mm_prefetch(_renderData, _MM_HINT_NTA);
#endif
	while (bCount--) {
		*_scrPos++ = _palette[*_renderData++];
	}
}

void Renderer::Render32BppFunction::PlotPixel(int x, int y, const SDL_Color &colour)
{
	if (x<0 || x>=_backSurface->w || y<0 || y>=_backSurface->h)
		return;

	byte* pos = (byte*)_backSurface->pixels + (_backSurface->pitch * y) + (x * 4);

	dword val = SDL_MapRGB(_backSurface->format, colour.r, colour.g, colour.b);
	*(dword *)pos = val;
}
