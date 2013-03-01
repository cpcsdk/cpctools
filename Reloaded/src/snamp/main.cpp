/* SNAMP - SNApshot Music Player
 * Copyright 2010, Adrien Destgues <pulkomandy@pulkomandy.ath.cx>
 * Distributed under the terms of the MIT Licence.
 */

#include <iostream>
#include <libgen.h>
#include <memory>

#include "core/emulator.h"
#include "core/video.h"
#include "core/snapshot.h"
#include "aop/portAudioAudioPlugin.h"

using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;

class SNAMPEmulator: public Emulator {
	private:
		void PressKey(uint32_t, uint32_t) {};
		void ReleaseKey(uint32_t, uint32_t) {};

	protected:
		SNAMPEmulator(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio)
			: Emulator(video, audio)
		{
		}

	public:
		~SNAMPEmulator() {};

		static inline SNAMPEmulator* createInstance(shared_ptr<VideoPlugin> video,
			shared_ptr<AudioPlugin> audio)
		{
			instance = new SNAMPEmulator(video, audio);
			return static_cast<SNAMPEmulator*>(instance);
		}

		static inline SNAMPEmulator* getInstance()
		{
			return (SNAMPEmulator*)instance;
		}

		void setCfgPath(char* p) {strcpy(_config_path, p);}
#ifdef USE_PTHREAD
		void waitForEnd() {pthread_join(emuthread, NULL); }
#endif
};

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << argv[0] << " file.sna" << endl;
		return -1;
	}

	cout << "SNAMP - SNApshot Music Player" << endl
		<< "Version 0.2 - Copyright 2010-2013, PulkoMandy and Reloaded team" << endl;

	auto audio = make_shared<PortAudioAudioPlugin>();
	auto video = make_shared<NullVideoPlugin>();

	SNAMPEmulator::createInstance(video, audio);
	SNAMPEmulator* m = SNAMPEmulator::getInstance();

	m->setCfgPath(dirname(argv[0]));
	m->Init();

	if (snapshot_load(*m, argv[1]) != 0) {
		cout << "Error loading snapshot!\n";
		exit(-1);
	}
	else
	{
#ifdef USE_PTHREAD
		m->waitForEnd();
#else
		while(1)
		{
			m->Emulate();
		}
#endif
	}

	delete m;
}
