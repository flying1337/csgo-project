#include "../../cheat.h"
#include "../../framework/config.h"
#include "visuals.h"


void c_visuals::draw(ImDrawList* d) {
	if (core->is_updating_entites || !csgo->local_player)
		return;

	if (config->b.vis_esp)
		draw_players(d);
	if (config->b.vis_droppedesp)
		draw_weapons(d);
	if (config->b.vis_projectiles)
		draw_projectiles(d);
	if (config->b.vis_hostages)
        draw_hostages(d);
	if (config->b.vis_pipiska && csgo->aw_damage > 7)
	    draw_aw_damage(d);



	draw_hitmarker(d);
}

void c_visuals::hitmarker() {
	auto w = 0, h = 0;
	csgo->engine->screen_size(w, h);
	
	auto cx = w / 2, cy = h / 2;

	hm.alpha = 255;
	hm.lt[0] = cx - 10;
	hm.lt[1] = cy - 10;
	hm.lt[2] = cx - 3;
	hm.lt[3] = cy - 3;
	hm.rt[0] = cx + 10;
	hm.rt[1] = cy - 10;
	hm.rt[2] = cx + 3;
	hm.rt[3] = cy - 3;
	hm.rb[0] = cx + 10;
	hm.rb[1] = cy + 10;
	hm.rb[2] = cx + 3;
	hm.rb[3] = cy + 3;
	hm.lb[0] = cx - 10;
	hm.lb[1] = cy + 10;
	hm.lb[2] = cx - 3;
	hm.lb[3] = cy + 3;
}


void c_visuals::draw_hitmarker(ImDrawList* d) {
	if (hm.alpha > 0) {
		d->AddLine(ImVec2(hm.lt[0], hm.lt[1]), ImVec2(hm.lt[2], hm.lt[3]), ImColor(255, 255, 255, hm.alpha), 2.f);
		d->AddLine(ImVec2(hm.rt[0], hm.rt[1]), ImVec2(hm.rt[2], hm.rt[3]), ImColor(255, 255, 255, hm.alpha), 2.f);
		d->AddLine(ImVec2(hm.rb[0], hm.rb[1]), ImVec2(hm.rb[2], hm.rb[3]), ImColor(255, 255, 255, hm.alpha), 2.f);
		d->AddLine(ImVec2(hm.lb[0], hm.lb[1]), ImVec2(hm.lb[2], hm.lb[3]), ImColor(255, 255, 255, hm.alpha), 2.f);

		hm.alpha -= 255 / 0.5f * csgo->globals->frametime;

		auto mf = 5 / 0.5f * csgo->globals->frametime;
		hm.lt[0] -= mf;
		hm.lt[1] -= mf;
		hm.lt[2] -= mf;
		hm.lt[3] -= mf;
		hm.rt[0] += mf;
		hm.rt[1] -= mf;
		hm.rt[2] += mf;
		hm.rt[3] -= mf;
		hm.rb[0] += mf;
		hm.rb[1] += mf;
		hm.rb[2] += mf;
		hm.rb[3] += mf;
		hm.lb[0] -= mf;
		hm.lb[1] += mf;
		hm.lb[2] -= mf;
		hm.lb[3] += mf;
	}
}
void c_visuals::draw_aw_damage(ImDrawList* d){
    if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game() || !csgo->local_player->health() > 0) return;
    auto x = 0, y = 0;
    auto damage = config->c.damagecolor;

    csgo->engine->screen_size(x, y);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    auto textsize = ImGui::CalcTextSize(std::to_string(csgo->aw_damage).c_str());
    d->AddTextSoftShadow(ImVec2(x / 2 - (textsize.x / 2), y / 2 + 20), ImColor(damage[0], damage[1], damage[2], damage[3]), std::to_string(csgo->aw_damage).c_str());

    ImGui::PopFont();
}
void c_visuals::work() {
    if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game() || !config->b.vis_esp) return;

    for (auto i = 0; i < 65; i++) {
        auto p = (c_player*)csgo->entities->get_entity(i);
        if (!p || !p->is_player()) { players[i].p = 0; continue; };
        if (p->index() == csgo->local_player->index()) { players[i].p = 0; continue; };
        if (p->team() == csgo->local_player->team() && !config->b.vis_teammates) { players[i].p = 0; continue; };

        auto& pd = players[i];
        pd.is_dormant = p->is_dormant();
        pd.index = p->index();
        pd.weapon = p->weapon();

        if (!pd.is_dormant) {
            pd.info = p->get_player_info();
            pd.has_helmet = p->has_helmet();
            pd.has_heavy_armor = p->has_heavy_armor();
            pd.armor = p->armor();
            pd.money = p->money();
            pd.is_scoped = p->is_scoped();
            pd.is_fake_ducking = p->is_fake_ducking();
            if (pd.real_health > 0) {
                pd.origin = p->get_abs_origin();
                player_origin_data[i] = pd.origin;

                if (pd.alpha <= 0 || pd.health <= 0)
                    pd.health = pd.real_health;
            } else
                pd.origin = player_origin_data[i];

            if (!pd.is_dormant)
                pd.dormancy_entry_time = csgo->globals->realtime;

            pd.real_health = p->health();
        }

        pd.p = p;

        pd.mins = ImCeil(pd.mins);
        pd.maxs = ImCeil(pd.maxs);
    }
}

