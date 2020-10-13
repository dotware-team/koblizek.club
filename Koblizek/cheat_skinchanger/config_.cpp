#include "config_.hpp"
#include "../cheat_others/Config.h"
#include <fstream>

config_ g_config;

item_setting* config_::get_by_definition_index(const int definition_index)
{
	auto it = std::find_if(std::begin(config.skinChanger.items), std::end(config.skinChanger.items), [definition_index](const item_setting& e)
	{
		return e.enabled && e.definition_index == definition_index;
	});

	return it == std::end(config.skinChanger.items) ? nullptr : &*it;
}