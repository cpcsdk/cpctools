#include "crtc.h"

class t_CRTC1: public t_CRTC {
	public:
		t_CRTC1(t_GateArray &ga, t_VDU &vdu) :t_CRTC(ga,vdu){};
		unsigned char ReadData() const;

	protected:
		void setReg3(unsigned char val);
		virtual unsigned char ReadStatus() const;
};
