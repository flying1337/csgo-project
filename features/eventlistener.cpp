#include "../cheat.h"
#include "../framework/config.h"

void c_eventlistener::initialize() {
	csgo->events->add_listener(this, str("player_hurt"), false);
    csgo->events->add_listener(this, str("player_death"), false);
	csgo->events->add_listener(this, str("game_newmap"), false);
	csgo->events->add_listener(this, str("item_purchase"), false);
	csgo->events->add_listener(this, str("bullet_impact"), false);
	csgo->events->add_listener(this, str("bomb_planted"), false);
	csgo->events->add_listener(this, str("bomb_defused"), false);
	csgo->events->add_listener(this, str("bomb_exploded"), false);
	csgo->events->add_listener(this, str("round_start"), false);
	csgo->events->add_listener(this, str("round_end"), false);
	csgo->events->add_listener(this, str("weapon_fire"), false);
	csgo->events->add_listener(this, str("round_prestart"), false);
	csgo->events->add_listener(this, str("round_freeze_end"), false);
    csgo->events->add_listener(this, str("round_end"), false);
    csgo->events->add_listener(this, str("player_footstep"), false);
    csgo->events->add_listener(this, str("player_jump"), false);
}

void c_eventlistener::release() {
	csgo->events->remove_listener(this);
}

std::string hitgroup_to_name(int hg) {
	if (hg == 0) return str("body");
	if (hg == 1) return str("head");
	if (hg == 2) return str("chest");
	if (hg == 4) return str("left arm");
	if (hg == 5) return str("right arm");
	if (hg == 6) return str("left leg");
	if (hg == 7) return str("right leg");
	return str("body");
}

