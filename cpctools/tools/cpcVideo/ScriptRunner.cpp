#include "ScriptRunner.h"

ScriptRunner::ScriptRunner(std::string& filename)
	: CCPCDataView()
{
	L = luaL_newstate();
	luaL_openlibs(L);


	// TODO register all the callbacks
	// Hardware
//	lua_register(L, "GetCRTC", LGetCRTC);
	lua_register(L, "SetCRTC", LSetCRTC);
//	lua_register(L, "GetMode", LGetMode);
	lua_register(L, "SetMode", LSetMode);
//	lua_register(L, "GetInk", LGetInk);
	lua_register(L, "SetInk", LSetInk);

	// Memory
	lua_register(L, "Peek", LPeek);
	lua_register(L, "Poke", LPoke);
#if 0
	lua_register(L, "GetMemory",);

	// drawing
	lua_register(L, "GetPixel",);
	lua_register(L, "GetAddress",);
	lua_register(L, "PutPixel",);
	lua_register(L, "DrawLine",);
	lua_register(L, "FillWindow",);
	lua_register(L, "MoveWindow",);
	lua_register(L, "CopyWindow",);
	lua_register(L, "SwapInks",);
	lua_register(L, "BC26",);

	// Overlay
	lua_register(L, "OverdrawHLine",);
	lua_register(L, "OverdrawVLine",);
	lua_register(L, "OverdrawWindow",);
#endif

	luaL_loadfile(L, filename.c_str());	
	lua_setglobal(L, "run");
}


ScriptRunner::~ScriptRunner()
{
	lua_close(L);
}


bool ScriptRunner::display(CCPCVideo& scr)
{
	scr.ClearChipChange();

	// Register the screen inside the lua state, for use in the callback methods
	// This is not so nice, as we don't need it on lua side, but need to link it
	// to the state somehow, so we don't have to handle it in lua code.
	lua_pushlightuserdata(L, &scr);
	lua_setglobal(L, "CCPCVideo");

	// Put the function to run back on the stack
	lua_getglobal(L, "run");
	lua_call(L, 0, 0);
	return true;
}


bool ScriptRunner::keyPressed(CCPCVideo& scr, const SDLKey& key,
	const SDLMod& mod, bool& redisplay)
{
	return false;
}


int ScriptRunner::LSetCRTC(lua_State* L)
{
	CCPCVideo* scr = getInstance(L);

	int n = lua_gettop(L);

	int reg = lua_tointeger(L, 1);
	int value = lua_tointeger(L, 2);

	if (n > 2)
	{
		int line = lua_tointeger(L, 3);
		int nop = lua_tointeger(L, 4);
		scr->AddCRTCRegisterChange(line, nop, reg, value);
	} else {
		scr->SetCRTCValue(reg, value);	
	}

	return 0;
}


int ScriptRunner::LSetMode(lua_State* L)
{
	CCPCVideo* scr = getInstance(L);

	int n = lua_gettop(L);

	int mode = lua_tointeger(L, 1);

	if (n > 1)
	{
		int line = lua_tointeger(L, 2);
		int nop = lua_tointeger(L, 3);
		scr->AddGAModeChange(line, nop, mode);
	} else {
		scr->SetMode(mode);	
	}

	return 0;
}


int ScriptRunner::LSetInk(lua_State* L)
{
	CCPCVideo* scr = getInstance(L);

	int n = lua_gettop(L);

	int ink = lua_tointeger(L, 1);
	int color = lua_tointeger(L, 2);

	if (n > 2)
	{
		int line = lua_tointeger(L, 3);
		int nop = lua_tointeger(L, 4);
		scr->AddGAColorChange(line, nop, ink, color);
	} else {
		scr->SetInk(ink, color);	
	}

	return 0;
}


int ScriptRunner::LPeek(lua_State* L)
{
	CCPCVideo* scr = getInstance(L);

	int address = lua_tointeger(L, 1);
	int value = scr->GetByte(address);

	lua_pushinteger(L, value);
	return 1;
}


int ScriptRunner::LPoke(lua_State* L)
{
	CCPCVideo* scr = getInstance(L);

	int address, value;

	address = lua_tointeger(L, 1);
	value = lua_tointeger(L, 2);

	scr->PutByte(address, value);

	return 0;
}
