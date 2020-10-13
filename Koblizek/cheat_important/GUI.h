#pragma once

#include <string>
#include "../cheat_imgui/imgui.h"
#include "../cheat_imgui/imgui_impl_dx9.h"
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include <stdint.h>
#include <chrono>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
extern ImFont* tabfont;
extern ImFont* menufont;
extern ImFont* subicon;
extern ImFont* titlefont;
extern ImFont* information_font;
extern IDirect3DTexture9 *model_img;
extern IDirect3DTexture9 *chams_img;
extern IDirect3DTexture9 *flat_img;
extern IDirect3DTexture9 *steam_avatar;

enum {
	TAB1,
	TAB2,
	TAB3,
	TAB4,
	TAB5,
	TAB6
};

class GUI final {
public:
	void bind(int&) noexcept;
	void render(IDirect3DDevice9 * device) noexcept;
	void menurender(IDirect3DDevice9 * device) noexcept;
    bool isOpen{ false };
	bool opened = NULL;
private:
    struct {
        bool skinChanger{ false };
		bool information{ false };
    } window;
};

static int menu_selected;
extern GUI gui;