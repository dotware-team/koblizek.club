#include <Windows.h>
#include <string.h>
#include "cheat_imgui/imgui.h"
#include "cheat_important/GUI.h"
#include "cheat_others/Config.h"
#include "cheat_features/Misc.h"
#include "cheat_features/Visuals.h"
#include "cheat_important/Hooks.h"
#include "cheat_imgui/imguiCustom.h"
#include "cheat_sdk//ConVar.h"
#include "cheat_features/SkinChanger.h"
#include <functional>
#include "cheat_sdk/InputSystem.h"
#include "cheat_imgui/imgui_internal.h"
#include "cheat_imgui/imgui_custom2.h"
#include "cheat_imgui/imgui_stdlib.h"
#include "cheat_important/Interfaces.h"
#include "cheat_others/font-flags.h"
#include "cheat_sdk/Surface.h"
#include "instr.h"
#include "obfuscation.hpp"
#include "cheat_interface/notifications.hpp"v
#include "cheat_protection/xor.hpp"
#include "skin_parser.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#define UNLEN 256

//DEFINE FONTS
ImFont* tabfont;
ImFont* menufont;
ImFont* subicon;
ImFont* titlefont;
ImFont* information_font;
extern LPDIRECT3DTEXTURE9 m_skin_texture = nullptr;
//END DEFINE FONTSa

bool CopyImageToTexture(UCHAR* pSrc, int xWidth, int yHeight, LPDIRECT3DTEXTURE9 pTex, int xPos, int yPos)
{
	D3DLOCKED_RECT d3drc;
	UCHAR r, g, b, a;
	UINT* pDest;
	int nRow, nPixel;

	// lock the texture

	if (FAILED(pTex->LockRect(0, &d3drc, NULL, 0)))
		return false;

	// adjust pitch from bytes to UINTs

	d3drc.Pitch >>= 2;

	// copy the image

	for (nRow = 0; nRow < yHeight; nRow++)
	{
		// set destination pointer for this row

		pDest = (UINT*)d3drc.pBits + (nRow + yPos) * d3drc.Pitch + xPos;

		// copy the row

		for (nPixel = 0; nPixel < xWidth; nPixel++)
		{
			// extract pixel data

			r = *pSrc++;
			g = *pSrc++;
			b = *pSrc++;
			a = *pSrc++;

			// write color word to texture

			(*pDest++) = b | (g << 8) | (r << 16) | (a << 24);
		}
	}

	// unlock texture

	pTex->UnlockRect(0);

	// return success

	return true;
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
	auto& style = ImGui::GetStyle();
	bool values[N] = { false };
	values[activetab] = true;
	style.Colors[ImGuiCol_Button] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	for (auto i = 0; i < N; ++i) {
		if (activetab == i)
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		}
		else
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		}
		if (ImGui::Button(names[i], ImVec2{ w, h })) {
			activetab = i;
		}
		if (sameline && i < N - 1)
			ImGui::SameLine();
	}
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
}

enum TeamID
{
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

void GUI::bind(int& key) noexcept
{
	key ? ImGui::TextColored(ImVec4(54, 54, 54, 255), XorStr("[%s]"), interfaces.inputSystem->virtualKeyToString(key)) : ImGui::TextColored(ImVec4(255, 242, 242, 255), XorStr("[-]"));

	if (!ImGui::IsItemHovered())
		return;

	ImGuiIO& keys = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(keys.KeysDown); i++)
		if (ImGui::IsKeyPressed(i) && i != config.koblizek_misc.menuKey)
			key = i != VK_ESCAPE ? i : 0;

	for (int i = 0; i < IM_ARRAYSIZE(keys.MouseDown); i++)
		if (ImGui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != config.koblizek_misc.menuKey)
			key = i + (i > 1 ? 2 : 1);
}

void GUI::render(IDirect3DDevice9* device) noexcept
{
	menurender(device);
}

void render_info()
{
	ImVec2 win_pos = ImGui::GetWindowPos();
	const auto window_size = ImVec2(715.f, 470.f);

	static char info[256];
	sprintf(info, XorStr("V2 for Counter-Strike: Global Offensive"));
	static const auto info_size = ImGui::CalcTextSize(info);

	static char website[256];
	sprintf(website, XorStr("https://koblizek.club"));
	static const auto website_size = ImGui::CalcTextSize(website);

	auto draw_tool = ImGui::GetWindowDrawList();
	const auto background_color = ImGui::GetColorU32(ImVec4(0.07f, 0.07f, 0.07f, 1.00f));
	const auto text_color = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 1.f));
	const auto start = ImVec2(win_pos.x, win_pos.y + window_size.y);
	const auto end = ImVec2(start.x + window_size.x, start.y - info_size.y - 4.f);

	draw_tool->AddRectFilled(start, end, background_color);
	draw_tool->AddText(ImVec2(start.x + 20.5f, start.y - info_size.y - 5.f), text_color, info);
	draw_tool->AddText(ImVec2(end.x - 20.5f - website_size.x, start.y - website_size.y - 5.f), text_color, website);
}

