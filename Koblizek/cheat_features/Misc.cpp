#include <sstream>
#include "../cheat_others/Config.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"
#include "../cheat_important/Netvars.h"
#include "Misc.h"
#include "../cheat_sdk/ConVar.h"
#include "../cheat_sdk/Surface.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/NetworkChannel.h"
#include "../cheat_imgui/imgui.h"
#include "../cheat_others/font-flags.h"
#include "../cheat_important/GUI.h"
#include "../cheat_imgui/imgui.h"
#include "../cheat_imgui/imgui_internal.h"
#include "../Kokesh.h"
#include "../cheat_sdk/Material.h"
#include "../cheat_sdk/MaterialSystem.h"
#include "../winapi_own.h"
#include "../Color.hpp"
#include "../cheat_sdk/ModelInfo.h"
#include "../fnv.h"
#include "../IViewRenderBeams.h"
#include "../cheat_sdk/time.hpp"
#include "../cheat_sdk/math.hpp"
#include "../cheat_protection/xor.hpp"
#include "../cheat_features/EnginePrediction.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define M_PI		3.14159265358979323846
#define M_PI_F		((float)(M_PI))	
#define M_PHI		1.61803398874989484820
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif
#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif
int flags_backup = 0;

enum class TeamID : int
{
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

std::string allcolors[] =
{
	" \x01", " \x02", " \x03", " \x04", " \x05", " \x06", " \x7", " \x8", " \x9", " \x0A", " \x0B", " \x0C", " \x0D", " \x0E", " \x0F", " \x10"
};

static constexpr bool worldToScreen(const Vector& in, Vector& out) noexcept
{
	const auto matrix = interfaces.engine->worldToScreenMatrix();
	float w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

	if (w > 0.001f) {
		const auto[width, height] = interfaces.surface->getScreenSize();
		out.x = width / 2 * (1 + (matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3]) / w);
		out.y = height / 2 * (1 - (matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3]) / w);
		out.z = 0.0f;
		return true;
	}
	return false;
}

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void misc::aspectratio() noexcept
{
	static auto aspectratio = interfaces.cvar->findVar(XorStr("r_aspectratio"));
	if (aspectratio)
	{
		aspectratio->setValue(config.koblizek_misc.aspectratio);
	}
}

void misc::fakePrime() noexcept
{
	static bool lastState = false;

	if (config.koblizek_misc.fakePrime != lastState) {
		lastState = config.koblizek_misc.fakePrime;

		if (DWORD oldProtect; VirtualProtectHidden(memory.fakePrime, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
			constexpr uint8_t patch[]{ 0x74, 0xEB };
			*memory.fakePrime = patch[config.koblizek_misc.fakePrime];
			VirtualProtectHidden(memory.fakePrime, 1, oldProtect, nullptr);
		}
	}
}

void misc::forceCrosshair() noexcept
{
	if (config.koblizek_misc.forceCrosshair) {
		static auto activate = interfaces.cvar->findVar(XorStr("weapon_debug_spread_show"));

		activate->setValue(3);
	}
	else {
		static auto deactivate = interfaces.cvar->findVar(XorStr("weapon_debug_spread_show"));

		deactivate->setValue(0);
	}
}

void misc::FPSBoost(FrameStage stage) noexcept
{
	if (stage != FrameStage::RENDER_START && stage != FrameStage::RENDER_END)
		return;

	*memory.disablePostProcessing = stage == FrameStage::RENDER_START && config.koblizek_visuals.disablePostProcessing;
}

void misc::Fullbright() noexcept
{
	static ConVar* full_bright = interfaces.cvar->findVar(XorStr("mat_fullbright"));
	if (!config.koblizek_misc.FullBright)
	{
		full_bright->setValue(0);
	}
	else
	{
		full_bright->setValue(1);
	}
}

void misc::ShowImpacts() noexcept
{
	static ConVar* show_impacts = interfaces.cvar->findVar(XorStr("sv_showimpacts"));

	if (!config.koblizek_misc.ShowImpacts)
	{
		show_impacts->setValue(0);
	}
	else
	{
		show_impacts->setValue(1);
	}
}

void misc::Spectatorlist()
{
	if (config.koblizek_misc.SpectatorList)
	{
		int specs = 0;
		std::string spect = "";

		if (interfaces.engine->isInGame()) {
			int localIndex = interfaces.engine->getLocalPlayer();
			auto pLocalEntity = interfaces.entityList->getEntity(localIndex);
			if (pLocalEntity) {
				for (int i = 1; i < interfaces.engine->getMaxClients(); i++)
				{
					Entity* pBaseEntity = interfaces.entityList->getEntity(i);

					if (!pBaseEntity)
						continue;

					if (pBaseEntity->health() > 0)
						continue;

					if (pBaseEntity == pLocalEntity)
						continue;

					if (pBaseEntity->isDormant())
						continue;

					if (!pLocalEntity->isAlive()) {
						if (pBaseEntity->getObserverTarget() != pLocalEntity->getObserverTarget())
							continue;
					}
					else {
						if (pBaseEntity->getObserverTarget() != pLocalEntity)
							continue;
					}

					PlayerInfo pInfo;
					interfaces.engine->getPlayerInfo(pBaseEntity->index(), pInfo);
					if (pInfo.ishltv)
						continue;

					spect += pInfo.name;
					spect += "\n";
					specs++;
				}
			}
		}

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushFont(titlefont);
		style.FramePadding = ImVec2(5, 7);
		style.WindowPadding = ImVec2(7, 8);
		style.ItemSpacing = ImVec2(15, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);
		if (ImGui::Begin(XorStr("Spectator List"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar));
		{
			ImGui::PushFont(menufont);
			style.FramePadding = ImVec2(5, 5);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);
			if (specs > 0) spect += XorStr("\n");

			auto windows_size = ImGui::GetCurrentWindow()->Size;

			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

			ImVec2 size = ImGui::CalcTextSize(spect.c_str());
			ImGui::SetWindowSize(ImVec2(150, 30 + size.y));

			ImGui::Separator();
			ImGui::Spacing();

			auto spect_name = spect.c_str();
			auto spect_text_size = ImGui::CalcTextSize(spect_name);
			ImGui::Text(spect_name);
			ImGui::PopStyleVar();
			ImGui::PopFont();
		}
		ImGui::PopStyleVar();
		ImGui::PopFont();
		style.WindowPadding = ImVec2(20, 20);
		style.ItemSpacing = ImVec2(8, 8);
		style.FramePadding = ImVec2(5, 5);
		ImGui::End();
	}
}

void misc::Kick(int userid) noexcept
{
	char command[1024];
	sprintf(command, XorStr("callvote kick %d"), userid);

	interfaces.engine->clientCmdUnrestricted(command);
}

void misc::Bunnyhop(UserCmd* cmd) noexcept
{
	const auto localPlayer{ interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer()) };

	static auto wasLastTimeOnGround{ localPlayer->flags() & 1 };

	if (config.koblizek_misc.bunnyHop && !(localPlayer->flags() & 1) && localPlayer->moveType() != MoveType::LADDER && !wasLastTimeOnGround)
		cmd->buttons &= ~UserCmd::IN_JUMP;

	wasLastTimeOnGround = localPlayer->flags() & 1;
}

void misc::VelocityIndicator() noexcept
{
	if (config.koblizek_misc.VelocityIndicator) {

		if (!interfaces.engine->isInGame())
			return;

		if (!interfaces.engine->isConnected())
			return;

		const auto player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (!player)
			return;

		if (!player->isAlive())
			return;

		if (player->moveType() == MoveType::NOCLIP)
			return;

		static unsigned font = interfaces.surface->createFont();
		auto renderY{ interfaces.surface->getScreenSize().second - 40 };
		std::wstring velocity{ L"  " + std::to_wstring(static_cast<int>(player->velocity().length2D())) + L"  " };
		const auto renderX{ interfaces.surface->getScreenSize().first / 2 - static_cast<int>((interfaces.surface->getTextSize(font, velocity.c_str())).first / 2) };

		static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, 800, 0, 0, FONTFLAG_ANTIALIAS);
		interfaces.surface->setTextPosition(renderX, renderY);
		interfaces.surface->setTextFont(font);

		static float old_speed = player->velocity().length2D();

		if (player->velocity().length2D() > old_speed)
		{
			interfaces.surface->setTextColor(25, 255, 100, 255);
		}
		else if (player->velocity().length2D() < old_speed)
		{
			interfaces.surface->setTextColor(255, 100, 100, 255);
		}
		else
		{
			interfaces.surface->setTextColor(255, 200, 100, 255);
		}
		static auto update_time = GetTickCount();
		if (GetTickCount() - update_time > 64)
		{
			old_speed = player->velocity().length2D();
			update_time = GetTickCount();
		}

		interfaces.surface->printText(velocity);
	}
}

