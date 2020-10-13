#pragma once

#include "Utils.h"
#include "../winapi_own.h"

struct ConVar;

struct ColorRGBA {
	ColorRGBA(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha = 255) {
		RGBA[0] = Red;
		RGBA[1] = Green;
		RGBA[2] = Blue;
		RGBA[3] = Alpha;
	}

	unsigned char RGBA[4];
};

class Cvar {
public:
	constexpr auto findVar(const char* name) noexcept
	{
		return callVirtualMethod<ConVar*, const char*>(this, 15, name);
	}

	typedef void(__cdecl* ConMsg)(const char* msg, va_list);
	void Msg(const char* conmsg, ...)
	{
		if (conmsg == nullptr)
			return;
		static ConMsg fn = (ConMsg)GetProcAddress(GetModuleHandleHidden("tier0.dll"), "Msg");
		char buffer[989];
		va_list list;
		va_start(list, conmsg);
		vsprintf(buffer, conmsg, list);
		va_end(list);
		fn(buffer, list);
	}
};