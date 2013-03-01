#include "crtc.h"

class t_CRTC1: public t_CRTC {
	public:
		t_CRTC1(shared_ptr<t_GateArray> ga, shared_ptr<t_VDU> vdu) :t_CRTC(ga,vdu){};
		unsigned char ReadData() const;

	protected:
		void Emulate(int repeat_count);
		void setReg3(unsigned char val);
		void setReg4(unsigned char val);
		void setReg6(unsigned char val);
		void setReg9(unsigned char val);
		unsigned char ReadStatus() const;
		void setReg12(unsigned char val);
		void setReg13(unsigned char val);
};
