#ifndef INCLUDED_CAPRICEAPP_H
#define INCLUDED_CAPRICEAPP_H

#include "CapriceWindowImpl.h"
#include "CapriceRegistersDialogImpl.h"
#include "emulator.h"


class CapriceApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int  OnRun();


private:
	CapriceWindowImpl 	*frame ;
	Emulator			*emulator ;

};
 
DECLARE_APP(CapriceApp)
 
#endif // INCLUDED_CAPRICEAPP_H 
