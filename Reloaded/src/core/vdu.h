//
// Caprice32 VDU emulator
//

#ifndef _VDU_H_
#define _VDU_H_

#include "cap32type.h"

#define MIN_VHOLD 250
#define MAX_VHOLD 380
#define MID_VHOLD 295
#define MIN_VHOLD_RANGE 46
#define MAX_VHOLD_RANGE 74

#define MAX_SYNC_DEC 80
#define MAX_SYNC_INC 80

#define MAX_DRAWN 270 // Max displayed scan line (+1)

class Renderer;

class t_VDU
{
private:
	Renderer & _renderer;

	word MinVSync;
	word MaxVSync;
	int iMonHSPeakPos;
	int iMonHSStartPos;
	int iMonHSEndPos;
	int iMonHSPeakToStart;
	int iMonHSStartToPeak;
	int iMonHSEndToPeak;
	int iMonHSPeakToEnd;
	int MonHSYNC;
	int MonFreeSync;
	int HSyncDuration;
	int MinHSync;
	int MaxHSync;
	int HadP;
	
	int scrln;
	int scanline;
	unsigned int flag_drawing;
	unsigned int frame_completed;
	
public:
	t_VDU(Renderer & render);

	void Reset();
	
	void Render(unsigned int memAddr, dword flags);
	
	void CheckMaxScanlineCount();
	void CheckMinScanlineCount();
	
	void StartHSync();
	void EndHSync();
	
	bool IsFrameFinished();
	void frame_finished(void);
	
	inline int GetScrLn() const		{ return scrln;		}
	
	inline int GetScanline() const	{ return scanline;	}
	void SetScanline(int scanline);
	
	void DisplayDebug();
};

#endif