void misc::DumpID()
{
	if (!interfaces.engine->isInGame()) {
		misc::console_warning(XorStr("[Koblizek.club] - "));
		interfaces.cvar->Msg(XorStr("Cannot use Dump SteamID! Please connect at matchmaking, community server! \n"));
		return;
	}

	static constexpr auto steam_id64_constant = 0x0110000100000000ULL;

	for (int i = 1; i <= interfaces.engine->getMaxClients(); i++) {
		auto entity = interfaces.entityList->getEntity(i);

		if (!entity)
			continue;

		PlayerInfo info;
		interfaces.engine->getPlayerInfo(i, info);

		auto steam_id64 = steam_id64_constant + info.steamId;

		std::stringstream output;

		std::string player_name = info.name;
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

		if (!info.fakeplayer) {
			misc::console_warning(XorStr("[Koblizek.club] - "));
			output << XorStr("Player: ") << player_name.c_str() << XorStr(" Link: ") << XorStr("steamcommunity.com/profiles/") << steam_id64 << "/" << "\n";
			interfaces.cvar->Msg("%s", output.str());
		}
	}
}

void misc::console_warning(const char* msg, ...)
{
	if (msg == nullptr)
		return;

	typedef void(__cdecl* console_warning_fn)(const char* msg, va_list);
	static console_warning_fn fn = (console_warning_fn)GetProcAddress(GetModuleHandleHidden(XorStr("tier0.dll")), XorStr("Warning"));
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}

void misc::AutoStrafe(UserCmd* cmd) noexcept
{
	const auto player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (GetAsyncKeyState(0X57)) {
		cmd->forwardmove = (10000.f) / player->velocity().length2D();
		cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
		if (cmd->forwardmove > 450.f)
			cmd->forwardmove = 450.f;

	}
	else if (GetAsyncKeyState(0X41)) {
		cmd->sidemove = (-10000.f) / player->velocity().length2D();
		cmd->forwardmove = (cmd->command_number % 2) == 0 ? 450.f : -450.f;
		if (cmd->sidemove < -450.f)
			cmd->sidemove = -450.f;
	}
	else if (GetAsyncKeyState(0x44)) {
		cmd->sidemove = (10000.f) / player->velocity().length2D();
		cmd->forwardmove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
		if (cmd->sidemove > 450.f)
			cmd->sidemove = 450.f;
	}
	else if (GetAsyncKeyState(0x53)) {
		cmd->forwardmove = (-10000.f) / player->velocity().length2D();
		cmd->sidemove = (cmd->command_number % 2) == 0 ? 450.f : -450.f;
		if (cmd->forwardmove < -450.f)
			cmd->forwardmove = -450.f;
	}
}
void misc::PrePredictionEdgeJump(UserCmd* cmd)
{
	const auto player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	flags_backup = player->flags();
}

void misc::PostPredictionEdgeJump(UserCmd* cmd) noexcept
{
	if (config.koblizek_misc.EdgeJump && GetAsyncKeyState(config.koblizek_misc.EdgeJumpKey)) {
		const auto player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (player->moveType() == MoveType::LADDER || player->moveType() == MoveType::NOCLIP)
			return;

		if (flags_backup & FL_ONGROUND && !(player->flags() & FL_ONGROUND))
			cmd->buttons |= UserCmd::IN_JUMP;

		if (config.koblizek_misc.EdgeJumpAutostrafer && !(player->flags() & FL_ONGROUND))
		{
			misc::AutoStrafe(cmd);
		}
	}
}

void misc::DrawIndicators()
{
	if (interfaces.engine->isInGame())
	{
		if (config.koblizek_misc.Indicators)
		{
			if (config.koblizek_misc.JumpBug && GetAsyncKeyState(config.koblizek_misc.JumpBugKey))
			{
				static unsigned font = interfaces.surface->createFont();
				static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, 800, 0, 0, FONTFLAG_ANTIALIAS);

				const auto renderY2{ interfaces.surface->getScreenSize().second - 70 };
				const auto renderX2{ interfaces.surface->getScreenSize().first / 2 - static_cast<int>((interfaces.surface->getTextSize(font, L"JB")).first / 2) };
				interfaces.surface->setTextPosition(renderX2, renderY2);
				interfaces.surface->setTextFont(font);
				interfaces.surface->setTextColor(255, 255, 255, 255);
				interfaces.surface->printText(L"JB");
			}

			if (config.koblizek_misc.EdgeJump && GetAsyncKeyState(config.koblizek_misc.EdgeJumpKey))
			{
				static unsigned font = interfaces.surface->createFont();
				static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, 800, 0, 0, FONTFLAG_ANTIALIAS);

				const auto renderY2{ interfaces.surface->getScreenSize().second - 70 };
				const auto renderX2{ interfaces.surface->getScreenSize().first / 2 - static_cast<int>((interfaces.surface->getTextSize(font, L"EJ")).first / 2) };
				interfaces.surface->setTextPosition(renderX2, renderY2);
				interfaces.surface->setTextFont(font);
				interfaces.surface->setTextColor(255, 255, 255, 255);
				interfaces.surface->printText(L"EJ");
			}

			if (config.koblizek_misc.EdgeBug && GetAsyncKeyState(config.koblizek_misc.EdgeBugKey))
			{
				static unsigned font = interfaces.surface->createFont();
				static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, 800, 0, 0, FONTFLAG_ANTIALIAS);

				const auto renderY2{ interfaces.surface->getScreenSize().second - 70 };
				const auto renderX2{ interfaces.surface->getScreenSize().first / 2 - static_cast<int>((interfaces.surface->getTextSize(font, L"EB")).first / 2) };
				interfaces.surface->setTextPosition(renderX2, renderY2);
				interfaces.surface->setTextFont(font);
				interfaces.surface->setTextColor(255, 255, 255, 255);
				interfaces.surface->printText(L"EB");
			}
		}
	}
}

void misc::jump_bug(UserCmd* cmd)
{
	if (!config.koblizek_misc.JumpBug)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!localPlayer)
		return;

	if (!localPlayer->isAlive())
		return;

	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;
	static bool bhopWasEnabled = false;

	if (GetAsyncKeyState(config.koblizek_misc.JumpBugKey) && (localPlayer->flags() & (1 << 0)))
	{
		if (config.koblizek_misc.bunnyHop) {
			config.koblizek_misc.bunnyHop = false;
			bhopWasEnabled = true;
		}

		bool unduck = cmd->buttons &= ~UserCmd::IN_DUCK;
		if (unduck) {
			cmd->buttons &= ~UserCmd::IN_DUCK; // duck
			cmd->buttons |= UserCmd::IN_JUMP; // jump
			unduck = false;
		}
		Vector pos = localPlayer->getAbsOrigin();
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK; // duck
				cmd->buttons &= ~UserCmd::IN_JUMP; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK; // duck
				cmd->buttons &= ~UserCmd::IN_JUMP; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK; // duck
				cmd->buttons &= ~UserCmd::IN_JUMP; // jump
				unduck = true;
			}
		}
	}
	else if (bhopWasEnabled) {
		config.koblizek_misc.bunnyHop = true;
		bhopWasEnabled = false;
	}
}

void misc::edge_bug(UserCmd* cmd) noexcept
{
	if (!config.koblizek_misc.EdgeBug)
		return;

	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!localPlayer)
		return;

	if (!localPlayer->isAlive())
		return;

	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;

	if (GetAsyncKeyState(config.koblizek_misc.EdgeBugKey) && (localPlayer->flags() & (1 << 0)))
	{
		bool unduck = cmd->buttons &= ~UserCmd::IN_DUCK;
		if (unduck) {
			cmd->buttons &= ~UserCmd::IN_DUCK; // duck
			unduck = false;
		}
		Vector pos = localPlayer->getAbsOrigin();
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK; // duck
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK; // duck
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 8192;

			Trace fag;

			Ray ray(pt, pt2);

			TraceFilter flt(localPlayer);
			interfaces.engineTrace->traceRay(ray, mask_playersolid, flt, fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				cmd->buttons |= UserCmd::IN_DUCK;
				unduck = true;
			}
		}
	}
}

void misc::ChatSpam()
{
	if (config.koblizek_misc.spam == 1) {
		static DWORD lastspammed = 0;
		if (GetTickCount() - lastspammed > 800) {
			lastspammed = GetTickCount();
			interfaces.engine->clientCmdUnrestricted(XorStr("say ﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽﷽ ﷽﷽"));
		}
	}

	if (config.koblizek_misc.spam == 2) {
		static clock_t start_t = clock();

		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;

		if (timeSoFar < 1)
			return;

		const char* radioCommands[] = {
				"coverme",
				"takepoint",
				"holdpos",
				"regroup",
				"followme",
				"takingfire",
				"go",
				"fallback",
				"sticktog",
				"report",
				"roger",
				"enemyspot",
				"needbackup",
				"sectorclear",
				"inposition",
				"reportingin",
				"getout",
				"negative",
				"enemydown",
		};

		interfaces.engine->clientCmdUnrestricted(radioCommands[std::rand() % std::size(radioCommands)]);

		start_t = clock();
	}

	if (config.koblizek_misc.spam == 3) {
		static DWORD lastspammed = 0;
		if (GetTickCount() - lastspammed > 800) {
			lastspammed = GetTickCount();
			interfaces.engine->clientCmdUnrestricted(XorStr("say Koblizek.club | Private CS:GO Cheat"));
		}
	}

	if (config.koblizek_misc.spam == 4) {
		std::vector<std::string> chatspamsss = { XorStr("Nice shot!") , XorStr("Good Game!") , XorStr("Good half!") , XorStr("Well played!") , XorStr("Good Job!") , XorStr("Good luck!"), XorStr("Have Fun!") };

		static DWORD lastspammed = 0;
		if (GetTickCount() - lastspammed > 800) {
			lastspammed = GetTickCount();
			auto say = XorStr("say ");
			std::string msg = say + chatspamsss[rand() % chatspamsss.size()];
			interfaces.engine->clientCmdUnrestricted(msg.c_str());
		}
	}
}

