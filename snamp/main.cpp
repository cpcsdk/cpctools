/* SNAMP - SNApshot Music Player
 * Copyright 2010, Adrien Destgues <pulkomandy@pulkomandy.ath.cx>
 * Distributed under the terms of the MIT Licence.
 */

#include <iostream>
#include <libgen.h>

#include "..\Reloaded\src\core\emulator.h"
#include "../Reloaded/src/core/snapshot.h"

bool sInitOnce = false;

class SNAMPEmulator: public Emulator {
	private:
		void PressKey(uint32_t, uint32_t) {};
		void ReleaseKey(uint32_t, uint32_t) {};

	public:
		~SNAMPEmulator() {};

		static inline SNAMPEmulator* getInstance()
		{
			if(sInitOnce == false) {
				sInitOnce = true;
				instance = new SNAMPEmulator();
			}
			return (SNAMPEmulator*)instance;
		}

		void setCfgPath(char* p) {strcpy(_config_path, p);}
		void waitForEnd() {pthread_join(emuthread, NULL); }

};

class NullVidPlugin : public VideoPlugin
{
	private:
	NullVidPlugin() 
		: VideoPlugin("No output", ALL, 0)
	{
		surf = NULL;
	}

	void* surf;

	public:
	static VideoPlugin* Create()
	{
		return (new NullVidPlugin());
	}

	virtual void* Init(int w,int h,int bpp,bool fs)
	{
		surf = malloc(w*h*bpp);
		return surf;
	}
	virtual void SetPalette(ColorARGB8888* c) {}
	virtual bool TryLock() {return true;}
	virtual void Unlock() {}
    virtual bool LockOutput() {return true;}
    virtual bool TryLockOutput() {return true;}
    virtual void UnlockOutput() {}
	virtual void Flip() {}
	virtual void Close() {}
    virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~NullVidPlugin() {free(surf);}
};

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << argv[0] << " file.sna\n";
		return -1;
	}

	std::cout << "SNAMP - SNApshot Music Player\n"
	<< "Version 0.1 - Copyright 2010, PulkoMandy\n";

	SNAMPEmulator* m = SNAMPEmulator::getInstance();

	m->setCfgPath(dirname(argv[0]));
	m->setVideoPlugin(&NullVidPlugin::Create);
	m->Init();

	if (snapshot_load(*m, argv[1]) != 0) {
		cout << "Error loading snapshot!\n";
		exit(-1);
	} else
		m->waitForEnd();

	
	delete m;
}
