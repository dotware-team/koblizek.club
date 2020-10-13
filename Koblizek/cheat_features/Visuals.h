#pragma once

#include "../cheat_others/Config.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/Engine.h"
#include "../cheat_sdk/EntityList.h"
#include "../cheat_sdk/GameEvent.h"

enum class FrameStage;
namespace Visuals {
	static bool update = true;
	void scheduleUpdate() noexcept;
	void colorWorld() noexcept;
    void thirdperson() noexcept;
    void removeBlur() noexcept;
    void updateBrightness() noexcept;

	void skybox(FrameStage stage) noexcept;

    constexpr void disablePostProcessing() noexcept
    {
        if (*memory.disablePostProcessing != config.koblizek_visuals.disablePostProcessing)
            *memory.disablePostProcessing = config.koblizek_visuals.disablePostProcessing;
    }

    constexpr void reduceFlashEffect() noexcept
    {
        interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->setProperty("m_flFlashMaxAlpha", 255.0f - config.koblizek_visuals.flashReduction * 2.55f);
    }

	constexpr bool removeSleeves(const char* modelName) noexcept
    {
        return config.koblizek_visuals.noSleeves && strstr(modelName, "sleeve");
    }
};