void c_visuals::draw_players(ImDrawList* d) {
	if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game()) return;

	auto spec = csgo->entities->get_entity_handle(csgo->local_player->spectator());
	for (auto& kv : players) {
		auto& p = kv.second;
	    if (!p.p) continue;

        p.is_offscreen = !p.p->bounding_box(&p.mins, &p.maxs, p.origin);
	    if (p.is_offscreen && config->b.vis_offscreen) {
	        if (spec && spec->index() == p.index) continue;

	        if (p.real_health <= 0) continue;
	        if (csgo->globals->realtime - p.dormancy_entry_time > 7.5f) continue;

            auto radius = (float)config->i.vis_offscreen_offset + 50.f;

            c_angle va;
            csgo->engine->get_view_angles(&va);

            int w, h;
            csgo->engine->screen_size(w, h);

            auto center = ImVec2(w / 2.f, h / 2.f);
            auto rot = utils->rad(va.y - utils->calc_angle(csgo->local_player->origin(), p.origin).y - 90.f);

            auto pos = ImVec2(center.x + radius * cosf(rot) * (2 * (0.5f + 10 * 0.5f * 0.01f)), center.y + radius * sinf(rot));
            auto line = pos - center;

            auto arrowBase = pos - (line * (config->i.vis_offscreen_size / (2 * (tanf(utils->rad(45)) / 2) * line.length())));
            auto normal = ImVec2(-line.y, line.x);
            auto left = arrowBase + normal * (config->i.vis_offscreen_size / (2 * line.length()));
            auto right = arrowBase + normal * (-config->i.vis_offscreen_size / (2 * line.length()));

            auto col = config->c.offscreen;
            auto alpha = utils->map(sinf(csgo->globals->realtime * 2.f), -1.f, 1.f, 0.2f, std::clamp(col[3], 0.2f, 1.f));
            d->AddTriangleFilled(left, right, pos, ImColor(col[0], col[1], col[2], p.is_dormant ? 0.2f : alpha));
	    } else {
	        if (p.is_offscreen) continue;
            if (p.mins.x == 0 || p.mins.y == 0 || p.maxs.x == 0 || p.maxs.y == 0) continue;

            p.mins = ImFloor(p.mins);
            p.maxs = ImFloor(p.maxs);

            p.d = d;

            if (!config->b.vis_noanim) {
                if (p.real_health <= 0) {
                    if (p.alpha > 0)
                        p.alpha -= 255 / 0.25f * csgo->globals->frametime;
                    else
                        p.d = 0;
                }
                else {
                    if (config->b.vis_dormant) {
                        if (p.is_dormant) {
                            if (p.alpha > 64)
                                p.alpha -= 255 / 0.25f * csgo->globals->frametime;
                            else {
                                if (csgo->globals->realtime - p.dormancy_entry_time > 7.5f && p.alpha > 0)
                                    p.alpha -= 64 / 0.25f * csgo->globals->frametime;
                            }
                        }
                        else {
                            if (p.alpha < 255)
                                p.alpha += 255 / 0.25f * csgo->globals->frametime;
                        }
                    }
                    else {
                        if (p.is_dormant && p.alpha > 0)
                            p.alpha -= 255 / 0.25f * csgo->globals->frametime;
                        else if (!p.is_dormant && p.alpha < 255)
                            p.alpha += 255 / 0.25f * csgo->globals->frametime;
                    }
                }

                if (p.real_health < p.health)
                    p.health -= abs(p.real_health - p.health) / 0.25f * csgo->globals->frametime;
                else
                    p.health = p.real_health;
            }
            else {
                if (p.real_health <= 0) {
                    p.d = 0;
                    p.alpha = 0;
                }
                else {
                    p.health = p.real_health;

                    if (config->b.vis_dormant) {
                        if (p.is_dormant) {
                            if (p.alpha > 64)
                                p.alpha = 64;
                            else {
                                if (csgo->globals->realtime - p.dormancy_entry_time > 7.5f && p.alpha > 0)
                                    p.alpha = 0;
                            }
                        }
                        else {
                            if (p.alpha < 255)
                                p.alpha = 255;
                        }
                    }
                    else {
                        if (p.is_dormant && p.alpha > 0)
                            p.alpha = 0;
                        else if (!p.is_dormant && p.alpha < 255)
                            p.alpha = 255;
                    }
                }
            }

            p.alpha = std::clamp(p.alpha, 0, 255);
            p.health = std::clamp(p.health, 0, 100);

            if (!p.d) continue;
            if (p.health == 0) continue;
            if (p.alpha == 0) continue;

            if (config->b.vis_boundingbox)
                draw_player_box(p);
            if (config->b.vis_healthbar)
                draw_player_health(p);
            if (config->b.vis_name)
                draw_player_name(p);
            if (config->is_anything_selected(config->m.vis_info))
                draw_player_sidebar(p);
            if (config->b.vis_ammo || config->i.vis_activeweapon > 0)
                draw_player_bottom(p);
	    }
	}
}

