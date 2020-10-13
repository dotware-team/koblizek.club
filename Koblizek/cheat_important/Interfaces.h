#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <Windows.h>
#include "../CHudChat.h"
#include "../winapi_own.h"
#include "../cheat_sdk/BaseFileSystem.hpp"
#include "../cheat_sdk/iv_debug_overlay.hpp"
#include "../steam_sdk/isteamuser.h"
#include "../steam_sdk/isteamclient.h"
#include "../steam_sdk/isteamhttp.h"
#include "../steam_sdk/isteamgamecoordinator.h"
#include "../steam_sdk/isteamfriends.h"
#include "../steam_sdk/li_fn.hpp"
#include "../steam_sdk/isteamutils.h"

class Client;
class Cvar;
class Engine;
class EngineTrace;
class EntityList;
class GameEventManager;
class GameUI;
class InputSystem;
class Localize;
class MaterialSystem;
class ModelInfo;
class ModelRender;
class Panel;
class RenderView;
class Surface;
class Sound;
class PhysicsSurfaceProps;
class Prediction;
class GameMovement;
class IBaseFileSystem;
class iv_debug_overlay;
class ISteamUser;
class ISteamHTTP;
class ISteamClient;
class ISteamFriends;
class ISteamUtils;

class Interfaces final {
public:
	Interfaces() noexcept;

	Client* client;
	Cvar* cvar;
	Engine* engine;
	EngineTrace* engineTrace;
	EntityList* entityList;
	GameEventManager* gameEventManager;
	GameUI* gameUI;
	InputSystem* inputSystem;
	Localize* localize;
	MaterialSystem* materialSystem;
	ModelInfo* modelInfo;
	ModelRender* modelRender;
	Panel* panel;
	RenderView* renderView;
	Surface* surface;
	Sound* sound;
	PhysicsSurfaceProps* physicsSurfaceProps;
	Prediction* prediction;
	GameMovement* gameMovement;
	IBaseFileSystem* FileSys;
	iv_debug_overlay* debug_overlay;
	ISteamUser* steam_user;
	ISteamHTTP* steam_http;
	ISteamClient* steam_client;
	ISteamFriends* steam_friends;
	ISteamUtils* steam_utils;
private:
	template <typename T>
	static auto find(const wchar_t* module, const char* name) noexcept
	{
		if (HMODULE moduleHandle = GetModuleHandleW(module))
			if (const auto createInterface = reinterpret_cast<std::add_pointer_t<T* (const char* name, int* returnCode)>>(GetProcAddress(moduleHandle, "CreateInterface")))
				if (T* foundInterface = createInterface(name, nullptr))
					return foundInterface;

		std::exit(EXIT_FAILURE);
	}
};

extern Interfaces interfaces;
