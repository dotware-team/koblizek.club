#pragma once

#include "../cheat_others/Config.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/Material.h"
#include "../cheat_sdk/ModelRender.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/RenderView.h"
#include "../cheat_protection/xor.hpp"

class Chams {
public:
	Chams() noexcept;
	bool render(void*, void*, const ModelRenderInfo&, matrix3x4*) const noexcept;
private:
	bool renderPlayers(void*, void*, const ModelRenderInfo&, matrix3x4*) const noexcept;

	enum ChamsId {
		ALLIES_VISIBLE = 0,
		ALLIES_OCCLUDED,
		ENEMIES_VISIBLE,
		ENEMIES_OCCLUDED,
		BACKTRACK
	};

	Material* normal;
	Material* flat;
	Material* glow;
	Material* blinking;

	constexpr auto dispatchMaterial(int id) const noexcept
	{
		switch (id) {
		default:
		case 0: return normal;
		case 1: return flat;
		case 2: return glow;
		}
	}

	constexpr void applyChams(decltype(config.koblizek_chams[0])& chams, bool ignorez, int health = 0) const noexcept
	{
		auto material = dispatchMaterial(chams.material);

		if (material == glow || material == blinking) {
			if (chams.healthBased && health) {
				material->findVar(XorStr("$envmaptint"))->setVectorValue(1.0f - health / 100.0f, health / 100.0f, 0.0f);
			}
			else if (chams.rainbow) {
				const auto [r, g, b] { rainbowColor(memory.globalVars->realtime, chams.rainbow) };
				material->findVar(XorStr("$envmaptint"))->setVectorValue(r, g, b);
			}
			else {
				material->findVar(XorStr("$envmaptint"))->setVectorValue(chams.color[0], chams.color[1], chams.color[2]);
			}
		}
		else {
			if (chams.healthBased && health) {
				material->colorModulate(1.0f - health / 100.0f, health / 100.0f, 0.0f);
			}
			else if (chams.rainbow) {
				const auto [r, g, b] { rainbowColor(memory.globalVars->realtime, chams.rainbow) };
				material->colorModulate(r, g, b);
			}
			else {
				material->colorModulate(chams.color[0], chams.color[1], chams.color[2]);
			}
		}

		bool blinking = false;
		const auto pulse = chams.alpha * (blinking ? std::sin(memory.globalVars->currenttime * 5) * 0.5f + 0.5f : 1.0f);

		if (material == glow)
			material->findVar(XorStr("$envmapfresnelminmaxexp"))->setVecComponentValue(9.0f * (1.2f - pulse), 2);
		else
			material->alphaModulate(pulse);

		material->setMaterialVarFlag(MaterialVar::IGNOREZ, ignorez);
		material->setMaterialVarFlag(MaterialVar::WIREFRAME, chams.wireframe);
		interfaces.modelRender->forceMaterialOverride(material);
	}
};
