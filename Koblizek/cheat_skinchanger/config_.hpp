#pragma once
#include "item_definitions.hpp"
#include "kit_parser.hpp"

#include <limits>
#include <unordered_map>
#include <array>
#include <algorithm>

#undef min

struct sticker_setting
{
	void update()
	{
		kit = game_data::sticker_kits[kit_vector_index].id;
	}

	int kit = 0;
	int kit_vector_index = 0;
	float wear = std::numeric_limits<float>::min();
	float scale = 1.f;
	float rotation = 0.f;
};

struct item_setting
{
	void update()
	{
		definition_index = game_data::weapon_names[definition_vector_index].definition_index;
		entity_quality_index = game_data::quality_names[entity_quality_vector_index].index;

		const std::vector<game_data::PaintKit>* kit_names;
		const game_data::weapon_name* defindex_names;

		if (definition_index == GLOVE_T_SIDE)
		{
			kit_names = &game_data::glove_kits;
			defindex_names = game_data::glove_names;
		}
		else
		{
			kit_names = &game_data::skin_kits;
			defindex_names = game_data::knife_names;
		}

		paint_kit_index = (*kit_names)[paint_kit_vector_index].id;
		definition_override_index = defindex_names[definition_override_vector_index].definition_index;

		for (auto& sticker : stickers)
			sticker.update();
	}

	bool enabled = true;
	int definition_vector_index = 0;
	int definition_index = 1;
	int entity_quality_vector_index = 0;
	int entity_quality_index = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	int currently_selected_skin = 0;
	float wear = std::numeric_limits<float>::min();
	char custom_name[32] = "";
	std::array<sticker_setting, 5> stickers;
};

class config_ {
public:
	item_setting* get_by_definition_index(int definition_index);
};

extern config_ g_config;