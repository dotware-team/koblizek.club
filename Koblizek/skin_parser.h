#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "parser.h"
#include <codecvt>
#include <Windows.h>

using namespace std;

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::wstring tagName;
};

static std::string GetModuleFilePath(HMODULE hModule)
{
	std::string ModuleName = "";
	char szFileName[MAX_PATH] = { 0 };

	if (GetModuleFileNameA(hModule, szFileName, MAX_PATH))
		ModuleName = szFileName;

	return ModuleName;
}

static std::string GetModuleBaseDir(HMODULE hModule)
{
	std::string ModulePath = GetModuleFilePath(hModule);
	return ModulePath.substr(0, ModulePath.find_last_of("\\/"));
}

std::unordered_map<std::string, std::vector<int>> weapon_skins;

int skin_parser()
{
	std::string game_path = GetModuleBaseDir(0);
	valve_parser::Document doc;
	auto r = doc.Load((char*)(game_path + "\\csgo\\scripts\\items\\items_game.txt").c_str(), valve_parser::ENCODING::UTF8);
	if (!r)
		return 0;

	valve_parser::Document english;
	r = english.Load((char*)(game_path + "\\csgo\\resource\\csgo_english.txt").c_str(), valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 0;

	auto weaponSkinCombo = doc.BreadthFirstSearch((wchar_t*)L"weapon_icons");
	if (!weaponSkinCombo || !weaponSkinCombo->ToObject())
		return 0;

	auto skinDataVec = doc.BreadthFirstSearchMultiple((wchar_t*)L"paint_kits");
	if (!skinDataVec.size())
		return 0;

	auto PaintKitNames = english.BreadthFirstSearch((wchar_t*)L"Tokens");
	if (!PaintKitNames || !PaintKitNames->ToObject())
		return 0;

	std::unordered_map<std::wstring, std::set<std::wstring>> weaponSkins;
	std::unordered_map<std::wstring, skinInfo> skinMap;
	std::unordered_map<std::wstring, std::wstring> skinNames;

	std::vector<std::wstring> weaponNames = {
	L"deagle",
	L"elite",
	L"fiveseven",
	L"glock",
	L"ak47",
	L"aug",
	L"awp",
	L"famas",
	L"g3sg1",
	L"galilar",
	L"m249",
	L"m4a1_silencer",
	L"m4a1",
	L"mac10",
	L"p90",
	L"ump45",
	L"xm1014",
	L"bizon",
	L"mag7",
	L"mp5sd",
	L"negev",
	L"sawedoff",
	L"tec9",
	L"hkp2000",
	L"mp7",
	L"mp9",
	L"nova",
	L"p250",
	L"scar20",
	L"sg556",
	L"ssg08",
	L"usp_silencer",
	L"cz75a",
	L"revolver",
	L"knife_m9_bayonet",
	L"weapon_bayonet",
	L"knife_flip",
	L"knife_gut",
	L"knife_karambit",
	L"knife_m9_bayonet",
	L"knife_tactical",
	L"knife_falchion",
	L"knife_survival_bowie",
	L"knife_butterfly",
	L"knife_push",
	L"knife_ursus",
	L"knife_gypsy_jackknife",
	L"knife_stiletto",
	L"knife_widowmaker",
	L"knife_css",
	L"knife_skeleton",
	L"knife_outdoor",
	L"knife_canis",
	L"knife_cord"
	};


	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName((wchar_t*)L"icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				if (pos != std::wstring::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName((wchar_t*)L"name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName((wchar_t*)L"description_tag");
					if (tagNode)
					{
						std::wstring tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::wstring::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName((wchar_t*)L"seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					skinMap[skinName] = si;
				}
			}
		}
	}

	//populate skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::wstring key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find(L"paintkit") != std::wstring::npos &&
				key.find(L"tag") != std::wstring::npos)
			{
				skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	for (auto weapon : weaponNames)
	{

		for (auto skin : weaponSkins[weapon])
		{
			if (skinMap[skin].paintkit < 1)
				continue;

			std::string converted_str = converter.to_bytes(weapon);
			weapon_skins[converted_str].push_back(skinMap[skin].paintkit);
		}
	}

	return 0;
}