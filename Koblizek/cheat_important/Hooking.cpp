#pragma once
#include <intrin.h>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

#include "../cheat_imgui/imgui.h"
#include "../cheat_imgui/imgui_impl_dx9.h"
#include "../cheat_imgui/imgui_impl_win32.h"

#include "../cheat_others/Config.h"
#include "../cheat_important/GUI.h"
#include "../cheat_important/Hooks.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"

#include "../cheat_features/Aimbot.h"
#include "../cheat_features/Backtrack.h"
#include "../cheat_features/Chams.h"
#include "../cheat_features/Glow.h"
#include "../cheat_features/Misc.h"
#include "../cheat_features/SkinChanger.h"
#include "../cheat_features/Visuals.h"

#include "../cheat_sdk/Engine.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/EntityList.h"
#include "../cheat_sdk/GameEvent.h"
#include "../cheat_sdk/GameEventManager.h"
#include "../cheat_sdk/GameUI.h"
#include "../cheat_sdk/InputSystem.h"
#include "../cheat_sdk/ModelRender.h"
#include "../cheat_sdk/Panel.h"
#include "../cheat_sdk/Surface.h"
#include "../cheat_sdk/UserCmd.h"
#include "../cheat_features/icon.h"
#include "../cheat_features/Triggerbot.h"
#include "../cheat_features/EnginePrediction.h"
#include "../cheat_others/font-flags.h"
#include "../cheat_protection/security.h"
#include "../visual_render/model.hpp"
#include "../cheat_minhook/MinHook.h"
#include "../cheat_features/ESP.h"
#include "../cheat_features/EventListener.h"
#include "../cheat_features/VelGraph.h"
#include "../cheat_features/trail.h"
#include "../cheat_protection/Schovat.h"
#include "../steam_sdk/li_fn.hpp"
#include "../obfuscation.hpp"
#include "../winapi_own.h"
#include "../cheat_protection/xor.hpp"
#include "../cheat_sdk/MaterialSystem.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "urlmon.lib")
IDirect3DTexture9 * model_img = nullptr;
IDirect3DTexture9 * chams_img = nullptr;
IDirect3DTexture9 * flat_img = nullptr;

#define ENCRYPT_START OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN
#define ENCRYPT_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END

#define JUNK_CODE_ONE        \
    __asm{push eax}            \
    __asm{xor eax, eax}        \
    __asm{setpo al}            \
    __asm{push edx}            \
    __asm{xor edx, eax}        \
    __asm{sal edx, 2}        \
    __asm{xchg eax, edx}    \
    __asm{pop edx}            \
    __asm{or eax, ecx}        \
    __asm{pop eax}

inline int AddSubOne(int One, int Two)
{
	JUNK_CODE_ONE
		return ( ( One + Two ) - 1 );
}

static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_KEYDOWN && LOWORD(wParam) == config.koblizek_misc.menuKey
		|| ( ( msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK ) && config.koblizek_misc.menuKey == VK_LBUTTON )
		|| ( ( msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK ) && config.koblizek_misc.menuKey == VK_RBUTTON )
		|| ( ( msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK ) && config.koblizek_misc.menuKey == VK_MBUTTON )
		|| ( ( msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK ) && config.koblizek_misc.menuKey == HIWORD(wParam) + 4 )) {
		gui.isOpen = !gui.isOpen;
		if (!gui.isOpen) {
			ImGui::GetIO().MouseDown[ 0 ] = false;
			interfaces.inputSystem->resetInputState();
		}
	}

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (gui.isOpen && msg >= WM_INPUT && !ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	return CallWindowProc(hooks.originalWndProc, window, msg, wParam, lParam);
}

