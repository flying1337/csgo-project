#include "../../cheat.h"

void c_hitlog::add(const char* who, const char* to, int damage, int left, bool hurt, bool yourself) {
	if (config->m.events[0] || config->m.events[1]) {
		if (hurt) {
			if (yourself)
				utils->console_log(str("-%ihp in the %s to yourself"), damage, to);
			else
				utils->console_log(str("-%ihp in the %s by %s"), damage, to, who);
		} else
			utils->console_log(str("-%ihp in the %s to %s (%ihp remaining)"), damage, to, who, left);
	}

	if (entries.size() > 10)
		entries.erase(entries.begin());

	entries.push_back({
		who, to, damage, left, hurt, yourself, 255, 255, csgo->globals->realtime
	});
}

void c_hitlog::draw(ImDrawList* d) {
	std::vector<s_hitlogentry> rm;

	auto w = 0, h = 0;
	csgo->engine->screen_size(w, h);

	auto x_away = 16, y_away = 16;

	auto offset = 0;
	for (auto i = (int)entries.size() - 1; i >= 0; i--) {
		auto& e = entries.at(i);

		char act[16] = { 0 };
		char text[1024] = { 0 };
		char dmg[16] = { 0 };
		char left[64] = { 0 };

		if (!e.hurt) {
			sprintf(act, str("HIT"));
			sprintf(text, str("%s in the %s for"), e.who.c_str(), e.to.c_str());
			sprintf(dmg, str("%i"), e.dmg);
			sprintf(left, str("and %i remaining"), e.left);
		}
		else {
			sprintf(act, str("HURT"));
			if (!e.yourself) {
				sprintf(text, str("by %s in the %s for"), e.who.c_str(), e.to.c_str());
				sprintf(dmg, str("%i"), e.dmg);
			}
			else {
				sprintf(text, str("yourself for"));
				sprintf(dmg, str("%i"), e.dmg);
			}
		}

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		auto actsize = ImGui::CalcTextSize(act);
		d->AddText(ImVec2(w / 2 + 1 + x_away, h / 2 + 16 + offset + 1 + y_away), ImColor(0, 0, 0, e.alpha), act);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		auto textsize = ImGui::CalcTextSize(text);
		d->AddText(ImVec2(w / 2 + actsize.x + 3 + x_away, h / 2 + 16 + offset + actsize.y / 2 - textsize.y / 2 + 1 + y_away), ImColor(0, 0, 0, e.alpha), text);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		auto dmgsize = ImGui::CalcTextSize(dmg);
		d->AddText(ImVec2(w / 2 + actsize.x + textsize.x + 7 + x_away, h / 2 + 16 + offset + 1 + y_away), ImColor(0, 0, 0, e.alpha), dmg);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		d->AddText(ImVec2(w / 2 + actsize.x + textsize.x + dmgsize.x + 9 + x_away, h / 2 + 16 + offset + actsize.y / 2 - textsize.y / 2 + 1 + y_away), ImColor(0, 0, 0, e.alpha), left);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		d->AddText(ImVec2(w / 2 + x_away, h / 2 + 16 + offset + y_away), e.hurt ? ImColor(220, 6, 6, e.alpha) : ImColor(85, 190, 7, e.alpha), act);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		d->AddText(ImVec2(w / 2 + actsize.x + 2 + x_away, h / 2 + 16 + offset + actsize.y / 2 - textsize.y / 2 + y_away), ImColor(255, 255, 255, e.alpha), text);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		d->AddText(ImVec2(w / 2 + actsize.x + textsize.x + 6 + x_away, h / 2 + 16 + offset + y_away), features->visuals->percent_to_color(100 - std::clamp(e.dmg, 0, 100), e.alpha), dmg);
		ImGui::PopFont();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		d->AddText(ImVec2(w / 2 + actsize.x + textsize.x + dmgsize.x + 8 + x_away, h / 2 + 16 + offset + actsize.y / 2 - textsize.y / 2 + 1 + y_away), ImColor(255, 255, 255, e.alpha), left);
		ImGui::PopFont();

		if (e.flash > 0) {
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
			d->AddText(ImVec2(w / 2 + x_away, h / 2 + 16 + offset + y_away), ImColor(255, 255, 255, e.flash), act);
			d->AddText(ImVec2(w / 2 + actsize.x + textsize.x + 6 + x_away, h / 2 + 16 + offset + y_away), ImColor(255, 255, 255, e.flash), dmg);
			ImGui::PopFont();

			e.flash -= 255.f / 0.5f * csgo->globals->frametime;
		}

		if (e.alpha > 0 && csgo->globals->realtime - e.creation > 5.f)
			e.alpha -= 255.f / 1.f * csgo->globals->frametime;		

		if (e.alpha <= 0)
			rm.push_back(e);

		offset += actsize.y + 2;
	}

	for (auto r : rm)
		entries.erase(std::find(entries.begin(), entries.end(), r));
}