void c_eventlistener::fire_game_event(c_event* e) {
	auto name = e->get_name();

	if (!strcmp(name, str("player_hurt")) && csgo->cmd) {
		auto attacker_uid = e->get_int(str("attacker"));
		auto hurt_uid = e->get_int(str("userid"));
		auto hitgroup = e->get_int(str("hitgroup"));
		auto health = e->get_int(str("health"));
		auto dmg_health = e->get_int(str("dmg_health"));

		scripting->player_hurt(e);
		auto attacker = (c_player*)csgo->find_cached(csgo->engine->get_entnum(attacker_uid));
		auto hurt = (c_player*)csgo->find_cached(csgo->engine->get_entnum(hurt_uid));

		if (hitgroup > 7 || hitgroup == 3) hitgroup = 0;
		if (csgo->shot.hitgroup > 7 || csgo->shot.hitgroup == 3) csgo->shot.hitgroup = 0;

		if (attacker && hurt) {
			if (csgo->local_player && attacker->index() == csgo->local_player->index()) {
				if (config->b.vis_hitmarker)
					features->visuals->hitmarker();
				if (config->b.vis_hitsound && !core->hide_from_obs)
					csgo->engine->client_cmd_unrestricted(str("play buttons/arena_switch_press_02"));

				if (health <= 0 && config->b.vis_killeffect && !core->hide_from_obs && csgo->local_player)
					csgo->local_player->healthshot_effect() = csgo->globals->servertime + config->f.vis_kf_length;

				if (attacker->index() != hurt->index()) {
				    if (dmg_health - csgo->shot.damage > 15)
                        csgo->brute_stage[csgo->shot.player->index()]++;
					if (config->m.events[0]) {
					    if (!config->b.misc_debuglogs) {
                            if (hitgroup != csgo->shot.hitgroup)
                                utils->log(str("-%ihp in the %s (intended to %s) to %s (%ihp remaining)"), dmg_health,
                                           hitgroup_to_name(hitgroup).c_str(),
                                           hitgroup_to_name(csgo->shot.hitgroup).c_str(), hurt->get_player_info().name,
                                           health);
                            else
                                utils->log(str("-%ihp in the %s to %s (%ihp remaining)"), dmg_health,
                                           hitgroup_to_name(hitgroup).c_str(), hurt->get_player_info().name, health);
                            }
                            else{

                                utils->log(str("-%ihp in the %s to %s (%ihp remaining) / d: %i / s: %i / b: %i"), dmg_health,
                                           hitgroup_to_name(hitgroup).c_str(), hurt->get_player_info().name, health, csgo->shot.damage, csgo->shot.side, csgo->baim_stage[hurt->index()]);
                            }



                    }

				}
				else {
					if (config->m.events[1])
						utils->log(str("-%ihp in the %s to yourself"), dmg_health, hitgroup_to_name(hitgroup).c_str());
				}

				if (csgo->shot.tick == csgo->cmd->tickcount && csgo->shot.player && csgo->shot.player->index() == hurt->index()) {
                    csgo->shot.hurt = true;
                    csgo->shot.hurtgroup = hitgroup;
                }
			}
			else {
				if (csgo->local_player && hurt->index() == csgo->local_player->index()) {
					if (config->m.events[1])
						utils->log(str("-%ihp in the %s by %s"), dmg_health, hitgroup_to_name(hitgroup).c_str(), attacker->get_player_info().name);
				}
			}
		}
	}

	if (!strcmp(name, str("item_purchase"))) {
		auto userid = e->get_int(str("userid"));
		auto team = e->get_int(str("team"));
		auto weapon = e->get_string(str("weapon"));

		auto user = (c_player*)csgo->find_cached(csgo->engine->get_entnum(userid));

		if (user && csgo->local_player && team != csgo->local_player->team()) {
			if (config->m.events[2])
				utils->log(str("%s bought %s"), user->get_player_info().name, weapon);
		}
	}

	if (!strcmp(name, str("player_death"))){
	    scripting->player_death(e);
	}

	if (!strcmp(name, str("game_newmap"))) {
		core->reset_fsn = true;
		core->reset_bloom = true;

		for (auto& i : csgo->brute_stage)
		    i = 0;
	}

	if (!strcmp(name, str("bullet_impact")) && csgo->cmd) {
		auto x = e->get_float(str("x"));
		auto y = e->get_float(str("y"));
		auto z = e->get_float(str("z"));
		auto userid = e->get_int(str("userid"));

		scripting->bullet_impact(e);

		auto user = (c_player*)csgo->find_cached(csgo->engine->get_entnum(userid));
		if (user && csgo->local_player && !core->hide_from_obs && !user->is_dormant()) {
			if (user->team() != csgo->local_player->team() && config->b.vis_bullettracer)
				features->bullettracer->data[user->index()].push_back(c_vector(x, y, z));
            if(user == csgo->local_player) {
                if (config->b.vis_impacts) {
                    csgo->debugoverlay->add_box_overlay(c_vector(x, y, z), c_vector(-2, -2, -2), c_vector(2, 2, 2),
                                                        c_vector(0, 0, 0), 0, 0, 220, 125, 4.f);
                }
            }
            else{
                static auto impacts = csgo->cvar->find_var(str("sv_showimpacts"));
                impacts->set(0);
            }
//			if (csgo->shot.tick == csgo->cmd->tickcount && csgo->shot.player && user->index() == csgo->local_player->index() && !csgo->shot.hit && user->health() > 0) {
//                auto mdl = csgo->modelinfo->get_studio_model(user->get_renderable()->get_model());
//                if (mdl) {
//
//                    auto set = mdl->get_hitbox_set(0);
//                    auto hbx = set->get_hitbox(csgo->shot.hitbox);
//
//                    if (utils->vector_collides(csgo->shot.shotpos, c_vector(x, y, z),
//                                               user->get_hitbox_pos(csgo->shot.hitbox, csgo->shot.bones), hbx->radius))
//                        csgo->shot.hit = true;
//                }
//			}h
            if (csgo->shot.tick == csgo->cmd->tickcount && csgo->shot.player && user->index() == csgo->local_player->index() && !features->ragebot->having_revolver && !csgo->shot.hit && user->health() > 0) {
                c_ray ray;
                ray.init(csgo->local_player->origin() + csgo->local_player->view_offset(), c_vector(x, y, z));

                c_tracefilter_entities_butentity filter;
                filter.skip = csgo->local_player;
                c_trace tr;
                csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_MONSTER, &filter, &tr);

                if (tr.did_hit() && tr.ent && tr.ent->index() == csgo->shot.player->index())
                    csgo->shot.hit = true;
            }



		}
	}

	if (!strcmp(name, str("round_start")) && csgo->cmd) {
        features->visuals->clear();

        for (auto i = 0; i < 65; i++)
            csgo->baim_stage[i] = 0;
    }

	if (!strcmp(name, str("weapon_fire")) && csgo->cmd) {
		auto userid = e->get_int(str("userid"));

		auto user = (c_player*)csgo->find_cached(csgo->engine->get_entnum(userid));
		if (user && user->index() == csgo->local_player->index() && features->ragebot->did_shot) {
			csgo->shot = features->ragebot->last_shot;
			csgo->shot.tick = csgo->cmd->tickcount;

            features->ragebot->did_shot = false;
		}
		if (!user || !csgo->local_player)
		    return;
        if (user->index() == csgo->local_player->index())
            return;
		features->bullettracer->sound_add(user);
	}
	if(!strcmp(name, str("player_footstep"))){
        auto userid = e->get_int(str("userid"));
        auto player = (c_player*)csgo->find_cached(csgo->engine->get_entnum(userid));
        if (!player || !csgo->local_player)
            return;
        if (player->index() == csgo->local_player->index())
            return;
        features->bullettracer->sound_add(player);
	}
    if(!strcmp(name, str("player_jump"))){
        auto userid = e->get_int(str("userid"));
        auto player = (c_player*)csgo->find_cached(csgo->engine->get_entnum(userid));
        if (!player || !csgo->local_player)
            return;
        if (player->index() == csgo->local_player->index())
            return;

        features->bullettracer->sound_add(player);
    }


	if (!strcmp(name, str("round_prestart"))) {
	    scripting->round_prestart();
        csgo->is_freezetime = true;
    }
	if (!strcmp(name, str("round_freeze_end")))
	    csgo->is_freezetime = false;

    if (!strcmp(name, str("round_end"))) {
        scripting->round_end(e);
    }
}