/*
 * Classe de visualisation de window
 */

#ifndef _CCPCWINDOWVIEW_H_
#define _CCPCWINDOWVIEW_H_

#include "CCPCDataView.h"

#define CPCWINDOW_DEFAULT_WIDTH 80
#define CPCWINDOW_DEFAULT_HEIGHT 200

class CCPCWindowView : public CCPCDataView
{
public:
	CCPCWindowView(unsigned char* data, unsigned int size, unsigned int w=CPCWINDOW_DEFAULT_WIDTH, unsigned int h=CPCWINDOW_DEFAULT_HEIGHT);
	CCPCWindowView(const CCPCDataView &data, unsigned int w=CPCWINDOW_DEFAULT_WIDTH, unsigned int h=CPCWINDOW_DEFAULT_HEIGHT);
	
	virtual bool display(CCPCVideo &scr);
	virtual bool keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay);
protected:

	int _width;
	int _height;
	bool _displayBox;

	void forceWindowSize(CCPCVideo &scr);
};

#endif //_CCPCWINDOWVIEW_H_
