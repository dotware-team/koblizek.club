#pragma once
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <vector>
namespace ImGui {
	bool ToggleButton(const char * label, bool * v, const ImVec2 & size_arg = ImVec2(0, 0));
	// Combo box helper allowing to pass an array of strings.
	bool Combo(const char * label, int * currIndex, std::vector<std::string>& values);
	bool BeginGroupBox(const char * name, const ImVec2 & size_arg = ImVec2(0, 0));
	void EndGroupBox();
	void DrawGroupName(const char * label);
}