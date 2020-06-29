#include "../../cheat.h"

void c_bombtimer::draw(ImDrawList* d) {
	if (!config->b.vis_bombtimer || !csgo->bomb || !csgo->local_player || !csgo->bomb->is_ticking()) return;

	auto explodeLeft = std::clamp(csgo->bomb->blowtime() - csgo->globals->servertime, 0.f, FLT_MAX);
	auto defuseLeft = std::clamp(csgo->bomb->defusetime() - csgo->globals->servertime, 0.f, FLT_MAX);

	auto w = 0, h = 0;
	csgo->engine->screen_size(w, h);

	auto bg = menu->get_dark_color();
	auto outline = menu->get_outline_color();
	auto shadow = menu->get_shadow_color();
	auto shadow_0 = menu->get_shadow_color(0.f);
	auto accent = menu->get_accent_color();

	auto x = w / 2 - 150, y = 150;

	auto size = ImVec2(300, 35);

	d->AddRectFilled(ImVec2(x, y), ImVec2(x, y) + size, bg);
	d->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x, y) + ImVec2(size.x, 6.f),
		shadow, shadow, shadow_0, shadow_0);
	d->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x, y) + ImVec2(6.f, size.y),
		shadow, shadow_0, shadow_0, shadow);
	d->AddRectFilledMultiColor(ImVec2(x, y) + ImVec2(0, size.y - 6.f), ImVec2(x, y) + size,
		shadow_0, shadow_0, shadow, shadow);
	d->AddRectFilledMultiColor(ImVec2(x, y) + ImVec2(size.x - 6.f, 0), ImVec2(x, y) + size,
		shadow_0, shadow, shadow, shadow_0);
	d->AddRect(ImVec2(x, y), ImVec2(x, y) + size, outline);

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
	d->AddText(ImVec2(x + 5, y + 5), ImColor(255, 255, 255), (std::string(str("Bombsite: ")) + (!csgo->bomb->site() ? str("A") : str("B"))).c_str());

	char buffer[128] = { 0 };

	if (csgo->bomb->defuser().index == 0xFFFFFFFF)
		sprintf(buffer, str("Damage: %i hp | Explode in %.1fs"),
			csgo->bomb->get_damage(csgo->local_player), explodeLeft);
	else {
        sprintf(buffer, str("Damage: %i hp | Defuse in %.1fs"),
                csgo->bomb->get_damage(csgo->local_player), defuseLeft);

        if (defuseLeft < explodeLeft)
            accent = ImColor(80, 140, 200);
    }

	auto tsz = ImGui::CalcTextSize(buffer);
	d->AddText(ImVec2(x, y) + size - ImVec2(5 + tsz.x, size.y - 5), ImColor(255, 255, 255), buffer);
	ImGui::PopFont();

	auto sub = 0.f;
	if (csgo->bomb->defuser().index == 0xFFFFFFFF)
		sub = (size.x - 10) - ((size.x - 10) / csgo->bomb->timer() * explodeLeft);
	else
		sub = (size.x - 10) - ((size.x - 10) / csgo->bomb->defuse() * defuseLeft);

	d->AddRectFilled(ImVec2(x + 5, y + size.y - 15), ImVec2(x + size.x - 5 - sub, y + size.y - 5), accent);
	d->AddRect(ImVec2(x + 5, y + size.y - 15), ImVec2(x + size.x - 5, y + size.y - 5), outline);
	d->AddRect(ImVec2(x + 5, y + size.y - 15) + ImVec2(1, 1), ImVec2(x + size.x - 5, y + size.y - 5) - ImVec2(1, 1), shadow);
}