void misc::animateClanTag(const char* tag) noexcept
{
	static float lastTime{ 0.0f };
	if (config.koblizek_misc.Clantag)
	{
		if (int(memory.globalVars->currenttime) != lastTime)
		{
			switch (int(memory.globalVars->currenttime) % 16)
			{
			case 0: setClanTag(XorStr("")); break;
			case 1: setClanTag(XorStr("")); break;
			case 2: setClanTag(XorStr("Ko")); break;
			case 3: setClanTag(XorStr("Kob")); break;
			case 4: setClanTag(XorStr("Kobl")); break;
			case 5: setClanTag(XorStr("Kobli")); break;
			case 6: setClanTag(XorStr("Kobliz")); break;
			case 7: setClanTag(XorStr("Koblize")); break;
			case 8: setClanTag(XorStr("Koblizek")); break;
			case 9: setClanTag(XorStr("Koblizek.")); break;
			case 10: setClanTag(XorStr("Koblizek.c")); break;
			case 11: setClanTag(XorStr("Koblizek.cl")); break;
			case 12: setClanTag(XorStr("Koblizek.clu")); break;
			case 13: setClanTag(XorStr("Koblizek.club")); break;
			case 14: setClanTag(XorStr("")); break;
			case 15: setClanTag(XorStr("")); break;
			}
		}
		lastTime = int(memory.globalVars->currenttime);
	}
}

void misc::removepanoramablur() noexcept
{
	ConVar* blur = interfaces.cvar->findVar(XorStr("@panorama_disable_blur"));
	blur->setValue(config.koblizek_misc.removeblurpan);
}

void misc::HitSound(GameEvent* event)
{
	if (config.koblizek_misc.hitSound && !strcmp(event->getName(), XorStr("player_hurt")) && interfaces.engine->getPlayerForUserID(event->getInt(XorStr("attacker"))) == interfaces.engine->getLocalPlayer())
	{
		interfaces.engine->clientCmdUnrestricted(XorStr("play buttons/arena_switch_press_02"));
	}
}

void misc::Hitmarker(GameEvent* event)
{
	if (config.koblizek_misc.hitmarker)
	{
		if (!interfaces.engine->isInGame())
			return;

		if (!interfaces.engine->isConnected())
			return;

		auto localplayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (!localplayer)
			return;

		if (!localplayer->isAlive())
			return;

		static float lastHitTime = 0.0f;

		if (event && interfaces.engine->getPlayerForUserID(event->getInt(XorStr("attacker"))) == interfaces.engine->getLocalPlayer()) {
			lastHitTime = memory.globalVars->realtime;
			return;
		}

		if (lastHitTime + 0.7f >= memory.globalVars->realtime) {
			if (config.koblizek_misc.hitmarker) {
				int screenSizeX, screenCenterX;
				int screenSizeY, screenCenterY;
				interfaces.engine->getScreenSize(screenSizeX, screenSizeY);

				screenCenterX = screenSizeX / 2;
				screenCenterY = screenSizeY / 2;

				const auto lineSize = 7;
				const auto alpha_hitmarker = 0.9f;
				interfaces.surface->setDrawColor(255, 255, 255, static_cast<int>(alpha_hitmarker * 255.f));
				interfaces.surface->drawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
				interfaces.surface->drawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
				interfaces.surface->drawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
				interfaces.surface->drawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
			}
		}
	}
}

void misc::nadePredict() noexcept
{
	if (config.koblizek_misc.GrenadePrediction)
	{
		static auto activate = interfaces.cvar->findVar(XorStr("cl_grenadepreview"));

		activate->setValue(1);
	}
	else
	{
		static auto deactivate = interfaces.cvar->findVar(XorStr("cl_grenadepreview"));

		deactivate->setValue(0);
	}
}

void misc::drawBombTimer() noexcept
{
	if (config.koblizek_misc.bombTimer) {
		const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		for (int i = interfaces.engine->getMaxClients(); i <= interfaces.entityList->getHighestEntityIndex(); i++) {
			Entity* entity = interfaces.entityList->getEntity(i);
			if (!entity || entity->isDormant() || entity->getClientClass()->classId != ClassId::PlantedC4 || !entity->c4Ticking())
				continue;

			Vector vecBombDistance = entity->origin() - localPlayer->origin();

			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((vecBombDistance.length() - b) / c);
			float flDamage = a * exp(-d * d);

			int ArmorValue = localPlayer->armor();
			float flArmorRatio = 0.5f;
			float flArmorBonus = 0.5f;
			if (ArmorValue > 0) {
				float flNew = flDamage * flArmorRatio;
				float flArmor = (flDamage - flNew) * flArmorBonus;

				if (flArmor > static_cast<float>(ArmorValue)) {
					flArmor = static_cast<float>(ArmorValue)* (1.f / flArmorBonus);
					flNew = flDamage - flArmor;
				}

				flDamage = flNew;
			}
			int bombDamage = max((int)ceilf(flDamage), 0);

			static unsigned font = interfaces.surface->createFont();
			static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS);
			interfaces.surface->setTextFont(font);

			interfaces.surface->setTextPosition(10, 600);

			if (bombDamage >= localPlayer->health())
			{
				interfaces.surface->setTextColor(240, 20, 0, 255);
				interfaces.surface->printText(L"FATAL");
			}
			else
			{
				interfaces.surface->setTextColor(75, 225, 0, 255);
				interfaces.surface->printText(L"SAFE");
			}
		}

		for (int i = interfaces.engine->getMaxClients(); i <= interfaces.entityList->getHighestEntityIndex(); i++) {
			Entity* entity = interfaces.entityList->getEntity(i);
			if (!entity || entity->isDormant() || entity->getClientClass()->classId != ClassId::PlantedC4 || !entity->c4Ticking())
				continue;

			static unsigned font = interfaces.surface->createFont();
			static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS);
			interfaces.surface->setTextFont(font);
			auto bombText{ (std::wstringstream{ } << L"" << (!entity->c4BombSite() ? 'A' : 'B') << L": " << std::fixed << std::showpoint << std::setprecision(1) << (std::max)(entity->c4BlowTime() - memory.globalVars->currenttime, 0.0f) << L"s").str() };
			interfaces.surface->setTextPosition(10, 630);
			if (entity->c4BlowTime() - memory.globalVars->currenttime > 25.f)
			{
				interfaces.surface->setTextColor(75, 225, 0, 255); //green
			}
			else if (entity->c4BlowTime() - memory.globalVars->currenttime > 10.f)
			{
				interfaces.surface->setTextColor(255, 200, 100, 255); //light orange
			}
			else
			{
				interfaces.surface->setTextColor(240, 20, 0, 255); //red
			}
			interfaces.surface->printText(bombText.c_str());

			if (entity->c4Defuser() != -1) {
				static PlayerInfo playerInfo;
				if (interfaces.engine->getPlayerInfo(interfaces.entityList->getEntityFromHandle(entity->c4Defuser())->index(), playerInfo)) {
					static wchar_t name[128];
					if (MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, -1, name, 128)) {
						const wchar_t* canDefuseText;

						if (entity->c4BlowTime() >= entity->c4DefuseCountDown()) {
							canDefuseText = L"DEFUSE";
							static unsigned font = interfaces.surface->createFont();
							interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS);
							interfaces.surface->setTextFont(font);
							interfaces.surface->setTextColor(75, 225, 0, 255);
						}
						else {
							canDefuseText = L"DEFUSE";
							static unsigned font = interfaces.surface->createFont();
							interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 23, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS);
							interfaces.surface->setTextFont(font);
							interfaces.surface->setTextColor(240, 20, 0, 255);
						}

						interfaces.surface->setTextPosition(10, 660);
						interfaces.surface->printText(canDefuseText);
					}
				}
			}
			break;
		}
	}
}

