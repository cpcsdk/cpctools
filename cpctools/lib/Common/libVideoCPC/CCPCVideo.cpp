/**
* @class CCPCVideo
* Classe permettant la gestion d'un ecran CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/                                                                                                           

#include "CCPCVideo.h"
#include "CCPCFileManager.h"

#include "CError.h"

#include "Cap32/crtc.h"
#include "Cap32/vdu.h"
#include "Cap32/gatearray.h"

#include <AntTweakBar.h>

#include <algorithm>
#include <functional>

#include <fstream>
#include <math.h>

float round(float x)
{
	if (fmod(x,1.0) < 0.5f)
		return floor(x);
	else
		return ceil(x);
}

//
// Tweak bar functions
//
#define CRTC_TWEAKBAR_CB( reg ) \
void TW_CALL SetCRTCR##reg(const void *value, void *clientData)					\
{																				\
	unsigned char val = *(const unsigned char*)value;							\
	((t_CRTC*)clientData)->SetRegisterValue(reg, val);							\
}																				\
void TW_CALL GetCRTCR##reg(void *value, void *clientData)						\
{																				\
	*(unsigned char *)value = ((t_CRTC*)clientData)->GetRegisterValue(reg);		\
}

CRTC_TWEAKBAR_CB(1)
CRTC_TWEAKBAR_CB(2)
CRTC_TWEAKBAR_CB(6)
CRTC_TWEAKBAR_CB(7)

void TW_CALL SetCRTCOffset(const void *value, void *clientData)
{
	unsigned short offset = *(const unsigned short*)value;
	((t_CRTC*)clientData)->SetRegisterValue(12, ((offset & 0xff00) >> 8 ) );
	((t_CRTC*)clientData)->SetRegisterValue(13, (offset & 0xff) );
}																			
void TW_CALL GetCRTCOffset(void *value, void *clientData)
{
	*(unsigned short *)value =	(((t_CRTC*)clientData)->GetRegisterValue(12) << 8) + 
								((t_CRTC*)clientData)->GetRegisterValue(13);
}

#define GA_TWEAKBAR_CB( reg ) \
void TW_CALL SetGAInk##reg(const void *value, void *clientData)						\
{																					\
	unsigned char val = t_GateArray::SoftToHardTable[*(const unsigned char*)value];	\
	((t_GateArray*)clientData)->SetInk(reg, val);						\
}																		\
void TW_CALL GetGAInk##reg(void *value, void *clientData)				\
{																		\
	*(unsigned char *)value = t_GateArray::HardToSoftTable[((t_GateArray*)clientData)->GetInk(reg)];	\
}

#define GA_DECLARE_TWEAK(reg) TwAddVarCB(GABar, "Col"#reg, TW_TYPE_UINT8, &SetGAInk##reg, &GetGAInk##reg, _gateArray, "group='Colors' max=26")

GA_TWEAKBAR_CB(0)
GA_TWEAKBAR_CB(1)
GA_TWEAKBAR_CB(2)
GA_TWEAKBAR_CB(3)
GA_TWEAKBAR_CB(4)
GA_TWEAKBAR_CB(5)
GA_TWEAKBAR_CB(6)
GA_TWEAKBAR_CB(7)
GA_TWEAKBAR_CB(8)
GA_TWEAKBAR_CB(9)
GA_TWEAKBAR_CB(10)
GA_TWEAKBAR_CB(11)
GA_TWEAKBAR_CB(12)
GA_TWEAKBAR_CB(13)
GA_TWEAKBAR_CB(14)
GA_TWEAKBAR_CB(15)
GA_TWEAKBAR_CB(16)

void TW_CALL SetGAMode(const void *value, void *clientData)
{
	unsigned char mode = *(const unsigned char*)value;
	((t_GateArray*)clientData)->SetMode(mode);

	int nbColor = (mode == 0) ? 16 : ((mode == 1) ? 4 : ((mode == 2) ? 2 : 4));
	
	for (int c=0 ; c < nbColor ; c++)
	{
		char def[64];
		sprintf(def, "GA/Col%d readwrite", c);
		TwDefine(def);
	}
	for (int c=nbColor ; c < 16 ; c++)
	{
		char def[64];
		sprintf(def, "GA/Col%d readonly", c);
		TwDefine(def);
	}
}																			
void TW_CALL GetGAMode(void *value, void *clientData)
{
	*(unsigned char *)value = ((t_GateArray*)clientData)->GetMode();
}

void TW_CALL SetVDUIntensity(const void *value, void *clientData)
{
	unsigned char val = *(const unsigned char*)value;
	((Renderer*)clientData)->SetMonitorIntensity(val);
}

void TW_CALL GetVDUIntensity(void *value, void *clientData)
{
	*(unsigned char *)value = ((Renderer*)clientData)->GetMonitorIntensity();
}

void TW_CALL SetVDUColor(const void *value, void *clientData)
{
	bool val = *(const bool*)value;
	((Renderer*)clientData)->SetMonitorColorTube(val);
}

void TW_CALL GetVDUColor(void *value, void *clientData)
{
	*(bool *)value = ((Renderer*)clientData)->IsMonitorColorTube();
}

//
// Change emulation value classes
//
CCPCVideo::ChipChangeKey::ChipChangeKey(const ChipChangeKey &key) :
Key(0),
Change(NULL)
{
	*this = key;
}

CCPCVideo::ChipChangeKey& CCPCVideo::ChipChangeKey::operator=(const ChipChangeKey &key)
{
	if (this == &key)
		return *this;
	delete Change;
	Key = key.Key;
	Change = key.Change->Clone();
	return *this;
}

bool CCPCVideo::ChipChangeKey::operator<(const ChipChangeKey &key) const
{
	return (Key < key.Key);
}

bool CCPCVideo::ChipChangeKey::operator==(const ChipChangeKey &key) const
{
	return (Key == key.Key);
}

//
// CRTC register change class
//
void CCPCVideo::CRTCChange::Execute()
{
	_crtc->SetRegisterValue(_reg, _value);
}
CCPCVideo::ChipChange* CCPCVideo::CRTCChange::Clone() const
{
	return new CRTCChange(*this);
}
//
// Gate Array color change class
//
void CCPCVideo::GateArrayChange::Execute()
{
	_gateArray->SetInk(_ink, t_GateArray::SoftToHardTable[_value]);
}
CCPCVideo::ChipChange* CCPCVideo::GateArrayChange::Clone() const
{
	return new GateArrayChange(*this);
}
//
// Gate Array mode change class
//
void CCPCVideo::GateArrayModeChange::Execute()
{
	_gateArray->SetMode(_mode);
}
CCPCVideo::ChipChange* CCPCVideo::GateArrayModeChange::Clone() const
{
	return new GateArrayModeChange(*this);
}

//
// Generic emulation interface class
//
CCPCVideo::EmulationInterface::EmulationInterface(CCPCVideo &video) :
_cpcVideo(video)
{
}
void CCPCVideo::EmulationInterface::VSyncUpdate()
{
}
void CCPCVideo::EmulationInterface::ScanlineUpdate(unsigned int line)
{
}
void CCPCVideo::EmulationInterface::NopUpdate(unsigned int cycle)
{
}

//
// Constructeur/destructeur
//
CCPCVideo::CCPCVideo(int mode) :
_cpcMemory(new byte [64*1024]),
_renderer(),
_vdu(NULL),
_gateArray(NULL),
_crtc(NULL),
_cycleCount(0),
_chipChangeArray(),
_chipChangeIndex(0),
_emulation(NULL),
_limitSpeed(true),
_displayFPS(true),
_timer(SDL_GetTicks()),
_FPSTimer(_timer),
_FPS(0.0f),
_frameCount(0)
{
	_renderer.SetVideoMode(VideoPlugin::OpenGLScale, 800, 600, 32, false);
	_renderer.SetOpenGLFilter(true);
	_renderer.SetMonitor(true, 15, true);

	if (!_renderer.Init()) {
		fprintf(stderr, "video_init() failed. Aborting.\n");
		exit(-1);
	}
	
	memset(_cpcMemory, 0, 64*1024);
	_renderer.SetMemory(_cpcMemory);

	_gateArray = new t_GateArray(_renderer);
	_vdu = new t_VDU(_renderer);
	_crtc = new t_CRTC(*_gateArray, *_vdu);

	_gateArray->Reset(mode);
	_crtc->Reset();
	_vdu->Reset();

	_crtc->SetRegisterValue(12,0);
	_crtc->SetRegisterValue(13,0);

	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	// Tell the window size to AntTweakBar
	TwWindowSize(_renderer.GetVideoPlugin()->GetWidth(), _renderer.GetVideoPlugin()->GetHeight());

	_renderer.GetVideoPlugin()->SetPostRenderCallBack((int (*)(void))&TwDraw);

	// Create a tweak bar
	TwBar *CRTCBar = TwNewBar("CRTC");

	// Add CRTC Register to bar
	TwAddVarCB(CRTCBar, "R1", TW_TYPE_UINT8, &SetCRTCR1, &GetCRTCR1, _crtc, "group=Size");
	TwAddVarCB(CRTCBar, "R6", TW_TYPE_UINT8, &SetCRTCR6, &GetCRTCR6, _crtc, "group=Size");
	TwAddVarCB(CRTCBar, "R2", TW_TYPE_UINT8, &SetCRTCR2, &GetCRTCR2, _crtc, "group='VSync/HSync Pos'");
	TwAddVarCB(CRTCBar, "R7", TW_TYPE_UINT8, &SetCRTCR7, &GetCRTCR7, _crtc, "group='VSync/HSync Pos'");
	TwAddVarCB(CRTCBar, "R12-13", TW_TYPE_UINT16, &SetCRTCOffset, &GetCRTCOffset, _crtc, "hexa group=Offset");
	TwDefine("CRTC iconify label='CRTC Registers' size='200 180'");

	TwBar *GABar = TwNewBar("GA");
	TwAddVarCB(GABar, "Border", TW_TYPE_UINT8, &SetGAInk16, &GetGAInk16, _gateArray, "group='Colors' max=26");
	GA_DECLARE_TWEAK(0);
	GA_DECLARE_TWEAK(1);
	GA_DECLARE_TWEAK(2);
	GA_DECLARE_TWEAK(3);
	GA_DECLARE_TWEAK(4);
	GA_DECLARE_TWEAK(5);
	GA_DECLARE_TWEAK(6);
	GA_DECLARE_TWEAK(7);
	GA_DECLARE_TWEAK(8);
	GA_DECLARE_TWEAK(9);
	GA_DECLARE_TWEAK(10);
	GA_DECLARE_TWEAK(11);
	GA_DECLARE_TWEAK(12);
	GA_DECLARE_TWEAK(13);
	GA_DECLARE_TWEAK(14);
	GA_DECLARE_TWEAK(15);
	TwAddVarCB(GABar, "Mode", TW_TYPE_UINT8, &SetGAMode, &GetGAMode, _gateArray, "max=3");
	TwDefine("GA iconify label='GateArray Registers' size='200 330'");

	// Create a tweak bar
	TwBar *VDUBar = TwNewBar("VDU");

	// Add CRTC Register to bar
	TwAddVarCB(VDUBar, "Intensity", TW_TYPE_UINT8, &SetVDUIntensity, &GetVDUIntensity, &_renderer, "max = 15");
	TwAddVarCB(VDUBar, "Color", TW_TYPE_BOOLCPP, &SetVDUColor, &GetVDUColor, &_renderer, "");
	TwDefine("VDU iconify label='Monitor' size='200 100'");

	Cls();
}

CCPCVideo::CCPCVideo(const CCPCVideo& i_scr) :
_renderer(),
_vdu(NULL),
_gateArray(NULL),
_crtc(NULL)
{
}

CCPCVideo::~CCPCVideo()
{
	delete _vdu;
	delete _gateArray;
	delete _crtc;
	delete[] _cpcMemory;
	
	// Terminate AntTweakBar
	TwTerminate();

	_renderer.Shutdown();
}


void CCPCVideo::InitTimer()
{
	_timer = SDL_GetTicks() + 64*312*5;
	_FPSTimer = _timer;
}

//
// Fonctions d'affichage (acces direct SDL)
//
bool CCPCVideo::Display(bool pause)
{
	Uint32 newTimer = SDL_GetTicks();
	Uint32 diffTimer = newTimer - _timer;
	if (((newTimer - _timer) <= (64*312/1000)) && _limitSpeed)
	{
		return false;
	}

	_timer = newTimer;

	if ((newTimer - _FPSTimer) > 1000)
	{
		_FPS = _frameCount;
		_frameCount = 0;
		_FPSTimer = newTimer;
	}

	if (pause)
	{
		if (! _renderer.BeginDisplay(_vdu->GetScrLn()))
			return false;

		TwDraw();

		if (_displayFPS)
		{
			char chStr[32];
			sprintf(chStr, "%3dFPS %3d%%", _FPS, _FPS * 100 / 50);
			_renderer.AddTextLocate(0, 0, chStr);
		}

		_renderer.EndDisplay(true);
		return true;
	}

	bool frameFinished = false;
	while (!frameFinished)
	{
		if (! _renderer.BeginDisplay(_vdu->GetScrLn()))
			continue;
		
		do 
		{
			ManageChip();

			bool inVSync = _crtc->GetFlagInVSync();
			_crtc->Emulate(1);
			frameFinished = _vdu->IsFrameFinished();

			if (!inVSync && _crtc->GetFlagInVSync())
			{
				_cycleCount = 0;
				_chipChangeIndex = 0;
				if (_emulation != NULL)
				{
					_emulation->VSyncUpdate();
				}
			}
			if (_cycleCount % 64 == 0 && _emulation != NULL)
			{
				_emulation->ScanlineUpdate(_cycleCount >> 6);
			}
			if (_emulation != NULL)
			{
				_emulation->NopUpdate(_cycleCount);
			}

			_cycleCount++;

		} while(!frameFinished);
		
		_frameCount++;

		if (_displayFPS)
		{
			char chStr[32];
			sprintf(chStr, "%3dFPS %3d%%", _FPS, _FPS * 100 / 50);
			_renderer.AddTextLocate(0, 0, chStr);
		}

		// emulation finished rendering a complete frame?
		_renderer.EndDisplay(frameFinished);
	}

	return true;
}

void CCPCVideo::AddCRTCRegisterChange(int line, int nop, unsigned int reg, unsigned int value)
{
	unsigned int cycle = line * 64 + nop;
	_chipChangeArray.push_back( ChipChangeKey(cycle, CRTCChange(_crtc, reg, value)) );
	sort(_chipChangeArray.begin(), _chipChangeArray.end(), less<ChipChangeKey>());
}
void CCPCVideo::AddGAColorChange(int line, int nop, unsigned int pen, unsigned int value)
{
	unsigned int cycle = line * 64 + nop;
	_chipChangeArray.push_back( ChipChangeKey(cycle, GateArrayChange(_gateArray, pen, value)) );
	sort(_chipChangeArray.begin(), _chipChangeArray.end(), less<ChipChangeKey>());
}
void CCPCVideo::AddGAModeChange(int line, int nop, unsigned int mode)
{
	unsigned int cycle = line * 64 + nop;
	_chipChangeArray.push_back( ChipChangeKey(cycle, GateArrayModeChange(_gateArray, mode)) );
	sort(_chipChangeArray.begin(), _chipChangeArray.end(), less<ChipChangeKey>());
}

void CCPCVideo::RemoveChipChange(int line, int nop)
{
	unsigned int cycle = line * 64 + nop;
	ChipChangeKey key;
	key.Key = cycle;
	vector<ChipChangeKey>::iterator it = find(_chipChangeArray.begin(), _chipChangeArray.end(), key);
	if (it != _chipChangeArray.end())
	{
		_chipChangeArray.erase(it);
	}
}

void CCPCVideo::ClearChipChange()
{
	_chipChangeArray.clear();
}

void CCPCVideo::SetCRTCValue(unsigned int reg, unsigned int val)
{
	_crtc->SetRegisterValue(reg, val);
}
unsigned int CCPCVideo::GetCRTCValue(unsigned int reg) const
{
	return _crtc->GetRegisterValue(reg);
}

void CCPCVideo::ManageChip()
{
	if (_chipChangeArray.empty())
		return;
	if (_chipChangeArray[_chipChangeIndex].Key != _cycleCount)
		return;

	while (_chipChangeArray[_chipChangeIndex].Key == _cycleCount && _chipChangeIndex < _chipChangeArray.size())
	{
		_chipChangeArray[_chipChangeIndex].Change->Execute();
		_chipChangeIndex++;
	}

	if (_chipChangeIndex == _chipChangeArray.size())
		_chipChangeIndex = 0;
}

void CCPCVideo::Cls()
{
	memset(_cpcMemory, 0, 64*1024);
}

//
// Fonctions gestion de palette
//
void CCPCVideo::SetMode(const unsigned int mode)
{
	_gateArray->SetMode(mode);
}
unsigned int CCPCVideo::GetMode() const
{
	return _gateArray->GetMode();
}
void CCPCVideo::SetColorTable(const unsigned int *color, const unsigned int nbColor, bool hard)
{
	for (int i=0;i<nbColor;i++)
	{
		SetInk(i,color[i], hard);
	}
}
void CCPCVideo::SetInk(const unsigned int i_ink,const unsigned int i_color, bool hard)
{
	if (hard)
	{
		_gateArray->SetInk(i_ink, i_color);
	}
	else
	{
		_gateArray->SetInk(i_ink, t_GateArray::SoftToHardTable[i_color]);
	}
}
unsigned int CCPCVideo::GetInk(const unsigned int i_ink, bool hard)
{
	if (hard)
		return _gateArray->GetInk(i_ink);
	else
		return t_GateArray::HardToSoftTable[_gateArray->GetInk(i_ink)];
}
void CCPCVideo::DisplayColorTable(ostream &io_o, bool hard)
{
	for (int i=0;i<16;i++)
	{
		io_o << GetInk(i, hard) << ",";
	}
	io_o << endl;
}
void CCPCVideo::SwapInkColorTable(unsigned int i_ink1, unsigned int i_ink2)
{
	unsigned int c1 = GetInk(i_ink1);
	unsigned int c2 = GetInk(i_ink2);
	SetInk(i_ink1, c2);
	SetInk(i_ink2, c1);
}

//
// Fonctions Plot/Get pixel, coordonnées CPC (fonction du mode)
//
unsigned int CCPCVideo::GetPixelAddress(const int i_posX, const int i_posY) const
{
	unsigned char r12 = _crtc->GetRegisterValue(12);
	unsigned char r13 = _crtc->GetRegisterValue(13);

	unsigned int pageBase = ((r12 & 0x30) << 10);
	unsigned int addrBase = (((r12 << 8) + r13) & 0x3ff) << 1;
	unsigned int addrMask = ((r12 & 0x0c) == 0x0c) ? 0x7fff : 0x3fff;

	unsigned char r1 = _crtc->GetRegisterValue(1);
	unsigned char charNbLine = _crtc->GetRegisterValue(9)+1;
	unsigned char r6 = _crtc->GetRegisterValue(6);

	unsigned int lineAddr = ((i_posY - (i_posY % charNbLine)) / charNbLine) * r1 * 2 + ((i_posY % charNbLine) * 0x800);
	lineAddr += addrBase;

	unsigned int modeDec = _gateArray->GetMode() + 1;

	unsigned int finalAddr = (((lineAddr & addrMask) + (i_posX >> modeDec)) & addrMask) + pageBase;
	finalAddr = finalAddr & 0xffff;

	return finalAddr;
}

unsigned int CCPCVideo::GetPixel(const int i_posX, const int i_posY) const
{
	unsigned int adr = GetPixelAddress(i_posX, i_posY);
	unsigned int ink = 0;

	switch(_gateArray->GetMode())
	{
	case 0:
		{
			unsigned char mask = 0xAA >> (i_posX & 1);
			unsigned char colorDec = 1 - (i_posX & 1);
			unsigned char color = (_cpcMemory[adr] & mask) >> colorDec;
			ink = (((color & (1 << 6)) >> 6) << 0)
				+ (((color & (1 << 4)) >> 4) << 2)
				+ (((color & (1 << 2)) >> 2) << 1)
				+ (((color & (1 << 0)) >> 0) << 3);
			break;
		}
	case 1:
		{
			unsigned char mask = 0x88 >> (i_posX & 3);
			unsigned char colorDec = 3 - (i_posX & 3);
			unsigned char color = (_cpcMemory[adr] & mask) >> colorDec;
			ink = (((color & (1 << 4)) >> 4) << 0)
				+ (((color & (1 << 0)) >> 0) << 1);
			break;
		}
	case 2:
		{
			int i=i_posX & 7;
			unsigned char mask = 1 << (7-i);
			ink = (_cpcMemory[adr] & mask) >> (7-i);
			break;
		}
	default:{TOOLS_ERRORMSG("Unknown video mode");break;}
	}

	return ink;
}

void CCPCVideo::PlotPixel(const unsigned int i_ink, const int i_posX, const int i_posY)
{
	unsigned int adr = GetPixelAddress(i_posX, i_posY);

	switch(_gateArray->GetMode())
	{
	case 0:
		{
			unsigned char color = 
				  (((i_ink & (1 << 0)) >> 0) << 7)
				+ (((i_ink & (1 << 1)) >> 1) << 3) 
				+ (((i_ink & (1 << 2)) >> 2) << 5) 
				+ (((i_ink & (1 << 3)) >> 3) << 1);
			unsigned char colorDec = (i_posX & 1);
			unsigned char mask = 0xff - (0xAA >> (i_posX & 1));
			
			_cpcMemory[adr] = _cpcMemory[adr] & mask | (color >> colorDec);
			break;
		}
	case 1:
		{
			unsigned char color = 
				  (((i_ink & (1 << 0)) >> 0) << 7) 
				+ (((i_ink & (1 << 1)) >> 1) << 3);

			unsigned char colorDec = (i_posX & 3);
			unsigned char mask = 0xff - (0x88 >> (i_posX & 3));
			
			_cpcMemory[adr] = _cpcMemory[adr] & mask | (color >> colorDec);
			break;
		}
	case 2:
		{
			int i=i_posX % 8;
			unsigned char mask = 0xff-(1 << (7-i));
			unsigned char color = ((i_ink & 1) << (7-i));
			_cpcMemory[adr] = _cpcMemory[adr] & mask | color;
			break;
		}
	default:{TOOLS_ERRORMSG("Unknown video mode");break;}
	}
}

//
// Fonctions affichage divers
//
void CCPCVideo::DrawLine(const unsigned int i_ink, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2)
{
	int yS,yD,xS,xD;
	if (i_y1 < i_y2)
	{
		yS = i_y1;xS = i_x1;
		yD = i_y2;xD = i_x2;
	}
	else
	{
		yS = i_y2;xS = i_x2;
		yD = i_y1;xD = i_x1;
	}

	if (yS != yD)
	{
		float iX = float(xD-xS)/float(yD-yS);
		float x = xS;
		for (int y=yS;y<yD;y++)
		{
			int nX = (int) round(x);
			x+=iX;
			if (iX < 0.0f)
			{
				if (y == yD-1)
					x = xD-1;
				while (nX > round(x))
				{
					PlotPixel(i_ink,nX,y);
					nX--;
				}
			}
			else
			{
				if (y == yD-1)
					x = xD+1;
				while (nX < round(x))
				{
					PlotPixel(i_ink,nX,y);
					nX++;
				}
			}
		}
	}
	else
	{
		if (xS>xD)
		{
			int t = xD;
			xD = xS;
			xS = t;
		}
		for (int x=xS;x<xD;x++)
		{
			PlotPixel(i_ink,round((float)x),yS);
		}
	}
}
void CCPCVideo::FillWindow(const unsigned int i_ink, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2)
{
	int xS = (i_x1<i_x2) ? i_x1 : i_x2;
	int xD = (i_x1<i_x2) ? i_x2 : i_x1;
	int yS = (i_y1<i_y2) ? i_y1 : i_y2;
	int yD = (i_y1<i_y2) ? i_y2 : i_y1;

	for (int y=yS;y<yD;y++)
	{
		for (int x=xS;x<xD;x++)
		{
			PlotPixel(i_ink, x, y);
		}
	}
}
void CCPCVideo::MoveWindow(const unsigned int i_xDest,const unsigned int i_yDest, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2)
{
	int x,y;
	int i;
	int *window = new int[(i_x2-i_x1)*(i_y2-i_y1)];
	
	i=0;
	for (y=i_y1;y<i_y2;y++)
	{
		for (x=i_x1;x<i_x2;x++)
		{
			window[i++] = GetPixel(x,y);
			PlotPixel(0, x, y);
		}
	}
	i=0;
	for (y=i_yDest;y<(i_yDest+(i_y2-i_y1));y++)
	{
		for (x=i_xDest;x<(i_xDest+(i_x2-i_x1));x++)
		{
			PlotPixel(window[i++],x,y);
		}
	}
	delete[] window;
}
void CCPCVideo::CopyWindow(const unsigned int i_xDest,const unsigned int i_yDest, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2)
{
	int x,y;
	int i;
	int *window = new int[(i_x2-i_x1)*(i_y2-i_y1)];
	
	i=0;
	for (y=i_y1;y<i_y2;y++)
	{
		for (x=i_x1;x<i_x2;x++)
		{
			window[i++] = GetPixel(x,y);
		}
	}
	i=0;
	for (y=i_yDest;y<(i_yDest+(i_y2-i_y1));y++)
	{
		for (x=i_xDest;x<(i_xDest+(i_x2-i_x1));x++)
		{
			PlotPixel(window[i++],x,y);
		}
	}
	delete[] window;
}
void CCPCVideo::SwapInkWindow(unsigned int i_ink1, unsigned int i_ink2, int i_x1,int i_y1,int i_x2,int i_y2)
{
	int xS = (i_x1<i_x2) ? i_x1 : i_x2;
	int xD = (i_x1<i_x2) ? i_x2 : i_x1;
	int yS = (i_y1<i_y2) ? i_y1 : i_y2;
	int yD = (i_y1<i_y2) ? i_y2 : i_y1;

	for (int y=yS;y<yD;y++)
	{
		for (int x=xS;x<xD;x++)
		{
			int ink = GetPixel(x,y);
			if (ink == i_ink1)
				PlotPixel(i_ink2,x,y);
			else
				if (ink == i_ink2)
					PlotPixel(i_ink1,x,y);
		}
	}
}

//
// Fonctions Plot/Get octet, adresse video
//
unsigned char CCPCVideo::GetByte(const unsigned int i_adr) const
{
	return (_cpcMemory[i_adr]);
}
void CCPCVideo::PutByte(const int i_adr, const unsigned char i_byte)
{
	_cpcMemory[i_adr] = i_byte;
}

void CCPCVideo::PutBytes(const int i_adr, const unsigned char *i_byte, const unsigned int i_nbByte)
{
	memcpy(_cpcMemory + i_adr, i_byte, i_nbByte);
}
void CCPCVideo::FillBytes(const int i_adr, const unsigned int i_nbByte, unsigned char value)
{
	memset(_cpcMemory + i_adr, value, i_nbByte);
}
void CCPCVideo::PutWindow(const int i_adr, const unsigned char *i_byte, const unsigned int i_sizeX,const unsigned int i_sizeY)
{
	unsigned int adr = i_adr;
	unsigned char *byte = (unsigned char*)i_byte;
	for (int i=0;i<i_sizeY;i++)
	{
		int nbBytes = ((adr+i_sizeX) < 64*1024) ? i_sizeX : 64*1024-1;
		for (int j=0;j<nbBytes;j++)
		{
			if ((adr+j) >= 0)
				_cpcMemory[adr+j] = byte[j];
		}
		byte += i_sizeX;
		adr = Bc26(adr);
	}
}

unsigned int CCPCVideo::Bc26(const unsigned int i_adr) const
{
	int crtc1 = _crtc->GetRegisterValue(1);
	unsigned int adr = i_adr;
	if ((i_adr + 0x800) >= 0x4000)
	{
		adr = (i_adr % 0x800) + crtc1*2;
	}
	else
	{
		adr = adr+0x800;
	}
	return adr;
}

//
// Fonctions overdraw ecran SDL
//
void CCPCVideo::BeginOverdraw()
{
}
void CCPCVideo::EndOverdraw()
{
}
void CCPCVideo::OverdrawHLine(unsigned int i_y, unsigned int i_x1, unsigned int i_x2, unsigned int i_r,unsigned int i_g,unsigned int i_b)
{
	/*
	int y,x1,x2;
	if (i_x1 < i_x2)
	{
		CPCToWindowCoordinate(i_x1,i_y,x1,y,(CPCSDLPixelPosition)(Left | (i_pos & 0x0030)));
		CPCToWindowCoordinate(i_x2-1,i_y,x2,y,(CPCSDLPixelPosition)(Right | (i_pos & 0x0030)));
	}
	else
	{
		CPCToWindowCoordinate(i_x2,i_y,x1,y,(CPCSDLPixelPosition)(Left | (i_pos & 0x0030)));
		CPCToWindowCoordinate(i_x1-1,i_y,x2,y,(CPCSDLPixelPosition)(Right | (i_pos & 0x0030)));
	}
	if (y >= Height || x1 >= Width || x2 >= Width)
		return;

	Uint32 color = colorRGB(i_r,i_g,i_b);
	
	int x=0;
	while (x<(x2-x1))
	{
		if (((x & 7) >> 2) == 0)
			Pixels[y*Width + x1 + x] = color;
		x++;
	}
	*/
}
void CCPCVideo::OverdrawVLine(unsigned int i_x, unsigned int i_y1, unsigned int i_y2, unsigned int i_r,unsigned int i_g,unsigned int i_b)
{
	/*
	int x,y1,y2;
	if (i_y1 < i_y2)
	{
		CPCToWindowCoordinate(i_x,i_y1,x,y1,(CPCSDLPixelPosition)(Up | (i_pos & 0x0003)));
		CPCToWindowCoordinate(i_x,i_y2-1,x,y2,(CPCSDLPixelPosition)(Down | (i_pos & 0x0003)));
	}
	else
	{
		CPCToWindowCoordinate(i_x,i_y2,x,y1,(CPCSDLPixelPosition)(Up | (i_pos & 0x0003)));
		CPCToWindowCoordinate(i_x,i_y1-1,x,y2,(CPCSDLPixelPosition)(Down | (i_pos & 0x0003)));
	}
	if (y1 >= Height || y2 >= Height || x >= Width)
		return;

	Uint32 color = colorRGB(i_r,i_g,i_b);
	
	int y=0;
	while (y<(y2-y1))
	{
		if (((y & 7) >> 2) == 0)
			Pixels[(y1+y)*Width + x] = color;
		y++;
	}
	*/
}

