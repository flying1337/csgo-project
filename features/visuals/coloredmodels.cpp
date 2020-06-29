#include "../../cheat.h"
#include "../../framework/config.h"

bool c_coloredmodels::work(c_entity* ent, void* a, void* b, c_drawmodelstate c, c_modelrenderinfo d, c_matrix3x4* e) {
    if (!csgo->local_player) return false;

	obj = a; arg_2 = b; state = c; render_info = d; bones = e;

	if (!glow_enemy)
	    glow_enemy = csgo->materialsystem->find_material(str("csgo_shaded_glow_walls"), str("Model textures"));
    if (!glow_team)
        glow_team = csgo->materialsystem->find_material(str("csgo_shaded_glow_walls"), str("Model textures"));
    if (!glow_self)
        glow_self = csgo->materialsystem->find_material(str("csgo_shaded_glow_normal"), str("Model textures"));

	if (type != config->i.vis_cm_type) {
		type = config->i.vis_cm_type;

		if (type == 0) {
			open = csgo->materialsystem->find_material(str("csgo_default_normal"), str("Model textures"));
			cover = csgo->materialsystem->find_material(str("csgo_default_walls"), str("Model textures"));
		}

		if (type == 1) {
			open = csgo->materialsystem->find_material(str("csgo_flat_normal"), str("Model textures"));
			cover = csgo->materialsystem->find_material(str("csgo_flat_walls"), str("Model textures"));
		}

		if (type == 2) {
			open = csgo->materialsystem->find_material(str("csgo_reflective_normal"), str("Model textures"));
			cover = csgo->materialsystem->find_material(str("csgo_reflective_walls"), str("Model textures"));
		}

        if (type == 3) {
            open = csgo->materialsystem->find_material(str("csgo_celshaded_normal"), str("Model textures"));
            cover = csgo->materialsystem->find_material(str("csgo_celshaded_walls"), str("Model textures"));
        }
	}

    if (backtrack_type != config->i.vis_cm_backtrack) {
        backtrack_type = config->i.vis_cm_backtrack;

        if (backtrack_type == 0)
            backtrack_cover = csgo->materialsystem->find_material(str("csgo_default_walls"), str("Model textures"));
        if (backtrack_type == 1)
            backtrack_cover = csgo->materialsystem->find_material(str("csgo_flat_walls"), str("Model textures"));
        if (backtrack_type == 2)
            backtrack_cover = csgo->materialsystem->find_material(str("csgo_reflective_walls"), str("Model textures"));
        if (backtrack_type == 3)
            backtrack_cover = csgo->materialsystem->find_material(str("csgo_celshaded_walls"), str("Model textures"));
    }

	if (team_type != config->i.vis_cm_team_type) {
		team_type = config->i.vis_cm_team_type;

		if (team_type == 0) {
			team_open = csgo->materialsystem->find_material(str("csgo_default_normal"), str("Model textures"));
			team_cover = csgo->materialsystem->find_material(str("csgo_default_walls"), str("Model textures"));
		}

		if (team_type == 1) {
			team_open = csgo->materialsystem->find_material(str("csgo_flat_normal"), str("Model textures"));
			team_cover = csgo->materialsystem->find_material(str("csgo_flat_walls"), str("Model textures"));
		}

		if (team_type == 2) {
			team_open = csgo->materialsystem->find_material(str("csgo_reflective_normal"), str("Model textures"));
			team_cover = csgo->materialsystem->find_material(str("csgo_reflective_walls"), str("Model textures"));
		}

        if (team_type == 3) {
            team_open = csgo->materialsystem->find_material(str("csgo_celshaded_normal"), str("Model textures"));
            team_cover = csgo->materialsystem->find_material(str("csgo_celshaded_walls"), str("Model textures"));
        }
	}

    if (fake_type != config->i.vis_cm_fake_type) {
        fake_type = config->i.vis_cm_fake_type;

        if (fake_type == 0)
            fake_cover = csgo->materialsystem->find_material(str("csgo_default_walls"), str("Model textures"));
        if (fake_type == 1)
            fake_cover = csgo->materialsystem->find_material(str("csgo_flat_walls"), str("Model textures"));
        if (fake_type == 2)
            fake_cover = csgo->materialsystem->find_material(str("csgo_reflective_walls"), str("Model textures"));
        if (fake_type == 3) {
            fake_cover = csgo->materialsystem->find_material(str("models/inventory_items/dogtags/dogtags_outline"),
                                                             str("Model textures"));
            fake_cover->set_flag(MATERIAL_VAR_IGNOREZ, true);
        }
        if (fake_type == 4)
            fake_cover = csgo->materialsystem->find_material(str("csgo_celshaded_walls"), str("Model textures"));

        if (fake_type == 5)
            fake_cover = csgo->materialsystem->find_material(str("glowOverlay"), str("Model textures"));
    }

    if (self_type != config->i.vis_cm_self_type) {
        self_type = config->i.vis_cm_self_type;

        if (self_type == 0)
            self_open = csgo->materialsystem->find_material(str("csgo_default_normal"), str("Model textures"));
        if (self_type == 1)
            self_open = csgo->materialsystem->find_material(str("csgo_flat_normal"), str("Model textures"));
        if (self_type == 2)
            self_open = csgo->materialsystem->find_material(str("csgo_reflective_normal"), str("Model textures"));
        if (self_type == 3)
            self_open = csgo->materialsystem->find_material(str("csgo_celshaded_normal"), str("Model textures"));
    }

	if (ent->is_player())
		return process_player((c_player*)ent);

	return false;
}