void misc::BlockBot(UserCmd* cmd) noexcept
{
	if (config.koblizek_misc.BlockBot && GetAsyncKeyState(config.koblizek_misc.blockbotkey))
	{
		if (!interfaces.engine->isInGame())
			return;

		if (!interfaces.engine->isConnected())
			return;

		auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (!localPlayer)
			return;

		if (!localPlayer->isAlive())
			return;

		float bestdist = 245.f;
		int index = -1;
		for (int i = 1; i <= interfaces.engine->getMaxClients(); i++) {
			auto entity = interfaces.entityList->getEntity(i);

			if (!entity)
				continue;

			if (!entity->isAlive() || entity->isDormant() || entity == localPlayer)
				continue;

			float dist = distance(localPlayer->origin(), entity->origin());
			if (dist < bestdist)
			{
				bestdist = dist;
				index = i;
			}
		}

		if (index == -1)
			return;

		auto target = interfaces.entityList->getEntity(index);

		if (!target || !target->isVisible())
			return;

		Vector angles = CalcAngle(localPlayer->origin(), target->origin());

		angles.y -= localPlayer->m_angEyeAngles().y;
		angles.normalize();
		angles.y = std::clamp(angles.y, -180.f, 180.f);

		if (angles.y < 0.0f)
			cmd->sidemove = 450.f;
		else if (angles.y > 0.0f)
			cmd->sidemove = -450.f;
	}
}

void misc::ModifySmoke() noexcept
{
	static constexpr const char* smokeMaterials[]{
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
	};

	for (const auto mat : smokeMaterials) {
		auto material = interfaces.materialSystem->findMaterial(mat);
		material->setMaterialVarFlag(MaterialVar::NO_DRAW, config.koblizek_misc.noSmoke);
		material->setMaterialVarFlag(MaterialVar::WIREFRAME, config.koblizek_misc.wireframeSmoke);
	}
}

void misc::RemoveShadows() noexcept
{
	static auto removeshadows = interfaces.cvar->findVar(XorStr("cl_csm_enabled"));
	removeshadows->setValue(!config.koblizek_misc.removeShadows);
}

void misc::RemoveBlood() noexcept
{
	static auto removeblood = interfaces.cvar->findVar(XorStr("violence_hblood"));
	removeblood->setValue(!config.koblizek_misc.removeBlood);
}

void misc::RemoveSky() noexcept
{
	static auto sky = interfaces.cvar->findVar(XorStr("r_3dsky"));
	sky->setValue(!config.koblizek_misc.nosky);
}

PlayerInfo GetInfo(int Index) {
	PlayerInfo Info;
	interfaces.engine->getPlayerInfo(Index, Info);
	return Info;
}

void misc::DamageIndicator(GameEvent* event)
{
	if (config.koblizek_misc.DamageLog)
	{
		if (!interfaces.engine->isInGame())
			return;

		if (!interfaces.engine->isConnected())
			return;

		auto localplayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (!localplayer)
			return;

		if (!localplayer->isAlive())
			return;

		if (!localplayer->team())
			return;

		if (!strcmp(event->getName(), XorStr("player_hurt")))
		{
			std::string event_name = event->getName();
			int attackerid = event->getInt(XorStr("attacker"));
			int entityid = interfaces.engine->getPlayerForUserID(attackerid);

			if (entityid == interfaces.engine->getLocalPlayer())
			{
				int nUserID = event->getInt(XorStr("attacker"));
				int nDead = event->getInt(XorStr("userid"));
				if (nUserID || nDead)
				{
					PlayerInfo killed_info = GetInfo(interfaces.engine->getPlayerForUserID(nDead));
					PlayerInfo killer_info = GetInfo(interfaces.engine->getPlayerForUserID(nUserID));

					std::string one = XorStr(" \x0C[Koblizek.club] \x07");
					std::string two = XorStr("hit ");
					std::string three = killed_info.name;
					std::string foura = XorStr(" for: ");
					std::string fivea = event->getString(XorStr("dmg_health"));
					std::string damage = XorStr(" damage with a ");
					std::string damageb = event->getString(XorStr("weapon"));;
					std::string fourb = XorStr(" [");
					std::string fiveb = event->getString(XorStr("health"));
					std::string six = XorStr(" hp remaining]");
					std::string newline = XorStr("\n");

					auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
					memory.g_ChatElement->ChatPrintf(0, cfilter, (one + two + three + foura + fivea + damage + damageb + fourb + fiveb + six + newline).c_str());
				}
			}
		}
	}
}

void misc::InfiniteCrouch(UserCmd* cmd)
{
	if (config.koblizek_misc.InfiniteCrouch)
	{
		if (!interfaces.engine->isInGame())
			return;

		if (!interfaces.engine->isConnected())
			return;

		auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		if (!local_player)
			return;

		if (!local_player->isAlive())
			return;

		cmd->buttons |= UserCmd::IN_BULLRUSH;
	}
}
#include <codecvt>
std::wstring stringStream2wstring(std::stringstream& strs)
{
	std::string str = strs.str();
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes(str);
}

void misc::watermark()
{
	if (config.koblizek_misc.watermark) {

		/* DEFINE */

		wchar_t user[256];
		DWORD BufferSize = 8192;
		RegGetValue(HKEY_CURRENT_USER, XorStr("Software\\Koblizekclub"), XorStr("User"), RRF_RT_ANY, NULL, (PVOID)&user, &BufferSize);
		std::string valuew((const char*)user);
		std::string username = std::string(valuew.begin(), valuew.end());

		const std::wstring user_converted = s2ws(username);

		wchar_t sub[256];
		DWORD sub_size = 8192;
		RegGetValue(HKEY_CURRENT_USER, XorStr("Software\\Koblizekclub"), XorStr("Subscription"), RRF_RT_ANY, NULL, (PVOID)&sub, &sub_size);
		std::string sub_value((const char*)sub);
		std::string subscription = std::string(sub_value.begin(), sub_value.end());

		const std::wstring sub_converted = s2ws(subscription);

		/*	END  */

		static unsigned font = interfaces.surface->createFont();
		static bool init = interfaces.surface->setFontGlyphSet(font, XorStr("Verdana"), 13, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);

		std::stringstream address_string;
		address_string << interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

		const std::wstring address_string_converted = stringStream2wstring(address_string);

		interfaces.surface->setTextFont(font);
		interfaces.surface->setTextPosition(5, 5);
		interfaces.surface->setTextColor(255, 255, 255, 255);
		interfaces.surface->printText(L"Koblizek.club | ");
		interfaces.surface->printText(user_converted);
		interfaces.surface->printText(L" | ");
		interfaces.surface->printText(sub_converted);

		if (interfaces.engine->isInGame())
		{
			interfaces.surface->printText(L" | ");
			interfaces.surface->printText(address_string_converted);
		}
	}
}

void misc::autopistol(UserCmd* cmd)
{
	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (config.koblizek_misc.AutoPistol && localPlayer) {
		const auto activeWeapon = localPlayer->getActiveWeapon();
		if (activeWeapon && activeWeapon->isPistol() && activeWeapon->nextPrimaryAttack() > memory.globalVars->serverTime()) {
			if (activeWeapon->itemDefinitionIndex2() == WeaponId::Revolver)
				cmd->buttons &= ~UserCmd::IN_ATTACK2;
			else
				cmd->buttons &= ~UserCmd::IN_ATTACK;
		}
	}
}

void misc::bulletbeams(GameEvent* event_)
{
	if (!config.koblizek_misc.bullet_beams_enabled)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return;

	if (interfaces.entityList->getEntity(interfaces.engine->getPlayerForUserID(event_->getInt(XorStr("userid")))) != local_player)
		return;

	Vector start = local_player->GetEyePosition();

	Barva color = config.koblizek_misc.bullet_beams_color;

	Vector end = { event_->getFloat("x"), event_->getFloat("y"), event_->getFloat("z") };

	BeamInfo_t beams;
	beams.m_nType = 0;
	beams.m_pszModelName = XorStr("sprites/purplelaser1.vmt");
	beams.m_nModelIndex = -1;
	beams.m_flHaloScale = 0.0;
	beams.m_flLife = config.koblizek_misc.bullet_beams_duration;
	beams.m_flWidth = 5;
	beams.m_flEndWidth = 5;
	beams.m_flFadeLength = 0.0;
	beams.m_flAmplitude = 2.0;
	beams.m_flBrightness = color.a();
	beams.m_flSpeed = 0.5;
	beams.m_nStartFrame = 0.;
	beams.m_flFrameRate = 0.;
	beams.m_flRed = color.r();
	beams.m_flGreen = color.g();
	beams.m_flBlue = color.b();
	beams.m_nSegments = 2;
	beams.m_bRenderable = true;
	beams.m_nFlags = 0;
	beams.m_vecStart = start;
	beams.m_vecEnd = end;

	memory.renderBeams->CreateBeamPoints(beams);
}

