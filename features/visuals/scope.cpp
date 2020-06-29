#include "../../cheat.h"

void c_scope::draw(ImDrawList* d) {
	if (!csgo->engine->is_connected() || !csgo->engine->is_in_game() || !csgo->local_player) return;
	if (!csgo->local_player->is_scoped()) return;

	auto w = 0, h = 0;
	csgo->engine->screen_size(w, h);

	auto wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());
	if (!wep) return;

	auto idx = wep->econ_index();
	if (idx != WEAPON_AWP && idx != WEAPON_SSG08 && idx != WEAPON_SCAR20 && idx != WEAPON_G3SG1) return;

	if (config->m.removals[3]) {
		d->AddLine(ImFloor(ImVec2(0, h / 2)), ImFloor(ImVec2(w, h / 2)), ImColor(0, 0, 0));
		d->AddLine(ImFloor(ImVec2(w / 2, 0)), ImFloor(ImVec2(w / 2, h)), ImColor(0, 0, 0));
	}


}