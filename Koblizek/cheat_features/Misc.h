#pragma once

#include "../cheat_important/Memory.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_sdk/FrameStage.h"
#include "../cheat_sdk/UserCmd.h"
#include "../cheat_sdk/WeaponId.h"
#include "../cheat_sdk/NetworkChannel.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/Client.h"
#include "../cheat_sdk/GameEvent.h"
#include "../cheat_sdk/GlobalVars.h"
#include <windows.h>
#include <mmsystem.h>
#include "zvuk.h"
#include "../cheat_others/Config.h"
#include "../cheat_sdk/bspflags.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "Winmm.lib") 

#define FL_ONGROUND				(1 << 0)
#define FL_DUCKING				(1 << 1)

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

#define FastSqrt(x)			(sqrt)(x)

#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#define M_PHI		1.61803398874989484820 // golden ratio

// NJS: Inlined to prevent floats from being autopromoted to doubles, as with the old system.
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

namespace misc
{
	void animateClanTag(const char* = nullptr) noexcept;
	void removepanoramablur() noexcept;
	void HitSound(GameEvent * event);
	void Hitmarker(GameEvent* = nullptr);
	void DamageIndicator(GameEvent* = nullptr);
	void nadePredict() noexcept;
	void drawBombTimer() noexcept;
	void BlockBot(UserCmd * cmd) noexcept;
	void ModifySmoke() noexcept;
	void RemoveShadows() noexcept;
	void RemoveBlood() noexcept;
	void RemoveSky() noexcept;
	void DrawIndicators();
	void InfiniteCrouch(UserCmd * cmd);
	void watermark();
	void autopistol(UserCmd * cmd);
	void bulletbeams(GameEvent* event_);
	void soundesp(GameEvent * event_);
	void purchase_log(GameEvent * event_);
	bool lby_breaker();
	void legit_antiaim(UserCmd * cmd, bool & sendPacket);
	void legit_antiaim_resolver();
	void fast_stop(UserCmd* cmd) noexcept;
	void console_warning(const char * msg, ...);
	void AutoStrafe(UserCmd * cmd) noexcept;
	void PrePredictionEdgeJump(UserCmd * cmd);
	void PostPredictionEdgeJump(UserCmd* cmd) noexcept;
	void jump_bug(UserCmd * cmd);
	void edge_bug(UserCmd * cmd) noexcept;
	void ChatSpam();
	void aspectratio() noexcept;
	void fakePrime() noexcept;
	void forceCrosshair() noexcept;
	void FPSBoost(FrameStage stage) noexcept;
	void Fullbright() noexcept;
	void ShowImpacts() noexcept;
	void Spectatorlist();
	void Kick(int userid) noexcept;
	void Bunnyhop(UserCmd* cmd) noexcept;

	void VelocityIndicator() noexcept;

	inline float resolved_real = 0.0f;

	void DumpID();

	constexpr void fixAnimationLOD(FrameStage stage) noexcept
	{
		if (config.koblizek_misc.fixAnimationLOD && stage == FrameStage::RENDER_START) {
			for (int i = 1; i <= interfaces.engine->getMaxClients(); i++) {
				if (i == interfaces.engine->getLocalPlayer()) continue;
				Entity* entity = interfaces.entityList->getEntity(i);
				if (!entity || entity->isDormant() || !entity->isAlive()) continue;
				*reinterpret_cast<int*>(entity + 0xA28) = 0;
				*reinterpret_cast<int*>(entity + 0xA30) = memory.globalVars->framecount;
			}
		}
	}

	constexpr float normalize_yaw(float& yaw) noexcept
	{
		while (yaw < 180.0f) yaw += 360.0f;
		while (yaw > 180.0f) yaw -= 360.0f;
		return yaw;
	}

	constexpr void chokePackets(bool& sendPacket) noexcept
	{
		sendPacket = interfaces.engine->getNetworkChannel()->chokedPackets >= config.koblizek_misc.chokedPackets;
	}

	constexpr void revealRanks(UserCmd* cmd) noexcept
	{
		if (config.koblizek_misc.revealRanks)
		{
			if (GetAsyncKeyState(VK_TAB))
			{
				interfaces.client->dispatchUserMessage(50, 0, 0, nullptr);
			}
		}
	}

	constexpr void fixMovement(UserCmd* cmd, float yaw) noexcept
	{
		if (config.koblizek_misc.fixMovement) {
			float oldYaw = yaw + (yaw < 0.0f ? 360.0f : 0.0f);
			float newYaw = cmd->viewangles.y + (cmd->viewangles.y < 0.0f ? 360.0f : 0.0f);
			float yawDelta = newYaw < oldYaw ? fabsf(newYaw - oldYaw) : 360.0f - fabsf(newYaw - oldYaw);
			yawDelta = 360.0f - yawDelta;

			const float forwardmove = cmd->forwardmove;
			const float sidemove = cmd->sidemove;
			cmd->forwardmove = std::cos(degreesToRadians(yawDelta)) * forwardmove + std::cos(degreesToRadians(yawDelta + 90.0f)) * sidemove;
			cmd->sidemove = std::sin(degreesToRadians(yawDelta)) * forwardmove + std::sin(degreesToRadians(yawDelta + 90.0f)) * sidemove;
		}
	}

	constexpr void setClanTag(const char* clanTag) noexcept
	{
		memory.setClanTag(clanTag, clanTag);
	}
}