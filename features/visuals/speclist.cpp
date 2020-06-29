#include "../../cheat.h"

void c_speclist::work() {
	if (!config->b.vis_speclist || !csgo->local_player || !csgo->engine->is_in_game()) return;

	std::vector<std::string> ns;
	for (auto i = 0; i < 65; i++) {
		auto p = (c_player*)csgo->entities->get_entity(i);
		if (!p || !p->is_player() || p->index() == csgo->local_player->index() || p->is_dormant()) continue;

		auto spec = (c_player*)csgo->entities->get_entity_handle(p->spectator());
		if (spec && spec->index() == csgo->local_player->index())
			ns.push_back(p->get_player_info().name);
	}

	spectators = ns;
}

void c_speclist::draw() {
	if (!config->b.vis_speclist || (!menu->is_opened && spectators.size() <= 0) || !csgo->engine->is_in_game()) return;

	if (!did_set_pos) {
		ImGui::SetNextWindowPos(ImVec2(config->i.vis_speclist_x, config->i.vis_speclist_y));
		did_set_pos = true;
	}

	ImGui::SetNextWindowSize(ImVec2(200, (spectators.size() + 1) * 16 + 15));
	ImGui::Begin(str("Spectators"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MessageBox);

	auto d = ImGui::GetWindowDrawList();

	auto offset = 0;
	for (auto s : spectators) {
		d->AddText(ImVec2(config->i.vis_speclist_x + 5, config->i.vis_speclist_y + ImGui::GetCurrentWindow()->TitleBarHeight() + 5 + offset), ImColor(255, 255, 255, 255), s.c_str());
		offset += 16;
	}

	if (!core->is_config_working) {
		config->i.vis_speclist_x = ImGui::GetWindowPos().x;
		config->i.vis_speclist_y = ImGui::GetWindowPos().y;
	}

	ImGui::End();
}