void c_visuals::clear() {
    players.clear();
    player_origin_data.clear();
    weapons.clear();
    projectiles.clear();
}

void c_visuals::draw_player_box(s_player p) {
	auto col = config->c.boundingbox;

	p.d->AddRect(p.mins - ImVec2(1, 1), p.maxs + ImVec2(1, 1), ImColor(0, 0, 0, std::clamp(std::clamp(int(col[3] * 255.f), 0, 150), 0, p.alpha)));
	p.d->AddRect(p.mins, p.maxs, ImColor(col[0], col[1], col[2], std::clamp(col[3], 0.f, p.alpha / 255.f)));
	p.d->AddRect(p.mins + ImVec2(1, 1), p.maxs - ImVec2(1, 1), ImColor(0, 0, 0, std::clamp(std::clamp(int(col[3] * 255.f), 0, 150), 0, p.alpha)));
}

void c_visuals::draw_player_health(s_player p) {
	auto h = fabs(p.maxs.y - p.mins.y);
	auto height = h - (p.health * h / 100);

	p.d->AddRectFilled(p.mins - ImVec2(7, 2), p.mins + ImVec2(-3, h + 1), ImColor(0, 0, 0, std::clamp(150, 0, p.alpha)));
	p.d->AddRectFilled(p.mins + ImVec2(-6, h - (h - height) - 1), p.mins + ImVec2(-4, h), percent_to_color(p.health, p.alpha));

	if (p.health < 90 && p.health > 0) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		auto fs = ImGui::CalcTextSize(std::to_string(p.health).c_str());

		p.d->AddTextShadow(p.mins + ImVec2(-9, height - 6), ImColor(255, 255, 255, p.alpha), std::to_string(p.health).c_str());
		ImGui::PopFont();
	}
}

void c_visuals::draw_player_name(s_player p) {
	auto name = config->c.name;

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	auto textsize = ImGui::CalcTextSize(p.info.name);

	p.d->AddTextShadow(p.mins + ImVec2((p.maxs.x - p.mins.x) / 2 - textsize.x / 2 - 2, -14), ImColor(name[0], name[1], name[2], std::clamp(name[3], 0.f, p.alpha / 255.f)), p.info.name);
	ImGui::PopFont();
}

