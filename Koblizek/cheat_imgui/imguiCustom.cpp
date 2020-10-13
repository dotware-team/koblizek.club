#include "imgui.h"
#include "imgui_internal.h"

#include "imguiCustom.h"

void customcolor::render(const char* name, float color[3], bool* enable) noexcept
{
	ImGui::PushID(name);
	if (enable) {
		ImGui::Checkbox("##check", enable);
		ImGui::SameLine(0.0f, 5.0f);
	}
	bool openPopup = ImGui::ColorButton("##btn", ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::TextUnformatted(name);
	
	if (openPopup)
		ImGui::OpenPopup("##popup");

	if (ImGui::BeginPopup("##popup"))
	{
		ImGui::ColorPicker3("##picker", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::EndPopup();
	}
	ImGui::PopID();
}