bool c_coloredmodels::process_player(c_player* p) {
	auto changed = false;

	if (p->index() == csgo->local_player->index()) {
	    auto as = p->get_anim_state();
	    if (config->b.vis_cm_fake && as && !config->b.vis_cm_fake_overlayed && !(keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style))) {
            csgo->renderview->set_blend(!p->is_scoped() ? config->c.fake[3] : std::clamp(config->c.fake[3], 0.f, 0.5f));
            csgo->renderview->set_color_modulation(config->c.fake);
            if (fake_type == 5) {
                auto found1 = false;
                auto var1 = fake_cover->find_var(str("$envmaptint"), &found1);
                if (found1 && var1)
                    var1->set_vec(config->c.fake, 3);
            }

            c_matrix3x4 nb[128];
            memcpy(nb, csgo->body_bones, sizeof(c_matrix3x4) * 128);

            if (config->b.vis_cm_nolagpos)
                utils->reposition_matrix(nb, csgo->sentpos, p->get_abs_origin());

            csgo->modelrender->forced_material_override(fake_cover);
            oDrawModelExecute(obj, arg_2, state, render_info, nb);
            csgo->modelrender->forced_material_override(0);
	    }

	    if (config->b.vis_cm_self) {
            csgo->renderview->set_blend(!p->is_scoped() ? config->c.self[3] : std::clamp(config->c.self[3], 0.f, 0.5f));
            csgo->renderview->set_color_modulation(config->c.self);

            csgo->modelrender->forced_material_override(self_open);
            oDrawModelExecute(obj, arg_2, state, render_info, bones);
            csgo->modelrender->forced_material_override(0);

            changed = true;
	    }

	    if (config->i.vis_glow_self == 2 && !p->is_scoped()) {
	        if (!changed)
                oDrawModelExecute(obj, arg_2, state, render_info, bones);

	        auto found = false;
	        auto var = glow_self->find_var(str("$envmaptint"), &found);
	        if (found && var)
	           var->set_vec(config->c.glow_self, 3);

            csgo->renderview->set_blend(config->c.glow_self[3]);
            csgo->renderview->set_color_modulation(config->c.glow_self);

            csgo->modelrender->forced_material_override(glow_self);
            oDrawModelExecute(obj, arg_2, state, render_info, bones);
            csgo->modelrender->forced_material_override(0);

            changed = true;
	    }

        if (config->b.vis_cm_fake && as && config->b.vis_cm_fake_overlayed && !(keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style))) {
            csgo->renderview->set_blend(!p->is_scoped() ? config->c.fake[3] : std::clamp(config->c.fake[3], 0.f, 0.5f));
            csgo->renderview->set_color_modulation(config->c.fake);
            if (fake_type == 5) {
                auto found2 = false;
                auto var2 = fake_cover->find_var(str("$envmaptint"), &found2);
                if (found2 && var2)
                    var2->set_vec(config->c.fake, 3);
            }
            c_matrix3x4 nb[128];
            memcpy(nb, csgo->body_bones, sizeof(c_matrix3x4) * 128);

            if (config->b.vis_cm_nolagpos)
                utils->reposition_matrix(nb, csgo->sentpos, p->get_abs_origin());

            csgo->modelrender->forced_material_override(fake_cover);
            oDrawModelExecute(obj, arg_2, state, render_info, nb);
            csgo->modelrender->forced_material_override(0);
        }

        csgo->renderview->set_blend(p->is_scoped() ? 0.5f : 1.f);
        csgo->renderview->set_color_modulation(new float[3] {1, 1, 1});

		return changed;
	}
	else {
		if (p->team() == csgo->local_player->team()) {			
			if (config->b.vis_cm_team) {
				csgo->renderview->set_blend(csgo->goes_thru[p->index()] ? 0.25f : config->c.cm_team[3]);
				csgo->renderview->set_color_modulation(config->c.cm_team);

				csgo->modelrender->forced_material_override(team_cover);
				oDrawModelExecute(obj, arg_2, state, render_info, bones);
				csgo->modelrender->forced_material_override(team_open);
				oDrawModelExecute(obj, arg_2, state, render_info, bones);
				csgo->modelrender->forced_material_override(0);

				changed = true;
			}

            if (config->i.vis_glow_team == 2) {
                if (!changed)
                    oDrawModelExecute(obj, arg_2, state, render_info, bones);

                auto found = false;
                auto var = glow_team->find_var(str("$envmaptint"), &found);
                if (found && var)
                    var->set_vec(config->c.glow_team, 3);

                csgo->renderview->set_blend(csgo->goes_thru[p->index()] ? 0.25f : config->c.glow_team[3]);
                csgo->renderview->set_color_modulation(config->c.glow_team);

                csgo->modelrender->forced_material_override(glow_team);
                oDrawModelExecute(obj, arg_2, state, render_info, bones);
                csgo->modelrender->forced_material_override(0);

                changed = true;
            }

			return changed;
		}
		else {
		    auto v = features->lc->get(p);
            if (config->b.vis_cm_backtrack && !v.empty()) {
                csgo->renderview->set_blend(config->c.cm_backtrack[3]);
                csgo->renderview->set_color_modulation(config->c.cm_backtrack);

                csgo->modelrender->forced_material_override(backtrack_cover);
                oDrawModelExecute(obj, arg_2, state, render_info, v.front().bones);
                csgo->modelrender->forced_material_override(0);
            }

			if (config->b.vis_cm_players_xqz) {
				csgo->renderview->set_blend(config->c.cm_players_xqz[3]);
				csgo->renderview->set_color_modulation(config->c.cm_players_xqz);

				csgo->modelrender->forced_material_override(cover);
				oDrawModelExecute(obj, arg_2, state, render_info, bones);
                csgo->modelrender->forced_material_override(0);

				changed = true;
			}

			if (config->b.vis_cm_players) {
				csgo->renderview->set_blend(config->c.cm_players[3]);
				csgo->renderview->set_color_modulation(config->c.cm_players);

				csgo->modelrender->forced_material_override(open);
				oDrawModelExecute(obj, arg_2, state, render_info, bones);
				csgo->modelrender->forced_material_override(0);

				changed = true;
			}

            if (config->i.vis_glow == 2) {
                if (!changed)
                    oDrawModelExecute(obj, arg_2, state, render_info, bones);

                auto found = false;
                auto var = glow_enemy->find_var(str("$envmaptint"), &found);
                if (found && var)
                    var->set_vec(config->c.glow, 3);

                csgo->renderview->set_blend(config->c.glow[3]);
                csgo->renderview->set_color_modulation(config->c.glow);

                csgo->modelrender->forced_material_override(glow_enemy);
                oDrawModelExecute(obj, arg_2, state, render_info, bones);
                csgo->modelrender->forced_material_override(0);

                changed = true;
            }

            if (!changed) {
                csgo->renderview->set_blend(1.f);
                csgo->renderview->set_color_modulation(new float[3] { 1.f, 1.f, 1.f });

                oDrawModelExecute(obj, arg_2, state, render_info, bones);
            }

			return true;
		}
	}
}