#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"

#include "../cheat_sdk/Engine.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/EntityList.h"
#include "../cheat_sdk/GameMovement.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/MoveHelper.h"
#include "../cheat_sdk/Prediction.h"

#include "EnginePrediction.h"

static int localPlayerFlags;

void EnginePrediction::run(UserCmd* cmd) noexcept
{
	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer)
		return;

	localPlayerFlags = localPlayer->flags();

	*memory.predictionRandomSeed = 0;

	const auto oldCurrenttime = memory.globalVars->currenttime;
	const auto oldFrametime = memory.globalVars->frametime;

	memory.globalVars->currenttime = memory.globalVars->serverTime();
	memory.globalVars->frametime = memory.globalVars->intervalPerTick;

	memory.movehelper->SetHost(localPlayer);
	interfaces.prediction->setupMove(localPlayer, cmd, memory.movehelper, memory.moveData);
	interfaces.gameMovement->ProcessMovement(localPlayer, memory.moveData);
	interfaces.prediction->finishMove(localPlayer, cmd, memory.moveData);
	memory.movehelper->SetHost(nullptr);

	*memory.predictionRandomSeed = -1;

	memory.globalVars->currenttime = oldCurrenttime;
	memory.globalVars->frametime = oldFrametime;
}

void EnginePrediction::end() noexcept
{
	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer)
		return;

	interfaces.gameMovement->FinishTrackPredictionErrors(localPlayer);
	memory.movehelper->SetHost(nullptr);

	*memory.predictionRandomSeed = -1;

	const auto oldCurrenttime = memory.globalVars->currenttime;

	memory.globalVars->currenttime = oldCurrenttime;
	memory.globalVars->frametime = oldCurrenttime;
}

int EnginePrediction::getFlags() noexcept
{
	return localPlayerFlags;
}