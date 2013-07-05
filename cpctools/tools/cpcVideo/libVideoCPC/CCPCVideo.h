/**
* @class CCPCVideo
* Classe permettant la gestion d'un ecran CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#ifndef _CCPCVIDEO_H_
#define _CCPCVIDEO_H_

#include <string>
#include "render.h"
#include "ptcVideo.h"

#include <vector>

using namespace std;

class t_VDU;
class t_GateArray;
class t_CRTC;

class CCPCVideo
{
public:
	class ChipChange
	{
	public:
		virtual ~ChipChange() {};

		virtual void Execute() = 0;
		virtual ChipChange* Clone() const = 0;
	};
	class CRTCChange : public ChipChange
	{
	protected:
		t_CRTC			*_crtc;
		unsigned int	_reg;
		unsigned int	_value;
	public:
		CRTCChange(t_CRTC *crtc, unsigned int reg, unsigned int val) : _crtc(crtc), _reg(reg), _value(val) {}
		CRTCChange(const CRTCChange &c) : _crtc(c._crtc), _reg(c._reg), _value(c._value) {}

		virtual void Execute();
		virtual ChipChange* Clone() const;
	};
	class GateArrayChange : public ChipChange
	{
	protected:
		t_GateArray		*_gateArray;
		unsigned int	_ink;
		unsigned int	_value;
	public:
		GateArrayChange(t_GateArray *ga, unsigned int ink, unsigned int val) : _gateArray(ga), _ink(ink), _value(val) {}
		GateArrayChange(const GateArrayChange &ga) : _gateArray(ga._gateArray), _ink(ga._ink), _value(ga._value) {}

		virtual void Execute();
		virtual ChipChange* Clone() const;
	};
	class GateArrayModeChange : public ChipChange
	{
	protected:
		t_GateArray		*_gateArray;
		unsigned int	_mode;
	public:
		GateArrayModeChange(t_GateArray *ga, unsigned int mode) : _gateArray(ga), _mode(mode) {}
		GateArrayModeChange(const GateArrayModeChange &ga) : _gateArray(ga._gateArray), _mode(ga._mode) {}

		virtual void Execute();
		virtual ChipChange* Clone() const;
	};

	class ChipChangeKey
	{
	public:
		unsigned int	Key;
		ChipChange*		Change;
	public:
		ChipChangeKey() : Key(0), Change(NULL) {}
		ChipChangeKey(unsigned int key, const ChipChange &change) : Key(key), Change(change.Clone()) {}
		ChipChangeKey(const ChipChangeKey &key);

		ChipChangeKey& operator=(const ChipChangeKey &key);
		bool operator<(const ChipChangeKey &key) const;
		bool operator==(const ChipChangeKey &key) const;
	};


	class EmulationInterface
	{
	protected:
		CCPCVideo	&_cpcVideo;
	public:
		EmulationInterface(CCPCVideo &video);

		//! Function call on VSync start
		virtual void VSyncUpdate();
		//! Function call on new line
		virtual void ScanlineUpdate(unsigned int line);
		//! Function call on each nop
		virtual void NopUpdate(unsigned int cycle);
	};

protected:
	byte					*_cpcMemory;
	Renderer				_renderer;
	t_VDU					*_vdu;
	t_GateArray				*_gateArray;
	t_CRTC					*_crtc;
	ptcVideo				_plugin;
	unsigned int			_cycleCount;

	unsigned int			_chipChangeIndex;
	vector< ChipChangeKey >	_chipChangeArray;
	EmulationInterface		*_emulation;
	
	//! FPS management
	bool					_limitSpeed;
	bool					_displayFPS;
	uint32_t				_timer;
	uint32_t				_FPSTimer;
	int						_FPS;
	int						_frameCount;

public:
	//! Constructeur modeVideo, R1, R6
	CCPCVideo(int mode = 1);
	//! Destructeur
	virtual ~CCPCVideo();
	
	//! Set window caption
	inline void SetCaption(const char *title, const char *icon) { /*_renderer.SetCaption(title, icon);*/ }

	//! Reset timer
	void InitTimer();
	//! Limit Speed to 50 FPS
	inline void SetLimitSpeed(bool limit)				{ _limitSpeed = limit;			}
	//! Display FPS
	inline void SetDisplayFPS(bool fps)					{ _displayFPS = fps;			}
	//! Set full screen mode
	inline void SetFullScreenMode(bool fs)				{ _renderer.SetFullScreen(fs);	}
	
	//! Display surface
	virtual bool Display(bool pause = false);
	//! Clear all CPC video (64ko)
	void Cls();

	//
	// Return Video
	//
	inline unsigned char* GetCPCMemory() const			{ return _cpcMemory;	}

	//
	// Gate array functions
	//
	void SetMode(const unsigned int mode);
	unsigned int GetMode() const;
	void SetColorTable(const unsigned int *color, const unsigned int nbColor, bool hard = true);
	void SetInk(const unsigned int i_ink,const unsigned int i_color, bool hard = true);
	unsigned int GetInk(const unsigned int i_ink, bool hard = true);
	
	void SwapInkColorTable(unsigned int i_ink1, unsigned int i_ink2);
	void DisplayColorTable(ostream &io_o, bool hard = true);

	//
	// CRTC functions
	//
	void SetCRTCValue(unsigned int reg, unsigned int val);
	unsigned int GetCRTCValue(unsigned int reg) const;

	//
	// CPC Video pixel functions
	// Video size is 160*200, 320*200, 640*200
	// Video structure is based on CRTC config at VSync
	//
	unsigned int GetPixel(const int i_posX, const int i_posY) const;
	unsigned int GetPixelAddress(const int i_posX, const int i_poxY) const;
	void PlotPixel(const unsigned int i_ink, const int i_posX, const int i_posY);

	//
	// CPC Video window functions
	//
	//! Draw line (coord LU-RD)
	void DrawLine(const unsigned int i_ink, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2);
	//! Fill a window (coord LU-RD)
	void FillWindow(const unsigned int i_ink, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2);
	//! Move a window (coord LU-RD)
	void MoveWindow(const unsigned int xDest,const unsigned int ydest, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2);
	//! Copy a window (coord LU-RD)
	void CopyWindow(const unsigned int xDest,const unsigned int ydest, const unsigned int i_x1, const unsigned int i_y1, const unsigned int i_x2, const unsigned int i_y2);
	//! Swap 2 ink on defined window
	void SwapInkWindow(unsigned int i_ink1, unsigned int i_ink2, int i_tx,int i_ty,int i_bx,int i_by);

	//
	// CPC Video byte functions
	//
	//! Return next line
	unsigned int Bc26(const unsigned int i_adr) const;
	unsigned char GetByte(const unsigned int i_adr) const;
	void PutByte(const int i_adr, const unsigned char i_byte);
	void PutBytes(const int i_adr, const unsigned char *i_byte, const unsigned int i_nbByte);
	void PutWindow(const int i_adr, const unsigned char *i_byte, const unsigned int i_sizeX,const unsigned int i_sizeY);
	void FillBytes(const int i_adr, const unsigned int i_nbByte, unsigned char value);

	//
	// Fonctions overdraw ecran SDL (coordonnées CPC !)
	// !!! NEED TO BEGIN/END OVERDRAW !!!
	//
	//! Active l'overdraw
	void BeginOverdraw();
	//! Desactive l'overdraw & display
	void EndOverdraw();
	//! Affiche une ligne horizontale
	void OverdrawHLine(unsigned int i_y, unsigned int i_x1, unsigned int i_x2, unsigned int i_r,unsigned int i_g,unsigned int i_b);
	//! Affiche une ligne verticale
	void OverdrawVLine(unsigned int i_x, unsigned int i_y1, unsigned int i_y2, unsigned int i_r,unsigned int i_g,unsigned int i_b);
	//! Affiche un window
	void OverdrawWindow(unsigned i_x1,unsigned int i_y1, unsigned int i_x2, unsigned int i_y2, unsigned int i_r,unsigned int i_g,unsigned int i_b);

	//
	// Realtime emulation part
	//
	//! Add CRTC register change
	void AddCRTCRegisterChange(int line, int nop, unsigned int reg, unsigned int value);
	//! Add GateArray color change
	void AddGAColorChange(int line, int nop, unsigned int pen, unsigned int value);
	//! Add CRTC register change
	void AddGAModeChange(int line, int nop, unsigned int mode);
	//! Remove chip change
	void RemoveChipChange(int line, int nop);
	//! Clear all chip change
	void ClearChipChange();

	//! Set emulation interface object, will be call on VSync, Scanline & Nop
	inline void SetEmulation(EmulationInterface *emul)	{ _emulation = emul;	}

protected:
	//! Manage chip modification
	void ManageChip();

private:
	//! Copy constructor
	CCPCVideo(const CCPCVideo& i_scr);
	
public:
	//! Convert a gfx defined as col,col to CPC bytes
	static void ConvertGFX(const vector<unsigned char> &inGfx, vector<unsigned char> &outGfx, unsigned int mode, unsigned char pixDecal, unsigned char fillByte);
};

#endif