void c_visuals::draw_player_sidebar(s_player p) {
	struct sideinfo_t {
		ImColor col;
		std::string text;
	};

	std::vector<sideinfo_t> inf;
	inf.reserve(16);

	auto info = config->m.vis_info;
	if (info[0]) {
		std::string av;
		if (p.has_helmet)
			av += str("H");
		if (p.has_heavy_armor || p.armor > 0)
			av += str("K");

		if (av != "")
			inf.push_back({ ImColor(1.f, 1.f, 1.f, p.alpha / 255.f), av });
	}

	if (info[1])
		inf.push_back({ ImColor(65, 244, 79, p.alpha),  str("$") + std::to_string(p.money) });

	if (info[2] && p.is_scoped)
		inf.push_back({ ImColor(65, 169, 244, p.alpha), str("ZOOM") });

	if (info[3] && p.is_fake_ducking)
		inf.push_back({ ImColor(244, 65, 65, p.alpha), str("DUCK") });

	if (info[4] && csgo->playerresources)
		inf.push_back({ ImColor(244, 149, 66, p.alpha), std::to_string(csgo->playerresources->ping(p.index)) + str("MS") });

	if (info[5])
		inf.push_back({ ImColor(1.f, 1.f, 1.f, p.alpha / 255.f), std::to_string(int(p.origin.dist(csgo->local_player->origin()) * 0.02f)) + str("M") });

	if (info[6] && csgo->playerresources && csgo->playerresources->c4_carrier() == p.index)
		inf.push_back({ ImColor(244, 65, 65, p.alpha), str("BOMB") });

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

	int offset = 0;
	for (auto i : inf) {
		p.d->AddTextShadow(p.maxs + ImVec2(4, -(p.maxs.y - p.mins.y) + offset - 2), i.col, i.text.c_str());
		offset += 8;
	}

	ImGui::PopFont();
}

void c_visuals::draw_player_bottom(s_player p) {
	struct bottominfo_t {
		ImColor color;
		std::string text;
		float max;
		float value;
		bool string;
	};

	std::vector<bottominfo_t> inf;
	inf.reserve(16);

	auto wep = (c_weapon*)csgo->entities->get_entity_handle(p.weapon);
	if (wep) {
		auto wi = csgo->weapon_infos[wep->index()];
		if (wi) {
			if (config->b.vis_ammo && wep->clip() > -1) {
				auto ammo = config->c.ammo;
				inf.push_back({ ImColor(ammo[0], ammo[1], ammo[2], std::clamp(ammo[3], 0.f, p.alpha / 255.f)),
					wep->clip() < 4 ? std::to_string(wep->clip()) : "",
					(float)wi->max_clip, (float)std::clamp(wep->clip(), 0, std::clamp(wi->max_clip, 0, 255)), false });
			}

			if (config->i.vis_activeweapon > 0) {
				auto wc = config->c.weapon;
				inf.push_back({ ImColor(wc[0], wc[1], wc[2], p.alpha / 255.f), wep->get_icon(config->i.vis_activeweapon == 1), 0, 0, true });
			}
		}
	}

	int offy = 0;
	for (auto i : inf) {
		if (i.string) {
			offy += 2.f;

			ImGui::PushFont(config->i.vis_activeweapon == 1 ? ImGui::GetIO().Fonts->Fonts[8] : ImGui::GetIO().Fonts->Fonts[2]);
			auto sz = ImGui::CalcTextSize(i.text.c_str());

			p.d->AddTextShadow(p.maxs - ImVec2(sz.x / 2.f, 0) - ImVec2((p.maxs.x - p.mins.x) / 2.f, 0) + ImVec2(0, offy) - ImVec2(1, 1), i.color, i.text.c_str());
			ImGui::PopFont();

			offy += sz.y + 2;
		}

		if (!i.string) {
			p.d->AddRectFilled(
				p.mins + ImVec2(0, p.maxs.y - p.mins.y) + ImVec2(-2, offy) + ImVec2(0, 2), 
				p.maxs + ImVec2(0, offy) + ImVec2(1, 6), ImColor(0.f, 0.f, 0.f, std::clamp(p.alpha, 0, 150) / 255.f));
			p.d->AddRectFilled(
				p.mins + ImVec2(0, p.maxs.y - p.mins.y) + ImVec2(0, offy) + ImVec2(0, 2) + ImVec2(-1, 1),
				p.maxs + ImVec2(0, offy) + ImVec2(0, 6) - ImVec2(1, 1) - ImVec2(p.maxs.x - p.mins.x - 2, 0) + ImVec2(float(p.maxs.x - p.mins.x - 1) / float(i.max) * float(i.value), 0),
				i.color);

			if (i.text != "") {
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
				p.d->AddTextShadow(
					p.maxs + ImVec2(0, offy) + ImVec2(0, 6) - ImVec2(1, 1) - ImVec2(p.maxs.x - p.mins.x - 2, 0) + 
					ImVec2(float(p.maxs.x - p.mins.x - 2) / float(i.max) * float(i.value), 0) - ImVec2(1, 6), ImColor(1.f, 1.f, 1.f, p.alpha / 255.f), i.text.c_str());
				ImGui::PopFont();
			}

			offy += 6;
		}
	}
}