void GUI::menurender(IDirect3DDevice9* device) noexcept
{
	/* DEFINITION */
	static int currentCategory{ 0 };
	static int currentespCategory = 0;
	static int currentespItem = 0;
	static int currentChamsCategory = 1;
	static int currentChamsItem = 0;
	static int currentChamsType = 0;
	int selected = currentespCategory * 3 + currentespItem * 1;
	/* END */

	/* Menu created by stuffy / stuffy#2444 */
	ImGui::SetNextWindowSize(ImVec2(715, 470)); /* x , y */
	ImGui::PushFont(titlefont);
	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(5, 20);
	style.FrameBorderSize = 0.f;
	style.ChildBorderSize = 1.f;
	style.WindowBorderSize = 1.5f;
	style.ScrollbarSize = 12.f;
	ImGui::Begin(XorStr("Koblizek.club"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);  /* Isnt allowed resize, collapse, scrollbar */
	{
		style.FramePadding = ImVec2(5, 5);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.085f, 0.085f, 0.085f, 1.00f);
		/* Little Changer end */

		/* Buttons */
		ImGui::PushFont(tabfont);
		if (ImGui::Button(XorStr("H"), ImVec2(106.f, 30.f))) menu_selected = TAB1; ImGui::SameLine(); //AIMBOT
		if (ImGui::Button(XorStr("D"), ImVec2(106.f, 30.f))) menu_selected = TAB2; ImGui::SameLine(); //VISUALS
		if (ImGui::Button(XorStr("E"), ImVec2(106.f, 30.f))) menu_selected = TAB3; ImGui::SameLine(); //MISC
		if (ImGui::Button(XorStr("F"), ImVec2(106.f, 30.f))) menu_selected = TAB4; ImGui::SameLine(); //EXTRAS
		if (ImGui::Button(XorStr("B"), ImVec2(106.f, 30.f))) menu_selected = TAB5; ImGui::SameLine(); //Skins
		if (ImGui::Button(XorStr("G"), ImVec2(104.8f, 30.f))) menu_selected = TAB6; //CFGS
		ImGui::PopFont();
		/* Buttons End */

		switch (menu_selected)
		{
		case TAB1:
		{
			ImGui::BeginChild(XorStr("###tabulka1"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
			{
				static int Pistol{ 0 };
				static int Rifle{ 1 };
				static int SMG{ 2 };
				static int ShotGun{ 3 };
				static int Sniper{ 4 };
				static int selected_tab = 0;
				ImGui::PushFont(subicon);
				static char* tabs_name[] = { (char*)"G", (char*)"W", (char*)"K", (char*)"b", (char*)"Z", (char*)"p" };
				float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
				style.WindowPadding = ImVec2(0, 0);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				{
					ImGui::BeginChild(XorStr("##Tabs"), ImVec2(0, 26), true);
					render_tabs(tabs_name, selected_tab, group_w / _countof(tabs_name), 25.0f, true);
					ImGui::EndChild();
					ImGui::BeginChild(XorStr("##Make_Space"), ImVec2(0, 25), false);
					ImGui::EndChild();
					style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				}
				ImGui::PopStyleVar();
				ImGui::PopFont();
				style.WindowPadding = ImVec2(20, 20);
				ImGui::Columns(2, nullptr, false);
				ImGui::BeginChild(XorStr("##aimbot_tab1"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
				{
					if (selected_tab == 0)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Pistol"));
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled"), &config.koblizek_aimbot[Pistol].enabled);
						ImGui::SameLine();
						bind(config.koblizek_aimbot[Pistol].key);
						ImGui::PushID(0);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Pistol].fov, 0.0f, 90.f, XorStr("Fov: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(1);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Pistol].smooth, 1.0f, 100.f, XorStr("Smooth: %.1f"));
						ImGui::PopID();
						ImGui::Checkbox(XorStr("Silent"), &config.koblizek_aimbot[Pistol].silent);
						ImGui::Checkbox(XorStr("Visible Only"), &config.koblizek_aimbot[Pistol].visibleOnly);
						ImGui::Checkbox(XorStr("Ignore Smoke"), &config.koblizek_aimbot[Pistol].ignoreSmoke);
						ImGui::Checkbox(XorStr("Jump Check"), &config.koblizek_aimbot[Pistol].jumpCheck);
						ImGui::Spacing();
						ImGui::Checkbox(XorStr("RCS"), &config.koblizek_aimbot[Pistol].recoilbasedFov);
						ImGui::SameLine();
						ImGui::PushID(2);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Pistol].recoilControlX, 0.0f, 1.0f, XorStr("RCS X: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(3);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Pistol].recoilControlY, 0.0f, 1.0f, XorStr("RCS Y: %.1f"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 1)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Rifle"));
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled##1"), &config.koblizek_aimbot[Rifle].enabled);
						ImGui::SameLine();
						bind(config.koblizek_aimbot[Rifle].key);
						ImGui::PushID(4);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Rifle].fov, 0.0f, 90.f, XorStr("Fov: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(5);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Rifle].smooth, 1.0f, 100.f, XorStr("Smooth: %.1f"));
						ImGui::PopID();
						ImGui::Checkbox(XorStr("Silent##1"), &config.koblizek_aimbot[Rifle].silent);
						ImGui::Checkbox(XorStr("Visible Only##1"), &config.koblizek_aimbot[Rifle].visibleOnly);
						ImGui::Checkbox(XorStr("Ignore Smoke##1"), &config.koblizek_aimbot[Rifle].ignoreSmoke);
						ImGui::Checkbox(XorStr("Jump Check##1"), &config.koblizek_aimbot[Rifle].jumpCheck);
						ImGui::Spacing();
						ImGui::Checkbox(XorStr("RCS##1"), &config.koblizek_aimbot[Rifle].recoilbasedFov);
						ImGui::SameLine();
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(6);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Rifle].recoilControlX, 0.0f, 1.0f, XorStr("RCS X: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(7);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Rifle].recoilControlY, 0.0f, 1.0f, XorStr("RCS Y: %.1f"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 2)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("SMG"));
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled##2"), &config.koblizek_aimbot[SMG].enabled);
						ImGui::SameLine();
						bind(config.koblizek_aimbot[SMG].key);
						ImGui::PushID(8);
						ImGui::SliderFloat("", &config.koblizek_aimbot[SMG].fov, 0.0f, 90.f, XorStr("Fov: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(9);
						ImGui::SliderFloat("", &config.koblizek_aimbot[SMG].smooth, 1.0f, 100.f, XorStr("Smooth: %.1f"));
						ImGui::PopID();
						ImGui::Checkbox(XorStr("Silent##2"), &config.koblizek_aimbot[SMG].silent);
						ImGui::Checkbox(XorStr("Visible Only##2"), &config.koblizek_aimbot[SMG].visibleOnly);
						ImGui::Checkbox(XorStr("Ignore Smoke##2"), &config.koblizek_aimbot[SMG].ignoreSmoke);
						ImGui::Checkbox(XorStr("Jump Check##2"), &config.koblizek_aimbot[SMG].jumpCheck);
						ImGui::Spacing();
						ImGui::Checkbox(XorStr("RCS##2"), &config.koblizek_aimbot[SMG].recoilbasedFov);
						ImGui::SameLine();
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(10);
						ImGui::SliderFloat("", &config.koblizek_aimbot[SMG].recoilControlX, 0.0f, 1.0f, XorStr("RCS X: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(11);
						ImGui::SliderFloat("", &config.koblizek_aimbot[SMG].recoilControlY, 0.0f, 1.0f, XorStr("RCS Y: %.1f"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 3)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("ShotGun"));
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled##3"), &config.koblizek_aimbot[ShotGun].enabled);
						ImGui::SameLine();
						bind(config.koblizek_aimbot[ShotGun].key);
						ImGui::PushID(12);
						ImGui::SliderFloat("", &config.koblizek_aimbot[ShotGun].fov, 0.0f, 90.f, XorStr("Fov: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(13);
						ImGui::SliderFloat("", &config.koblizek_aimbot[ShotGun].smooth, 1.0f, 100.f, XorStr("Smooth: %.1f"));
						ImGui::PopID();
						ImGui::Checkbox(XorStr("Silent##3"), &config.koblizek_aimbot[ShotGun].silent);
						ImGui::Checkbox(XorStr("Visible Only##3"), &config.koblizek_aimbot[ShotGun].visibleOnly);
						ImGui::Checkbox(XorStr("Ignore Smoke##3"), &config.koblizek_aimbot[ShotGun].ignoreSmoke);
						ImGui::Checkbox(XorStr("Jump Check##3"), &config.koblizek_aimbot[ShotGun].jumpCheck);
						ImGui::Spacing();
						ImGui::Checkbox(XorStr("RCS##3"), &config.koblizek_aimbot[ShotGun].recoilbasedFov);
						ImGui::SameLine();
						ImGui::PushID(14);
						ImGui::SliderFloat("", &config.koblizek_aimbot[ShotGun].recoilControlX, 0.0f, 1.0f, XorStr("RCS X: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(15);
						ImGui::SliderFloat("", &config.koblizek_aimbot[ShotGun].recoilControlY, 0.0f, 1.0f, XorStr("RCS Y: %.1f"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 4)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Sniper"));
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled##4"), &config.koblizek_aimbot[Sniper].enabled);
						ImGui::SameLine();
						bind(config.koblizek_aimbot[Sniper].key);
						ImGui::PushID(16);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Sniper].fov, 0.0f, 90.f, XorStr("Fov: %.1f"));
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::PushID(17);
						ImGui::SliderFloat("", &config.koblizek_aimbot[Sniper].smooth, 1.0f, 100.f, XorStr("Smooth: %.1f"));
						ImGui::PopID();
						ImGui::Checkbox(XorStr("Silent##4"), &config.koblizek_aimbot[Sniper].silent);
						ImGui::Checkbox(XorStr("Visible Only##4"), &config.koblizek_aimbot[Sniper].visibleOnly);
						ImGui::Checkbox(XorStr("Ignore Smoke##4"), &config.koblizek_aimbot[Sniper].ignoreSmoke);
						ImGui::Checkbox(XorStr("Scoped Only##4"), &config.koblizek_aimbot[Sniper].scopedOnly);
						ImGui::Spacing();
						ImGui::Checkbox(XorStr("Jump Check##4"), &config.koblizek_aimbot[Sniper].jumpCheck);
						ImGui::PopFont();
					}
					else if (selected_tab == 5)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("General"));
						ImGui::Checkbox(XorStr("Backtrack"), &config.koblizek_backtrack.enabled);
						if (config.koblizek_backtrack.enabled)
						{
							ImGui::SameLine();
							ImGui::PushItemWidth(90.f);
							ImGui::SliderInt("", &config.koblizek_backtrack.timeLimit, 1, 200, XorStr("%d ms"));
						}
						ImGui::PopFont();
					}
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				ImGui::BeginChild(XorStr("##aimbot_tab2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					if (selected_tab == 0)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Triggerbot"));
						ImGui::Checkbox(XorStr("Enabled"), &config.koblizek_triggerbot[Pistol].enabled);
						ImGui::SameLine();
						bind(config.koblizek_triggerbot[Pistol].key);
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(30);
						ImGui::SliderInt(XorStr("Delay"), &config.koblizek_triggerbot[Pistol].shotDelay, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::Spacing();
						ImGui::PushID(31);
						ImGui::SliderInt(XorStr("Hit Chance"), &config.koblizek_triggerbot[Pistol].hitChanceAmmount, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 1)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Triggerbot"));
						ImGui::Checkbox(XorStr("Enabled##1"), &config.koblizek_triggerbot[Rifle].enabled);
						ImGui::SameLine();
						bind(config.koblizek_triggerbot[Rifle].key);
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(32);
						ImGui::SliderInt(XorStr("Delay##1"), &config.koblizek_triggerbot[Rifle].shotDelay, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::Spacing();
						ImGui::PushID(33);
						ImGui::SliderInt(XorStr("Hit Chance##1"), &config.koblizek_triggerbot[Rifle].hitChanceAmmount, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 2)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Triggerbot"));
						ImGui::Checkbox(XorStr("Enabled##2"), &config.koblizek_triggerbot[SMG].enabled);
						ImGui::SameLine();
						bind(config.koblizek_triggerbot[SMG].key);
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(34);
						ImGui::SliderInt(XorStr("Delay##2"), &config.koblizek_triggerbot[SMG].shotDelay, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::Spacing();
						ImGui::PushID(35);
						ImGui::SliderInt(XorStr("Hit Chance##2"), &config.koblizek_triggerbot[SMG].hitChanceAmmount, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 3)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Triggerbot"));
						ImGui::Checkbox(XorStr("Enabled##3"), &config.koblizek_triggerbot[ShotGun].enabled);
						ImGui::SameLine();
						bind(config.koblizek_triggerbot[ShotGun].key);
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(36);
						ImGui::SliderInt(XorStr("Delay##3"), &config.koblizek_triggerbot[ShotGun].shotDelay, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::Spacing();
						ImGui::PushID(37);
						ImGui::SliderInt(XorStr("Hit Chance##3"), &config.koblizek_triggerbot[ShotGun].hitChanceAmmount, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 4)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Triggerbot"));
						ImGui::Checkbox(XorStr("Enabled##4"), &config.koblizek_triggerbot[Sniper].enabled);
						ImGui::SameLine();
						bind(config.koblizek_triggerbot[Sniper].key);
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(38);
						ImGui::SliderInt(XorStr("Delay##4"), &config.koblizek_triggerbot[Sniper].shotDelay, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::Spacing();
						ImGui::PushID(39);
						ImGui::SliderInt(XorStr("Hit Chance##4"), &config.koblizek_triggerbot[Sniper].hitChanceAmmount, 0, 100, XorStr("%d"));
						ImGui::PopID();
						ImGui::PopFont();
					}
					else if (selected_tab == 5)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Extra"));
						ImGui::PopFont();
					}
				}
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		break;

		/* Buttons */
		case TAB2:
		{
			ImGui::BeginChild(XorStr("###tabulka2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
			{
				ImGui::PushFont(menufont);
				static int selected_tab = 0;
				static char* tabs_name[] = { (char*)"Chams", (char*)"Glow", (char*)"ESP", (char*)"Extra" };
				float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
				style.WindowPadding = ImVec2(0, 0);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				{
					ImGui::BeginChild(XorStr("##Tabs"), ImVec2(0, 26), true);
					render_tabs(tabs_name, selected_tab, group_w / _countof(tabs_name), 25.0f, true);
					ImGui::EndChild();
					ImGui::BeginChild(XorStr("##Make_Space"), ImVec2(0, 25), false);
					ImGui::EndChild();
					style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				}
				ImGui::PopStyleVar();
				ImGui::PopFont();
				style.WindowPadding = ImVec2(20, 20);
				ImGui::Columns(2, nullptr, false);
				ImGui::BeginChild(XorStr("##body_contentt"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					if (selected_tab == 0)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Chams"));
						ImGui::Separator();
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled"), &config.koblizek_chams[currentChamsItem].enabled);
						ImGui::SameLine(0.0f, 180.f);
						customcolor::render("", config.koblizek_chams[currentChamsItem].color);
						ImGui::Checkbox(XorStr("Health based"), &config.koblizek_chams[currentChamsItem].healthBased);

						ImGui::Spacing();

						ImGui::Text(XorStr("Configuration"));
						ImGui::Separator();
						ImGui::Combo(XorStr("##8811"), &config.koblizek_chams[currentChamsItem].material, XorStr("Normal\0Flat\0Glow\0"));
						ImGui::PushItemWidth(90.0f);
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Combo(XorStr("##9563"), &currentChamsCategory, XorStr("Allies\0Enemies\0Backtrack\0"));
						ImGui::SameLine(0.0f, 5.0f);
						if (currentChamsCategory <= 3) {
							ImGui::PushItemWidth(80.0f);
							ImGui::Combo("##4789", &currentChamsType, XorStr("Visible\0Non Visible\0"));
							currentChamsItem = currentChamsCategory * 2 + currentChamsType;
						}
						else {
							currentChamsItem = currentChamsCategory + 4;
						}

						ImGui::PopStyleVar();
						ImGui::PopFont();
					}
					else if (selected_tab == 1)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Glow"));
						ImGui::Separator();
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
						static int currentGlowCategory{ 1 };
						static int currentGlowItem{ 0 };
						static int currentGlowType{ 0 };
						ImGui::PushItemWidth(90.f);
						ImGui::Checkbox(XorStr("Enabled##8777"), &config.koblizek_glow[currentGlowItem].enabled);
						ImGui::SameLine(0.0f, 180.f);
						customcolor::render("", config.koblizek_glow[currentGlowItem].color);
						ImGui::Checkbox(XorStr("Health based##8880"), &config.koblizek_glow[currentGlowItem].healthBased);

						ImGui::Spacing();

						ImGui::Text(XorStr("Configuration"));
						ImGui::Separator();
						ImGui::PushItemWidth(90.f);
						ImGui::PushID(6);
						ImGui::Combo(XorStr("##2020"), &currentGlowCategory, XorStr("Allies\0Enemies\0"));
						ImGui::PopID();
						ImGui::SameLine(0.0f, 5.0f);
						if (currentGlowCategory <= 3) {
							ImGui::PushItemWidth(90.f);
							ImGui::PushID(7);
							ImGui::Combo(XorStr("##8888"), &currentGlowType, XorStr("Visible\0Non Visible\0"));
							ImGui::PopID();
							currentGlowItem = currentGlowCategory * 2 + currentGlowType;
						}
						else {
							currentGlowItem = currentGlowCategory + 4;
						}
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::PushItemWidth(80.0f);
						ImGui::PushID(8);
						ImGui::SliderFloat(XorStr("##3333"), &config.koblizek_glow[currentGlowItem].thickness, 0.0f, 1.0f, XorStr("Alpha: %.2f"));
						ImGui::PopID();
						ImGui::PopStyleVar();
						ImGui::PopFont();
					}
					else if (selected_tab == 2)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Players"));
						ImGui::Separator();
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
						ImGui::Checkbox(XorStr("Enabled##3"), &config.koblizek_esp.players[3].enabled);
						customcolor::render(XorStr("Box"), config.koblizek_esp.players[3].box.color, &config.koblizek_esp.players[3].box.enabled);
						ImGui::SameLine();
						ImGui::PushItemWidth(110.f);
						ImGui::Combo(XorStr("##3"), &config.koblizek_esp.players[3].boxType, XorStr("2D\0""2D Corners\0""3D\0"));
						ImGui::Checkbox(XorStr("Name"), &config.koblizek_esp.players[3].name);
						ImGui::Checkbox(XorStr("Health"), &config.koblizek_esp.players[3].health);
						ImGui::Checkbox(XorStr("Weapon"), &config.koblizek_esp.players[3].activeWeapon);
						ImGui::SameLine(0.0f, 180.f);
						customcolor::render("", config.koblizek_esp.players[3].weapon_icon_color);
						ImGui::PopStyleVar();
						ImGui::PopFont();
					}
					else if (selected_tab == 3)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Extra"));
						ImGui::Separator();
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
						ImGui::Checkbox(XorStr("Radar"), &config.koblizek_misc.radarHack);
						ImGui::Checkbox(XorStr("Grenade Prediction"), &config.koblizek_misc.GrenadePrediction);
						ImGui::Checkbox(XorStr("Force Crosshair"), &config.koblizek_misc.forceCrosshair);
						ImGui::Checkbox(XorStr("Show Impacts"), &config.koblizek_misc.ShowImpacts);
						ImGui::Checkbox(XorStr("Full Bright"), &config.koblizek_misc.FullBright);
						ImGui::Checkbox(XorStr("Indicators"), &config.koblizek_misc.Indicators);
						ImGui::Separator();
						ImGui::PushItemWidth(-1);
						ImGui::PushID(480);
						ImGui::Combo(XorStr("##SkyBox"), &config.koblizek_visuals.skybox, XorStr("Default\0cs_baggage_skybox_\0cs_tibet\0embassy\0italy\0jungle\0nukeblank\0office\0sky_cs15_daylight01_hdr\0sky_cs15_daylight02_hdr\0sky_cs15_daylight03_hdr\0sky_cs15_daylight04_hdr\0sky_csgo_cloudy01\0sky_csgo_night_flat\0sky_csgo_night02\0sky_day02_05_hdr\0sky_day02_05\0sky_dust\0sky_l4d_rural02_ldr\0sky_venice\0vertigo_hdr\0vertigo\0vertigoblue_hdr\0vietnam\0sky_lunacy\0"));
						ImGui::PopID();
						ImGui::PopItemWidth();
						ImGui::Separator();
						ImGui::PushItemWidth(-1);
						ImGui::PushID(423);
						ImGui::SliderInt(XorStr("##FoV"), &config.koblizek_visuals.viewmodelFov, -60, 100, XorStr("%.2f (FOV)"));
						ImGui::PopID();
						ImGui::PushItemWidth(-1);
						ImGui::PushID(428);
						ImGui::SliderInt(XorStr("##AR"), &config.koblizek_misc.aspectratio, 0.0f, 7.0f, XorStr("%.2f (Aspect Ratio)"));
						ImGui::PopID();
						ImGui::Separator();
						ImGui::PushID(429);
						ImGui::SliderFloat(XorStr("##ViewmodelOffsetX"), &config.koblizek_misc.viewmodel_offset_x, -30.f, 30.f, XorStr("Offset X: %.1f"));
						ImGui::PopID();
						ImGui::PushID(430);
						ImGui::SliderFloat(XorStr("##ViewmodelOffsetY"), &config.koblizek_misc.viewmodel_offset_y, -30.f, 30.f, XorStr("Offset Y: %.1f"));
						ImGui::PopID();
						ImGui::PushID(431);
						ImGui::SliderFloat(XorStr("##ViewmodelOffsetZ"), &config.koblizek_misc.viewmodel_offset_z, -30.f, 30.f, XorStr("Offset Z: %.1f"));
						ImGui::PopID();
						ImGui::Separator();

						ImGui::PopStyleVar();
						ImGui::PopFont();
					}
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				ImGui::BeginChild(XorStr("##body_contentt2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					if (selected_tab == 0)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Visual Preview"));
						if (!config.koblizek_chams[currentChamsItem].enabled)
						{
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
							ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2.f - 114.f / 2.f);
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetColumnWidth() / 2.f - 200.f / 2.f);
							ImGui::Image(model_img, ImVec2(114.f, 200.f));
							ImGui::PopStyleVar();
							ImGui::PopStyleVar();
						}
						else
						{
							if (config.koblizek_chams[currentChamsItem].material == 1)
							{
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2.f - 114.f / 2.f);
								ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetColumnWidth() / 2.f - 200.f / 2.f);
								ImGui::Image(flat_img, ImVec2(114.f, 200.f), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(config.koblizek_chams[currentChamsItem].color[0], config.koblizek_chams[currentChamsItem].color[1], config.koblizek_chams[currentChamsItem].color[2], 1.f)); //Flat chams + apply color modulate from flat chams in Visuals tab.
							}
							else
							{
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2.f - 114.f / 2.f);
								ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetColumnWidth() / 2.f - 200.f / 2.f);
								ImGui::Image(chams_img, ImVec2(114.f, 200.f), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(config.koblizek_chams[currentChamsItem].color[0], config.koblizek_chams[currentChamsItem].color[1], config.koblizek_chams[currentChamsItem].color[2], 1.f));
							}
						}
						ImGui::PopFont();
					}
					else if (selected_tab == 1) //Glow
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Visual Preview"));
						ImGui::Text(XorStr("There be preview for the Glow"), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
						ImGui::PopFont();
					}
					else if (selected_tab == 2) //ESP
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("World"));
						ImGui::Separator();
						ImGui::Text(XorStr("There is nothing"));
						ImGui::PopFont();
					}
					else if (selected_tab == 3)
					{
						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Misc"));
						ImGui::Separator();
						ImGui::Checkbox(XorStr("Remove Post-Processing"), &config.koblizek_misc.fpsboost);
						ImGui::Checkbox(XorStr("Remove Shadows"), &config.koblizek_misc.removeShadows);
						ImGui::Checkbox(XorStr("Remove Blood"), &config.koblizek_misc.removeBlood);
						ImGui::Checkbox(XorStr("Remove Sky"), &config.koblizek_misc.nosky);
						ImGui::Separator();
						ImGui::Checkbox(XorStr("No smoke"), &config.koblizek_misc.noSmoke);
						ImGui::Checkbox(XorStr("Wireframe Smoke"), &config.koblizek_misc.wireframeSmoke);
						ImGui::Separator();
						ImGui::PopFont();
					}
				}
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.085f, 0.085f, 0.085f, 1.00f);
			ImGui::EndChild();
		}
		break;
		/* Buttons end */

		/* Misc */
		case TAB3:
		{
			ImGui::BeginChild(XorStr("###tabulka5"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImGui::Columns(2, nullptr, false);
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				ImGui::BeginChild(XorStr("##misc_content"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Misc"));
					ImGui::Checkbox(XorStr("Bunny Hop"), &config.koblizek_misc.bunnyHop);
					ImGui::Checkbox(XorStr("Auto Accept"), &config.koblizek_misc.autoAccept);
					ImGui::Checkbox(XorStr("Clan Tag (Sync)"), &config.koblizek_misc.Clantag);
					ImGui::Checkbox(XorStr("Reveal ranks"), &config.koblizek_misc.revealRanks);
					ImGui::Checkbox(XorStr("Bomb Timer"), &config.koblizek_misc.bombTimer);
					ImGui::Checkbox(XorStr("Overwatch Revealer"), &config.koblizek_visuals.overwatch);
					ImGui::Checkbox(XorStr("Velocity Indicator"), &config.koblizek_misc.VelocityIndicator);
					ImGui::Checkbox(XorStr("Velocity Graph"), &config.koblizek_misc.velocity_graph_enabled);
					if (config.koblizek_misc.velocity_graph_enabled)
					{
						ImGui::SameLine();
						ImGui::PushID(667);
						customcolor::render("", config.koblizek_misc.velocity_graph_color);
						ImGui::PopID();
					}
					ImGui::Checkbox(XorStr("Jump Trail"), &config.koblizek_misc.trail_enabled);
					if (config.koblizek_misc.trail_enabled)
					{
						ImGui::SameLine();
						ImGui::PushID(513);
						customcolor::render("", config.koblizek_misc.trail_color);
						ImGui::PopID();
						ImGui::PushID(555);
						ImGui::SliderFloat(XorStr("##Duration"), &config.koblizek_misc.trail_duration, 1.0f, 5.f, XorStr("Trail Duratin: %.1f"));
						ImGui::PopID();
					}
					ImGui::Checkbox(XorStr("Bullet Beams"), &config.koblizek_misc.bullet_beams_enabled);
					if (config.koblizek_misc.bullet_beams_enabled)
					{
						ImGui::SameLine();
						ImGui::PushID(514);
						customcolor::render("", config.koblizek_misc.bullet_beams_color);
						ImGui::PopID();
						ImGui::PushID(516);
						ImGui::SliderFloat(XorStr("##Duration"), &config.koblizek_misc.bullet_beams_duration, 1.0f, 5.f, XorStr("Beams Duration: %.1f"));
						ImGui::PopID();
					}
					ImGui::Checkbox(XorStr("Visualize Sound"), &config.koblizek_misc.sound_esp_enabled);
					if (config.koblizek_misc.sound_esp_enabled)
					{
						ImGui::SameLine();
						ImGui::PushID(515);
						customcolor::render("", config.koblizek_misc.sound_esp_color);
						ImGui::PopID();
					}
					ImGui::Checkbox(XorStr("Fake prime"), &config.koblizek_misc.fakePrime);
					ImGui::PushItemWidth(87.0f);
					ImGui::InputInt(XorStr("##FakelagAmount"), &config.koblizek_misc.chokedPackets, 1, 12);
					config.koblizek_misc.chokedPackets = std::clamp(config.koblizek_misc.chokedPackets, 0, 12);
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(100.f);
					static const char* Spammers[]{ "None", "Chat Break", "Radio Spammer", "Cheat Spammer", "Trust Factor" };
					ImGui::Combo(XorStr("##SpamSelect"), &config.koblizek_misc.spam, Spammers, IM_ARRAYSIZE(Spammers));
					ImGui::PopItemWidth();

					ImGui::PopFont();
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				ImGui::BeginChild(XorStr("##misc_content2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Extra"));
					ImGui::Checkbox(XorStr("Spectator List"), &config.koblizek_misc.SpectatorList);
					ImGui::Checkbox(XorStr("Remove Sleeves"), &config.koblizek_visuals.noSleeves);
					ImGui::Checkbox(XorStr("Hit Sound"), &config.koblizek_misc.hitSound);
					ImGui::Checkbox(XorStr("Hit Marker"), &config.koblizek_misc.hitmarker);
					ImGui::Checkbox(XorStr("Damage Log"), &config.koblizek_misc.DamageLog);
					ImGui::Checkbox(XorStr("Purchase Log"), &config.koblizek_misc.purchase_log);
					ImGui::Checkbox(XorStr("Edge Jump"), &config.koblizek_misc.EdgeJump);
					ImGui::SameLine();
					bind(config.koblizek_misc.EdgeJumpKey);
					if (config.koblizek_misc.EdgeJump)
					{
						ImGui::Checkbox(XorStr("Autostrafer"), &config.koblizek_misc.EdgeJumpAutostrafer);
					}
					ImGui::Checkbox(XorStr("Jump Bug"), &config.koblizek_misc.JumpBug);
					ImGui::SameLine();
					bind(config.koblizek_misc.JumpBugKey);
					ImGui::Checkbox(XorStr("Edge Bug"), &config.koblizek_misc.EdgeBug);
					ImGui::SameLine();
					bind(config.koblizek_misc.EdgeBugKey);
					ImGui::Checkbox(XorStr("Block Bot"), &config.koblizek_misc.BlockBot);
					ImGui::SameLine();
					bind(config.koblizek_misc.blockbotkey);
					ImGui::Checkbox(XorStr("Legit AA (Resolver)"), &config.koblizek_misc.legit_antiaim_resolver);
					ImGui::Checkbox(XorStr("Third Person"), &config.koblizek_visuals.thirdperson);
					if (config.koblizek_visuals.thirdperson)
					{
						ImGui::SameLine();
						bind(config.koblizek_visuals.thirdpersonKey);
					}
					ImGui::Checkbox(XorStr("Infinite Crouch"), &config.koblizek_misc.InfiniteCrouch);
					ImGui::Checkbox(XorStr("Fast Stop"), &config.koblizek_misc.fast_stop);
					ImGui::Checkbox(XorStr("Unlock Inventory"), &config.koblizek_misc.unlock_inventory);
					ImGui::Checkbox(XorStr("Bypass sv_pure"), &config.koblizek_misc.bypass_svpure);
					ImGui::Checkbox(XorStr("Watermark"), &config.koblizek_misc.watermark);
					ImGui::PushItemWidth(100.f);
					ImGui::PushID(10);
					ImGui::SliderFloat(XorStr("##Brightness"), &config.koblizek_visuals.brightness, 0.0f, 1.0f, XorStr("%.2f"));
					ImGui::PopID();
					ImGui::PopItemWidth();
					ImGui::SameLine();
					customcolor::render(XorStr("World Color"), config.koblizek_visuals.worldColor);

					if (ImGui::Button(XorStr("Update")))
						Visuals::scheduleUpdate();

					ImGui::SameLine();

					if (ImGui::Button(XorStr("Dump SteamID")))
						misc::DumpID();

					ImGui::PopFont();
				}
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.085f, 0.085f, 0.085f, 1.00f);
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
		break;
		/* Misc end */

		/* Extras */
		case TAB4:
		{
			ImGui::BeginChild(XorStr("###tabulka6"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
			{
				static int currentPlayer = -1;
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				ImGui::BeginGroupBox(XorStr("PlayerListArray"), ImVec2(0, ImGui::GetWindowSize().y / 2.60));
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Player list"));
					ImGui::PopFont();
					style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
					ImGui::BeginGroup();
					{
						ImGui::PushFont(menufont);
						ImGui::Columns(7, nullptr, false);
						style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
						ImGui::Text(XorStr("Name")); ImGui::NextColumn();
						ImGui::Text(XorStr("Team")); ImGui::NextColumn();
						ImGui::Text(XorStr("Money")); ImGui::NextColumn();
						ImGui::Text(XorStr("Clan")); ImGui::NextColumn();
						ImGui::Text(XorStr("Rank")); ImGui::NextColumn();
						ImGui::Text(XorStr("Wins")); ImGui::NextColumn();
						ImGui::Text(XorStr("Ping")); ImGui::NextColumn();
						ImGui::PopFont();
					}
					ImGui::EndGroup();

					ImGui::BeginGroup();
					{
						style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
						ImGui::BeginGroup();
						{
							ImGui::PushFont(menufont);
							ImGui::Columns(7, nullptr, false);
							style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
							std::unordered_map<int, std::vector<int>> players = { { TEAM_UNASSIGNED,{} }, { TEAM_SPECTATOR,{} }, { TEAM_TERRORIST,{} }, { TEAM_COUNTER_TERRORIST,{} }, };
							for (int i = 1; i < interfaces.engine->getMaxClients(); i++)
							{
								auto ent = (Entity*)interfaces.entityList->getEntity(i);
								auto g_LocalPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
								if (!ent)
									continue;

								if (ent == g_LocalPlayer)
									continue;

								players[ent->team()].push_back(i);
							}

							for (int team = TEAM_UNASSIGNED; team <= TEAM_COUNTER_TERRORIST; team++)
							{
								char* teamName = strdup("");
								switch (team)
								{
								case TEAM_SPECTATOR:
									teamName = strdup(XorStr("SPEC"));
									break;
								case TEAM_TERRORIST:
									teamName = strdup(XorStr("   T"));
									break;
								case TEAM_COUNTER_TERRORIST:
									teamName = strdup(XorStr(" CT"));
									break;
								}

								for (auto it : players[team])
								{
									std::string id = "";
									auto player = interfaces.entityList->getEntity(it);
									if (player)
									{
										if (player->isAlive())
											id = "Alive";
										else
											id = "Dead";
									}
									PlayerInfo entityInformation;
									interfaces.engine->getPlayerInfo(it, entityInformation);

									if (ImGui::Selectable(entityInformation.name, it == currentPlayer, ImGuiSelectableFlags_SpanAllColumns))
										currentPlayer = it;
									ImGui::NextColumn();

									ImGui::Text(XorStr("%s"), teamName);
									ImGui::NextColumn();

									ImGui::Text(XorStr("%d$"), player->accountID());
									ImGui::NextColumn();

									ImGui::Text(XorStr("%s"), (*memory.playerResource)->GetClan(it));
									ImGui::NextColumn();

									ImGui::Text(XorStr("  %s"), CSGOMMRanks[*(*memory.playerResource)->GetCompetitiveRanking(it)]);
									ImGui::NextColumn();

									ImGui::Text(XorStr(" %d"), *(*memory.playerResource)->GetCompetitiveWins(it));
									ImGui::NextColumn();

									ImGui::Text(XorStr(" %d"), (*memory.playerResource)->GetPing(it));
									ImGui::NextColumn();
								}
							}
							ImGui::PopFont();
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroup();
				}
				ImGui::EndGroupBox();

				if (!interfaces.engine->isInGame())
				{
					currentPlayer = -1;
				}

				auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
				auto player = interfaces.entityList->getEntity(currentPlayer);

				if (!local_player)
					return;

				if (!player)
				{
					currentPlayer = -1;
				}

				if (currentPlayer > -1)
				{
					ImGui::Columns(3, nullptr, false);

					ImGui::BeginGroupBox(XorStr("##PlayerInformations"), ImVec2(0, 0));
					{
						auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
						auto player = interfaces.entityList->getEntity(currentPlayer);

						if (!local_player)
							return;

						if (!player)
						{
							currentPlayer = -1;
						}

						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Informations"));
						PlayerInfo entityInformation;
						interfaces.engine->getPlayerInfo(currentPlayer, entityInformation);

						int steam_level = interfaces.steam_friends->GetFriendSteamLevel((CSteamID)(uint64)entityInformation.steamID64);
						ImGui::Text(XorStr("(S) Name: %s"), entityInformation.name);
						ImGui::Text(XorStr("(S) ID: %s"), entityInformation.steamIdString);
						ImGui::Text(XorStr("(S) Level: %d"), steam_level);

						if (player->team() == local_player->team())
						{
							if (ImGui::Button(XorStr("Vote Kick"), ImVec2(-1, 25)))
							{
								misc::Kick(entityInformation.userId);
							}
						}

						ImGui::PopFont();
					}
					ImGui::EndGroupBox();

					ImGui::NextColumn();

					ImGui::BeginGroupBox(XorStr("##PlayerStats"), ImVec2(0, 0));
					{
						auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
						auto player = interfaces.entityList->getEntity(currentPlayer);

						if (!local_player)
							return;

						if (!player)
						{
							currentPlayer = -1;
						}

						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Stats"));

						PlayerInfo entityInformation;
						interfaces.engine->getPlayerInfo(currentPlayer, entityInformation);

						ImGui::Text(XorStr("Kills: %d"), (*memory.playerResource)->GetKills(currentPlayer));
						ImGui::Text(XorStr("Assists: %d"), (*memory.playerResource)->GetAssists(currentPlayer));
						ImGui::Text(XorStr("Deaths: %d"), (*memory.playerResource)->GetDeaths(currentPlayer));
						ImGui::Text(XorStr("Score: %d"), (*memory.playerResource)->GetScore(currentPlayer));
						ImGui::Text(XorStr("MVPs: %d"), (*memory.playerResource)->GetMVPs(currentPlayer));

						ImGui::PopFont();
					}
					ImGui::EndGroupBox();

					ImGui::NextColumn();

					ImGui::BeginGroupBox(XorStr("##PlayerInfo2"), ImVec2(0, 0));
					{
						auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
						auto player = interfaces.entityList->getEntity(currentPlayer);

						if (!local_player)
							return;

						if (!player)
						{
							currentPlayer = -1;
						}

						ImGui::PushFont(menufont);
						ImGui::DrawGroupName(XorStr("Profile"));

						PlayerInfo entityInformation;
						interfaces.engine->getPlayerInfo(currentPlayer, entityInformation);

						std::vector<ImTextureID> texturesToRelease;
						int AvatarImage = interfaces.steam_friends->GetLargeFriendAvatar((CSteamID)(uint64)entityInformation.steamID64);
						if (AvatarImage > 0)
						{
							uint32 WIDTH = 0, HEIGHT = 0;
							interfaces.steam_utils->GetImageSize(AvatarImage, &WIDTH, &HEIGHT);

							if (WIDTH > 0 && HEIGHT > 0)
							{
								int sz = WIDTH * HEIGHT * 4;

								uint8* dest = (uint8*)malloc(sz);

								if (dest)
								{
									if (interfaces.steam_utils->GetImageRGBA(AvatarImage, dest, sz))
									{
										LPDIRECT3DTEXTURE9 texture;
										D3DXCreateTexture(device, WIDTH, HEIGHT, 0, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture);
										if (texture)
										{
											CopyImageToTexture(dest, WIDTH, HEIGHT, texture, 0, 0);

											ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2.f - 84.f / 2.f);

											ImGui::Image((void*)(uintptr_t)texture, ImVec2(72, 72));

											texturesToRelease.push_back((ImTextureID)(void*)(uintptr_t)texture);

										}
									}
									free(dest);
								}
							}
						}

						if (ImGui::Button(XorStr("Open Profile"), ImVec2(-1, 25)))
						{
							interfaces.steam_friends->ActivateGameOverlayToUser(XorStr("steamid"), (CSteamID)(uint64)entityInformation.steamID64);
							gui.isOpen = false;
						}
						ImGui::PopFont();
					}
					ImGui::EndGroupBox();
				}
			}
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.085f, 0.085f, 0.085f, 1.00f);
			ImGui::EndChild();
		}
		break;
		/* Extras end */

		/* Skinchanger Start*/
		case TAB5:
		{
			ImGui::BeginChild(XorStr("###tabulka8"), ImVec2(0, 0), true, ImGuiWindowFlags_NoResize);
			{
				ImGui::Columns(2, nullptr, false);
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				ImGui::BeginChild(XorStr("##skins_1"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Changer"));
					static auto itemIndex = 0;

					ImGui::PushItemWidth(-1);
					ImGui::Combo(XorStr("##ItemSelect"), &itemIndex, [](void* data, int idx, const char** out_text) {
						*out_text = game_data::weapon_names[idx].name;
						return true;
					}, nullptr, IM_ARRAYSIZE(game_data::weapon_names), 5);
					ImGui::PopItemWidth();

					auto& selected_entry = config.skinChanger.items[itemIndex];
					selected_entry.definition_vector_index = itemIndex;

					ImGui::PushItemWidth(-1);
					if (itemIndex == 0) {
						ImGui::Combo(XorStr("##KnifeSelect"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = game_data::knife_names[idx].name;
							return true;
						}, nullptr, IM_ARRAYSIZE(game_data::knife_names), 5);
					}
					else if (itemIndex == 1) {
						ImGui::Combo(XorStr("##GloveSelect"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = game_data::glove_names[idx].name;
							return true;
						}, nullptr, IM_ARRAYSIZE(game_data::glove_names), 5);
					}
					ImGui::PopItemWidth();

					ImGui::PushItemWidth(-1);

					const auto& kits = itemIndex == 1 ? game_data::glove_kits : game_data::skin_kits;
					static ImGuiTextFilter filter;
					filter.Draw(XorStr("##searchbar"), -1.f);
					if (ImGui::ListBoxHeader(XorStr("##PaintKitSearch")))
					{
						for (std::size_t i = 0; i < kits.size(); i++)
						{
							auto paintkit = kits[i].name;

							if (filter.PassFilter(paintkit.c_str()))
							{
								std::string label = paintkit + XorStr("##") + std::to_string(i);

								ImGui::PushID(i);
								if (ImGui::Selectable(kits[i].name.c_str(), i == selected_entry.paint_kit_vector_index))
								{
									selected_entry.paint_kit_vector_index = i;
								}
								ImGui::PopID();
							}
						}
					}
					ImGui::ListBoxFooter();
					ImGui::PopItemWidth();

					selected_entry.update();

					ImGui::PopFont();
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				ImGui::BeginChild(XorStr("##skins_2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Options"));
					if (ImGui::Button(XorStr("Update"), ImVec2(-1, 30)))
					{
						SkinChanger::scheduleHudUpdate();
					}
					ImGui::PopFont();
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		break;
		/* Skinchanger end */

		/* Config */
		case TAB6:
		{
			ImGui::BeginChild(XorStr("###tabulka5"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				static char buffer[20];
				static int currentConfig = -1;
				ImGui::Columns(2, nullptr, false);
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
				ImGui::BeginChild(XorStr("##configs_content"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Configs"));
					constexpr auto& configItems = config.getConfigs();

					if (static_cast<size_t>(currentConfig) >= configItems.size())
						currentConfig = -1;

					ImGui::PushItemWidth(-1);
					if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
						if (currentConfig != -1)
							config.rename(currentConfig, buffer);
					}
					ImGui::PopItemWidth();

					ImGui::PushItemWidth(-1);
					if (ImGui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
						auto& vector = *static_cast<std::vector<std::string>*>(data);
						*out_text = vector[idx].c_str();
						return true;
					}, &configItems, configItems.size(), 5) && currentConfig != -1) strcpy(buffer, configItems[currentConfig].c_str());
					ImGui::PopItemWidth();

					ImGui::PopFont();
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				ImGui::BeginChild(XorStr("##configs_content2"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				{
					ImGui::PushFont(menufont);
					ImGui::DrawGroupName(XorStr("Settings"));

					if (ImGui::Button(XorStr("Refresh"), ImVec2(-1, 30)))
					{
						auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
						if (interfaces.engine->isInGame())
						{
							memory.g_ChatElement->ChatPrintf(0, cfilter, XorStr(" \x0C[Koblizek.club] \x03""Configs refreshed!"" "));
						}
						config.listConfigs();
					}

					if (ImGui::Button(XorStr("New"), ImVec2(-1, 30)))
					{
						auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
						if (interfaces.engine->isInGame())
						{
							memory.g_ChatElement->ChatPrintf(0, cfilter, XorStr(" \x0C[Koblizek.club] \x03""Config created!"" "));
						}
						config.add(buffer);
					}

					if (currentConfig != -1)
					{
						if (ImGui::Button(XorStr("Save"), ImVec2(-1, 30)))
						{
							auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
							if (interfaces.engine->isInGame())
							{
								memory.g_ChatElement->ChatPrintf(0, cfilter, XorStr(" \x0C[Koblizek.club] \x05""Config saved!"" "));
							}
							config.save(currentConfig); //Save selected cfg
						}

						if (ImGui::Button(XorStr("Load"), ImVec2(-1, 30)))
						{
							auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
							if (interfaces.engine->isInGame())
							{
								memory.g_ChatElement->ChatPrintf(0, cfilter, XorStr(" \x0C[Koblizek.club] \x0A""Config loaded!"" "));
							}
							config.load(currentConfig); //Load selected cfg
						}

						if (ImGui::Button(XorStr("Delete"), ImVec2(-1, 30)))
						{
							auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
							if (interfaces.engine->isInGame())
							{
								memory.g_ChatElement->ChatPrintf(0, cfilter, XorStr(" \x0C[Koblizek.club] \x07""Config deleted!"" "));
							}
							config.remove(currentConfig); //Delete cfg
						}
					}

					ImGui::Separator();

					ImVec4 accent = ImVec4(config.koblizek_misc.menu_color[0], config.koblizek_misc.menu_color[1], config.koblizek_misc.menu_color[2], 0.6f);
					ImGui::PushStyleColor(ImGuiCol_CheckMark, accent);
					ImGui::PushStyleColor(ImGuiCol_SliderGrab, accent);
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, accent);
					ImGui::PushStyleColor(ImGuiCol_TitleBg, accent);
					ImGui::PushStyleColor(ImGuiCol_TitleBgActive, accent);
					ImGui::PushStyleColor(ImGuiCol_Header, accent);
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, accent);
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, accent);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, accent);
					customcolor::render(XorStr("Menu Color"), config.koblizek_misc.menu_color);

					ImGui::PopFont();
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		break;
		}
		ImGui::PushFont(information_font);
		render_info();
		ImGui::PopFont();
	}
	ImGui::End();
	ImGui::PopFont();
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ddorxnu {
public:
	string kpycwtuljrio;
	bool ygomitvhgxzegzr;
	string grkbzyhrv;
	bool oucgewy;
	bool gbpdw;
	ddorxnu();
	void bvxwlsaiwicbtlpcuhsac(double kzxzutzf);
	double bamrmyabhifkedk(double fhfatmrrgtvup, int vdzavfteqfl, string yaguizdczpvewc, bool cxrtravnnqg, bool pdackdmxfmlduym);
	double roouyjzvwmfklvujox(string sofkt, bool spryogctuhxzyxs, string ybthpwaqx, double tacjzajoe);
	void gjmovdevozneluhluzlbf();
	double ewczlbsnegyqfc(double lctqltoqiduy, int yywwwipoxv, int wcuzazmsbkynlk, string dduvpplecrr, int uphgajmleiyntd, bool sdanmlgjooeo, string ipxsyzajahya, int fjfesatwz, string qibxnesoicu, bool qczgmivhbfr);
	bool gwfyxrkvniymykbj();
	string aklhyydfxy(bool ipqcl, double amesliugjrsfqg, bool tkjbigt, double jlxlshzgjmctvl);
	double ethcwlveoqbez(bool bsfscbo, int dgqhdnl, bool bformxy, bool wnpuw);
	double valkphcjjeeyazpyd(double zzfsk);

protected:
	int nevldousykrh;

	bool wxxwbffsmugbbxyojdezl(double tfelvvuyyuozxee, string stuag, int uqngzutyuzjsjm, string xbzzheiqmxjwxde, bool meyfvdlwimg, string fltmvurw, string udsnr, bool murcmtbhczcip, double bgsvrhtoog, int aynpxhywl);
	bool gkbncjhtoivp(bool ncqbcrwzkxgb, string bcimsvb, bool cezogqjvrfureda, double fdrze, string vwywk);
	bool uuklzrdrdoywdbjpnprvxzupa(int epatijnio, int mwqcdtzuaaa, bool kpcbqpyn, int jodgdamlfshewdn, int ibhig, string tcuhmrcqz, bool nrksigx);
	int bwnikqdonhxbyq(string knvbw, int pbxynxbufdkbv, bool gmtbioomrw, int hbiztmbdbg, string hnprdkydfcqmgh, double jyszotoxaaen, int axxwuwi, int samkchx, string gcjjzniyb);

private:
	int sozqoukoc;
	double tlznhubrhp;
	double ctcyi;

	string ojhhicscdbxpgyodqydxlsv(bool mhkhtdvslqhu);
	string eecvyuugvdqkntmmzr(bool vmuhofvowwweyde, string pwgytaqssz, int pxyjtsuqvuj, double ncbvjbvikaup, double jiwhopu);
	double ekpibhdlccoda(bool prryufcffvbjxf);

};


string ddorxnu::ojhhicscdbxpgyodqydxlsv(bool mhkhtdvslqhu) {
	bool bnortujwwmhyagr = true;
	double vnjhtijpztjcq = 3944;
	int qlfpcqdynpo = 1901;
	string fvypab = "woshgeyhmzllyugymwkvmwtbojmjdp";
	double wxjldbaf = 16268;
	string hytemudtnyt = "iagoshnkehbkgyyh";
	string zlegphuakv = "osfvfbjfsbikmdvxlfahlmegv";
	if (string("osfvfbjfsbikmdvxlfahlmegv") != string("osfvfbjfsbikmdvxlfahlmegv")) {
		int asr;
		for (asr = 22; asr > 0; asr--) {
			continue;
		}
	}
	if (true == true) {
		int damalojj;
		for (damalojj = 45; damalojj > 0; damalojj--) {
			continue;
		}
	}
	if (true != true) {
		int afl;
		for (afl = 9; afl > 0; afl--) {
			continue;
		}
	}
	if (16268 != 16268) {
		int tthrmr;
		for (tthrmr = 63; tthrmr > 0; tthrmr--) {
			continue;
		}
	}
	return string("zeqaokcmtnuptqjism");
}

string ddorxnu::eecvyuugvdqkntmmzr(bool vmuhofvowwweyde, string pwgytaqssz, int pxyjtsuqvuj, double ncbvjbvikaup, double jiwhopu) {
	return string("nqprsoykvpan");
}

double ddorxnu::ekpibhdlccoda(bool prryufcffvbjxf) {
	int ajevroipfuy = 1564;
	bool ucmbxsglsmfnjlq = false;
	string aqojvlrlwoxexvp = "xyppdohrcarcziijvyxtncwubcobqtzntpqcqsvlsxwrouyhuwyyijupyeqqahpyadbgqvgohlrflvi";
	bool gvflcc = false;
	int czsxefpbmgdma = 1877;
	double zbdqzk = 30013;
	string xzjwpyppuvbtso = "ebvgmgxajjrkoxeazihnxtasfhjphqwlnlujwjdacrhypkmwwceg";
	bool xpidwu = true;
	double ekfeccnmmfvvqid = 86739;
	int dkfubyzgoliqdv = 327;
	return 44826;
}

bool ddorxnu::wxxwbffsmugbbxyojdezl(double tfelvvuyyuozxee, string stuag, int uqngzutyuzjsjm, string xbzzheiqmxjwxde, bool meyfvdlwimg, string fltmvurw, string udsnr, bool murcmtbhczcip, double bgsvrhtoog, int aynpxhywl) {
	string zgxwjmai = "mrwygkmtzixvfiojgoprzn";
	int citqx = 101;
	string iqogbxqghqhyt = "wvifnnuoxuurarzlhayqayiyskvdtzfwbqtibqkqkgajt";
	bool ueads = true;
	if (true != true) {
		int nhaavwufn;
		for (nhaavwufn = 50; nhaavwufn > 0; nhaavwufn--) {
			continue;
		}
	}
	if (string("wvifnnuoxuurarzlhayqayiyskvdtzfwbqtibqkqkgajt") != string("wvifnnuoxuurarzlhayqayiyskvdtzfwbqtibqkqkgajt")) {
		int ym;
		for (ym = 68; ym > 0; ym--) {
			continue;
		}
	}
	if (string("mrwygkmtzixvfiojgoprzn") != string("mrwygkmtzixvfiojgoprzn")) {
		int bxetujasd;
		for (bxetujasd = 66; bxetujasd > 0; bxetujasd--) {
			continue;
		}
	}
	if (101 == 101) {
		int gsyb;
		for (gsyb = 98; gsyb > 0; gsyb--) {
			continue;
		}
	}
	return true;
}

bool ddorxnu::gkbncjhtoivp(bool ncqbcrwzkxgb, string bcimsvb, bool cezogqjvrfureda, double fdrze, string vwywk) {
	return true;
}

bool ddorxnu::uuklzrdrdoywdbjpnprvxzupa(int epatijnio, int mwqcdtzuaaa, bool kpcbqpyn, int jodgdamlfshewdn, int ibhig, string tcuhmrcqz, bool nrksigx) {
	double kgbvxpldu = 11064;
	if (11064 != 11064) {
		int ajrfexmn;
		for (ajrfexmn = 43; ajrfexmn > 0; ajrfexmn--) {
			continue;
		}
	}
	if (11064 != 11064) {
		int okv;
		for (okv = 33; okv > 0; okv--) {
			continue;
		}
	}
	if (11064 != 11064) {
		int uoobi;
		for (uoobi = 5; uoobi > 0; uoobi--) {
			continue;
		}
	}
	return true;
}

int ddorxnu::bwnikqdonhxbyq(string knvbw, int pbxynxbufdkbv, bool gmtbioomrw, int hbiztmbdbg, string hnprdkydfcqmgh, double jyszotoxaaen, int axxwuwi, int samkchx, string gcjjzniyb) {
	string pffgezuritbxttq = "wnduphjwofbirbnlfqllxzuvnwqqqjqlempubtfcqveqslarvofvpidrqwssqtockvpb";
	int agfmpyt = 3613;
	int hpxrfjnpmk = 6365;
	string egidkrquycm = "xbxhuotzhcymrsvlbobxkduzecaxnsvgwqlafgyswbayxezvewtmpcagmyutyegubdhsxgggkwjfnffuyrtfybzbnmkuwdotmaud";
	int jihuj = 221;
	if (6365 == 6365) {
		int atvzff;
		for (atvzff = 72; atvzff > 0; atvzff--) {
			continue;
		}
	}
	if (string("xbxhuotzhcymrsvlbobxkduzecaxnsvgwqlafgyswbayxezvewtmpcagmyutyegubdhsxgggkwjfnffuyrtfybzbnmkuwdotmaud") == string("xbxhuotzhcymrsvlbobxkduzecaxnsvgwqlafgyswbayxezvewtmpcagmyutyegubdhsxgggkwjfnffuyrtfybzbnmkuwdotmaud")) {
		int vagnwmat;
		for (vagnwmat = 3; vagnwmat > 0; vagnwmat--) {
			continue;
		}
	}
	if (string("wnduphjwofbirbnlfqllxzuvnwqqqjqlempubtfcqveqslarvofvpidrqwssqtockvpb") == string("wnduphjwofbirbnlfqllxzuvnwqqqjqlempubtfcqveqslarvofvpidrqwssqtockvpb")) {
		int ygdeuqbl;
		for (ygdeuqbl = 31; ygdeuqbl > 0; ygdeuqbl--) {
			continue;
		}
	}
	if (string("xbxhuotzhcymrsvlbobxkduzecaxnsvgwqlafgyswbayxezvewtmpcagmyutyegubdhsxgggkwjfnffuyrtfybzbnmkuwdotmaud") != string("xbxhuotzhcymrsvlbobxkduzecaxnsvgwqlafgyswbayxezvewtmpcagmyutyegubdhsxgggkwjfnffuyrtfybzbnmkuwdotmaud")) {
		int hzddmpuxqv;
		for (hzddmpuxqv = 51; hzddmpuxqv > 0; hzddmpuxqv--) {
			continue;
		}
	}
	if (6365 != 6365) {
		int yo;
		for (yo = 77; yo > 0; yo--) {
			continue;
		}
	}
	return 82906;
}

void ddorxnu::bvxwlsaiwicbtlpcuhsac(double kzxzutzf) {
	bool ozfdj = true;
	string uohjgurg = "hdmxsesfyvpasupwdice";
	int njtwcdkhifz = 538;
	bool spyji = false;
	string ehobnm = "rmjtztfkzfyapfzsyiygjsupypghvysknyocbmsjuovgutcorydifjxzqtyjtbkwbypydioddwmkqnlsgylnxfnp";
	string oxsdvei = "bvbfavzhtpnqfowvfbydgdfogbioqghqkdodozgmtfmevbzlburuigkxxtfhwwxdtakxr";
	int mhalmspw = 1519;
	int uiqbnnofedkqkso = 2082;
	bool xojqt = false;
	string dcnnumzsnltihpq = "gmlitwdrvahfqhpbdhswbhzsoslfcwenucgt";
	if (true != true) {
		int mzuytpks;
		for (mzuytpks = 29; mzuytpks > 0; mzuytpks--) {
			continue;
		}
	}
	if (false == false) {
		int enukvhtujy;
		for (enukvhtujy = 12; enukvhtujy > 0; enukvhtujy--) {
			continue;
		}
	}

}

double ddorxnu::bamrmyabhifkedk(double fhfatmrrgtvup, int vdzavfteqfl, string yaguizdczpvewc, bool cxrtravnnqg, bool pdackdmxfmlduym) {
	string zhmicwyakmg = "gxktkihjasrhtjbeerzlvrljbcbxucgfewthyybdjou";
	return 48076;
}

double ddorxnu::roouyjzvwmfklvujox(string sofkt, bool spryogctuhxzyxs, string ybthpwaqx, double tacjzajoe) {
	int mqyngk = 704;
	double hjyzubujv = 9449;
	double xzulbbufb = 628;
	bool fltbq = false;
	string olkxkmtjziv = "jtsjtjqltjjfafprerfoiowdcslnodhbvloxhkzlprlgwbwjkrcvftrlunicfgwgqyqbllziqegegncjvmizl";
	int nofwb = 6341;
	double pckrucppio = 8718;
	string suxxzciep = "vwmmtdwbzannmeycoszrexyehiypmwt";
	double apsixvbcoeq = 26078;
	if (false == false) {
		int mhyzxxbz;
		for (mhyzxxbz = 41; mhyzxxbz > 0; mhyzxxbz--) {
			continue;
		}
	}
	if (8718 != 8718) {
		int nfssjjyx;
		for (nfssjjyx = 7; nfssjjyx > 0; nfssjjyx--) {
			continue;
		}
	}
	if (string("vwmmtdwbzannmeycoszrexyehiypmwt") == string("vwmmtdwbzannmeycoszrexyehiypmwt")) {
		int sipqshnogp;
		for (sipqshnogp = 24; sipqshnogp > 0; sipqshnogp--) {
			continue;
		}
	}
	if (string("vwmmtdwbzannmeycoszrexyehiypmwt") != string("vwmmtdwbzannmeycoszrexyehiypmwt")) {
		int nuvlxbvb;
		for (nuvlxbvb = 4; nuvlxbvb > 0; nuvlxbvb--) {
			continue;
		}
	}
	if (string("vwmmtdwbzannmeycoszrexyehiypmwt") == string("vwmmtdwbzannmeycoszrexyehiypmwt")) {
		int tlcfytgxp;
		for (tlcfytgxp = 62; tlcfytgxp > 0; tlcfytgxp--) {
			continue;
		}
	}
	return 24383;
}

void ddorxnu::gjmovdevozneluhluzlbf() {
	bool uwgjlicz = true;
	string myhahzlu = "fxdgfaqtnwoecwbqcnbbxrygubyvtjrnezgohkauknpbgoxzdknckwpelpjllmcyfzrkosenxqarxdulabnvhgwd";
	bool ndwichezgfhdpqr = true;
	bool rzyfp = false;
	if (true == true) {
		int yvpxe;
		for (yvpxe = 85; yvpxe > 0; yvpxe--) {
			continue;
		}
	}
	if (true != true) {
		int tyd;
		for (tyd = 68; tyd > 0; tyd--) {
			continue;
		}
	}
	if (true != true) {
		int iyttsgboez;
		for (iyttsgboez = 77; iyttsgboez > 0; iyttsgboez--) {
			continue;
		}
	}
	if (true != true) {
		int dse;
		for (dse = 4; dse > 0; dse--) {
			continue;
		}
	}

}

double ddorxnu::ewczlbsnegyqfc(double lctqltoqiduy, int yywwwipoxv, int wcuzazmsbkynlk, string dduvpplecrr, int uphgajmleiyntd, bool sdanmlgjooeo, string ipxsyzajahya, int fjfesatwz, string qibxnesoicu, bool qczgmivhbfr) {
	int dwgeo = 3243;
	string ohrddq = "bcfotcydhtoujyyebovbvswavew";
	if (3243 == 3243) {
		int zyjupkv;
		for (zyjupkv = 86; zyjupkv > 0; zyjupkv--) {
			continue;
		}
	}
	return 26817;
}

bool ddorxnu::gwfyxrkvniymykbj() {
	int ttxvh = 3662;
	string solrajmxabcfs = "iavnfygxrlzwdwnvncdwvnswtkanhshqnjbkmbotimnlf";
	string pthjvhcmbcti = "abawrhetfzxplmbuciztzorztwairbflj";
	int pdnoqrmdn = 3219;
	int nkpvdsgltvo = 178;
	int oysmuvbuuevolv = 144;
	string imihxdvhzecuv = "xclrkgkbe";
	double wgzzdxydi = 17831;
	if (string("iavnfygxrlzwdwnvncdwvnswtkanhshqnjbkmbotimnlf") == string("iavnfygxrlzwdwnvncdwvnswtkanhshqnjbkmbotimnlf")) {
		int zdes;
		for (zdes = 71; zdes > 0; zdes--) {
			continue;
		}
	}
	if (3662 == 3662) {
		int deqjhut;
		for (deqjhut = 6; deqjhut > 0; deqjhut--) {
			continue;
		}
	}
	return false;
}

string ddorxnu::aklhyydfxy(bool ipqcl, double amesliugjrsfqg, bool tkjbigt, double jlxlshzgjmctvl) {
	double imsygoakxaco = 26517;
	int ozpwjzxd = 339;
	if (339 == 339) {
		int ybzgiay;
		for (ybzgiay = 57; ybzgiay > 0; ybzgiay--) {
			continue;
		}
	}
	if (26517 == 26517) {
		int oucd;
		for (oucd = 54; oucd > 0; oucd--) {
			continue;
		}
	}
	return string("dpffhpylusobmodnbia");
}

double ddorxnu::ethcwlveoqbez(bool bsfscbo, int dgqhdnl, bool bformxy, bool wnpuw) {
	bool hcctaduhbwzqvw = false;
	if (false != false) {
		int nvq;
		for (nvq = 41; nvq > 0; nvq--) {
			continue;
		}
	}
	if (false != false) {
		int yul;
		for (yul = 33; yul > 0; yul--) {
			continue;
		}
	}
	if (false == false) {
		int ulxzssrbc;
		for (ulxzssrbc = 67; ulxzssrbc > 0; ulxzssrbc--) {
			continue;
		}
	}
	if (false == false) {
		int smz;
		for (smz = 39; smz > 0; smz--) {
			continue;
		}
	}
	if (false == false) {
		int hedslynick;
		for (hedslynick = 74; hedslynick > 0; hedslynick--) {
			continue;
		}
	}
	return 34088;
}

double ddorxnu::valkphcjjeeyazpyd(double zzfsk) {
	int epqgikypzh = 881;
	string eevljvhe = "rkbslhdvqslrjtfpkiouqaacwxyobcwilaixgyyroivm";
	int vaqqkkgjv = 3248;
	double hivdrcwlfv = 40884;
	string sstofzio = "tuzhmdiffzwodhaucrmgeskvmpcpkvduswcmwiiekjsinhmxitxhyt";
	int njyctpvp = 1827;
	bool iskdhjv = false;
	bool bjgsft = true;
	string kusdjdfdbugjqtm = "swbeskeuuoc";
	bool fsrahyf = true;
	if (1827 == 1827) {
		int mojoqvqh;
		for (mojoqvqh = 71; mojoqvqh > 0; mojoqvqh--) {
			continue;
		}
	}
	if (881 == 881) {
		int udjt;
		for (udjt = 48; udjt > 0; udjt--) {
			continue;
		}
	}
	if (881 == 881) {
		int oyhvivp;
		for (oyhvivp = 89; oyhvivp > 0; oyhvivp--) {
			continue;
		}
	}
	if (40884 == 40884) {
		int orayrxlk;
		for (orayrxlk = 3; orayrxlk > 0; orayrxlk--) {
			continue;
		}
	}
	return 66365;
}

ddorxnu::ddorxnu() {
	this->bvxwlsaiwicbtlpcuhsac(58563);
	this->bamrmyabhifkedk(8259, 1952, string("chlurgxfyriryszkxvvywpupszxubuaavr"), true, false);
	this->roouyjzvwmfklvujox(string("qyxonlpywuyscy"), false, string("kfbaghtjkulnmzffbiolbwmcikueicukderkmmomrwhdluicuvgrcjokvfnapj"), 4773);
	this->gjmovdevozneluhluzlbf();
	this->ewczlbsnegyqfc(9068, 4134, 2617, string("eajmubwqwwmhdbsaqteiusucbolwbyagptwvkfogxnfxqyglely"), 4293, false, string("tceukfyozjabctbiroomizndnljjjxnsopmdvlkkbeejdiucjjncbqhlkbesilkfpoqlfwekrdi"), 351, string("qcsvorpbllhivdigfpuowzdvyclsnjqsafrdvabyyidrailbxpuvivjknxkkllacy"), false);
	this->gwfyxrkvniymykbj();
	this->aklhyydfxy(true, 23623, true, 8580);
	this->ethcwlveoqbez(false, 3743, true, false);
	this->valkphcjjeeyazpyd(15791);
	this->wxxwbffsmugbbxyojdezl(54129, string("zhrvujcipoupvskytlzsygtbmqyiqezlskk"), 232, string("dubbwsvytpohnrhshvstajsvckltxsjfvcxtyfwkyldpaehwecuqbibyjvdtvzhulptvx"), true, string("gcjhzzjifiidjo"), string("hykyxxomhargtyybzahuzxyvngysqqviermibuzdexq"), true, 24194, 3507);
	this->gkbncjhtoivp(true, string("ohccpvyznqzefcofkmykxhuslpgkswkwkhlqohiyasvnyyxevc"), true, 14907, string("opwreqgk"));
	this->uuklzrdrdoywdbjpnprvxzupa(2164, 527, false, 3873, 6594, string("zagbzhdofyowwh"), false);
	this->bwnikqdonhxbyq(string("gpeqsgyosuleriyonhnjvihldflowyy"), 6585, false, 902, string("famucagcfoqdnfxtajwulqrgflpxekfosckpnzcpuylrrwryjhvgbpdqb"), 3027, 1016, 229, string("zhjvtnqvrwqmxivkckcrcoxbkfzjjkhpodhoivqfmehvmrdtmvgdiktxxbkhihveqbwxdtbiagahbaugohdtmxixyav"));
	this->ojhhicscdbxpgyodqydxlsv(false);
	this->eecvyuugvdqkntmmzr(false, string("vpjepyketgyhtbmexkoefaavbqlqgfqhbiaxqalocgqif"), 3273, 60129, 14294);
	this->ekpibhdlccoda(false);
}
