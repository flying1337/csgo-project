//
// Created by ruppet on 24.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"
#include "antiaims.h"


void c_antiaims::work(c_usercmd* c, float& sidemove, float& forwardmove) {
    if (config->b.aa_disableonfreeze && csgo->is_freezetime)
        return;

    static auto cl_sidespeed = csgo->cvar->find_var(str("cl_sidespeed"));
    static auto cl_forwardspeed = csgo->cvar->find_var(str("cl_forwardspeed"));
    static auto cl_backspeed = csgo->cvar->find_var(str("cl_backspeed"));
    if (keymanager->check(config->i.aa_slowmo, config->i.aa_slowmo_style)) {
        cl_sidespeed->set(135.f * (config->i.aa_slowmo_speed * 0.01f));
        cl_forwardspeed->set(135.f * (config->i.aa_slowmo_speed * 0.01f));
        cl_backspeed->set(135.f * (config->i.aa_slowmo_speed * 0.01f));

        if (config->i.aa_slowmo_type == 1) {
            auto maxchoke = floorf(10 * (config->i.aa_slowmo_speed * 0.01f));
            if (csgo->choke >= maxchoke) {
                if (fabsf(csgo->velocity.length()) > 10.1f) {
                    auto direction = csgo->velocity.angle();
                    direction.y = c->viewangles.y - direction.y;

                    auto sine_p = sin(utils->rad(direction.p)), cosine_p = cos(utils->rad(direction.p)),
                            sine_y = sin(utils->rad(direction.y)), cosine_y = cos(utils->rad(direction.y));

                    auto negated_direction =
                            c_vector(cosine_p * cosine_y, cosine_p * sine_y, -sine_p) * -csgo->velocity.length();

                    forwardmove = negated_direction.x;
                    sidemove = negated_direction.y;
                } else {
                    forwardmove = 0;
                    sidemove = 0;
                }
            }
        }
    } else {
        cl_sidespeed->set(450.f);
        cl_forwardspeed->set(450.f);
        cl_backspeed->set(450.f);
    }

    auto wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());

    c_grenade* nade = nullptr;
    if (wep) {
        if (wep->get_type() == WEAPONTYPE_GRENADE) {
            nade = (c_grenade*)wep;
            if (nade->throwtime() != 0.f) {
                if (csgo->globals->servertime >= nade->throwtime())
                    skip = 1;
            }
        }
    }

    if (skip) { features->fakelag->max_choke = 1; skip--; return; }

    if ((c->buttons & IN_USE || c->buttons & IN_ATTACK || (features->ragebot->having_revolver && c->buttons & IN_ATTACK)) && !nade) return;
    if (csgo->local_player->movetype() == MOVETYPE_NOCLIP || csgo->local_player->movetype() == MOVETYPE_LADDER) return;

    if (!(csgo->local_player->flags() & FL_ONGROUND))
        set(c, 2);
    else {
        if (csgo->local_player->velocity().length() < 1.02f)
            set(c, 0);
        else
            set(c, 1);
    }
}

