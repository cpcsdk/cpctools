/*
 * Classe de visualisation de font
 */

#ifndef _CCPCFONTVIEW_H_
#define _CCPCFONTVIEW_H_

#include "CCPCDataView.h"

#define CPCFONT_DEFAULT_WIDTH 2
#define CPCFONT_DEFAULT_HEIGHT 8
#define CPCFONT_DEFAULT_NBCHAR 1

class CCPCFontView : public CCPCDataView
{
public:
	CCPCFontView(unsigned char* data, unsigned int size, unsigned int w=CPCFONT_DEFAULT_WIDTH, unsigned int h=CPCFONT_DEFAULT_HEIGHT, unsigned int nbChar=CPCFONT_DEFAULT_NBCHAR);
	CCPCFontView(const CCPCDataView &data, unsigned int w=CPCFONT_DEFAULT_WIDTH, unsigned int h=CPCFONT_DEFAULT_HEIGHT, unsigned int nbChar=CPCFONT_DEFAULT_NBCHAR);
	
	virtual bool display(CCPCVideo &scr);
	virtual bool keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay);
protected:

	int _charWidth;
	int _charHeight;
	int _nbChar;
	bool _verticalMode;
	bool _displayBox;

	void forceFontSize(CCPCVideo &scr);
};

#endif //_CCPCFONTVIEW_H_