ImColor c_visuals::percent_to_color(int per, int alpha) {
	int red = per < 50 ? 200 : std::clamp(floorf(200 - (per * 2 - 100) * 200.f / 100.f), 0.f, 124.f);
	int green = per > 50 ? 200 : std::clamp(floorf((per * 2) * 200.f / 100.f), 0.f, 195.f);

	return ImColor(red, green, 13, alpha);
}

void c_visuals::draw_weapons(ImDrawList* d) {
	if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game()) return;

	for (auto i = 65; i < csgo->entities->get_highest_index(); i++) {
		auto w = (c_weapon*)csgo->entities->get_entity(i);
		if (!w || !w->is_weapon()) continue;
		
		auto owner = csgo->entities->get_entity_handle(w->owner());
		if (owner) continue;

		ImVec2 mins, maxs;
		if (!w->bounding_box(&mins, &maxs))
			continue;
		else {
			mins = ImCeil(mins);
			maxs = ImCeil(maxs);

			if (mins.x == 0 || mins.y == 0 || maxs.x == 0 || maxs.y == 0) continue;

			auto& wd = weapons[w->index()];
			if (wd.d) {
				wd.mins = mins;
				wd.maxs = maxs;
				wd.alpha = utils->map(std::clamp(csgo->local_player->origin().dist(w->origin()) * 0.02f, 0.f, config->i.vis_droppeddd == 26 ? 9999.f : config->i.vis_droppeddd),
					0.f, config->i.vis_droppeddd == 26 ? 9999.f : config->i.vis_droppeddd, 255.f, 0.f);
			}
			else {
				wd.d = d;
				wd.alpha = utils->map(std::clamp(csgo->local_player->origin().dist(w->origin()) * 0.02f, 0.f, config->i.vis_droppeddd == 26 ? 9999.f : config->i.vis_droppeddd),
					0.f, config->i.vis_droppeddd == 26 ? 9999.f : config->i.vis_droppeddd, 255.f, 0.f);
				wd.mins = mins;
				wd.maxs = maxs;
			}

			wd.w = w;

			if (config->b.vis_droppedam || config->i.vis_droppedaw)
				draw_weapon_bottom(wd);
			if (config->b.vis_droppeddist)
				draw_weapon_sidebar(wd);
		}
	}
}


