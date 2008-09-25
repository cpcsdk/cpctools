#ifndef _CAPRICEABOUTIMPL_H_
#define _CAPRICEABOUTIMPL_H_

#include "CapriceUI.h"

class CapriceAboutImpl : public CapriceAbout
{
	public:
	CapriceAboutImpl() : CapriceAbout(NULL)
	{

	}

	protected:

	virtual void OnAboutClose( wxCloseEvent& event );

	private:

};
#endif