void misc::soundesp(GameEvent* event_)
{
	if (!config.koblizek_misc.sound_esp_enabled)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return;

	auto player = interfaces.entityList->getEntity(interfaces.engine->getPlayerForUserID(event_->getInt(XorStr("userid"))));

	if (!player)
		return;

	if (player->isDormant())
		return;

	static int timer;
	timer += 1;

	if (timer > 1)
		timer = 0;

	if (player->team() != local_player->team())
	{
		if (player && timer < 1) {

			Barva color = config.koblizek_misc.sound_esp_color;

			Vector position = player->origin();

			BeamInfo_t beam_info;
			beam_info.m_nType = TE_BEAMRINGPOINT;
			beam_info.m_pszModelName = XorStr("sprites/physbeam.vmt");
			beam_info.m_nModelIndex = interfaces.modelInfo->getModelIndex(XorStr("sprites/physbeam.vmt"));
			beam_info.m_nHaloIndex = -1;
			beam_info.m_flHaloScale = 5;
			beam_info.m_flLife = .50f;
			beam_info.m_flWidth = 10.f;
			beam_info.m_flFadeLength = 1.0f;
			beam_info.m_flAmplitude = 0.f;
			beam_info.m_flRed = color.r();
			beam_info.m_flGreen = color.g();
			beam_info.m_flBlue = color.b();
			beam_info.m_flBrightness = color.a();
			beam_info.m_flSpeed = 0.f;
			beam_info.m_nStartFrame = 0.f;
			beam_info.m_flFrameRate = 60.f;
			beam_info.m_nSegments = -1;
			beam_info.m_nFlags = FBEAM_FADEOUT;
			beam_info.m_vecCenter = position + Vector(0, 0, 5);
			beam_info.m_flStartRadius = 20.f;
			beam_info.m_flEndRadius = 640.f;

			auto beam = memory.renderBeams->CreateBeamRingPoint(beam_info);
			if (beam)
				memory.renderBeams->DrawBeam(beam);
		}
	}
}

void misc::purchase_log(GameEvent* event_)
{
	if (!config.koblizek_misc.purchase_log)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return;

	if (!local_player->team())
		return;

	auto buyer = event_->getInt(XorStr("userid"));
	auto player_index = interfaces.engine->getPlayerForUserID(buyer);
	Entity* player = (Entity*)interfaces.entityList->getEntity(player_index);
	PlayerInfo buyer_info = GetInfo(interfaces.engine->getPlayerForUserID(buyer));

	if (player->team() == local_player->team())
		return;

	auto filter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
	static int green = 3;
	static int yellow = 15;
	static int white = 0;
	static int blue = 11;

	std::stringstream text;
	std::string buyer_text = buyer_info.name;

	text << allcolors[blue] << XorStr("[Buy]") << " " << allcolors[green] << XorStr("Player ") << allcolors[white] << buyer_text << allcolors[green] << XorStr("bought") << allcolors[white] << event_->getString(XorStr("weapon"));

	memory.g_ChatElement->ChatPrintf(0, filter, text.str().c_str());
}

bool misc::lby_breaker()
{
	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	static float NextUpdate = 0;
	float velocity = local_player->velocity().length2D();
	float time = memory.globalVars->serverTime();

	if (!(local_player->flags() & 1) || (!(local_player->isAlive())))
	{
		return false;
	}
	if (velocity > 0.1f) {
		NextUpdate = time + 0.22f;
	}
	if (NextUpdate <= time)
	{
		NextUpdate = time + 1.1f;
		return true;
	}

	return false;
}

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / memory.globalVars->intervalPerTick ) )

void misc::legit_antiaim(UserCmd* cmd, bool& sendPacket)
{
	if (!config.koblizek_misc.legit_antiaim)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return;

	if (cmd->buttons & (UserCmd::IN_ATTACK | UserCmd::IN_ATTACK2))
		return;

	if (local_player->moveType() == MoveType::LADDER || local_player->moveType() == MoveType::NOCLIP)
		return;

	float latency = 0.0f;
	if (auto networkChannel = interfaces.engine->getNetworkChannel(); networkChannel && networkChannel->getLatency(0) > 0.0f)
		latency = networkChannel->getLatency(0);

	if (latency > config.koblizek_misc.legit_antiaim_maxping)
		return;

	if (local_player->throwing(cmd))
		return;
}

void misc::legit_antiaim_resolver()
{
	if (!config.koblizek_misc.legit_antiaim_resolver)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return;

	for (int i = 0; i <= memory.globalVars->maxClients; i++)
	{
		auto ent = interfaces.entityList->getEntity(i);
		if (!ent || ent == local_player)
			continue;

		if (!ent->isAlive() || ent->isDormant() || ent->getClientClass()->classId != ClassId::CSPlayer) continue;

		PlayerInfo info;

		interfaces.engine->getPlayerInfo(i, info);

		if (info.fakeplayer)
			return;

		auto animstate = ent->getAnimstate();
		if (!animstate)
			continue;

		auto lby = ent->m_flLowerBodyYawTarget();
		auto lby_delta = lby - ent->m_angEyeAngles().y;
		auto abs_lby_delta = std::abs(normalize_yaw(lby_delta));

		float vel = ent->velocity().length2D();
		bool statics = false;
		if (vel > 0.0f && vel <= 5.0f)
			statics = true;

		resolved_real = animstate->GoalFeetYaw;

		if (abs_lby_delta >= 35.0f && animstate->footSpeed2 < 0.1f)
		{
			float desync_delta = ent->getMaxDesyncAngle();
			resolved_real = ent->m_angEyeAngles().y;
			if (lby_delta > 0)
				resolved_real += statics ? desync_delta : -desync_delta;
			else if (lby_delta < 0)
				resolved_real -= statics ? desync_delta : -desync_delta;
		}

		normalize_yaw(resolved_real);
		animstate->GoalFeetYaw = resolved_real;
	}
}

void misc::fast_stop(UserCmd* cmd) noexcept
{
	if (!config.koblizek_misc.fast_stop)
		return;

	if (!interfaces.engine->isInGame())
		return;

	if (!interfaces.engine->isConnected())
		return;

	auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!localPlayer)
		return;

	if (!localPlayer->isAlive())
		return;

	if (localPlayer->moveType() == MoveType::NOCLIP || localPlayer->moveType() == MoveType::LADDER || !(localPlayer->flags() & 1) || cmd->buttons & UserCmd::IN_JUMP)
		return;

	if (cmd->buttons & (UserCmd::IN_MOVELEFT | UserCmd::IN_MOVERIGHT | UserCmd::IN_FORWARD | UserCmd::IN_BACK))
		return;

	auto VectorAngles = [](const Vector& forward, Vector& angles)
	{
		if (forward.y == 0.0f && forward.x == 0.0f)
		{
			angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
			angles.y = 0.0f;
		}
		else
		{
			angles.x = atan2(-forward.z, forward.length2D()) * -180 / M_PI;
			angles.y = atan2(forward.y, forward.x) * 180 / M_PI;

			if (angles.y > 90)
				angles.y -= 180;
			else if (angles.y < 90)
				angles.y += 180;
			else if (angles.y == 90)
				angles.y = 0;
		}

		angles.z = 0.0f;
	};
	auto AngleVectors = [](const Vector& angles, Vector* forward)
	{
		float	sp, sy, cp, cy;

		sy = sin(degreesToRadians(angles.y));
		cy = cos(degreesToRadians(angles.y));

		sp = sin(degreesToRadians(angles.x));
		cp = cos(degreesToRadians(angles.x));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	};

	Vector velocity = localPlayer->velocity();
	Vector direction;
	VectorAngles(velocity, direction);
	float speed = velocity.length2D();
	if (speed < 15.f)
		return;

	direction.y = cmd->viewangles.y - direction.y;

	Vector forward;
	AngleVectors(direction, &forward);

	Vector negated_direction = forward * speed;

	cmd->forwardmove = negated_direction.x;
	cmd->sidemove = negated_direction.y;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class yxsmtxd {
public:
	bool pitxnvh;
	yxsmtxd();
	double hskkruvcdaxpzzj();
	bool wpmiuvvooekxddtwz(bool sgbri, bool miyncbhkvnbqkv, string bxrxmco, string zyekzwcrgpvsnb, int ukflntgzvaxisk, string xgnudrvaiwap);
	string hfkwzutugtz(double xudysbkyl, string xeuhpr, double mahmvgd, int xcjocgh, string qvaqdkwu, string bmxcbq, double gicnbz, string nbuaxshglsdq);
	bool ilfpviqvvohvumnayybl(string yuvindcsahqb, double acrdiawnbgjjjyy, bool rejuxlksqlehbl, int ldpxqdvruunxjc, double drdndtbq, double kuhejdswhxx, double nzxchzrrsdkzgfr, string vbqpqvhxbnbqfqk, double oscdjwrtkxct);
	double vofdiubywxrdujqzvky(int eobotdnigx, int vbpxkocme);
	string dgtnsfwzewtz(bool crhpxfqdnxlt, bool ddmwqtixssmtbh, int xrusazrggolq, int bgsuq, string bxcqdnmn, string ahbilymko, double fwtgskyzxtdr, int lcaxpuwzitzur, bool vcetimmf);
	double dtmkmtfxqdamwrerlmbajfg(bool mkentgfiyntyonz, bool nhsbxbbavjp, int ucinaftlsod, bool rlvynmoapozc, bool szmstrzq);
	double fokttnshtghnlsxy(double icnjrftfaenna, int aamfncxkyrx, bool ufhjifopo, bool kiwbqwehqvczsji, double whazb, bool tenivpllnddpy, bool qeoznijaitmfvv);
	double pimkmccnebqqayy(bool mmhjcwix, int cmxzjfmspbcq, double ktztxev, int qjkvh, string mjowmv);
	string ghiolwwqznqachcfnzwopd(bool buxlzvy, double thxetmskz, double dmnzupyypzu, int barsbwfgewstfjw, string vminnt, string vhepzsedcqvxcu, string ozyslnbvr, string djuecvdfcgnnhpm);

protected:
	bool mqpotwcuvzzknc;
	bool nownqfihrq;

	void xfqtohtlggekhozjbq(int kvqbyxjrckpc);
	void pdckvszvwtvs(string pzhsmrwk, bool wnohojrhymvlkww, int pboomtt, int qaizutqijqthzto, int iykgnhir, int fnacxfz, bool umslsbuum, double puwhjkcbamxoltd);

private:
	double qtplbhqxiavwb;
	bool hpwkvcijfq;
	double harfsymfbnj;

	int cakcdnsjla(int ffykxxhtbd, double fpwas, double cmiotx, string bsgmwj, int lawbjckt, bool xssocq, double gughppbcg, string lefjumhenmdp, string okcuxp);
	void lvjxkmcluwtxbbwzhmkdisjmj(bool wkthzsnyajqz, bool tmmzfsfiyunkybh, string fdcguguhi, bool ehvkvuewkikm, int fbyuefafcdgurmc, string qasjvvyquzebs, string sdyhy, string fwerinb, double lsnmshyqhnuugj);
	int gffyodegyq(string mmfxb, string xhhrpb, int nwbfbfienhjvm, double keoxmzuprw, bool vbhmvndbdulyc, int lkseismdhnhbf, int zkbnrnzyan, int uclvqzvdsxkplj, bool ifdpvbefymkj);
	int hklawrjwjuqso(string ochzcuaes, bool flavksfesquekgc, int ctgwvcf, double shhtyb, bool zzidguamp, string arynqdzyb);
	bool qmrrjadsjrcz(double uezykxmcaee, string jhtckqbvh, bool gmvajnpr, string ypzhczpamcjks, bool tvzeu, bool ykkegwmaaedzf, string fqifvpdegczha);
	string ybcqhfzhekdonekkdlg(bool ecabcudsv, bool skebvjpahlcoe, string muxjzrjnpfqmqy);

};