void c_visuals::draw_weapon_bottom(s_weapon w) {
	struct bottominfo_t {
		ImColor color;
		std::string text;
		float max;
		float value;
		bool string;
	};

	std::vector<bottominfo_t> inf;
	inf.reserve(16);

	auto wep = w.w;
	if (wep) {
		auto wi = csgo->weapon_infos[wep->index()];
		if (wi) {
			if (config->b.vis_droppedam && wep->clip() > -1) {
				auto ammo = config->c.droppedam;
				inf.push_back({ ImColor(ammo[0], ammo[1], ammo[2], std::clamp(ammo[3], 0.f, w.alpha / 255.f)),
					wep->clip() < 4 ? std::to_string(wep->clip()) : "",
					(float)wi->max_clip, (float)std::clamp(wep->clip(), 0, std::clamp(wi->max_clip, 0, 255)), false });
			}

			if (config->i.vis_droppedaw > 0) {
				auto wc = config->c.droppedaw;
				inf.push_back({ ImColor(wc[0], wc[1], wc[2], w.alpha / 255.f), wep->get_icon(config->i.vis_droppedaw == 1), 0, 0, true });
			}
		}
	}

	int offy = 0;
	for (auto i : inf) {
		if (i.string) {
			offy += 2.f;

			ImGui::PushFont(config->i.vis_droppedaw == 1 ? ImGui::GetIO().Fonts->Fonts[8] : ImGui::GetIO().Fonts->Fonts[2]);
			auto sz = ImGui::CalcTextSize(i.text.c_str());

			w.d->AddTextShadow(w.maxs - ImVec2(sz.x / 2.f, 0) - ImVec2((w.maxs.x - w.mins.x) / 2.f, 0) + ImVec2(0, offy) - ImVec2(1, 1), i.color, i.text.c_str());
			ImGui::PopFont();
			offy += sz.y + 2;
		}

		if (!i.string) {
			w.d->AddRectFilled(
				w.mins + ImVec2(0, w.maxs.y - w.mins.y) + ImVec2(-2, offy) + ImVec2(0, 2),
				w.maxs + ImVec2(0, offy) + ImVec2(1, 6), ImColor(0.f, 0.f, 0.f, std::clamp(w.alpha, 0, 150) / 255.f));
			w.d->AddRectFilled(
				w.mins + ImVec2(0, w.maxs.y - w.mins.y) + ImVec2(0, offy) + ImVec2(0, 2) + ImVec2(-1, 1),
				w.maxs + ImVec2(0, offy) + ImVec2(0, 6) - ImVec2(1, 1) - ImVec2(w.maxs.x - w.mins.x - 2, 0) + ImVec2(float(w.maxs.x - w.mins.x - 1) / float(i.max) * float(i.value), 0),
				i.color);

			if (i.text != "") {
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
				w.d->AddTextShadow(
					w.maxs + ImVec2(0, offy) + ImVec2(0, 6) - ImVec2(1, 1) - ImVec2(w.maxs.x - w.mins.x - 2, 0) +
					ImVec2(float(w.maxs.x - w.mins.x - 2) / float(i.max) * float(i.value), 0) - ImVec2(1, 6), ImColor(1.f, 1.f, 1.f, w.alpha / 255.f), i.text.c_str());
				ImGui::PopFont();
			}
			offy += 6;
		}
	}
}

void c_visuals::draw_weapon_sidebar(s_weapon w) {
	struct sideinfo_t {
		ImColor col;
		std::string text;
	};

	std::vector<sideinfo_t> inf;
	inf.reserve(16);
	inf.push_back({ ImColor(1.f, 1.f, 1.f, w.alpha / 255.f), std::to_string(int(w.w->origin().dist(csgo->local_player->origin()) * 0.02f)) + str("M") });

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

	int offset = 0;
	for (auto i : inf) {
		w.d->AddTextShadow(w.maxs + ImVec2(4, -(w.maxs.y - w.mins.y) + offset - 2), i.col, i.text.c_str());
		offset += 8;
	}

	ImGui::PopFont();
}


void c_visuals::draw_molotov(ImDrawList* d){ //TODO: Finish this bs
    if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game()) return;

    for (auto i = 65; i < csgo->entities->get_highest_index(); i++) {
        auto e = csgo->entities->get_entity(i);
        if (!e) continue;
        auto client_class = e->get_client_class();

        if (client_class->class_id != DT_MolotovProjectile)
            continue;

        const auto origin = e->get_abs_origin();
        c_vector screen_org;
        c_vector last_origin;

        if (!utils->world_to_screen(origin, &screen_org))
            continue;

            const auto spawn_time = e->get_spawn_time();
            const auto time_left = spawn_time + e->get_expiry_time() - csgo->globals->servertime;

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            d->AddTextSoftShadow(ImVec2(last_origin.x, last_origin.y), ImColor(255, 255, 255, 255),
                                 std::to_string(time_left).c_str());
            ImGui::PopFont();

    }
}


void c_visuals::draw_projectiles(ImDrawList* d) {
	if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game()) return;

	for (auto i = 65; i < csgo->entities->get_highest_index(); i++) {
		auto e = csgo->entities->get_entity(i);

		if (!e) continue;
		if (!e->is_projectile()) continue;

		auto p = (c_projectile*)e;

		ImVec2 mins, maxs;
		if (!p->bounding_box(&mins, &maxs))
			continue;
		else {
			mins = ImCeil(mins);
			maxs = ImCeil(maxs);

			if (mins.x == 0 || mins.y == 0 || maxs.x == 0 || maxs.y == 0) continue;

			auto& pd = projectiles[p->index()];
			if (pd.d) {
				pd.mins = mins;
				pd.maxs = maxs;
			}
			else {
				pd.d = d;
				pd.mins = mins;
				pd.maxs = maxs;
			}

			pd.p = p;


			if (config->i.vis_projectilesna)
				draw_projectile_bottom(pd);
		}
	}
}



