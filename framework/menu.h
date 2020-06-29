#pragma once

class c_menu {
public:
	void initialize(IDirect3DDevice9* device);
	void draw_begin();
	void draw();
	void draw_end();
	void setup();

    ImColor get_outline_color(float alpha = 1.f);
	ImColor get_light_color(float alpha = 1.f);
	ImColor get_dark_color(float alpha = 1.f);
	ImColor get_shadow_color(float alpha = 1.f);
	ImColor get_accent_color(float alpha = 1.f);

	bool is_opened = false;
	bool is_initialized = false;

	IDirect3DTexture9* spinners[8] = {0};

	c_ui_window* wnd = nullptr;
    c_ui* ui = nullptr;

private:
    void draw_ext_elements(const char* tab, const char* group);

	int tab = 0;

	bool is_message_shown = false;
	char* title = 0;
	char* message = 0;

	void ragebot_tab(c_ui_tab* tab);
	void antiaims_tab(c_ui_tab* tab);
	void legitbot_tab(c_ui_tab* tab);
	void visuals_tab(c_ui_tab* tab);
	void misc_tab(c_ui_tab* tab);
	void skins_tab(c_ui_tab* tab);
	void extensions_tab(c_ui_tab* tab);
};