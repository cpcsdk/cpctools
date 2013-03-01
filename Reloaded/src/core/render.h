//
// Render function used by CRTC & GateArray
//

#ifndef _RENDER_H_
#define _RENDER_H_
#include "cap32type.h"

#include <cstring>
#include <string>
#include <vector>
#include <exception>
#include <memory>

using std::exception;
using std::string;
using std::vector;
using std::shared_ptr;

class Emulator;
class VideoPlugin;

class RendererException: public exception
{
	virtual const char* what() const throw()
	{
		return "RendererException happened";
	}
};

class Renderer
{
    private:
	// Render function
	class RenderFunction
	{
	    protected:
		//! Array of number of byte to render each time
		byte			*_renderWidth;
		//! Address of prerenderered data
		byte			*_renderData;

		//! back surface
		void		*_backSurface;
		
		//! Renderer surface width
		int				_renderSurfaceWidth;
		//! Renderer surface height
		int				_renderSurfaceHeight;
		//! Renderer surface pitch
		int				_renderSurfacePitch;
		//! Renderer surface bpp
		int				_renderSurfaceBPP;

		//! Screen Position (pointer to SDL surface)
		unsigned int	*_scrPos;

		//! Real palette index 
		//! (include index to antialiasing color for displaying)
		unsigned int	_palette[20];

	    public:
		RenderFunction() : 
			_renderWidth(0),
			_renderData(NULL),
			_backSurface(NULL),
			_scrPos(NULL)
		{
		    for( int i = 0; i < 20; i++)
		    {
			_palette[i] = 0;
		    }
		}

		virtual ~RenderFunction() { }

		inline void SetRenderSurfaceWidth(int width) { _renderSurfaceWidth = width; }
		inline void SetRenderSurfaceHeight(int height) { _renderSurfaceHeight = height; }

		inline int GetRenderSurfaceWidth() const {return _renderSurfaceWidth;}
		inline int GetRenderSurfaceHeight() const {return _renderSurfaceHeight;}

		inline int GetRenderSurfacePitch() const {return _renderSurfacePitch;}
		inline void SetRenderSurfacePitch(int pitch) {_renderSurfacePitch = pitch;}

		inline int GetRenderSurfaceBPP() const {return _renderSurfaceBPP;}
		inline void SetRenderSurfaceBPP(int BPP) {_renderSurfaceBPP = BPP;}

		inline unsigned int MapRGB(const ColorARGB8888 &colour)
		{
			unsigned int palColour = 0;
			switch(_renderSurfaceBPP)
			{
			case 32:
				palColour = (colour.r << 16) + (colour.g << 8) + (colour.b);
				break;
			case 24:
				palColour = (colour.b << 16) + (colour.g << 8) + (colour.r);
				break;
			case 16:
				palColour = (colour.r & 0xF8) << 8;
				palColour |= (colour.g & 0xFC) << 3;
				palColour |= (colour.b & 0xF8) >> 3;
				break;
			case 15:
				palColour = (colour.r & 0xF8) << 7;
				palColour |= (colour.g & 0xF8) << 2;
				palColour |= (colour.b & 0xF8) >> 3;
				break;
			default:
				break;
			}
			return palColour;
		}

		inline void SetBackSurface(void *surface) { _backSurface = surface; }
		inline void * GetBackSurface() const { return _backSurface; }

		void SetPalette(unsigned int pen, const ColorARGB8888 &colour);
		void SetAntiAliasingColour(const ColorARGB8888 &colour);

		virtual void PlotPixel(int x, int y, const ColorARGB8888 &colour) = 0;

		inline unsigned int GetScreenPosition() const
		{
		    //return _scrPos - (dword *)_backSurface->pixels;
			return _scrPos - (dword *)_backSurface;
		}
		inline void SetScreenPosition(unsigned int v)
		{
		    //_scrPos = (dword *)_backSurface->pixels + v;
			_scrPos = (dword *)_backSurface + v;
		}

