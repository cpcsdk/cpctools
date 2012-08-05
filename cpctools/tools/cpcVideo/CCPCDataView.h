/*
 * Classe de visualisation de data (classe mere)
 */

#ifndef _CCPCDATAVIEW_H_
#define _CCPCDATAVIEW_H_

#include "SDL.h"
#include "CCPCVideo.h"

class CCPCDataView
{
public:
	virtual bool display(CCPCVideo &scr) = 0;
	virtual bool keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay) = 0;

	virtual ~CCPCDataView();
protected:
	/// Constructeur / destructeur
	CCPCDataView();
	CCPCDataView(unsigned char *data, unsigned int size);
	CCPCDataView(const CCPCDataView &data);

	CCPCDataView& operator=(const CCPCDataView &data);

protected:
	unsigned char	*_data;
	unsigned int	_dataSize;
};

#endif //_CCPCDATAVIEW_H_