static HRESULT __stdcall present(IDirect3DDevice9 * device, const RECT * src, const RECT * dest, HWND windowOverride, const RGNDATA * dirtyRegion) noexcept
{
	JUNK_CODE_ONE

		ENCRYPT_START

		static bool imguiInit{ ImGui_ImplDX9_Init(device) };

	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DVertexDeclaration9 * vertexDeclaration;
	device->GetVertexDeclaration(&vertexDeclaration);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (model_img == nullptr) //default
		D3DXCreateTextureFromFileInMemoryEx(device, &playermodel, sizeof(playermodel), 114, 200, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &model_img);

	if (chams_img == nullptr) //normal chams
		D3DXCreateTextureFromFileInMemoryEx(device, &playermodel_normalchams, sizeof(playermodel_normalchams), 114, 200, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &chams_img);

	if (flat_img == nullptr) //flat chams
		D3DXCreateTextureFromFileInMemoryEx(device, &playermodel_flatchams, sizeof(playermodel_flatchams), 114, 200, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &flat_img);

	if (gui.isOpen)
	{
		gui.render(device);
	}

	if (config.koblizek_misc.SpectatorList)
	{
		misc::Spectatorlist();
	}
	VelGraph::Get().run_present();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetVertexDeclaration(vertexDeclaration);
	vertexDeclaration->Release();

	JUNK_CODE_ONE

		ENCRYPT_END

		return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static HRESULT __stdcall reset(IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = hooks.originalReset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

void viewmodel_offset_changer()
{
	JUNK_CODE_ONE

		/* STATIC SPOOF */
		auto spoof_minspec = interfaces.cvar->findVar(XorStr("sv_competitive_minspec"));
	spoof_minspec->setValue(0);
	/* SET MINSPEC ON 0, TO ALLOW CHANGING X Y Z */

	/* STATIC & SET THE VALUE FROM CONFIG (FLOAT) */
	auto viewmodel_x = interfaces.cvar->findVar(XorStr("viewmodel_offset_x"));
	viewmodel_x->setValue(config.koblizek_misc.viewmodel_offset_x);
	auto viewmodel_y = interfaces.cvar->findVar(XorStr("viewmodel_offset_y"));
	viewmodel_y->setValue(config.koblizek_misc.viewmodel_offset_y);
	auto viewmodel_z = interfaces.cvar->findVar(XorStr("viewmodel_offset_z"));
	viewmodel_z->setValue(config.koblizek_misc.viewmodel_offset_z);
	/*                    END                     */

	/* FIX THE BOB CYCLE - FIX SHAKY HANDS - FIX VIEWMODEL*/
	interfaces.engine->clientCmdUnrestricted(XorStr("cl_bobcycle 0.98"));
	interfaces.engine->clientCmdUnrestricted(XorStr("viewmodel_fov 68"));
	/*                 END                 */

	JUNK_CODE_ONE
}

static bool __fastcall SendNetMsg(void * networkchannel, void * edx, NetworkMessage & msg, bool bForceReliable, bool bVoice)
{
	JUNK_CODE_ONE

		ENCRYPT_START

		auto original = hooks.networkChannel.getOriginal<bool, NetworkMessage &, bool, bool>(40, msg, bForceReliable, bVoice);

	if (msg.getType() == 14 && config.koblizek_misc.bypass_svpure)
		return false;

	if (msg.getType() == 9)
		bVoice = true;

	return original(networkchannel, msg, bForceReliable, bVoice);

	JUNK_CODE_ONE

		ENCRYPT_END
}

static bool __stdcall createMove(float inputSampleTime, UserCmd * cmd) noexcept
{
	JUNK_CODE_ONE

		ENCRYPT_START

		auto result = hooks.clientMode.callOriginal<bool, 24>(inputSampleTime, cmd);

	if (!cmd->command_number)
		return result;

	uintptr_t * framePointer;
	__asm mov framePointer, ebp;
	bool & sendPacket = *reinterpret_cast< bool * >( *reinterpret_cast< uintptr_t * >( static_cast< char * >( _AddressOfReturnAddress() ) - 0x4 ) - 0x1C );

	static auto previousViewAngles{ cmd->viewangles };
	const auto currentViewAngles{ cmd->viewangles };

	memory.globalVars->serverTime(cmd);
	misc::nadePredict();
	misc::animateClanTag();
	misc::revealRanks(cmd);
	misc::Bunnyhop(cmd);
	misc::forceCrosshair();
	misc::ChatSpam();
	misc::Fullbright();
	misc::aspectratio();
	misc::BlockBot(cmd);
	misc::ShowImpacts();
	misc::fast_stop(cmd);
	misc::InfiniteCrouch(cmd);
	viewmodel_offset_changer();
	misc::PrePredictionEdgeJump(cmd);
	VelGraph::Get().run_cm(cmd);
	Trail::Get().run_cm();
	EnginePrediction::run(cmd);
	Aimbot::run(cmd);
	Triggerbot::run(cmd);
	Backtrack::run(cmd);
	misc::PostPredictionEdgeJump(cmd);
	misc::jump_bug(cmd);
	misc::edge_bug(cmd);
	EnginePrediction::end();

	NetworkChannel * network = interfaces.engine->getNetworkChannel();

	if (!hooks.networkChannel.isInit()) {
		if (network && interfaces.engine->isInGame() && interfaces.engine->isConnected())
		{
			hooks.networkChannel.init(network);
			hooks.networkChannel.hookAt(40, SendNetMsg);
			MH_EnableHook(( *reinterpret_cast< void *** >( network ) )[ 40 ]);
		}
	}

	if (!( cmd->buttons & ( UserCmd::IN_ATTACK | UserCmd::IN_ATTACK2 ) ))
	{
		misc::chokePackets(sendPacket);
	}

	auto viewAnglesDelta{ cmd->viewangles - previousViewAngles };
	viewAnglesDelta.normalize();
	viewAnglesDelta.x = std::clamp(viewAnglesDelta.x, -config.koblizek_misc.maxAngleDelta, config.koblizek_misc.maxAngleDelta);
	viewAnglesDelta.y = std::clamp(viewAnglesDelta.y, -config.koblizek_misc.maxAngleDelta, config.koblizek_misc.maxAngleDelta);

	cmd->viewangles = previousViewAngles + viewAnglesDelta;
	cmd->viewangles.normalize();
	misc::fixMovement(cmd, currentViewAngles.y);

	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);

	previousViewAngles = cmd->viewangles;

	JUNK_CODE_ONE

		ENCRYPT_END

		return false;
}

std::uint8_t * PatternScan(void * module, const char * signature)
{
	static auto pattern_to_byte = [ ](const char * pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast< char * >( pattern );
		auto end = const_cast< char * >( pattern ) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = ( PIMAGE_DOS_HEADER ) module;
	auto ntHeaders = ( PIMAGE_NT_HEADERS ) ( ( std::uint8_t * )module + dosHeader->e_lfanew );

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast< std::uint8_t * >( module );

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[ i + j ] != d[ j ] && d[ j ] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return &scanBytes[ i ];
		}
	}
	return nullptr;
}

bool __stdcall IsConnected()
{
	ENCRYPT_START

		JUNK_CODE_ONE

		auto original = hooks.engine.callOriginal<bool, 27>();

	auto returnAddress = reinterpret_cast< uintptr_t >( _ReturnAddress() );

	if (config.koblizek_misc.unlock_inventory)
	{
		if (returnAddress == memory.unlock_inventory)
		{
			return false;
		}
	}

	return original;

	JUNK_CODE_ONE

		ENCRYPT_END
}

static int __stdcall doPostScreenEffects(int param) noexcept
{
	JUNK_CODE_ONE
		ENCRYPT_START
		if (interfaces.engine->isInGame()) {
			Visuals::thirdperson();
			misc::removepanoramablur();
			misc::ModifySmoke();
			misc::RemoveShadows();
			misc::RemoveBlood();
			misc::RemoveSky();
			Visuals::disablePostProcessing();
			Visuals::removeBlur();
			Visuals::updateBrightness();
			Glow::render();
		}
	return hooks.clientMode.callOriginal<int, 44>(param);
	JUNK_CODE_ONE
		ENCRYPT_END
}

