/*
 * Viewer de fichier
 */

#include <fstream>

#include "CError.h"
#include "CCPCVideo.h"

#include "CCPCFileManager.h"

#include "CCPCScreenView.h"
#include "CCPCWindowView.h"
#include "CCPCFontView.h"

#include "Cap32/crtc.h"
#include "Cap32/video.h"
#include "Cap32/gatearray.h"
#include "Cap32/vdu.h"
#include "Cap32/render.h"

CCPCDataView *File = NULL;

bool loadScreen(std::string &i_filename)
{
	unsigned char *data;
	unsigned int size;

	COCPFileManager::LoadScreen(i_filename,data,size);

	if (File != NULL)
		delete File;
	File = new CCPCScreenView(data,size);
	delete[] data;
	
	return true;
}

bool loadWindow(std::string &i_filename)
{
	unsigned char *data, *nData;
	unsigned int size,width, widtho, height;

	CCPCFileManager::LoadRawData(i_filename,data,size);
	COCPFileManager::ReadWindow(data,size,nData,width,widtho,height);
	if (File != NULL)
		delete File;
	File = new CCPCWindowView(nData,size,widtho,height);
	
	delete[] data;
	delete[] nData;
	return true;
}

bool loadFont(std::string &i_filename)
{
	unsigned char *data;
	unsigned int size;

	CCPCFileManager::LoadRawData(i_filename,data,size);
	if (File != NULL)
		delete File;
	File = new CCPCFontView(data,size);
	
	delete[] data;
	return true;
}

void loadFile(std::string &filename, CCPCVideo &scr)
{
	std::string fileExt;
	fileExt = filename.substr(filename.find_last_of('.')+1,filename.size()-(filename.find_last_of('.')+1));
	for (int i=0;i<fileExt.size();i++)
		fileExt[i] = toupper(fileExt[i]);

	if (fileExt == std::string("WIN"))
	{
		try
		{
			std::string pal;
			
			pal = filename.substr(0,filename.find_last_of('.')+1)+"PAL";
			std::ifstream fichier( pal.c_str() ); 
    		if (fichier.fail()){
				pal = filename.substr(0,filename.find_last_of('.')+1)+"pal";
			} 

			int mode;
			unsigned int palette[16];
			if (COCPFileManager::LoadColorTable(pal,mode,palette))
			{ 
				scr.SetMode(mode);
				scr.SetColorTable(palette,17, true);
			}
		}
		catch (tools::CException &e)
		{
			CERR(e);
		}
		loadWindow(filename);
		return;
	}
	if (fileExt == std::string("SCR"))
	{
		try
		{
			std::string pal;
			pal = filename.substr(0,filename.find_last_of('.')+1)+"PAL";
            std::ifstream fichier( pal.c_str() );
            if (fichier.fail()){
                pal = filename.substr(0,filename.find_last_of('.')+1)+"pal";
            }



			int mode;
			unsigned int palette[16];
			if (COCPFileManager::LoadColorTable(pal,mode,palette))
			{
				scr.SetMode(mode);
				scr.SetColorTable(palette,17,true);
			}
		}
		catch (tools::CException &e)
		{
			CERR(e);
		}
		loadScreen(filename);
		return;
	}
	
	loadScreen(filename);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s filename\n", argv[0]);
		exit(-2);
	}

	std::string filename = argv[1];

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0) { // initialize SDL
		fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
		exit(-1);
	}

	COUT("cpcFileViewer (c) Ramlaid 2004");

	if (argc != 2)
	{
		CERR("No file to view !");
		return -1;
	}

	try
	{
		std::string filename = argv[1];

		CCPCVideo *scr = new CCPCVideo;

		//TODO put the default color of the CPC
		unsigned int palette[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ;
        scr->SetMode(1);
        scr->SetColorTable(palette,17,true);

		loadFile(filename,*scr);

		bool quit = false;
		while (!quit)
		{
			File->display(*scr);

			SDL_Event event;

			while(SDL_PollEvent(&event))
			{
				switch( event.type )
				{
				case SDL_QUIT:
					{
						quit = true;
						break;
					}
				case SDL_KEYDOWN:
					{
						Uint8 *keys = SDL_GetKeyState(NULL);
						quit = (keys[SDLK_ESCAPE] == SDL_PRESSED);
						break;
					}
				}
			}
		}

		delete scr;
		if (File != NULL)
			delete File;
	}
	catch (tools::CException &e)
	{
		CERR(e);
		exit(-1);
	}
	return 0;
}

			/*
			SDLKey key;
			SDLMod mod;
			if (CSDLVideo::getKeyPressed(key,mod))
			{
				if (!File->keyPressed(*scr,key,mod,redisplay))
				{
					switch(mod & 0xfff)
					{
					case KMOD_NONE:
					{
						switch (key)
						{
						case SDLK_F1:{loadScreen(filename);redisplay = true;break;}
						case SDLK_F2:{loadWindow(filename);redisplay = true;break;}
						case SDLK_F3:{loadFont(filename);redisplay = true;break;}
						case SDLK_F8:
							{
								std::string fileColorTable = filename.substr(0,filename.find_last_of('.')) + ".txt";
								std::ofstream outHardColorTable(fileColorTable.c_str());
								scr->displaySoftColorTable(outHardColorTable);
								scr->displayHardColorTable(outHardColorTable);
								break;
							 };
						case SDLK_0 :{scr->setMode(Mode0);redisplay = true;break;}
						case SDLK_1 :{scr->setMode(Mode1);redisplay = true;break;}
						case SDLK_2 :{scr->setMode(Mode2);redisplay = true;break;}
						default:{break;}
						}
						break;
					}

					case KMOD_RCTRL:
					case KMOD_LCTRL:
						{
							int r1,r6;
							scr->getCRTCValue(r1,r6);
							switch (key)
							{
							case SDLK_UP:
								{
									r6--;
									scr->setCRTCValue(r1,r6);
									redisplay = true;
									break;
								}
							case SDLK_DOWN:
								{
									r6++;
									scr->setCRTCValue(r1,r6);
									redisplay = true;
									break;
								}
							case SDLK_LEFT:
								{
									r1--;
									scr->setCRTCValue(r1,r6);
									redisplay = true;
									break;
								}
							case SDLK_RIGHT:
								{
									r1++;
									scr->setCRTCValue(r1,r6);
									redisplay = true;
									break;
								}
							default:{break;}
							}
							break;
						}
					default:{break;}
					}
				}
			}
			*/
