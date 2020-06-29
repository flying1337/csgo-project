#include "../cheat.h"
#include "../ui/bindsfont.h"
#include "../ui/wepfont.h"
#include "../ui/spinners.h"
#include "config.h"
#include "../features/visuals/debuginfo.h"
#include "menu.h"


#ifdef _DEBUG
#define MAXTABS 8
#else
#define MAXTABS 7
#endif

using namespace std::chrono_literals;
void c_menu::initialize(IDirect3DDevice9* device) {
	if (is_initialized) return;

	D3DXCreateTextureFromFileInMemoryEx(device, spinner_0, 355, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[0]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_1, 350, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[1]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_2, 329, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[2]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_3, 347, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[3]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_4, 350, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[4]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_5, 348, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[5]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_6, 329, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[6]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_7, 347, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[7]);

	ImGui::CreateContext();

	auto io = &ImGui::GetIO();
	auto style = &ImGui::GetStyle();

	ImGui::StyleColorsDark();
	style->WindowBorderSize = 0.f;
	style->FrameBorderSize = 0.f;
	style->AntiAliasedFill = false;
	style->AntiAliasedLines = false;

	is_opened = true;

	csgo->window = FindWindowA(str("Valve001"), 0);

	if (!ImGui_ImplDX9_Init(device)) return;
	if (!ImGui_ImplWin32_Init(csgo->window)) return;

	ui = new c_ui;
	ui->setup();

	ImFontConfig cfg;
	io->Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromFileTTF(str("C:/windows/fonts/verdana.ttf"), 24.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromFileTTF(str("C:/windows/fonts/tahoma.ttf"), 14.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromFileTTF(str("C:/windows/fonts/verdanab.ttf"), 18.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromFileTTF(str("C:/windows/fonts/verdanab.ttf"), 24.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromFileTTF(str("c:/windows/fonts/verdana.ttf"), 13.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
	io->Fonts->AddFontFromMemoryTTF(weapons_font, 47320, 14.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());

	setup();
	is_initialized = true;
}

void c_menu::draw_begin() {
	if (!is_initialized) return;

	auto style = &ImGui::GetStyle();
	style->AntiAliasedFill = false;
	style->AntiAliasedLines = false;

    for (auto s : scripting->load_queue)
        scripting->load(s);

    scripting->load_queue.clear();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void _d(const char* s, ...) {
	if (config->b.show_desc) {
		if (ImGui::IsItemHovered()) {
		    auto buf = (char*)malloc(1024);
		    va_list va;
		    va_start(va, s);
		    vsprintf(buf, s, va);
		    va_end(va);

			ImGui::BeginTooltip();
			ImGui::Text(str("%s"), buf);
			ImGui::EndTooltip();
		}
	}
}

bool cmp_strings_ci(const char* a, const char* b) {
    auto v1 = (char*)malloc(strlen(a) + 1);
    auto v2 = (char*)malloc(strlen(b) + 1);

    strcpy(v1, a);
    strcpy(v2, b);

    for (auto i = 0; i < strlen(v1); i++) v1[i] = std::tolower(v1[i]);
    for (auto i = 0; i < strlen(v2); i++) v2[i] = std::tolower(v2[i]);

    return !strcmp(v1, v2);
}

bool sub_strings_ci(const char* a, const char* b) {
    auto v1 = (char*)malloc(strlen(a) + 1);
    auto v2 = (char*)malloc(strlen(b) + 1);

    strcpy(v1, a);
    strcpy(v2, b);

    for (auto i = 0; i < strlen(v1); i++) v1[i] = std::tolower(v1[i]);
    for (auto i = 0; i < strlen(v2); i++) v2[i] = std::tolower(v2[i]);

    return strstr(v1, v2);
}

void c_menu::draw_ext_elements(const char *tab, const char *group) {

}

void c_menu::draw() {
    if (!is_initialized) return;

    ui->style.accent = ImColor(config->c.menu[0], config->c.menu[1], config->c.menu[2]);

    ui->draw();
}

void c_menu::draw_end() {
	if (!is_initialized) return;

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

ImColor c_menu::get_outline_color(float alpha) {
	return ImColor(config->c.outline[0], config->c.outline[1], config->c.outline[2], alpha);
}

ImColor c_menu::get_light_color(float alpha) {
	return ImColor(config->c.lightbg[0], config->c.lightbg[1], config->c.lightbg[2], alpha);
}

ImColor c_menu::get_dark_color(float alpha) {
	return ImColor(config->c.darkbg[0], config->c.darkbg[1], config->c.darkbg[2], alpha);
}

ImColor c_menu::get_shadow_color(float alpha) {
	return ImColor(config->c.shadow[0], config->c.shadow[1], config->c.shadow[2], alpha);
}

ImColor c_menu::get_accent_color(float alpha) {
	return ImColor(config->c.menu[0], config->c.menu[1], config->c.menu[2], alpha);
}

void c_menu::setup() {
    wnd = new c_ui_window;
    wnd->setup(str("uc-sflying for Counter-Strike: Global Offensive"));
    wnd->set_active(true);
    wnd->set_pos(ImVec2(50, 50));
    wnd->set_size(ImVec2(800, 500));

    static auto ragebot = new c_ui_tab;
    ragebot->setup(str("Ragebot"));
    ragebot->set_active(true);
    ragebot->set_tab(0, 6);
    {
        ragebot_tab(ragebot);
    }

    static auto antiaims = new c_ui_tab;
    antiaims->setup(str("Anti-aims"));
    antiaims->set_tab(1, 6);
    {
        antiaims_tab(antiaims);
    }

    static auto legitbot = new c_ui_tab;
    legitbot->setup(str("Legitbot"));
    legitbot->set_tab(2, 6);
    {
        legitbot_tab(legitbot);
    }

    static auto visuals = new c_ui_tab;
    visuals->setup(str("Visuals"));
    visuals->set_tab(3, 6);
    {
        visuals_tab(visuals);
    }

    static auto misc = new c_ui_tab;
    misc->setup(str("Miscellaneous"));
    misc->set_tab(4, 6);
    {
        misc_tab(misc);
    }

//    static auto skinchanger = new c_ui_tab;
//    skinchanger->setup(str("Skinchanger"));
//    skinchanger->set_tab(5, 7);
//    {
//        skins_tab(skinchanger);
//    }

    static auto extensions = new c_ui_tab;
    extensions->setup(str("Extensions"));
    extensions->set_tab(5, 6);
    {
        extensions_tab(extensions);
    }

    wnd->add(ragebot);
    wnd->add(antiaims);
    wnd->add(legitbot);
    wnd->add(visuals);
    wnd->add(misc);
   // wnd->add(skinchanger);
    wnd->add(extensions);

    ui->add(wnd);
}