void c_antiaims::set(c_usercmd* c, int s) {
    csgo->aa_worked = true;

    c_angle va;
    csgo->engine->get_view_angles(&va);

    static bool jitter = false;
    if (csgo->want_sp) jitter = !jitter;

    if (config->i.aa_pitch[s]) {
        if (config->i.aa_pitch[s] == 1) c->viewangles.p = 89.f;
        if (config->i.aa_pitch[s] == 2) c->viewangles.p = -89.f;
        if (config->i.aa_pitch[s] == 3) c->viewangles.p = jitter ? 89.f : -89.f;
    }

    if (config->i.aa_yaw[s]) {
        if (config->i.aa_yaw[s] == 1) c->viewangles.y = config->i.aa_yaw_offset[s];
        if (config->i.aa_yaw[s] == 2) c->viewangles.y = va.y + 180.f + config->i.aa_yaw_offset[s]; //
        if (config->i.aa_yaw[s] == 3) c->viewangles.y = va.y + 180.f + config->i.aa_yaw_offset[s] * (jitter ? 1.f : -1.f); //+ 180.f
    }

    if (config->b.aa_manual) {
        static auto nh = false;
        if (keymanager->is_key_down(config->i.aa_manual_positive)) {
            if (!nh) {
                nh = true;
                override = override == 1 ? 0 : 1;
            }
        } else nh = false;

        static auto ph = false;
        if (keymanager->is_key_down(config->i.aa_manual_negative)) {
            if (!ph) {
                ph = true;
                override = override == 2 ? 0 : 2;
            }
        } else ph = false;

        static auto oh = false;
        if (keymanager->is_key_down(config->i.aa_manual_opposite)) {
            if (!oh) {
                oh = true;
                if (override == 0) override = 3;
                else override = 0;
            }
        } else oh = false;

        if (override == 1)
            c->viewangles.y += 90;
        if (override == 2)
            c->viewangles.y -= 90;
        if (override == 3)
            c->viewangles.y += 180;
    } else override = 0;

    csgo->real = c->viewangles;

    if (config->b.aa_desync[s]) {
        csgo->aa_desync = true;
        if (!csgo->want_sp)
            c->viewangles.y += keymanager->check(config->i.aa_invert, config->i.aa_invert_style) ? -120.f : 120.f;
    } else
        csgo->aa_desync = false;

    last_ang = c->viewangles;
}
void c_antiaims::player_animation(){
    if (!csgo->local_player)
        return;

    for (int idx = 1; idx < csgo->globals->maxclients; idx++){
        auto player = (c_player*)csgo->entities->get_entity(idx);

        auto& data = m_player_states.at(idx);

        if (!player || player->is_dormant() || !player->health() > 0)
        {
            data.m_init = false;
            continue;
        }

        if (player->index() == csgo->engine->get_local_player())
            continue;

        if (data.m_spawntime != player->spawntime() || m_should_update_entity_animstate){
            data.m_init = false;
            data.m_spawntime = player->spawntime();
        }

        if (!data.m_state)
            data.m_init = false;

        if (!data.m_init)
        {
            data.m_state = (c_csgo_animstate*)csgo->memory->alloc(sizeof(c_csgo_animstate));

            if (data.m_state)
                player->create_anim_state(data.m_state);

            data.m_init = true;
        }

        auto anim_state = reinterpret_cast<c_csgo_animstate*> (data.m_state);
        anim_state->last_anim_frame -= 3;
        anim_state->last_anim_update -= 3;

        if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
            * reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

        if (data.m_last_simtime == player->simtime())
        {
            /*if (config->b.rage_resolver)
            {
                const auto add = g_features.resolver.rdata.at(player->ent_index()).desync_add;


                player->update_animation_state(data.m_state, QAngle(player->eye_angles().pitch, player->eye_angles().yaw + add, 0));
            }*/

            if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
                * reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

        }
        else
        {
            player->update_anim_state(data.m_state, player->eyeangle());
             if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
                * reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

            // set var to setup bones and set them up
            player->invalidate_bone_cache();

            data.m_should_recalculate_matrix = true;
//

            data.m_got_matrix = player->setup_bones(data.m_matrix, 128, 0x0007FF00, 0.f);

            data.m_should_recalculate_matrix = false;

        }
        data.m_last_simtime = player->simtime();
    }

    m_should_update_entity_animstate = false;
}
void c_antiaims::update_body() {

    if (!csgo->engine->is_connected() || !csgo->engine->is_in_game() || !csgo->local_player) return;

    auto alloc = !animstate;
    auto change = !alloc && selfhandle != &csgo->local_player->get_ref_ehandle();
    auto reset = !alloc && !change && csgo->local_player->spawntime() != spawntime;

    if (change)
        VirtualFree(animstate, sizeof(c_csgo_animstate), MEM_RELEASE);

    if (reset) {
        csgo->local_player->reset_anim_state(animstate);
        spawntime = csgo->local_player->spawntime();
    }

    if (alloc || change) {
        animstate = (c_csgo_animstate*)csgo->memory->alloc(sizeof(c_csgo_animstate));
        if (animstate)
            csgo->local_player->create_anim_state(animstate);

        selfhandle = (c_ehandle*)&csgo->local_player->get_ref_ehandle();
        spawntime = csgo->local_player->spawntime();
    }

    if (!alloc && !change && !reset && csgo->local_player->simtime() != simtime) {
        simtime = csgo->local_player->simtime();

        csgo->allow_anims = false;
        auto as = csgo->local_player->get_anim_state();

        auto pp = new float[24] {};
        auto layers = new c_animlayer[13] {};

        memcpy(pp, csgo->local_player->poseparams(), sizeof(float) * 24);
        memcpy(layers, csgo->local_player->animlayers(), sizeof(c_animlayer) * 13);

        auto frametime = csgo->globals->frametime;
        auto servertime = csgo->globals->servertime;

        csgo->globals->frametime = csgo->globals->tickinterval;
        csgo->globals->servertime = csgo->local_player->simtime();

        auto vel = csgo->local_player->velocity();

        csgo->local_player->velocity() = c_vector(0, 0, 0);
        csgo->local_player->update_anim_state(animstate, csgo->sent);

        //animstate->duck_amount = as->duck_amount;
        csgo->local_player->animlayers()[12].weight = FLT_EPSILON;


        csgo->gfy_delta = csgo->proper_abs;
        csgo->gfy_delta -= animstate->abs_yaw;

        csgo->local_player->invalidate_bone_cache();
        csgo->local_player->setup_bones(csgo->body_bones, 128, 0x7FF00, csgo->local_player->tickbase() * csgo->globals->tickinterval);

        csgo->globals->frametime = frametime;
        csgo->globals->servertime = servertime;

        memcpy(csgo->local_player->poseparams(), pp, sizeof(float) * 24);
        memcpy(csgo->local_player->animlayers(), layers, sizeof(c_animlayer) * 13);

        csgo->local_player->velocity() = vel;

        csgo->sentpos = csgo->local_player->get_abs_origin();
        csgo->allow_anims = true;
    }
}

