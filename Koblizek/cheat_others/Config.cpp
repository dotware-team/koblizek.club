#include <fstream>
#include <ShlObj.h>
#include "../cheat_others/ArchiveX.h"
#include "../cheat_others/Config.h"
#include "../cheat_imgui/imgui.h"
#include "../obfuscation.hpp"
#include "../instr.h"

#define ENCRYPT_START OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN
#define ENCRYPT_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END

Config::Config(const char* name) noexcept
{
	if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
		path = pathToDocuments;
		path /= name;
		CoTaskMemFree(pathToDocuments);
	}

	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}
}

void Config::load(size_t id) noexcept
{
	std::ifstream in{ path / configs[id] };

	ImGuiStyle& style = ImGui::GetStyle();

	ArchiveX<std::ifstream>{ in } >> koblizek_aimbot >> koblizek_backtrack >> koblizek_glow >> koblizek_chams >> koblizek_visuals >> koblizek_misc >> skinChanger >> koblizek_esp >> koblizek_triggerbot >> style;
}

void Config::save(size_t id) const noexcept
{
	std::ofstream out{ path / configs[id] };

	ImGuiStyle& style = ImGui::GetStyle();

	ArchiveX<std::ofstream>{ out } << koblizek_aimbot << koblizek_backtrack << koblizek_glow << koblizek_chams << koblizek_visuals << koblizek_misc << skinChanger << koblizek_esp << koblizek_triggerbot << style;
}

void Config::add(const char* name) noexcept
{
	if (*name && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
		configs.emplace_back(name);
}

void Config::remove(size_t id) noexcept
{
	std::filesystem::remove(path / configs[id]);
	configs.erase(configs.cbegin() + id);
}

void Config::rename(size_t item, const char* newName) noexcept
{
	std::filesystem::rename(path / configs[item], path / newName);
	configs[item] = newName;
}

void Config::reset() noexcept
{
	koblizek_aimbot = { };
	koblizek_backtrack = { };
	koblizek_glow = { };
	koblizek_chams = { };
	koblizek_visuals = { };
	skinChanger = { };
	koblizek_misc = { };
	koblizek_esp = { };
	koblizek_triggerbot = { };
}

void Config::listConfigs() noexcept
{
	configs.clear();

	std::error_code ec;
	std::transform(std::filesystem::directory_iterator{ path, ec },
		std::filesystem::directory_iterator{ },
		std::back_inserter(configs),
		[](const auto& entry) { return std::string{ (const char*)entry.path().filename().u8string().c_str() }; });
}