void CCPCVideo::OverdrawWindow(unsigned i_x1,unsigned int i_y1, unsigned int i_x2, unsigned int i_y2, unsigned int i_r,unsigned int i_g,unsigned int i_b)
{
	/*
	int x,y,x1,x2,y1,y2;

	if (i_x1<(i_x2-1))
	{
		x1=i_x1;
		x2=i_x2-1;
	}
	else
	{
		x1=i_x2;
		x2=i_x1-1;
	}
	if (i_y1<(i_y2-1))
	{
		y1=i_y1;
		y2=i_y2-1;
	}
	else
	{
		y1=i_y2;
		y2=i_y1-1;
	}

	CPCToWindowCoordinate(x1,y1,x1,y1,(CPCSDLPixelPosition)(Up | Left));
	CPCToWindowCoordinate(x2,y2,x2,y2,(CPCSDLPixelPosition)(Down | Right));

	if (y1 >= Height || y2 >= Height || x1 >= Width || x2 >= Width)
		return;

	Uint32 color = colorRGB(i_r,i_g,i_b);

	y=0;
	while (y<(y2-y1))
	{
		if (((y & 7) >> 2) == 0)
			Pixels[(y1+y)*Width + x1] = color;
		y++;
	}
	y=0;
	while (y<(y2-y1))
	{
		if (((y & 7) >> 2) == 0)
			Pixels[(y1+y)*Width + x2] = color;
		y++;
	}
	x=0;
	while (x<(x2-x1))
	{
		if (((x & 7) >> 2) == 0)
			Pixels[y1*Width + x1 + x] = color;
		x++;
	}
	x=0;
	while (x<(x2-x1))
	{
		if (((x & 7) >> 2) == 0)
			Pixels[y2*Width + x1 + x] = color;
		x++;
	}
	*/
}