static float __stdcall getViewModelFov() noexcept
{
	ENCRYPT_START

		JUNK_CODE_ONE

		return hooks.clientMode.callOriginal<float, 35>() + config.koblizek_visuals.viewmodelFov;

	JUNK_CODE_ONE

		ENCRYPT_END
}

static void __stdcall drawModelExecute(void * ctx, void * state, const ModelRenderInfo & info, matrix3x4 * customBoneToWorld) noexcept
{
	JUNK_CODE_ONE
		if (interfaces.engine->isInGame())
		{
			Visuals::colorWorld();
			if (Visuals::removeSleeves(info.model->name))
				return;
			const auto isOverridden = interfaces.modelRender->isMaterialOverridden();
			static Chams chams;

			if (chams.render(ctx, state, info, customBoneToWorld))
				hooks.modelRender.callOriginal<void, 21>(ctx, state, std::cref(info), customBoneToWorld);
			if (!isOverridden)
				interfaces.modelRender->forceMaterialOverride(nullptr);
		}
		else
			hooks.modelRender.callOriginal<void, 21>(ctx, state, std::cref(info), customBoneToWorld);
	JUNK_CODE_ONE
}

static void __stdcall paintTraverse(unsigned int panel, bool forceRepaint, bool allowForce) noexcept
{
	JUNK_CODE_ONE
		if (interfaces.panel->getName(panel) == XorStr("MatSystemTopPanel"))
		{
			misc::drawBombTimer();
			misc::VelocityIndicator();
			misc::DrawIndicators();
			misc::watermark();
			Esp::render();
		}
	hooks.panel.callOriginal<void, 41>(panel, forceRepaint, allowForce);
	JUNK_CODE_ONE
}

static void __stdcall frameStageNotify(FrameStage stage) noexcept
{
	JUNK_CODE_ONE
		static auto backtrackInit = ( Backtrack::init(), false );

	if (stage == FrameStage::RENDER_START)
	{
		misc::fakePrime();
	}

	if (interfaces.engine->isInGame())
	{
		misc::fixAnimationLOD(stage);
		SkinChanger::run(stage);
		misc::FPSBoost(stage);
		Visuals::skybox(stage);
		misc::legit_antiaim_resolver();
		Backtrack::update(stage);
	}
	hooks.client.callOriginal<void, 37>(stage);
	JUNK_CODE_ONE
}

struct SoundData
{
	std::byte data[ 12 ];
	const char * soundEntry;
	std::byte pad[ 56 ];
};

static void __stdcall emitSound(SoundData data) noexcept
{
	JUNK_CODE_ONE
		if (config.koblizek_misc.autoAccept && !strcmp(data.soundEntry, XorStr("UIPanorama.popup_accept_match_beep"))) {
			memory.acceptMatch("");
			auto window = FindWindowA(XorStr("Valve001"), NULL);
			FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
			FlashWindowEx(&flash);
			ShowWindow(window, SW_RESTORE);
		}
	hooks.sound.callOriginal<void, 5>(data);
	JUNK_CODE_ONE
}

static bool __stdcall shouldDrawFog() noexcept
{
	return !config.koblizek_visuals.noFog;
}

static void __stdcall lockCursor() noexcept
{
	if (gui.isOpen)
		return interfaces.surface->unlockCursor();
	return hooks.surface.callOriginal<void, 67>();
}

static void __stdcall setDrawColor(int r, int g, int b, int a) noexcept
{
	auto returnAddress = reinterpret_cast< uintptr_t >( _ReturnAddress() );
	if (config.koblizek_visuals.noScopeOverlay && ( returnAddress == memory.scopeArc || returnAddress == memory.scopeLens )) {
		a = 0;
		*memory.disablePostProcessing = true;
	}
	hooks.surface.callOriginal<void, 15>(r, g, b, a);
}

static bool __stdcall fireEventClientSide(GameEvent * event) noexcept
{
	ENCRYPT_START
		JUNK_CODE_ONE
		misc::HitSound(event);
	misc::Hitmarker(event);
	misc::DamageIndicator(event);
	SkinChanger::overrideHudIcon(event);
	return hooks.gameEventManager.callOriginal<bool, 9>(event);
	JUNK_CODE_ONE
		ENCRYPT_END
}

struct ViewSetup
{
	std::byte pad[ 176 ];
	float fov;
};

static void __stdcall overrideView(ViewSetup * setup) noexcept
{
	JUNK_CODE_ONE
		if (interfaces.engine->isInGame()
			&& !interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->getProperty<bool>(XorStr("m_bIsScoped")))
			setup->fov += config.koblizek_visuals.fov;
	hooks.clientMode.callOriginal<void, 18>(setup);
	JUNK_CODE_ONE
}

static int __stdcall render2dEffectsPreHud(int param) noexcept
{
	JUNK_CODE_ONE
		ENCRYPT_START
		misc::Hitmarker();
	return hooks.viewRender.callOriginal<int, 39>(param);
	ENCRYPT_END
		JUNK_CODE_ONE
}

extern void initializeNSkinz();

static void * __stdcall getDemoPlaybackParameters() noexcept
{
	JUNK_CODE_ONE
		auto result = hooks.engine.callOriginal<void *, 218>();
	if (config.koblizek_visuals.overwatch)
	{
		if (uintptr_t returnAddress = uintptr_t(_ReturnAddress()); returnAddress == memory.test || returnAddress == memory.test2)
			return nullptr;
	}
	return result;
	JUNK_CODE_ONE
}

HKEY OpenKey(HKEY hRootKey, LPCSTR strKey)
{
	ENCRYPT_START
		JUNK_CODE_ONE

		HKEY hKey;
	LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS, &hKey);

	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}

	if (nError)
		throw std::system_error(GetLastError(), std::generic_category());

	return hKey;

	JUNK_CODE_ONE
		ENCRYPT_END
}

