/*
 * Classe de visualisation de window
 */

#ifndef _CCPCSCREENVIEW_H_
#define _CCPCSCREENVIEW_H_

#include "CCPCDataView.h"

#define CPCSCREEN_DEFAULT_WIDTH 10
#define CPCSCREEN_DEFAULT_HEIGHT 10

class CCPCScreenView : public CCPCDataView
{
public:
	CCPCScreenView(unsigned char* data, unsigned int size);
	CCPCScreenView(const CCPCDataView &data);
	
	virtual bool display(CCPCVideo &scr);
	virtual bool keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay);
protected:
	unsigned int _offset;
};

#endif //_CCPCSCREENVIEW_H_
