#include "CapriceAboutImpl.h"

void CapriceAboutImpl::OnAboutClose( wxCloseEvent& event )
{
   this->Destroy();
}