int yxsmtxd::cakcdnsjla(int ffykxxhtbd, double fpwas, double cmiotx, string bsgmwj, int lawbjckt, bool xssocq, double gughppbcg, string lefjumhenmdp, string okcuxp) {
	int sqhoqyje = 1454;
	double ovthrvlsoympjei = 20472;
	if (20472 != 20472) {
		int zgbldd;
		for (zgbldd = 78; zgbldd > 0; zgbldd--) {
			continue;
		}
	}
	return 21468;
}

void yxsmtxd::lvjxkmcluwtxbbwzhmkdisjmj(bool wkthzsnyajqz, bool tmmzfsfiyunkybh, string fdcguguhi, bool ehvkvuewkikm, int fbyuefafcdgurmc, string qasjvvyquzebs, string sdyhy, string fwerinb, double lsnmshyqhnuugj) {
	bool qjqhcawrvibna = true;
	int vrgqt = 1845;
	int jlcwxbndrwntzm = 3616;
	int klzsrpyqcxdaify = 108;
	double avzqmwl = 40857;
	int wogmzgkr = 1180;

}

int yxsmtxd::gffyodegyq(string mmfxb, string xhhrpb, int nwbfbfienhjvm, double keoxmzuprw, bool vbhmvndbdulyc, int lkseismdhnhbf, int zkbnrnzyan, int uclvqzvdsxkplj, bool ifdpvbefymkj) {
	string pdmvlgiwee = "lesfh";
	bool wjetxdyr = true;
	double raynf = 7699;
	int sxxrdrq = 729;
	int kmeyktrtc = 161;
	int avscoxhbwfxyfhj = 1807;
	if (161 == 161) {
		int dj;
		for (dj = 27; dj > 0; dj--) {
			continue;
		}
	}
	if (true != true) {
		int lscazt;
		for (lscazt = 69; lscazt > 0; lscazt--) {
			continue;
		}
	}
	if (161 != 161) {
		int teepbi;
		for (teepbi = 90; teepbi > 0; teepbi--) {
			continue;
		}
	}
	if (string("lesfh") == string("lesfh")) {
		int vnwal;
		for (vnwal = 98; vnwal > 0; vnwal--) {
			continue;
		}
	}
	if (1807 == 1807) {
		int fkth;
		for (fkth = 88; fkth > 0; fkth--) {
			continue;
		}
	}
	return 16613;
}

int yxsmtxd::hklawrjwjuqso(string ochzcuaes, bool flavksfesquekgc, int ctgwvcf, double shhtyb, bool zzidguamp, string arynqdzyb) {
	string bzbuuzjfod = "wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp";
	if (string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp") != string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp")) {
		int bxgfy;
		for (bxgfy = 41; bxgfy > 0; bxgfy--) {
			continue;
		}
	}
	if (string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp") == string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp")) {
		int gybjcevdqp;
		for (gybjcevdqp = 97; gybjcevdqp > 0; gybjcevdqp--) {
			continue;
		}
	}
	if (string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp") == string("wgclzktikqzybhbkgpudrrznnvzdgjhjurnfepiqvsuglixcaealhwpecluikp")) {
		int ehb;
		for (ehb = 80; ehb > 0; ehb--) {
			continue;
		}
	}
	return 93353;
}

bool yxsmtxd::qmrrjadsjrcz(double uezykxmcaee, string jhtckqbvh, bool gmvajnpr, string ypzhczpamcjks, bool tvzeu, bool ykkegwmaaedzf, string fqifvpdegczha) {
	double fnplhpzdtldmi = 12800;
	string djntqhtddd = "mscvqvczaxoqdfvvhacgkswqikrlwomdbkhemjrzkwkzfbognuyzjzbumeyqvvtthflalyihz";
	bool hdhjgmsoagb = true;
	bool rmggdhkxssfshr = true;
	string sgdqlqhpjla = "olyhbnpityzewowxjycybymrylpiglhpjbdxgpmsflzfrvdmywicyhxltyueoyxrywmpexsxogreiqvbqsnlzlz";
	bool ihnmjtwptopzr = true;
	bool rggitmvwwnt = false;
	return true;
}

string yxsmtxd::ybcqhfzhekdonekkdlg(bool ecabcudsv, bool skebvjpahlcoe, string muxjzrjnpfqmqy) {
	double lbwwhowkxsqsl = 47385;
	int rcwvtlhflru = 773;
	int dvtnejsubeate = 58;
	int ypxzvweec = 2617;
	if (773 == 773) {
		int tojwfktsey;
		for (tojwfktsey = 93; tojwfktsey > 0; tojwfktsey--) {
			continue;
		}
	}
	if (58 != 58) {
		int pglkqvuq;
		for (pglkqvuq = 71; pglkqvuq > 0; pglkqvuq--) {
			continue;
		}
	}
	if (773 == 773) {
		int oiip;
		for (oiip = 80; oiip > 0; oiip--) {
			continue;
		}
	}
	if (47385 == 47385) {
		int bq;
		for (bq = 68; bq > 0; bq--) {
			continue;
		}
	}
	if (58 != 58) {
		int byxqfgzsx;
		for (byxqfgzsx = 9; byxqfgzsx > 0; byxqfgzsx--) {
			continue;
		}
	}
	return string("bwzunvmc");
}

void yxsmtxd::xfqtohtlggekhozjbq(int kvqbyxjrckpc) {
	string uiiixtarnzdgv = "gisxogwwpnxntzurmikrhdfqrbpx";
	double wnkqyfbr = 21376;
	string yjczk = "sggmvghrekjrkzygyesbjocfapxlljyaobgmb";
	if (string("sggmvghrekjrkzygyesbjocfapxlljyaobgmb") != string("sggmvghrekjrkzygyesbjocfapxlljyaobgmb")) {
		int wvaqbkcbz;
		for (wvaqbkcbz = 100; wvaqbkcbz > 0; wvaqbkcbz--) {
			continue;
		}
	}
	if (21376 != 21376) {
		int chqog;
		for (chqog = 64; chqog > 0; chqog--) {
			continue;
		}
	}
	if (string("gisxogwwpnxntzurmikrhdfqrbpx") != string("gisxogwwpnxntzurmikrhdfqrbpx")) {
		int pnnvv;
		for (pnnvv = 52; pnnvv > 0; pnnvv--) {
			continue;
		}
	}

}

