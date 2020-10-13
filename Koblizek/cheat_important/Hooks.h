#pragma once

#include <d3d9.h>
#include <type_traits>

#include "Interfaces.h"
#include "Memory.h"
#include "../cheat_sdk/Cvar.h"
#include "MinHook.h"

using HookType = MinHook;

class Hooks final {
public:
	int security_check();
	Hooks() noexcept;
	void restore() noexcept;

	WNDPROC originalWndProc;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> originalPresent;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> originalReset;
	
	HookType client;
	HookType clientMode;
	HookType engine;
	HookType gameEventManager;
	HookType modelRender;
	HookType panel;
	HookType sound;
	HookType surface;
	HookType viewRender;
	HookType unlock_inventory;
	HookType networkChannel;
};

extern Hooks hooks;