		virtual void Render() = 0;
		inline byte* GetRenderWidth() const { return _renderWidth; }
		inline void SetRenderWidth(byte* v) { _renderWidth = v; }
		// inline byte* GetRenderData() const { return _renderData; }
		inline void SetRenderData(byte* v) { _renderData = v; }
	};

        class Render32BppFunction : public RenderFunction
        {
			friend class RenderFunction;
            virtual ~Render32BppFunction() { }

            public:
                virtual void Render();
                virtual void PlotPixel(int x, int y, const ColorARGB8888 &colour);
        };
        class Render24BppFunction : public RenderFunction
        {
			friend class RenderFunction;
            virtual ~Render24BppFunction() { }

            public:
                virtual void Render();
                virtual void PlotPixel(int x, int y, const ColorARGB8888 &colour);
        };
        class Render16BppFunction : public RenderFunction
        {
			friend class RenderFunction;
            virtual ~Render16BppFunction() { }

            public:
                virtual void Render();
                virtual void PlotPixel(int x, int y, const ColorARGB8888 &colour);
        };
        class Render8BppFunction : public RenderFunction
        {
			friend class RenderFunction;
            virtual ~Render8BppFunction() { }

            public:
                virtual void Render();
                virtual void PlotPixel(int x, int y, const ColorARGB8888 &colour);
        };
		class Render0BppFunction : public RenderFunction
		{
			friend class RenderFunction;
			~Render0BppFunction() {};

			public:
				void Render() {};
				void PlotPixel(int x, int y, const ColorARGB8888 &colour) {};
		};

	class PreRenderStandardFunction
    {
	    protected:
		//! Prerender buffer pointer
		dword			*_renderPos;
		//! Current mode map (conversion from CPC memory to SDL memory)
		const uint64_t	*_modeMap;
		//! CPC memory
		byte			*_memory;
		//! Current CPC mode
		unsigned int	_mode;
	public:
		inline PreRenderStandardFunction() : _renderPos(NULL), _modeMap(NULL), _memory(NULL), _mode(0) {}
	    ~PreRenderStandardFunction() {}

		inline void SetMode(unsigned int m)	{ _mode = m; UpdateMode();	}

		inline dword* GetRenderPos() const	{ return _renderPos;		}
		inline void SetRenderPos(dword* v)	{ _renderPos = v;			}

		inline void SetMemory(byte *memory)	{ _memory = memory;			}

		void PreRenderSync(unsigned int);
		void PreRenderBorder(unsigned int);
		void PreRenderNormal(unsigned int);

	private:
	    static const uint64_t M0Map[0x100];
	    static const uint64_t M1Map[0x100];
	    static const uint64_t M2Map[0x100];
	    static const uint64_t M3Map[0x100];
	protected:
	    void UpdateMode();
    };


	//! Text display class
	class TextDisplay
	{
	    public:
		int		PosX;
		int		PosY;
		string	Text;
		bool	Shadow;
	    public:
		TextDisplay(int x, int y, const string &text, bool shw) : PosX(x), PosY(y), Text(text), Shadow(shw) {}
	};

    private:
        static const double ColoursRGB[32][3];
        static double ColoursPersonal[32][3];
        static const double ColoursHiFi[32][3];
        static const double ColoursGrey[32];
        static const double ColoursGreen[32];
        static const byte Font[768];

		//! Position to char index number of shift
		static const byte	_pos2CharShift = 4;

    private:
	//! Render buffer to be fill by preRenderer
	byte				_renderBuffer[800];
	//!	Start address for preRenderer
	dword				*_renderStart;

	bool _scrFullScreen;
		// TODO move this to the video plug-in, it's not our problem

	//! Current horizontal position (char * 256)
	//! Used to handle monitor displaying
	int					_horizontalPosition;
	//! Current horizontal char index
	byte				_horizontalCurrentChar;
	//! Number maximum of char displayed in horizontal
	byte				_horizontalCharMax;
	//! Horizontal pixel width array
	byte				_horizontalPixelWidth[49];

