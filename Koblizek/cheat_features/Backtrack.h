#pragma once
#include <deque>
#include <algorithm>
#include "../cheat_important/Memory.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_sdk/Engine.h"
#include "../cheat_sdk/ConVar.h"
#include "../cheat_sdk/Cvar.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/NetworkChannel.h"
#include "../cheat_sdk/ModelRender.h"
#include "../cheat_protection/xor.hpp"


enum class FrameStage;
struct UserCmd;

namespace Backtrack {
    void update(FrameStage) noexcept;
    void run(UserCmd*) noexcept;

	struct Record {
		Vector origin;
		float simulationTime;
		matrix3x4 matrix[256];
	};

	extern std::deque<Record> records[65];

    struct Cvars {
        ConVar* updateRate;
        ConVar* maxUpdateRate;
        ConVar* interp;
        ConVar* interpRatio;
        ConVar* minInterpRatio;
        ConVar* maxInterpRatio;
        ConVar* maxUnlag;
    };

	float getLerp() noexcept;
	bool valid(float simtime) noexcept;
	int timeToTicks(float time) noexcept;
	void init() noexcept;
}
