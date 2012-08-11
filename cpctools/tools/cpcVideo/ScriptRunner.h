#include "CCPCDataView.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


class ScriptRunner: public CCPCDataView
{
	public:
		ScriptRunner(std::string& filename);
		~ScriptRunner();

		bool display(CCPCVideo& scr);
		bool keyPressed(CCPCVideo& scr, const SDLKey& key, const SDLMod& mod,
			bool& redisplay);
	private:
		lua_State* L;

		static inline CCPCVideo* getInstance(lua_State* L)
		{
			lua_getglobal(L, "CCPCVideo");
			return (CCPCVideo*)lua_touserdata(L, -1);
		}

		// The Lua callbacks !
		static int LGetCRTC(lua_State* L);
		static int LSetCRTC(lua_State* L);
		static int LGetMode(lua_State* L);
		static int LSetMode(lua_State* L);
		static int LGetInk(lua_State* L);
		static int LSetInk(lua_State* L);

		static int LPeek(lua_State* L);
		static int LPoke(lua_State* L);
		static int LGetMemory(lua_State* L);

		static int LGetPixel(lua_State* L);
		static int LGetAddress(lua_State* L);
		static int LPutPixel(lua_State* L);
		static int LDrawLine(lua_State* L);
		static int LFillWindow(lua_State* L);
		static int LMoveWindow(lua_State* L);
		static int LCopyWindow(lua_State* L);
		static int LSwapInks(lua_State* L);
		static int LBC26(lua_State* L);

		static int LOverdrawHLine(lua_State* L);
		static int LOverdrawVLine(lua_State* L);
		static int LOverdrawWindow(lua_State* L);
};