void yxsmtxd::pdckvszvwtvs(string pzhsmrwk, bool wnohojrhymvlkww, int pboomtt, int qaizutqijqthzto, int iykgnhir, int fnacxfz, bool umslsbuum, double puwhjkcbamxoltd) {
	string lahtqlbkpyoesm = "qtxrflipykfiqvtfhjkadviagnwbgidsulpntjvjjojpecsdfgsrsgmmtwqmsgcevyzpxcdiwwkzmryzztcpzghfavdiwanqx";
	bool wjcpdgpiedf = false;
	bool txwyzy = true;
	int okqhzueqykvimb = 5210;
	string poxjdupbta = "uhzpaiqngpikttgvbryqkekrzmazoqsypnvtsnubesscaizaqqahlbakdutginbkjnxzihojjfvkwalykld";
	double wkepmtsjm = 47680;
	if (47680 == 47680) {
		int gncqtdrxl;
		for (gncqtdrxl = 2; gncqtdrxl > 0; gncqtdrxl--) {
			continue;
		}
	}
	if (string("qtxrflipykfiqvtfhjkadviagnwbgidsulpntjvjjojpecsdfgsrsgmmtwqmsgcevyzpxcdiwwkzmryzztcpzghfavdiwanqx") == string("qtxrflipykfiqvtfhjkadviagnwbgidsulpntjvjjojpecsdfgsrsgmmtwqmsgcevyzpxcdiwwkzmryzztcpzghfavdiwanqx")) {
		int hfqbx;
		for (hfqbx = 99; hfqbx > 0; hfqbx--) {
			continue;
		}
	}

}

double yxsmtxd::hskkruvcdaxpzzj() {
	bool vwxdwiqbciu = true;
	double gzcngklh = 49872;
	double jtdcgdhnqqpatur = 76387;
	int vujvx = 4282;
	bool mbdhvtbp = true;
	double lyilqktxmmte = 36989;
	int dvmdycsbdcgccul = 966;
	string tecykkmxlzn = "ldfcvjfcpqbxjiabhzrf";
	string csumsrnms = "exugpjzahudeujkb";
	if (true == true) {
		int exmzvo;
		for (exmzvo = 17; exmzvo > 0; exmzvo--) {
			continue;
		}
	}
	if (49872 == 49872) {
		int tel;
		for (tel = 43; tel > 0; tel--) {
			continue;
		}
	}
	if (76387 == 76387) {
		int wyayonhky;
		for (wyayonhky = 83; wyayonhky > 0; wyayonhky--) {
			continue;
		}
	}
	if (966 == 966) {
		int xurymtqay;
		for (xurymtqay = 49; xurymtqay > 0; xurymtqay--) {
			continue;
		}
	}
	if (string("ldfcvjfcpqbxjiabhzrf") == string("ldfcvjfcpqbxjiabhzrf")) {
		int qwwt;
		for (qwwt = 4; qwwt > 0; qwwt--) {
			continue;
		}
	}
	return 91083;
}

bool yxsmtxd::wpmiuvvooekxddtwz(bool sgbri, bool miyncbhkvnbqkv, string bxrxmco, string zyekzwcrgpvsnb, int ukflntgzvaxisk, string xgnudrvaiwap) {
	string rumcxyjfqqmj = "qfxgeejsjbqrinsatbepvkzizlrjifyoolyin";
	int vulrwonxih = 5918;
	int qxpmtoumsi = 1178;
	if (5918 == 5918) {
		int onfjvkqm;
		for (onfjvkqm = 15; onfjvkqm > 0; onfjvkqm--) {
			continue;
		}
	}
	return true;
}

string yxsmtxd::hfkwzutugtz(double xudysbkyl, string xeuhpr, double mahmvgd, int xcjocgh, string qvaqdkwu, string bmxcbq, double gicnbz, string nbuaxshglsdq) {
	bool pcqnimjke = false;
	int dnjchssmwiu = 2783;
	double nujehg = 16981;
	string wpcaljrxrwkgni = "anrdhptqgxnqepw";
	double tekjlwvz = 6544;
	bool pkozyrjkkthds = false;
	bool jjqdwiudx = true;
	double agjhooc = 47225;
	if (6544 == 6544) {
		int lhgojeofnw;
		for (lhgojeofnw = 42; lhgojeofnw > 0; lhgojeofnw--) {
			continue;
		}
	}
	if (false == false) {
		int xj;
		for (xj = 71; xj > 0; xj--) {
			continue;
		}
	}
	if (6544 != 6544) {
		int gmlvp;
		for (gmlvp = 21; gmlvp > 0; gmlvp--) {
			continue;
		}
	}
	if (false != false) {
		int afmarsp;
		for (afmarsp = 98; afmarsp > 0; afmarsp--) {
			continue;
		}
	}
	return string("fbpcwe");
}

bool yxsmtxd::ilfpviqvvohvumnayybl(string yuvindcsahqb, double acrdiawnbgjjjyy, bool rejuxlksqlehbl, int ldpxqdvruunxjc, double drdndtbq, double kuhejdswhxx, double nzxchzrrsdkzgfr, string vbqpqvhxbnbqfqk, double oscdjwrtkxct) {
	double izbebqbovfotjbx = 369;
	int mkqsksmdenqozxt = 3811;
	int qgekvnadvw = 1870;
	double rxudmlpgm = 3619;
	int kwiqfygfvrmt = 3222;
	bool ejxnbjgmdncrk = true;
	int uiaicr = 2304;
	bool vtnjczosjgj = false;
	int dxsge = 270;
	int cxswffkzgt = 30;
	return true;
}

double yxsmtxd::vofdiubywxrdujqzvky(int eobotdnigx, int vbpxkocme) {
	string ngdszx = "spxe";
	bool quyvgfu = true;
	int xlzeuimpo = 808;
	string bcqhwhjfvjqlowk = "dsgmiioppjzrfrjpxqbbddtncp";
	int jisajra = 893;
	string cfdygl = "sxbyjublaysdgeqppfcpzirhbikdrouiaharifamlzryifllrhaptrhvtynhmzlvbidjxxadrmaybfuzfbgtdcfckanuofyzhg";
	if (893 == 893) {
		int bbuu;
		for (bbuu = 63; bbuu > 0; bbuu--) {
			continue;
		}
	}
	if (808 != 808) {
		int vwxgoa;
		for (vwxgoa = 79; vwxgoa > 0; vwxgoa--) {
			continue;
		}
	}
	return 15816;
}

string yxsmtxd::dgtnsfwzewtz(bool crhpxfqdnxlt, bool ddmwqtixssmtbh, int xrusazrggolq, int bgsuq, string bxcqdnmn, string ahbilymko, double fwtgskyzxtdr, int lcaxpuwzitzur, bool vcetimmf) {
	double sjimrfm = 56139;
	bool rfznlmczwhiwoj = false;
	string judjn = "kjqpysdfflgibgdlycsxd";
	bool mkwncdhotll = true;
	int nmjjbdxqtimhgvm = 9253;
	bool finrmoxspmxker = true;
	if (true == true) {
		int azax;
		for (azax = 65; azax > 0; azax--) {
			continue;
		}
	}
	return string("vqlzmeecdyanowqgrzmu");
}

double yxsmtxd::dtmkmtfxqdamwrerlmbajfg(bool mkentgfiyntyonz, bool nhsbxbbavjp, int ucinaftlsod, bool rlvynmoapozc, bool szmstrzq) {
	int mjebknsudy = 586;
	bool fdpetcgjnrym = false;
	int mzvfybhzfwrqje = 4540;
	if (4540 == 4540) {
		int wqmpcmqe;
		for (wqmpcmqe = 75; wqmpcmqe > 0; wqmpcmqe--) {
			continue;
		}
	}
	if (586 == 586) {
		int ugxe;
		for (ugxe = 42; ugxe > 0; ugxe--) {
			continue;
		}
	}
	if (false == false) {
		int uaene;
		for (uaene = 47; uaene > 0; uaene--) {
			continue;
		}
	}
	if (586 == 586) {
		int luowxtwxp;
		for (luowxtwxp = 20; luowxtwxp > 0; luowxtwxp--) {
			continue;
		}
	}
	if (4540 != 4540) {
		int cofj;
		for (cofj = 66; cofj > 0; cofj--) {
			continue;
		}
	}
	return 38812;
}

double yxsmtxd::fokttnshtghnlsxy(double icnjrftfaenna, int aamfncxkyrx, bool ufhjifopo, bool kiwbqwehqvczsji, double whazb, bool tenivpllnddpy, bool qeoznijaitmfvv) {
	bool nqbhhmqxuzxydv = true;
	int dlzpbiaxnelfdpv = 658;
	double wvgeqr = 35730;
	bool vxrylnehi = false;
	string piqmtjqjd = "tffcqepecjzshsecmzj";
	string zpgrffwyzs = "crrouyoblrappzvlmwfhsxtuhutlyiaamvnziiwafwnusddwkarvpaxchhszeddftkcompiekdbotazzumxytjwgcrhn";
	double erodosl = 27200;
	if (string("tffcqepecjzshsecmzj") == string("tffcqepecjzshsecmzj")) {
		int qws;
		for (qws = 82; qws > 0; qws--) {
			continue;
		}
	}
	if (true != true) {
		int rylizi;
		for (rylizi = 76; rylizi > 0; rylizi--) {
			continue;
		}
	}
	if (27200 == 27200) {
		int raoigwv;
		for (raoigwv = 98; raoigwv > 0; raoigwv--) {
			continue;
		}
	}
	if (false != false) {
		int qmn;
		for (qmn = 41; qmn > 0; qmn--) {
			continue;
		}
	}
	if (string("crrouyoblrappzvlmwfhsxtuhutlyiaamvnziiwafwnusddwkarvpaxchhszeddftkcompiekdbotazzumxytjwgcrhn") == string("crrouyoblrappzvlmwfhsxtuhutlyiaamvnziiwafwnusddwkarvpaxchhszeddftkcompiekdbotazzumxytjwgcrhn")) {
		int orgefayfei;
		for (orgefayfei = 19; orgefayfei > 0; orgefayfei--) {
			continue;
		}
	}
	return 73189;
}