void CCPCVideo::ConvertGFX(const vector<unsigned char> &inGfx, vector<unsigned char> &outGfx, unsigned int mode, unsigned char decal, unsigned char fillByte)
{
	outGfx.clear();

	int ppb = 2 << mode;
	int pixDecal = decal & (ppb-1);

	int nbByte = (inGfx.size() + pixDecal) >> (mode+1);
	nbByte += (((inGfx.size() + pixDecal) & (ppb - 1)) != 0) ? 1 : 0;

	outGfx.resize(nbByte, fillByte);
	int outGfxAdr = 0;

	for (int p=0 ; p<inGfx.size() ; p++)
	{
		unsigned char ink = inGfx[p];

		switch(mode)
		{
		case 0:
			{
				unsigned char color = 
					  (((ink & (1 << 0)) >> 0) << 7)
					+ (((ink & (1 << 1)) >> 1) << 3) 
					+ (((ink & (1 << 2)) >> 2) << 5) 
					+ (((ink & (1 << 3)) >> 3) << 1);
				unsigned char colorDec = ((p+pixDecal) & 1);
				unsigned char mask = 0xff - (0xAA >> colorDec);
				
				outGfx[outGfxAdr] = outGfx[outGfxAdr] & mask | (color >> colorDec);
				break;
			}
		case 1:
			{
				unsigned char color = 
					  (((ink & (1 << 0)) >> 0) << 7) 
					+ (((ink & (1 << 1)) >> 1) << 3);

				unsigned char colorDec = ((p+pixDecal) & 3);
				unsigned char mask = 0xff - (0x88 >> colorDec);
				
				outGfx[outGfxAdr] = outGfx[outGfxAdr] & mask | (color >> colorDec);
				break;
			}
		case 2:
			{
				int i=(p+pixDecal) % 8;
				unsigned char mask = 0xff-(1 << (7-i));
				unsigned char color = ((ink & 1) << (7-i));
				outGfx[outGfxAdr] = outGfx[outGfxAdr] & mask | color;
				break;
			}
		default:{TOOLS_ERRORMSG("Unknown video mode");break;}
		}

		outGfxAdr += ((p+pixDecal & (ppb-1)) == (ppb-1)) ? 1 : 0;
	}
}