void c_visuals::draw_projectile_bottom(s_projectile p) {
	struct bottominfo_t {
		ImColor color;
		std::string text;
		float max;
		float value;
		bool string;
	};

	std::vector<bottominfo_t> inf;
	inf.reserve(16);

	auto wc = config->c.projectilesna;
	inf.push_back({ ImColor(wc[0], wc[1], wc[2], wc[3]), p.p->get_icon(config->i.vis_projectilesna == 1), 0, 0, true });


	int offy = 0;
	for (auto i : inf) {
		if (i.string) {
			offy += 2.f;

			ImGui::PushFont(config->i.vis_projectilesna == 1 ? ImGui::GetIO().Fonts->Fonts[8] : ImGui::GetIO().Fonts->Fonts[2]);
			auto sz = ImGui::CalcTextSize(i.text.c_str());

			p.d->AddTextShadow(p.maxs - ImVec2(sz.x / 2.f, 0) - ImVec2((p.maxs.x - p.mins.x) / 2.f, 0) + ImVec2(0, offy) - ImVec2(1, 1), i.color, i.text.c_str());
			ImGui::PopFont();

			offy += sz.y + 2;
		}
	}
}

void c_visuals::draw_hostages(ImDrawList *d) {
    if (!csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game()) return;

    for (auto i = 65; i < csgo->entities->get_highest_index(); i++) {
        auto e = csgo->entities->get_entity(i);

        if (!e) continue;
        if (!e->is_hostage()) continue;

        auto p = (c_hostage*)e;
        if (csgo->local_player->origin().dist(p->origin()) > 1000.f) continue;

        ImVec2 mins, maxs;
        if (!p->bounding_box(&mins, &maxs))
            continue;
        else {
            mins = ImCeil(mins);
            maxs = ImCeil(maxs);

            if (mins.x == 0 || mins.y == 0 || maxs.x == 0 || maxs.y == 0) continue;

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            auto textsize = ImGui::CalcTextSize(str("Hostage"));

            d->AddTextShadow(mins + ImVec2((maxs.x - mins.x) / 2 - textsize.x / 2 - 2, -14),
                    ImColor(config->c.hostages[0], config->c.hostages[1], config->c.hostages[2], config->c.hostages[3]), str("Hostage"));
            ImGui::PopFont();
        }
    }
}

struct SndInfo_t
{
    int			m_nGuid;
    void*       m_filenameHandle;
    int			m_nSoundSource;
    int			m_nChannel;
    int			m_nSpeakerEntity;
    float		m_flVolume;
    float		m_flLastSpatializedVolume;
    float		m_flRadius;
    int			m_nPitch;
    c_vector	*m_pOrigin;
    c_vector	*m_pDirection;
    bool		m_bUpdatePositions;
    bool		m_bIsSentence;
    bool		m_bDryMix;
    bool		m_bSpeaker;
    bool		m_bFromServer;
};

void c_visuals::update_positions() {
    if (csgo->local_player) {
        CUtlVector<SndInfo_t> snds;
        g_vfunc(void(__thiscall*)(void*, CUtlVector<SndInfo_t> & snd), csgo->enginesound, 19)(csgo->enginesound, snds);

        for (auto i = 0; i < snds.Count(); i++) {
            auto s = snds[i];
            if (s.m_nSoundSource == 0 || s.m_nSoundSource > 64)  continue;
            if (!s.m_pOrigin) continue;

            auto ent = csgo->entities->get_entity(s.m_nSoundSource);
            if (!ent || ent->index() == csgo->local_player->index())
                continue;

            if (ent->is_dormant() && ent->is_player()) {
                auto pd = &features->visuals->players[ent->index()];
                pd->origin = *s.m_pOrigin;
                pd->dormancy_entry_time = csgo->globals->realtime;
            }
        }
    }
}
