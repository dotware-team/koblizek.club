#pragma once
#include <sstream>
#include <string_view>
#include <Windows.h>
#include <Psapi.h>
#include "../KeyValues.h"
#include "../winapi_own.h"
#include "../cheat_sdk/PlayerSource.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

static uintptr_t FindSignature(const char* szModule, const char* szSignature)
{
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleHidden(szModule);
	MODULEINFO miModInfo;
	GetModuleInformation(GetCurrentProcessHidden(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;
		}
		else
		{
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}

class ClientMode;
class Input;
class ItemSchema;
struct GlobalVars;
struct GlowObjectManager;
struct Vector;
struct Trace;
class Entity;
class ViewRender;
class MoveHelper;
class MoveData;
class CHudChat;
class IViewRenderBeams;
class C_CSPlayerResource;

class Memory final {
public:
	Memory() noexcept;

	uintptr_t present;
	uintptr_t reset;

	ClientMode* clientMode;
	Input* input;
	GlobalVars* globalVars;
	GlowObjectManager* glowObjectManager;

	bool* disablePostProcessing;

	std::add_pointer_t<void __fastcall(const char*) noexcept> loadSky;
	std::add_pointer_t<void __fastcall(const char*, const char*) noexcept> setClanTag;
	int* smokeCount;
	uintptr_t cameraThink;
	std::add_pointer_t<bool __stdcall(const char*) noexcept> acceptMatch;
	std::add_pointer_t<bool __cdecl(Vector, Vector, short) noexcept> lineGoesThroughSmoke;
	int(__thiscall* getSequenceActivity)(void*, int);
	uintptr_t scopeArc;
	uintptr_t scopeLens;
	bool(__thiscall* isOtherEnemy)(Entity*, Entity*);
	uintptr_t hud;
	int*(__thiscall* findHudElement)(uintptr_t, const char*);
	int(__thiscall* clearHudWeapon)(int*, int);
	std::add_pointer_t<ItemSchema*()> itemSchema;
	void(__thiscall* setAbsOrigin)(Entity*, const Vector&);
	std::add_pointer_t<bool __cdecl(float, float, float, float, float, float, Trace&) noexcept> traceToExit;
	ViewRender* viewRender;
	uintptr_t test;
	uintptr_t test2;
	std::add_pointer_t<void __cdecl(const std::array<std::uint8_t, 4>& color, const char* msg, ...)> conColorMsg;
	uint8_t* fakePrime;
	int* predictionRandomSeed;
	MoveData* moveData;
	MoveHelper* movehelper;
	std::uintptr_t keyValuesFromString;
	KeyValues*(__thiscall* keyValuesFindKey)(KeyValues* keyValues, const char* keyName, bool create);
	void(__thiscall* keyValuesSetString)(KeyValues* keyValues, const char* value);
	CHudChat* g_ChatElement;
	IViewRenderBeams* renderBeams;
	C_CSPlayerResource** playerResource;
	uintptr_t unlock_inventory;

private:
	template <typename T = uintptr_t>
	static auto findPattern(const wchar_t* module, const char* pattern, size_t offset = 0) noexcept
	{
		if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcessHidden(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) {
			auto start{ static_cast<const char*>(moduleInfo.lpBaseOfDll) };
			auto end{ start + moduleInfo.SizeOfImage };

			auto first{ start };
			auto second{ pattern };

			while (first < end && *second) {
				if (*first == *second || *second == '?') {
					first++;
					second++;
				}
				else {
					first = ++start;
					second = pattern;
				}
			}

			if (!*second)
				return reinterpret_cast<T>(const_cast<char*>(start) + offset);
		}
		exit(EXIT_FAILURE);
	}
};

extern Memory memory;