/*
//
// Fonctions internes
//
void CCPCVideo::PlotPixelInVideo(const unsigned int i_ink, const int i_posX, const int i_posY)
{
	Uint32 color = _colorTable[i_ink];
	switch(_videoMode)
	{
	case Mode0:
		{
			int p = i_posY*2*Width + i_posX*4;
			Pixels[p++] = color;
			Pixels[p++] = color;
			Pixels[p++] = color;
			Pixels[p++] = color;
			p+= Width-4;
			Pixels[p++] = color;
			Pixels[p++] = color;
			Pixels[p++] = color;
			Pixels[p] = color;
			break;
		}
	case Mode1:
		{
			int p = i_posY*2*Width + i_posX*2;
			Pixels[p++] = color;
			Pixels[p++] = color;
			p+= Width-2;
			Pixels[p++] = color;
			Pixels[p] = color;
			break;
		}
	case Mode2:
		{
			int p = i_posY*2*Width + i_posX;
			Pixels[p] = color;
			p+= Width;
			Pixels[p] = color;
			break;
		}
	default:{TOOLS_ERRORMSG("Unknown video mode");break;}
	}
}

void CCPCVideo::PlotPixelInMemory(const unsigned int i_ink, const int i_posX, const int i_posY)
{
	switch(_videoMode)
	{
	case Mode0:
		{
			unsigned int adr = (i_posY >> 3)*_CRTCR1*2 
				+ (i_posY % 8) * 0x800 
				+ (i_posX >> 1);
			if (adr < _videoCPCSize)
			{
				unsigned char mask;
				unsigned char color = (((i_ink & (1 << 0)) >> 0) << 7) 
					+ (((i_ink & (1 << 1)) >> 1) << 3) 
					+ (((i_ink & (1 << 2)) >> 2) << 5) 
					+ (((i_ink & (1 << 3)) >> 3) << 1);
				switch ((i_posX % 2))
				{
				case 0 : {mask = 0xff-0xAA;color = color >> 0;break;}
				case 1 : {mask = 0xff-0x55;color = color >> 1;break;}
				}
				_videoCPC[adr] = _videoCPC[adr] & mask | color;
			}
			break;
		}
	case Mode1:
		{
			unsigned int adr = (i_posY >> 3)*_CRTCR1*2 
				+ (i_posY % 8) * 0x800 
				+ (i_posX >> 2);
			if (adr < _videoCPCSize)
			{
				unsigned char mask;
				unsigned char color = (((i_ink & (1 << 0)) >> 0) << 7) 
					+ (((i_ink & (1 << 1)) >> 1) << 3);
				switch ((i_posX % 4))
				{
				case 0 : {mask = 0xff-0x88;color = color >> 0;break;}
				case 1 : {mask = 0xff-0x44;color = color >> 1;break;}
				case 2 : {mask = 0xff-0x22;color = color >> 2;break;}
				case 3 : {mask = 0xff-0x11;color = color >> 3;break;}
				}
				_videoCPC[adr] = _videoCPC[adr] & mask | color;
			}
			break;
		}
	case Mode2:
		{
			unsigned int adr = (i_posY >> 3)*_CRTCR1*2 
				+ (i_posY % 8) * 0x800 
				+ (i_posX >> 3);
			if (adr < _videoCPCSize)
			{
				int i=i_posX % 8;
				unsigned char mask = 0xff-(1 << (7-i));
				unsigned char color = ((i_ink & 1) << (7-i));
				_videoCPC[adr] = _videoCPC[adr] & mask | color;
			}
			break;
		}
	default:{TOOLS_ERRORMSG("Unknown video mode");break;}
	}
}
*/

