/*
 *   $Id$
 *	 Reloded an Amstrad CPC emulator
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

// TODO: Renderer doesn't support scanline pitch
// TODO: Renderer doesn't support 8bpp mode

//
// Render function used by CRTC & GateArray
//

#include "render.h"
#include "video.h"

#include "log.h"

#define FNT_CHAR_WIDTH 8
#define FNT_CHAR_HEIGHT 8
#define FNT_CHARS 96
#define FNT_MIN_CHAR 32
#define FNT_MAX_CHAR FNT_MIN_CHAR+FNT_CHARS
#define FNT_BAD_CHAR 95

#include <iostream>
#include <cassert>

// SSE Intrinsics
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif

RendererException RendererEx;

Renderer::Renderer(int bpp) :
_renderFunc(NULL),
_preRenderFunc(),
_videoPlugin(NULL),
_monitorIntensity(10),
_monitorRemanency(false),
_displayFPS(false),
_monitorMode(ColoursMode)
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

	switch(bpp)
	{
		case 32:
			_renderFunc = new Render32BppFunction();
			break;
		case 16:
			_renderFunc = new Render16BppFunction();
			break;
		case 8:
			_renderFunc = new Render8BppFunction();
			break;
		case 0:
			_renderFunc = new Render0BppFunction();
			break;
	}
}

Renderer::~Renderer()
{
	Shutdown();
	delete _renderFunc;
	_renderFunc = NULL;
}

bool Renderer::BeginDisplay(int screenLine)
{
    assert(_videoPlugin);
	assert(_renderFunc);
	if (!_videoPlugin->TryLock()) { // lock the video buffer
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
    assert(_videoPlugin);
    if (frameCompleted)
    {
    // Display texts
//        if(config->scr_fps)
//        if(_displayFPS)
        {
            for (unsigned int t=0 ; t < _textArray.size() ; t++)
            {
                Print(_textArray[t].PosX, _textArray[t].PosY, _textArray[t].Text, _textArray[t].Shadow);
            }
            _textArray.clear();
        }

        _videoPlugin->Unlock();
        _videoPlugin->Flip(); // update PC display
    }
    else
    {
        _videoPlugin->Unlock();
    }
}


void *Renderer::GetBackSurface()
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
	
	return Init(_videoPlugin);
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
	ColorARGB8888 white;
	white.r = white.g = white.b = 255;
	ColorARGB8888 black;
	black.r = black.g = black.b = 0;

	if(x >= _renderFunc->GetRenderSurfaceWidth() || y >= _renderFunc->GetRenderSurfaceHeight())
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
		if (lineXPos >= _renderFunc->GetRenderSurfaceWidth())
		{
			lineXPos = 0;
			lineYPos += FNT_CHAR_HEIGHT;
			
			if(y >= _renderFunc->GetRenderSurfaceHeight())
			{
				return;
			}
		}
	}
}


void Renderer::InitPalette()
{
    switch (_monitorMode)
    {
        case PersonalMode:
            {
                for (int n = 0; n < 32; n++)
                {
                    dword red = (dword)(ColoursPersonal[n][0] * (_monitorIntensity / 10.0) * 255);
                    if (red > 255) {red = 255;} // limit to the maximum
                    dword green = (dword)(ColoursPersonal[n][1] * (_monitorIntensity / 10.0) * 255);
                    if (green > 255) {green = 255;}
                    dword blue = (dword)(ColoursPersonal[n][2] * (_monitorIntensity / 10.0) * 255);
                    if (blue > 255) {blue = 255;}
                    _colours[n].r = red;
                    _colours[n].g = green;
                    _colours[n].b = blue;
                }
                break;
            }
        case ColoursMode:
            {
                for (int n = 0; n < 32; n++)
                {
                    dword red = (dword)(ColoursRGB[n][0] * (_monitorIntensity / 10.0) * 255);
                    if (red > 255) {red = 255;}
                    dword green = (dword)(ColoursRGB[n][1] * (_monitorIntensity / 10.0) * 255);
                    if (green > 255) {green = 255;}
                    dword blue = (dword)(ColoursRGB[n][2] * (_monitorIntensity / 10.0) * 255);
                    if (blue > 255) {blue = 255;}
                    _colours[n].r = red;
                    _colours[n].g = green;
                    _colours[n].b = blue;
                }
                break;
            }
        case ColoursHiFiMode:
		default:
            {
                for (int n = 0; n < 32; n++)
                {
                    dword red = (dword)(ColoursHiFi[n][0] * (_monitorIntensity / 10.0) * 255);
                    if (red > 255) {red = 255;}
                    dword green = (dword)(ColoursHiFi[n][1] * (_monitorIntensity / 10.0) * 255);
                    if (green > 255) {green = 255;}
                    dword blue = (dword)(ColoursHiFi[n][2] * (_monitorIntensity / 10.0) * 255);
                    if (blue > 255) {blue = 255;}
                    _colours[n].r = red;
                    _colours[n].g = green;
                    _colours[n].b = blue;
                }
                break;
            }
        case GreenMode:
            {
                for (int n = 0; n < 32; n++)
                {
                    dword green = (dword)(ColoursGreen[n] * (_monitorIntensity / 10.0) * 255);
                    if (green > 255) {green = 255;}
                    _colours[n].r = 0;
                    _colours[n].g = green;
                    _colours[n].b = 0;
                }
                break;
            }
        case GreyMode:
            {
                for (int n = 0; n < 32; n++)
                {
                    dword grey = (dword)(ColoursGrey[n] * (_monitorIntensity / 10.0) * 255);
                    if (grey > 255) {grey = 255;}
                    _colours[n].r = grey;
                    _colours[n].g = grey;
                    _colours[n].b = grey;
                }
                break;
            }
    }

    _videoPlugin->SetPalette(_colours);

    if (_renderFunc != NULL)
    {
        for (int i=0 ; i<17 ; i++)
        {
            _renderFunc->SetPalette(i, _colours[_palette[i]]);
        }

        ColorARGB8888 colour;
        colour.r = ((dword)_colours[_palette[18]].r + (dword)_colours[_palette[19]].r) >> 1;
        colour.g = ((dword)_colours[_palette[18]].g + (dword)_colours[_palette[19]].g) >> 1;
        colour.b = ((dword)_colours[_palette[18]].b + (dword)_colours[_palette[19]].b) >> 1;

        _renderFunc->SetAntiAliasingColour(colour);
    }
}


void Renderer::SetMemory(byte *memory)
{
	_preRenderFunc.SetMemory(memory);
}


void Renderer::SetOpenGLFilter(bool val)
{ 
	_videoPluginOpenGLFilter = val; 
}

void Renderer::SetMonitor(MonitorMode mode, unsigned int intensity, bool remanency)
{
	_monitorMode = mode;
	_monitorIntensity = intensity;
	_monitorRemanency = remanency;
}


int Renderer::Init(shared_ptr<VideoPlugin> plugin)
{
	delete _renderFunc;
	_renderFunc = NULL;

	_videoPlugin = plugin;
	_videoPlugin->SetOption("OpenGLFilter", _videoPluginOpenGLFilter);
	_videoPlugin->SetOption("Remanency", _monitorRemanency);
	
	void *backSurface = _videoPlugin->GetSurface();

	switch(_videoPlugin->GetRenderSurfaceBPP())
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
	default:
		_renderFunc = new Render0BppFunction;
		break;
	}

	_videoPlugin->TryLock(); // TODO: Seen if need real lock.
	
	_scrLineOffset = _videoPlugin->GetRenderSurfacePitch() / 4;
	_scrPos = _scrBase = 0; // memory address of back buffer
	
	_renderFunc->SetBackSurface(backSurface);
	_renderFunc->SetRenderSurfaceWidth(_videoPlugin->GetRenderSurfaceWidth());
	_renderFunc->SetRenderSurfaceHeight(_videoPlugin->GetRenderSurfaceHeight());
	_renderFunc->SetScreenPosition(_scrPos);
	_renderFunc->SetRenderSurfacePitch(_videoPlugin->GetRenderSurfacePitch());
	_renderFunc->SetRenderSurfaceBPP(_videoPlugin->GetRenderSurfaceBPP());

	Reset();

	_videoPlugin->Unlock();

	InitPalette(); // init CPC colours
	
	return 0;
}


void Renderer::Shutdown ()
{
	if (_renderFunc->GetBackSurface() != NULL) 
	{
		_videoPlugin->Unlock();
	}
	_videoPlugin->Close();
}


void Renderer::Reset()
{
	static const int pixelWidth = 16;

	for (int i = 0; i < 48; i++) {
		_horizontalPixelWidth[i] = pixelWidth;
	}
	_horizontalPixelWidth[48] = 0;
	_renderStart = (dword *)&_renderBuffer[pixelWidth];
	_preRenderFunc.SetRenderPos( (dword *)&_renderBuffer[0] );
	_renderFunc->SetRenderData( (byte *)_renderStart );
	_renderFunc->SetRenderWidth( &_horizontalPixelWidth[0] );
	
	_horizontalPosition = 0x500;
	_horizontalCurrentChar = 0x04;
	_horizontalCharMax = 48;
}


void Renderer::SetMode(unsigned int mode)
{
	_preRenderFunc.SetMode(mode);
}


void Renderer::SetPalette(unsigned int pen, unsigned int ga_colour)
{
	_renderFunc->SetPalette(pen, _colours[ga_colour]);
	//_palette[pen] = colour;
}


void Renderer::SetAntiAliasingColour(unsigned int col0, unsigned int col1)
{
    if (col0<32 && col1<32)
    {
    ColorARGB8888 colour;
	colour.r = ((dword)_colours[col0].r + (dword)_colours[col1].r) >> 1;
	colour.g = ((dword)_colours[col0].g + (dword)_colours[col1].g) >> 1;
	colour.b = ((dword)_colours[col0].b + (dword)_colours[col1].b) >> 1;
	
	_renderFunc->SetAntiAliasingColour(colour);
}
    else
    {
        ErrorLogMessage("Renderer::SetAntiAliasingColour : color index overflow !");
    }
}


void Renderer::Render(unsigned int memAddr, dword flags)
{
	if (_horizontalCurrentChar < _horizontalCharMax) 
	{ 
		// below horizontal cut-off?

		if (flags == 0x03ff0000)
		{
			_preRenderFunc.PreRenderNormal(memAddr);
		} else if(!(word)flags) {
			_preRenderFunc.PreRenderBorder(memAddr);
		} else {
			_preRenderFunc.PreRenderSync(memAddr);
		}
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
		_preRenderFunc.SetRenderPos( _renderStart );
		_horizontalCurrentChar--;
	} 
	else 
	{
		// When have some deformation due to monitor
		// Adapt renderer
		_preRenderFunc.SetRenderPos( (dword *)&_renderBuffer[val] );
		int tmp = (byte *)_renderStart - (byte *)_preRenderFunc.GetRenderPos();
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


void Renderer::PreRenderStandardFunction::PreRenderBorder(unsigned int /*memAddr*/)
{
	static const uint64_t dwVal = 0x1010101010101010ULL;
	*(uint64_t*)_renderPos = dwVal;
	*(uint64_t*)(_renderPos + 2) = dwVal;
	_renderPos += 4;
}