void c_antiaims::update_lby() {
}

void c_antiaims::sidemove() {
    if (csgo->velocity.length() > 1.02f)
        return;

    if (!csgo->aa_worked || !csgo->aa_desync)
        return;

    if (csgo->next_sidemove > csgo->cmd->tickcount || csgo->next_sidemove < 0)
        return;

    if (
            csgo->cmd->buttons & IN_FORWARD ||
            csgo->cmd->buttons & IN_BACK ||
            csgo->cmd->buttons & IN_LEFT ||
            csgo->cmd->buttons & IN_RIGHT ||
            csgo->cmd->buttons & IN_MOVELEFT ||
            csgo->cmd->buttons & IN_MOVERIGHT ||
            csgo->cmd->buttons & IN_JUMP
    )
        return;

    auto amt = csgo->cmd->buttons & IN_DUCK ? 3.f : 1.01f;
    csgo->cmd->sidemove = csgo->switch_sidemove ? -amt : amt;

    if (features->fakelag->max_choke == 1)
        csgo->next_sidemove = csgo->cmd->tickcount + 2;
    else {
        csgo->next_sidemove = csgo->cmd->tickcount + 4;
        if (csgo->choke < features->fakelag->max_choke - 1)
            csgo->next_sidemove--;
    }
    csgo->switch_sidemove = !csgo->switch_sidemove;

    if (!csgo->want_sp && csgo->choke < features->fakelag->max_choke - 1)
        csgo->cmd->viewangles.y += 240.f * (keymanager->check(config->i.aa_invert, config->i.aa_invert_style) ? 1.f : -1.f);
}

void c_movfix::rotate() {
    auto f1 = (ang.y < 0.f) ? (360.f + ang.y) : ang.y;
    auto f2 = (csgo->cmd->viewangles.y < 0.f) ? (360.f + csgo->cmd->viewangles.y) : csgo->cmd->viewangles.y;

    auto yaw_delta = (f2 < f1) ? fabsf(f2 - f1) : (360.0f - fabsf(f1 - f2));
    yaw_delta = 360.0f - yaw_delta;

    csgo->cmd->forwardmove = cosf(utils->rad(yaw_delta)) * forwardmove + cosf(utils->rad(yaw_delta + 90.f)) * sidemove;
    csgo->cmd->sidemove = sinf(utils->rad(yaw_delta)) * forwardmove + sinf(utils->rad(yaw_delta + 90.f)) * sidemove;
}