double yxsmtxd::pimkmccnebqqayy(bool mmhjcwix, int cmxzjfmspbcq, double ktztxev, int qjkvh, string mjowmv) {
	bool jndgaykhprdkrs = false;
	int jxinvb = 1374;
	int uxyfpthcce = 2169;
	if (false != false) {
		int tm;
		for (tm = 34; tm > 0; tm--) {
			continue;
		}
	}
	if (2169 != 2169) {
		int fpbsl;
		for (fpbsl = 5; fpbsl > 0; fpbsl--) {
			continue;
		}
	}
	if (1374 == 1374) {
		int vcyav;
		for (vcyav = 1; vcyav > 0; vcyav--) {
			continue;
		}
	}
	return 9031;
}

string yxsmtxd::ghiolwwqznqachcfnzwopd(bool buxlzvy, double thxetmskz, double dmnzupyypzu, int barsbwfgewstfjw, string vminnt, string vhepzsedcqvxcu, string ozyslnbvr, string djuecvdfcgnnhpm) {
	double vztqusldrd = 10409;
	int bkjlgbjatt = 1627;
	int gycweb = 3273;
	string nbaxtdrwifx = "neqyfsrwyfxyprglrbjuuiirmwjpmkiwaabbonmnyskszgrvqbghibebzzuc";
	int ekxltv = 770;
	int kjzdxhrxbvdulzd = 460;
	double xjfzyyfzg = 35365;
	bool jtlfjwzqtri = false;
	double ujptl = 8724;
	string wxbxjkexfsawmbf = "ahzsomfgfonepnscxhgotwlqx";
	if (string("ahzsomfgfonepnscxhgotwlqx") == string("ahzsomfgfonepnscxhgotwlqx")) {
		int mmv;
		for (mmv = 41; mmv > 0; mmv--) {
			continue;
		}
	}
	if (string("neqyfsrwyfxyprglrbjuuiirmwjpmkiwaabbonmnyskszgrvqbghibebzzuc") == string("neqyfsrwyfxyprglrbjuuiirmwjpmkiwaabbonmnyskszgrvqbghibebzzuc")) {
		int nhlaolechz;
		for (nhlaolechz = 61; nhlaolechz > 0; nhlaolechz--) {
			continue;
		}
	}
	if (3273 == 3273) {
		int uwenjvf;
		for (uwenjvf = 33; uwenjvf > 0; uwenjvf--) {
			continue;
		}
	}
	if (string("ahzsomfgfonepnscxhgotwlqx") != string("ahzsomfgfonepnscxhgotwlqx")) {
		int vifqzj;
		for (vifqzj = 90; vifqzj > 0; vifqzj--) {
			continue;
		}
	}
	return string("humzigyzfdyfqyuyprx");
}

yxsmtxd::yxsmtxd() {
	this->hskkruvcdaxpzzj();
	this->wpmiuvvooekxddtwz(false, false, string("fkjgmkkztdhtggsezvsabgzieunnwsigqepgympdzqiqkirqlgjjrsmkzrwfublrnnqbxnzprhvvzpqoxcgjtwljjrsmhnwpdj"), string("csnnwoskwdcsykokmgpoewlsvfdmlrxigsnfzqgcwemhpobcnbtjraxytkovwzxqwjvizbetxpkxjfzillfwnkjyypredhkcode"), 1688, string("neaahwnxfpdqbbgsuxxtmohzhpefwhprscpcwnfb"));
	this->hfkwzutugtz(30096, string("pjarssembejuiaqan"), 32464, 3135, string("vtuayoxkarmlacybkkfhffdhouvxvizkzbxfnkggkwkgiezeqyanvywmd"), string("fgzcpuydekowqepqgjscwtwykxeqvnnpoxtzhbsgdkfmrkxwectnhmamvwpuimfyfrwnzfdal"), 56754, string("lvxsqgovjkxauatkibsifzrb"));
	this->ilfpviqvvohvumnayybl(string("jcqqxpvwradtpvcdrcwxjgwoofaadxhvwioetjubrnedetumgmjcjtxrrcirjtf"), 9178, false, 4720, 5260, 16634, 17399, string("ytkyifmleojmrgvslktsdvxptnjluetcyvkpzevvyesvlxhkuqrfxhrqniywofaaavveiwbdrdgbjzsedxlqeviucosfudi"), 19032);
	this->vofdiubywxrdujqzvky(1893, 2040);
	this->dgtnsfwzewtz(false, true, 1992, 351, string("lclfajkscytqgzvhvzvwjzonkgefgusqeonrvtapxvgeganiaf"), string("jqqfxduufqtuvkscuamscjxoasvlxrjnslbszlstgeuexgiqpabpzzvdjmwtqmjjnbydoatgfjgusgr"), 5918, 6062, true);
	this->dtmkmtfxqdamwrerlmbajfg(true, true, 1409, false, false);
	this->fokttnshtghnlsxy(30578, 45, false, true, 6104, true, false);
	this->pimkmccnebqqayy(true, 8499, 6665, 232, string("sgquysjsmepisuekbizujvauxuync"));
	this->ghiolwwqznqachcfnzwopd(false, 59384, 79221, 2178, string("exwoikicajegmgxiybwfutdnlbyetnboqcbrukfnczwlvkqwvgfqntpivcytpqlhpwiqejqjop"), string("bkhycqtjjxzsccibwzkarewhptraydflmabwzgnyqftdsxwwcvzdypxvmyudnugvhzjyyvobhoiymeglkqagvczsurpppfbvqds"), string("sffuggznssrjdaqxopvdemrkfzcmaoegdrpxsvhahgef"), string("llbjvwrkprjoirnkvynrgcxcpyaihkhsvdlqlptvkcmjiprtpwchqbsgbrqdeymhshyzqmjqrcabkrmpdqdrqwanrrkjdcemlz"));
	this->xfqtohtlggekhozjbq(848);
	this->pdckvszvwtvs(string("vjnlrhgffhupinrjotnmdsfowlngusqlahmlwdmqpawyxtvnhvdmvplumdgupp"), true, 5430, 5027, 4834, 1971, true, 53822);
	this->cakcdnsjla(3530, 49319, 31573, string("rybikyfqxsbbkohhhydhkqotvqciyybeawjixbcynfnns"), 4036, false, 63130, string("vyuirctegukgaktvhrcdaurlqiwqzyqnapodfhufgvorklgwdrcrtqjfsppyjdbhtves"), string("vncxgqpkbkzljprrhzdrirgekcoicdblqebuccmcnuklkovnyhlucrqufrmmhzjgtlppyemvakjemrmbegklfynridndnr"));
	this->lvjxkmcluwtxbbwzhmkdisjmj(false, false, string("zohuaklglyivlvyhroqetfrdgiegqneefnqjtfpasiexijeumfjbfspeqewoicrgalchyvgtjgdfwcll"), true, 1463, string("ovlhzalnfkplryugpoknkupbyhqpnmmjb"), string("nhyjqungezbtqaxjfmyiynnigjhtdjzspozsrvsclqjfcsvhzvsfxtcpsnxltxhqekytbrezuvnexqf"), string("zbokqyyeyduviymgcfsxzcgrqwqlsbxaandyapcjrctpnwo"), 8532);
	this->gffyodegyq(string("jfmejamtwjqkfydyygtnlkkxescyqapnmuycnutuqadqbolvfofcfnjyrsoqsdol"), string("sehprueiciaesofghcuifjsildsttxbewdsruvygjzzseubhnwu"), 6258, 79103, true, 2579, 1096, 8124, true);
	this->hklawrjwjuqso(string("uinmsjceqpbvfmaqfxylgfpantomepgb"), false, 281, 4811, true, string("uhgjdsuplnjhoifvshffjwteabmthevnpuddadzdehtkkqnzhypcbunowireeoblfeuqgxcvxkregukfmaxdtkpcdc"));
	this->qmrrjadsjrcz(27082, string("qitqnwvprernkytmxfeenmxzljbzotrlretjvknvqvqknnwevzrgc"), true, string("zjphmakuerayskggiqqnbguyeeinbajhweqlhnqorfivvqbexvxgjoitcbiuizxiksa"), true, false, string("klkdjlwyhzs"));
	this->ybcqhfzhekdonekkdlg(false, true, string("ervvugfoxippvlhdtuhjfkydaapfeamusxgjpjnswsitfvlafwhwbeflqbrlopikxtrgirzpghceif"));
}