void Renderer::PreRenderStandardFunction::PreRenderSync(unsigned int /*memAddr*/)
{
	static const uint64_t dwVal = 0x1111111111111111ULL;
	*(uint64_t*)_renderPos = dwVal;
	*(uint64_t*)(_renderPos + 2) = dwVal;
	_renderPos += 4;
}


void Renderer::PreRenderStandardFunction::PreRenderNormal(unsigned int memAddr)
{
	/*register byte bVidMem = *(_memory + memAddr);
	*_renderPos = *(_modeMap + (bVidMem * 2));
	*(_renderPos + 1) = *(_modeMap + (bVidMem * 2) + 1);
	bVidMem = *(_memory + memAddr + 1);
	*(_renderPos + 2) = *(_modeMap + (bVidMem * 2));
	*(_renderPos + 3) = *(_modeMap + (bVidMem * 2) + 1);
	_renderPos += 4;*/
	const register byte bVidMem = *(_memory + memAddr);
	const register byte bVidMem2 = *(_memory + memAddr + 1);
#if 0
#ifdef __SSE__
	// TODO voire pour clang
	_mm_prefetch((_modeMap + (bVidMem * 2)), _MM_HINT_NTA);
	_mm_prefetch((_modeMap + (bVidMem2 * 2)), _MM_HINT_NTA);
#endif
#endif
	*(uint64_t*)_renderPos = _modeMap[bVidMem];
	*(uint64_t*)(_renderPos + 2) = _modeMap[bVidMem2];
	_renderPos += 4;
}