void SetVal(HKEY hKey, LPCTSTR lpValue, const char * data)
{
	LONG nError = RegSetValueEx(hKey, lpValue, NULL, REG_SZ, ( LPBYTE ) data, strlen(data) * sizeof(char));

	if (nError)
		throw std::system_error(GetLastError(), std::generic_category());
}

std::string GetVal(HKEY hKey, LPCTSTR lpValue)
{
	ENCRYPT_START
		JUNK_CODE_ONE
		DWORD dwType = REG_SZ;
	char value[ 1024 ];
	DWORD value_length = 1024;

	RegQueryValueEx(hKey, lpValue, NULL, &dwType, ( LPBYTE ) &value, &value_length);
	return std::string(value);

	JUNK_CODE_ONE
		ENCRYPT_END
}

std::wstring string_to_wstring(const std::string & str)
{
	JUNK_CODE_ONE
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[ 0 ], ( int ) str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[ 0 ], ( int ) str.size(), &wstrTo[ 0 ], size_needed);
	return wstrTo;
	JUNK_CODE_ONE
}

int Hooks::security_check()
{

	initializeNSkinz();
	ImGui::CreateContext();
	auto window = FindWindowA(XorStr("Valve001"), NULL);
	ImGui_ImplWin32_Init(window);

	/* Set Imgui style on dark color & change some size/style's */
	ImGui::StyleColorsDark();
	ImGuiStyle & style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(20, 20);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowBorderSize = 1.0f;
	style.TabBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8, 8);
	style.ItemInnerSpacing = ImVec2(8, 8);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 0.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 60.0f;
	ImGuiIO & io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	/*                           END                            */

	/* ADD FONT FROM Font.h */
	menufont = io.Fonts->AddFontFromMemoryTTF(bluntmenu, 240456, 13.f);
	titlefont = io.Fonts->AddFontFromMemoryTTF(menufont2, 692616, 13.f);
	tabfont = io.Fonts->AddFontFromMemoryTTF(harrypotter, 9800, 20.f);
	subicon = io.Fonts->AddFontFromMemoryTTF(rawData, 47320, 15.f);
	information_font = io.Fonts->AddFontFromMemoryTTF(menufont2, 692616, 11.f);
	/*         END          */

	originalWndProc = reinterpret_cast< WNDPROC >(
		SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(wndProc))
		);

	originalPresent = **reinterpret_cast< decltype( originalPresent ) ** >( memory.present );
	**reinterpret_cast< decltype( present ) *** >( memory.present ) = present;
	originalReset = **reinterpret_cast< decltype( originalReset ) ** >( memory.reset );
	**reinterpret_cast< decltype( reset ) *** >( memory.reset ) = reset;

	if constexpr (std::is_same_v<HookType, MinHook>)
		MH_Initialize();

	/* INIT HOOKS */
	client.init(interfaces.client);
	clientMode.init(memory.clientMode);
	engine.init(interfaces.engine);
	gameEventManager.init(interfaces.gameEventManager);
	modelRender.init(interfaces.modelRender);
	panel.init(interfaces.panel);
	sound.init(interfaces.sound);
	surface.init(interfaces.surface);
	viewRender.init(memory.viewRender);
	/*     END    */

	/* HOOKIN */
	client.hookAt(37, frameStageNotify);
	clientMode.hookAt(17, shouldDrawFog);
	clientMode.hookAt(18, overrideView);
	clientMode.hookAt(24, createMove);
	clientMode.hookAt(44, doPostScreenEffects);
	clientMode.hookAt(35, getViewModelFov);
	engine.hookAt(218, getDemoPlaybackParameters);
	engine.hookAt(27, IsConnected);
	gameEventManager.hookAt(9, fireEventClientSide);
	modelRender.hookAt(21, drawModelExecute);
	panel.hookAt(41, paintTraverse);
	sound.hookAt(5, emitSound);
	surface.hookAt(15, setDrawColor);
	surface.hookAt(67, lockCursor);
	viewRender.hookAt(39, render2dEffectsPreHud);
	/*   END  */

	/* CHANGE LOBBY COLOR TO GREY */
	interfaces.engine->clientCmdUnrestricted(XorStr("cl_color 255 255 255"));
	interfaces.engine->clientCmdUnrestricted(XorStr("clear"));
	/*            END             */

	/* INITIALIZE EVENT MANAGER (LISTENER) */
	EventManager::Get().init();
	/*                END                  */

	if constexpr (std::is_same_v<HookType, MinHook>)
		MH_EnableHook(MH_ALL_HOOKS);

	return 0;
}

Hooks::Hooks() noexcept
{
	JUNK_CODE_ONE
		security_check();
	JUNK_CODE_ONE
}

