//
// Created by ruppet on 24.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"
#include "fakelag.h"


void c_fakelag::estimate_max_choke(c_usercmd* c) {
    noclamp = false;

    if ((config->b.aa_disableonfreeze && csgo->is_freezetime) || csgo->engine->is_voice_recoding())
        return force_choke(1);

    if (!(csgo->local_player->flags() & FL_ONGROUND))
        estimate(2);
    else {
        if (csgo->local_player->velocity().length() < 1.02f)
            estimate(0);
        else
            estimate(1);
    }

    if (features->ragebot->wep && features->ragebot->wep->get_type() != WEAPONTYPE_GRENADE && !features->ragebot->having_revolver) {
        if (config->b.aa_fl_disableshooting && c->buttons & IN_ATTACK && !keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style))
            return force_choke(1);
    }
}

void c_fakelag::estimate(int s) {
    if (s == 0) speed = 0.f;

    if (!config->i.aa_fakelag_mode[s])
        return force_choke(1);

    if (config->i.aa_fakelag_mode[s] == 1 || (config->i.aa_fakelag_mode[s] == 2 && speed < csgo->local_player->velocity().length())) {
        if (config->i.aa_fakelag_mode[s] == 2) speed = csgo->local_player->velocity().length();

        if (config->i.aa_fakelag_style[s] == 0)
            max_choke = variance(config->i.aa_fakelag_limit[s], config->i.aa_fakelag_var[s]);
        if (config->i.aa_fakelag_style[s] == 1) {
            max_choke = fluctate ? 1 : variance(config->i.aa_fakelag_limit[s], config->i.aa_fakelag_var[s]);
            fl_tick = variance(config->i.aa_fakelag_limit[s], config->i.aa_fakelag_var[s]);
        }
        if (config->i.aa_fakelag_style[s] == 2)
            max_choke = variance(config->i.aa_fakelag_limit[s], config->i.aa_fakelag_var[s]) / 250.f
                    * std::clamp(fabsf(csgo->local_player->velocity().length()), 0.f, 250.f);
        if (config->i.aa_fakelag_style[s] == 3)
            max_choke = rand() % 13 + 1;
    }

    if (keymanager->check(config->i.aa_slowmo, config->i.aa_slowmo_style) && config->i.aa_slowmo_type == 1)
        max_choke = 14;

    if (config->b.rage_silent)
        max_choke = std::clamp(max_choke, 1, 5);

    csgo->want_sp = max_choke <= csgo->choke;
}

int c_fakelag::variance(int limit, int variance) {
    auto min = (float)limit - (float)limit / 100.f * (float)variance;
    return min + (rand() % int(limit - min + 1));
}

void c_fakelag::choke(c_usercmd* c) {
    if (noclamp)
        max_choke = std::clamp(max_choke, 1, 14);

    if (features->ragebot->wep && features->ragebot->wep->get_type() != WEAPONTYPE_GRENADE) {
        if (config->b.aa_fl_disableshooting && c->buttons & IN_ATTACK && !features->ragebot->having_revolver &&
            !keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style))
            max_choke = 1;
        if (config->b.aa_fl_disableshooting && !features->ragebot->having_revolver &&
            !keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style) &&
            csgo->globals->tickcount - csgo->shot.tick < 3)
            max_choke = 1;
    }

    if (csgo->choke < max_choke)
        csgo->allow_sp = false;
    else
        csgo->allow_sp = true;

    if (csgo->globals->tickcount - last_change >= fl_tick) {
        fluctate = !fluctate;
        last_change = csgo->globals->tickcount;
    }
}

void c_fakelag::force_choke(int v, bool force) {
    max_choke = v;
    csgo->want_sp = max_choke <= csgo->choke;

    if (force)
        noclamp = true;
}