void Renderer::RenderFunction::SetPalette(unsigned int pen, const ColorARGB8888 &colour)
{
	if(_renderSurfaceBPP == 8)
	{
		_palette[pen] = pen;
	}
	else
	{
		_palette[pen] = MapRGB(colour);
	}
}

void Renderer::RenderFunction::SetAntiAliasingColour(const ColorARGB8888 &colour)
{
	if(_renderSurfaceBPP == 8)
	{
		_palette[18] = 18;
	}
	else
	{
		_palette[18] = MapRGB(colour);
	}
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

void Renderer::Render8BppFunction::PlotPixel(int x, int y, const ColorARGB8888 &colour)
{
	if (x<0 || x>=_renderSurfaceWidth || y<0 || y>=_renderSurfaceHeight)
	{
		return;
	}

	//byte* pos = (byte*)_backSurface->pixels + (_renderSurfacePitch * y) + x;
	byte* pos = (byte*)_backSurface + (_renderSurfacePitch * y) + x;

	//TODO: Rework 8bpp rendering
	dword val = MapRGB(colour);
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

void Renderer::Render16BppFunction::PlotPixel(int x, int y, const ColorARGB8888 &colour)
{
	if (x<0 || x>=_renderSurfaceWidth || y<0 || y>=_renderSurfaceHeight)
	{
		return;
	}

	//byte* pos = (byte*)_backSurface->pixels + (_renderSurfacePitch * y) + (x * 2);
	byte* pos = (byte*)_backSurface + (_renderSurfacePitch * y) + (x * 2);

	dword val = MapRGB(colour);
	*(word *)pos = (word)val;
}

void Renderer::Render24BppFunction::Render(void)
{
	register byte bCount = *_renderWidth++;
#ifdef __SSE__
	_mm_prefetch(_palette, _MM_HINT_NTA);
	//_mm_prefetch(_renderData, _MM_HINT_NTA);
#endif
	while (bCount>4) {
		bCount -= 3;

		*_scrPos++ = (_palette[*_renderData]&0xFFFFFF) | (_palette[*++_renderData]<<24);
		*_scrPos++ = (_palette[*_renderData]>>8) | (_palette[*++_renderData]<<16);
		*_scrPos++ = (_palette[*_renderData]>>16) | (_palette[*++_renderData]<<8);
		_renderData++;
	}
}

void Renderer::Render24BppFunction::PlotPixel(int x, int y, const ColorARGB8888 &colour)
{
	if (x<0 || x>=_renderSurfaceWidth || y<0 || y>=_renderSurfaceHeight)
	{
		return;
	}

	//byte* pos = (byte*)_backSurface->pixels + (_renderSurfacePitch * y) + (x * 3);
	byte* pos = (byte*)_backSurface + (_renderSurfacePitch * y) + (x * 3);

	dword val = MapRGB(colour);
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

void Renderer::Render32BppFunction::PlotPixel(int x, int y, const ColorARGB8888 &colour)
{
	if (x<0 || x>=_renderSurfaceWidth || y<0 || y>=_renderSurfaceHeight)
	{
		return;
	}

	//byte* pos = (byte*)_backSurface->pixels + (_renderSurfacePitch * y) + (x * 4);
	//_backSurface->pitch;
	byte* pos = (byte*)_backSurface + (_renderSurfacePitch * y) + (x * 4);

	dword val = MapRGB(colour);
	*(dword *)pos = val;
}