void Hooks::restore() noexcept
{
	JUNK_CODE_ONE
		if constexpr (std::is_same_v<HookType, MinHook>) {
			MH_DisableHook(MH_ALL_HOOKS);
			MH_Uninitialize();
		}

	client.restore();
	clientMode.restore();
	engine.restore();
	gameEventManager.restore();
	modelRender.restore();
	panel.restore();
	sound.restore();
	surface.restore();
	viewRender.restore();
	networkChannel.restore();

	netvars.restore();

	**reinterpret_cast< void *** >( memory.present ) = originalPresent;
	**reinterpret_cast< void *** >( memory.reset ) = originalReset;
	JUNK_CODE_ONE
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class lyonrsf
{
public:
	bool wirkmnobgmmlkyv;
	string caesfswnwadtfkw;
	bool qxtfckustwsrkfo;
	bool rbcjnilmmeqs;
	int wnqunoiccrh;
	lyonrsf();
	int uwaaysatuwbkpzv(double rujexlby, string sbeffvonjj);
	int tlklaanljqslim(string xoplkvgtyjxugb, bool shckb, double vczfpcqrcdvol, int kchhuroxxh, bool krljcjxcyau, double ltzjoixiuajdzy, double jknlckpyuzkm, string khelhbimmzs, int rftmzzbmtsa, bool jcvpk);
	double fuvmfceicghxdcaamcl(string vplerax, int zncpmxupghx, string gmxfohvy, bool ebyusrzyhlqca, int qkqvkdh, string lzhfh, int elejai, double tjfzheggnf);
	int yzhoaorsyxezyab();

protected:
	string ftqofbnfyhrsry;
	bool xruwpmmdqnhoft;

	void dykdixusmpcqyhbfzklyo(int vtirtrkjmprenzj, double yapgzdomchbwgpg, int jnfjcmcqxynhywt, int ginsdlo, double bbcflskl, string ijxjpwfzqyvizm, string ofhmkbmueadaep);
	bool sdkinvyxrhfihinaopnn(int ctdcspqd, bool xlvbkofy, bool oztuzjha, int sgezjkus);
	string cazqyhaqwzykuopqfhuo(bool nctycbekbckqisa, bool hoewtrw, double kuryexztujtujj, string iswnqlxujby, string ihqwybehibn);
	string nnqoykbzlliufty(bool aehxybd, bool qwcmhrwqdaz, int ydauqppxy, bool czrzclpywffnkz, string nhsvcjsgf);

private:
	int vcyqk;

	double prkoyirsewzhiohjevwqj(bool yuvzdxvitq, string yvolhmayohcm, bool qncwn, int qswclelxrjuxm, bool icnjd, int khvbhsz, string mwpkqj, bool mukvnmgsgrxw);
	string bcznivseoras(double qhrqroqswjese, int gseepqwkxdmd, bool dtypf, double aacznmwpee);
	void fioahoipqwxvbublvzbjowmcp(string vgihvpgit, int gncrpdmnlmazzqw, int jgdvnjorcx);
	double clrrirgctsj(string tdykjvaulppsc, string hvhwe);
	void ojtizpusicldidsd(double lpnshoaagijbyp, double mmrzmnyiemm, int msawalbgyn, double kzehgweerwwc, string fpuqrgkmvgdcn, bool ocgyu, bool tbyxlfcro, int wbfooykr, string ulgzbwcwmqu);
	int uxsoqtmvtuddulwbddduizden(string seswzxisgzchs, bool sieelmrqmvgkdgc, int fxrutybsdoh, int tywxcw, int uzoztgdok, string hyrdmlltam);
	double hqewcrhqmozzpqvtvk(double qhrloosdrsvgvql, string jiaszgpxjkutf, int rvipyfkhb);
	int pxnzgbchlbthpibajhtbf(string rvpnof, int jvuzymksdmxuqrh, bool ligrycqff);

};


double lyonrsf::prkoyirsewzhiohjevwqj(bool yuvzdxvitq, string yvolhmayohcm, bool qncwn, int qswclelxrjuxm, bool icnjd, int khvbhsz, string mwpkqj, bool mukvnmgsgrxw)
{
	double soskjhtzbsuyzo = 16936;
	double tcmianxi = 11909;
	double omwfmmeu = 10115;
	int vrpweg = 61;
	int nbvrmerd = 2524;
	int ffxdtuqkqwpjdt = 2307;
	if (10115 != 10115) {
		int wfi;
		for (wfi = 4; wfi > 0; wfi--) {
			continue;
		}
	}
	if (2307 != 2307) {
		int iivqvnu;
		for (iivqvnu = 26; iivqvnu > 0; iivqvnu--) {
			continue;
		}
	}
	if (2307 == 2307) {
		int gkzkwpw;
		for (gkzkwpw = 45; gkzkwpw > 0; gkzkwpw--) {
			continue;
		}
	}
	if (2307 != 2307) {
		int pwvr;
		for (pwvr = 93; pwvr > 0; pwvr--) {
			continue;
		}
	}
	if (61 != 61) {
		int lcfah;
		for (lcfah = 7; lcfah > 0; lcfah--) {
			continue;
		}
	}
	return 58389;
}

string lyonrsf::bcznivseoras(double qhrqroqswjese, int gseepqwkxdmd, bool dtypf, double aacznmwpee)
{
	return string("tdipmyzapvrauz");
}

void lyonrsf::fioahoipqwxvbublvzbjowmcp(string vgihvpgit, int gncrpdmnlmazzqw, int jgdvnjorcx)
{
	double qxkhycyzxew = 80854;
	string kdujquwyjq = "o";
	bool schfxwrbjrakma = true;
	int cwctwmllovuw = 1093;
	double jjsxv = 6064;

}

double lyonrsf::clrrirgctsj(string tdykjvaulppsc, string hvhwe)
{
	return 18736;
}

void lyonrsf::ojtizpusicldidsd(double lpnshoaagijbyp, double mmrzmnyiemm, int msawalbgyn, double kzehgweerwwc, string fpuqrgkmvgdcn, bool ocgyu, bool tbyxlfcro, int wbfooykr, string ulgzbwcwmqu)
{
	int awhzshad = 1264;
	bool htlzhb = true;
	double amyys = 77202;
	double dcxivkulkprxmvc = 14308;
	bool mwuhmbdrkx = true;
	int gyhjmpofsfp = 2743;
	if (14308 != 14308) {
		int ni;
		for (ni = 23; ni > 0; ni--) {
			continue;
		}
	}
	if (14308 == 14308) {
		int sktkg;
		for (sktkg = 42; sktkg > 0; sktkg--) {
			continue;
		}
	}
	if (14308 != 14308) {
		int wsmmt;
		for (wsmmt = 79; wsmmt > 0; wsmmt--) {
			continue;
		}
	}
	if (2743 == 2743) {
		int zctfc;
		for (zctfc = 39; zctfc > 0; zctfc--) {
			continue;
		}
	}

}

int lyonrsf::uxsoqtmvtuddulwbddduizden(string seswzxisgzchs, bool sieelmrqmvgkdgc, int fxrutybsdoh, int tywxcw, int uzoztgdok, string hyrdmlltam)
{
	bool aciqkvtfgfvxocw = false;
	double ikdlplcof = 22717;
	int gklmytgn = 3293;
	if (3293 != 3293) {
		int wopsbl;
		for (wopsbl = 74; wopsbl > 0; wopsbl--) {
			continue;
		}
	}
	if (22717 == 22717) {
		int udoqd;
		for (udoqd = 93; udoqd > 0; udoqd--) {
			continue;
		}
	}
	if (3293 == 3293) {
		int vpexj;
		for (vpexj = 74; vpexj > 0; vpexj--) {
			continue;
		}
	}
	return 35698;
}

double lyonrsf::hqewcrhqmozzpqvtvk(double qhrloosdrsvgvql, string jiaszgpxjkutf, int rvipyfkhb)
{
	string ixmrbvv = "rwcqggblqwhurvzhbwqliiosahpbamrln";
	int vvbfoeec = 1039;
	string jhkceoafrsjgp = "ynityhirgdqly";
	double cngdippyvdm = 2766;
	int iyccxjafdcydj = 8280;
	string yphgs = "zzovwvihkftczktkjoghkbkjnaqryctnwfotngtbqlmiohuodtzdsoamfotcovprfoszckklhlbtxiywgopvczglgzvqqgxd";
	string izcbetcevxlyvk = "oqv";
	int lrldi = 1059;
	bool zejyogsf = true;
	bool coanj = false;
	if (false == false) {
		int gatb;
		for (gatb = 15; gatb > 0; gatb--) {
			continue;
		}
	}
	return 50456;
}

int lyonrsf::pxnzgbchlbthpibajhtbf(string rvpnof, int jvuzymksdmxuqrh, bool ligrycqff)
{
	bool yzhqzlkhy = true;
	int gqawfutlbqovbx = 1576;
	bool hslfcutatboln = true;
	double gnicxbsosmzy = 10384;
	string ffqzgjcnqdsupn = "jxxnhdggyycsqrdlqivyetfxmibnufbwttlmmnepdgxrrzbigzrhittqfspdjmbg";
	bool gdaxkqmbyoywqq = false;
	string wjicp = "aknrzsagcjbmjiugptrntdqghrnopnsvxscxwhltxynxfgdmhw";
	string doshzkoi = "dgmvzlhjccarbhgqziuswltdmatjpcmnuuusjkbqvihcbidfcnzaavicxeowvkyzgeslioy";
	if (1576 != 1576) {
		int yczxch;
		for (yczxch = 97; yczxch > 0; yczxch--) {
			continue;
		}
	}
	if (string("aknrzsagcjbmjiugptrntdqghrnopnsvxscxwhltxynxfgdmhw") == string("aknrzsagcjbmjiugptrntdqghrnopnsvxscxwhltxynxfgdmhw")) {
		int nmsyuzr;
		for (nmsyuzr = 10; nmsyuzr > 0; nmsyuzr--) {
			continue;
		}
	}
	if (string("dgmvzlhjccarbhgqziuswltdmatjpcmnuuusjkbqvihcbidfcnzaavicxeowvkyzgeslioy") == string("dgmvzlhjccarbhgqziuswltdmatjpcmnuuusjkbqvihcbidfcnzaavicxeowvkyzgeslioy")) {
		int srgqjhr;
		for (srgqjhr = 35; srgqjhr > 0; srgqjhr--) {
			continue;
		}
	}
	if (true != true) {
		int gwepvtsw;
		for (gwepvtsw = 93; gwepvtsw > 0; gwepvtsw--) {
			continue;
		}
	}
	return 52720;
}

void lyonrsf::dykdixusmpcqyhbfzklyo(int vtirtrkjmprenzj, double yapgzdomchbwgpg, int jnfjcmcqxynhywt, int ginsdlo, double bbcflskl, string ijxjpwfzqyvizm, string ofhmkbmueadaep)
{
	double rfakewjxdqtuoe = 13865;
	bool znhnmizq = true;
	if (true == true) {
		int fxbrb;
		for (fxbrb = 75; fxbrb > 0; fxbrb--) {
			continue;
		}
	}

}

bool lyonrsf::sdkinvyxrhfihinaopnn(int ctdcspqd, bool xlvbkofy, bool oztuzjha, int sgezjkus)
{
	int rjlziygnu = 1029;
	bool fwqpjtey = false;
	string lgqtdmwabms = "qcmnyhovbhwwkeefiyurmevzebuueoqmkcrpppjkxgbmeqsxmwngbrirvyswpeavpsayqbw";
	bool zbyvholqu = false;
	string lqayuinflk = "rixzfaakzenojvhoqqvaaglhtgywglmmckhhel";
	string zrtqrqwefv = "axyheiruuvkblkdbhlsgqjtfguxsjkouwlckinodojkzijdjpjubxxgufk";
	double wwbsvpv = 31180;
	int felwgkfu = 3114;
	double wtdigkl = 12233;
	int irawnul = 334;
	if (false != false) {
		int uehlqgdv;
		for (uehlqgdv = 68; uehlqgdv > 0; uehlqgdv--) {
			continue;
		}
	}
	if (string("rixzfaakzenojvhoqqvaaglhtgywglmmckhhel") != string("rixzfaakzenojvhoqqvaaglhtgywglmmckhhel")) {
		int odk;
		for (odk = 24; odk > 0; odk--) {
			continue;
		}
	}
	if (1029 == 1029) {
		int lw;
		for (lw = 66; lw > 0; lw--) {
			continue;
		}
	}
	if (false != false) {
		int awqv;
		for (awqv = 70; awqv > 0; awqv--) {
			continue;
		}
	}
	return true;
}

string lyonrsf::cazqyhaqwzykuopqfhuo(bool nctycbekbckqisa, bool hoewtrw, double kuryexztujtujj, string iswnqlxujby, string ihqwybehibn)
{
	bool ywvubjzesr = false;
	int xtumiqwwinzcp = 2356;
	int hhpcpyulidren = 3552;
	bool uizblgvzddavdp = false;
	bool nzdokvdmuywnmw = true;
	string jxtcgkk = "hbyjdwfysgxotmzlmyrkaoswqeepxhhcvavsvfd";
	int fptrfluki = 1842;
	double gmowaosifzo = 22576;
	string iqhow = "zbhdcbthshmhqtxigubcvygccyzsbigtwkuryvfqcednseipokzqmabspwpnuwfypbvlmlawqnq";
	if (false != false) {
		int bmu;
		for (bmu = 1; bmu > 0; bmu--) {
			continue;
		}
	}
	if (22576 == 22576) {
		int aeycrklml;
		for (aeycrklml = 94; aeycrklml > 0; aeycrklml--) {
			continue;
		}
	}
	if (3552 == 3552) {
		int lazgyuinn;
		for (lazgyuinn = 7; lazgyuinn > 0; lazgyuinn--) {
			continue;
		}
	}
	if (1842 == 1842) {
		int ybntgqewd;
		for (ybntgqewd = 88; ybntgqewd > 0; ybntgqewd--) {
			continue;
		}
	}
	if (3552 != 3552) {
		int nqs;
		for (nqs = 57; nqs > 0; nqs--) {
			continue;
		}
	}
	return string("btc");
}

string lyonrsf::nnqoykbzlliufty(bool aehxybd, bool qwcmhrwqdaz, int ydauqppxy, bool czrzclpywffnkz, string nhsvcjsgf)
{
	double xudstr = 12337;
	double xwtrtkqdmbbty = 17022;
	int vorzbwd = 5122;
	bool greynw = false;
	double mviuml = 20279;
	string ftupuxgwbbmx = "x";
	double cqhyp = 4460;
	int oekclzladvy = 6817;
	if (12337 == 12337) {
		int brh;
		for (brh = 36; brh > 0; brh--) {
			continue;
		}
	}
	if (5122 != 5122) {
		int ahd;
		for (ahd = 57; ahd > 0; ahd--) {
			continue;
		}
	}
	if (12337 != 12337) {
		int sgyemngwh;
		for (sgyemngwh = 80; sgyemngwh > 0; sgyemngwh--) {
			continue;
		}
	}
	if (5122 == 5122) {
		int tidoe;
		for (tidoe = 37; tidoe > 0; tidoe--) {
			continue;
		}
	}
	if (6817 == 6817) {
		int efvxzriekz;
		for (efvxzriekz = 73; efvxzriekz > 0; efvxzriekz--) {
			continue;
		}
	}
	return string("anbt");
}

int lyonrsf::uwaaysatuwbkpzv(double rujexlby, string sbeffvonjj)
{
	bool mjqgflflhuw = true;
	double vpugat = 36741;
	int senwparbi = 1136;
	int bmsfonu = 5328;
	if (1136 != 1136) {
		int coa;
		for (coa = 67; coa > 0; coa--) {
			continue;
		}
	}
	if (5328 == 5328) {
		int hyfxu;
		for (hyfxu = 10; hyfxu > 0; hyfxu--) {
			continue;
		}
	}
	return 65370;
}

int lyonrsf::tlklaanljqslim(string xoplkvgtyjxugb, bool shckb, double vczfpcqrcdvol, int kchhuroxxh, bool krljcjxcyau, double ltzjoixiuajdzy, double jknlckpyuzkm, string khelhbimmzs, int rftmzzbmtsa, bool jcvpk)
{
	string yxdjmujgqzb = "omgjwxdjkgbexmymkjiqbplsxjbrvjlvevracrzuwmrqpzzrothsvyqabqhioooxxyaaymrhlczivnqumeaxsjkpw";
	double qsmbtxqfnv = 54408;
	int zklhu = 1947;
	double fcnhpr = 77302;
	if (string("omgjwxdjkgbexmymkjiqbplsxjbrvjlvevracrzuwmrqpzzrothsvyqabqhioooxxyaaymrhlczivnqumeaxsjkpw") == string("omgjwxdjkgbexmymkjiqbplsxjbrvjlvevracrzuwmrqpzzrothsvyqabqhioooxxyaaymrhlczivnqumeaxsjkpw")) {
		int mtrbivltlh;
		for (mtrbivltlh = 85; mtrbivltlh > 0; mtrbivltlh--) {
			continue;
		}
	}
	if (string("omgjwxdjkgbexmymkjiqbplsxjbrvjlvevracrzuwmrqpzzrothsvyqabqhioooxxyaaymrhlczivnqumeaxsjkpw") != string("omgjwxdjkgbexmymkjiqbplsxjbrvjlvevracrzuwmrqpzzrothsvyqabqhioooxxyaaymrhlczivnqumeaxsjkpw")) {
		int szifdq;
		for (szifdq = 63; szifdq > 0; szifdq--) {
			continue;
		}
	}
	if (54408 == 54408) {
		int kdritxd;
		for (kdritxd = 16; kdritxd > 0; kdritxd--) {
			continue;
		}
	}
	if (1947 != 1947) {
		int lphnjcp;
		for (lphnjcp = 99; lphnjcp > 0; lphnjcp--) {
			continue;
		}
	}
	if (77302 != 77302) {
		int kvcox;
		for (kvcox = 48; kvcox > 0; kvcox--) {
			continue;
		}
	}
	return 37111;
}

double lyonrsf::fuvmfceicghxdcaamcl(string vplerax, int zncpmxupghx, string gmxfohvy, bool ebyusrzyhlqca, int qkqvkdh, string lzhfh, int elejai, double tjfzheggnf)
{
	string lkhrlnxmd = "vrdgoqparnpiujyucrxmtdzlkzfquinry";
	double dxdbxx = 36881;
	string mtodugtpnzis = "zvvnkycteuxnwspjkvncftrkmacjhupsvxiyujimhqrdehyczmiljlmuyh";
	double ykddbslvqhfmjha = 8975;
	string qdyzllnrumq = "eclytiwhhklasdvmfiebgwab";
	int rlxhzyad = 293;
	int avpqmy = 9242;
	int ekgjgcvlh = 2022;
	string pvzdyatpjqnh = "neoudgls";
	double fkaebinbxhqy = 18304;
	return 98354;
}

int lyonrsf::yzhoaorsyxezyab()
{
	bool veqevglrdxktia = true;
	string ltpsuwkw = "ugddbmrsxmuzantubxioxymbsungavytjvewefvmomqqcatntsdyzmiafkukkmovfcnkhbbywphspoajpabhzppvzouqvclrb";
	int ohuxdfds = 2600;
	double cwluuqbcgmiy = 17803;
	double ocetfusfi = 1139;
	double zzyalnvrutlyi = 23548;
	double xjznsaywbdtbgq = 56167;
	if (23548 != 23548) {
		int flsglyhxx;
		for (flsglyhxx = 81; flsglyhxx > 0; flsglyhxx--) {
			continue;
		}
	}
	if (17803 != 17803) {
		int kzlnrjjcxq;
		for (kzlnrjjcxq = 73; kzlnrjjcxq > 0; kzlnrjjcxq--) {
			continue;
		}
	}
	if (true != true) {
		int sysvc;
		for (sysvc = 56; sysvc > 0; sysvc--) {
			continue;
		}
	}
	return 56054;
}

lyonrsf::lyonrsf()
{
	this->uwaaysatuwbkpzv(60023, string("cuitfo"));
	this->tlklaanljqslim(string("wdfszncwdtulhemvgodfmhjtafgr"), false, 31713, 3785, true, 22337, 8774, string("wxyqywribazzprdlykklscfhamnmormbuefcpcchnlejvvqmryrdbqrxqjlpw"), 5319, true);
	this->fuvmfceicghxdcaamcl(string("gsrjtjjueidkemqkkmwzpjbkfyhdujeuxgessgjsqealpfllrsadgmiceq"), 45, string("lvhundzyidqqfgrxhylnycaqnxqtnzpppracxjrbkaowfrcpdjrpclqreauzroabavxwukmghtudzrwimfmmksf"), false, 5669, string("kpxchebycnzkbihnt"), 1605, 14343);
	this->yzhoaorsyxezyab();
	this->dykdixusmpcqyhbfzklyo(840, 67114, 3266, 1829, 56095, string("zestaruflzmoqwkmuvvquirlnxsopnqxphybxpayklsqibicvwklzqhrodzxwdqkzhmxhtnyrpwlqzitrndleatxkwgljpppdrbo"), string("zfbzljvpaijzhhreopmmogrqvgjzcpoyvpnvwgkdmanoxccommppskyhxiksavjbnourwmjqojqfmfhrlcgzyhkrultab"));
	this->sdkinvyxrhfihinaopnn(2439, true, false, 4091);
	this->cazqyhaqwzykuopqfhuo(true, false, 3659, string("ztrxomqjwizptzomuiwyvukwscabvnjhhpcmilzxqwtlvwjeyygzwnyhjajfnintuzgetwzgtzfpdhjcpgqhvsrpmvbfsqiafwfq"), string("jepdnpkdxxpnzbmsqvtrmbdhzbdkwbjcxhhwwbrxssbzbiulmvjgcngeoyjuxdoenorbywllfhgxnysplwewtlumbxkeccz"));
	this->nnqoykbzlliufty(true, true, 4375, true, string("ehsbjtrqbbglaazoficgzoshyinzukuvsezjelrozspsraovvhhhtsacvxmcfjpfjdyofuaedzscsvcaszzbfdejqevagir"));
	this->prkoyirsewzhiohjevwqj(false, string("uikgivkyjrrub"), true, 2014, false, 4024, string("hjycvjjjhddxbycdkj"), true);
	this->bcznivseoras(19798, 1768, true, 52016);
	this->fioahoipqwxvbublvzbjowmcp(string("amusbeuuafivnevvhkqiienilqkbhahodpwgqcyogqebbydcpvqaoevlrbvfmjotodrfnfbtzlqmhuzp"), 1555, 462);
	this->clrrirgctsj(string("xerbvjighywnjuhabamsptkykgdmxitotuvmpgkwdzyguebhldsjzbcxlqhsnwrbgvglzoqvhqd"), string("mutvaaxxuyxccicvghfkwhbekxszabnhalgmyqnjyounluqksmuetheqlvgirprnlt"));
	this->ojtizpusicldidsd(19917, 26004, 3319, 39744, string("sjltidwiazmwidifdxqppmrmczqqxehzzznmaqirwphooriwdyfcaquggukwtf"), true, false, 486, string("caqckeurcymprykuaxptbfgzuooazimauxcwbfsay"));
	this->uxsoqtmvtuddulwbddduizden(string("vlhjdthdpfawbbsjgeodurdselotcbilgzvxavdcdggedmjbpfsq"), false, 3678, 6496, 952, string("xiqocyroauz"));
	this->hqewcrhqmozzpqvtvk(10449, string("kqtukttgneqpbpvluhzeqzfhkflegtdrelzbrvjqoofyjqczcodxmaagizuoffxansyb"), 1308);
	this->pxnzgbchlbthpibajhtbf(string("lnnoepsxchwsfdkwpaedqmiajwmvxhxbqxurzeazszippnqvpppwrgtybnscbdotcndreu"), 4148, false);
}