	//! Render to SDL surface function
	RenderFunction		*_renderFunc;
	//! Current preRender function
	PreRenderStandardFunction	_preRenderFunc;

	//! Current CPC Palette
	unsigned int		_palette[20];

	// Output surface properties
	//! Output surface palette
	ColorARGB8888		_colours[32];

	//! Video plugin used for rendering
	shared_ptr<VideoPlugin>			_videoPlugin;

	//! Current screen line offset (in DWord)
	unsigned int		_scrLineOffset;
	unsigned int		_scrPitch;
	//! Current screen base rendering
	unsigned int		_scrBase;
	//! Current screen position
	unsigned int		_scrPos;

	//! Use OpenGL filter for video
	bool				_videoPluginOpenGLFilter;

	//! Monitor intensity
	unsigned int		_monitorIntensity;
	//! Enable monitor remanency
	bool				_monitorRemanency;
	
	//! Enable display FPS counter in OSR
	bool _displayFPS;

	//! Display text array
	vector<TextDisplay>	_textArray;
    public:
    enum MonitorMode
    {
        GreenMode,
        GreyMode,
        ColoursMode,
        ColoursHiFiMode,
        PersonalMode,
    };

	void InitPalette();

	//! Constructor
	Renderer(int bpp);
	//! Destructor
	~Renderer();

	void * GetBackSurface();
	inline shared_ptr<VideoPlugin> GetVideoPlugin() const { return _videoPlugin; }

	void SetMemory(byte *memory);
	void SetOpenGLFilter(bool val);
	void SetMonitor(MonitorMode mode, unsigned int intensity, bool remanency);
	
	inline void DisplayFPS(bool fps) { _displayFPS = fps;}

	//! Set monitor intensity
	inline void SetMonitorIntensity(unsigned int intensity)	{ _monitorIntensity = intensity; InitPalette(); }
	//! Get monitor intensity
	inline unsigned int GetMonitorIntensity() const	{ return _monitorIntensity ;}
	//! Set monitor color mode
	void SetMonitorColorTube(MonitorMode mode)	{ _monitorMode = mode; InitPalette(); }
	//! Return true if monitor is color
    bool IsMonitorColorTube() const
    {
        if(_monitorMode == ColoursMode)
        {
            return true;
        }
        else return false;
    }

	bool SetFullScreen(bool fs);
	bool ToggleFullScreen();

	int Init(shared_ptr<VideoPlugin>);

	bool BeginDisplay(int screenLine);
	void EndDisplay(bool frameCompleted);

	void Reset();

	// Renderer emulation

	//! Set CPC Mode (update ModeMap pointer)
	void SetMode(unsigned int mode);
	//! Set CPC Ink
	void SetPalette(unsigned int pen, unsigned int ga_colour);
	//! Set CPC antialiasing color
	void SetAntiAliasingColour(unsigned int col0, unsigned int col1);

	/* RenderFunc accessors */
	inline int mapGAEntryToRGB(char pal)
	{
		return _renderFunc->MapRGB(_colours[(int)pal]);
	}

	inline int GetScreenPosition() { return _renderFunc->GetScreenPosition();}


	//! Render CPC memory
	void Render(unsigned int memAddr, dword flags);
	//! HSync reach
	void HSyncCycle(int horzPos, unsigned int flag_drawing);
	//! Update CPC horizontal position
	inline int UpdateHorzPos()
	{
	    _horizontalPosition += 0x100;
	    _horizontalCurrentChar++;

	    return _horizontalPosition;
	}

	// Pixel rendering
	//! Plot a pixel in video
	void PlotPixel(int x, int y, int r, int g, int b);

	//! Add text with locate coord
	void AddTextLocate(int x, int y, const string &text, bool shadow = true);
	//! Add text with pixel coord
	void AddText(int x, int y, const string &text, bool shadow = true);
    private:
	//! Print with pixel position
	void Print(int x, int y, const string &text, bool shadow = true);

	void SetPreRender(dword flags);

	void Shutdown();

	//! Color monitor mode
    MonitorMode _monitorMode;
};

#endif
