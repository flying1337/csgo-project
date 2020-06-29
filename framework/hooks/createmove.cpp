//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include "../../features/visuals/debuginfo.h"
#include "../config.h"

bool __stdcall hkCreateMove(float a, c_usercmd* cmd) {
    csgo->cache_entities();

    if (!core->is_fully_loaded || !csgo->local_player || !cmd || !cmd->commandnumber)
        return oCreateMove(csgo->clientmode, a, cmd);

    csgo->aa_worked = false;
    csgo->cmd = cmd;

    features->antiaims->player_animation();
    features->speclist->work();
    features->visuals->update_positions();
    features->visuals->work();


    if (csgo->local_player->health() > 0) {
        if (menu->wnd->get_active()) {
            cmd->buttons &= ~IN_ATTACK;
            cmd->buttons &= ~IN_ATTACK2;
        }
        csgo->in_camera = true;
        csgo->velocity = csgo->local_player->velocity();

        auto wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());
        if (wep) {
            config->i.skins_weapon = wep->econ_index();

            if ((cmd->buttons & IN_ATTACK) && !features->ragebot->is_able_to_shoot() && wep->get_type() != WEAPONTYPE_GRENADE)
                cmd->buttons &= ~IN_ATTACK;
        }

        features->ragebot->wep = wep;

        features->fakelag->estimate_max_choke(cmd);
        features->autojump->work(cmd);
        features->prediction->save(cmd);
        {

            features->ragebot->work(cmd);
            features->future->work();
            features->fakeduck->work(cmd);

            c_movfix mf;
            mf.forwardmove = cmd->forwardmove;
            mf.sidemove = cmd->sidemove;
            mf.ang = cmd->viewangles;

            features->antiaims->work(cmd, mf.sidemove, mf.forwardmove);
            features->ragebot->cock();

            mf.rotate();
        }
        features->prediction->restore();
        features->antiaims->sidemove();
        features->fakelag->choke(cmd);

        if (config->b.aa_fix_legs && csgo->local_player->movetype() != MOVETYPE_LADDER) {
            cmd->buttons &= ~(IN_FORWARD);
            cmd->buttons &= ~(IN_BACK);
            cmd->buttons &= ~(IN_LEFT);
            cmd->buttons &= ~(IN_RIGHT);
            cmd->buttons &= ~(IN_MOVELEFT);
            cmd->buttons &= ~(IN_MOVERIGHT);
        }

        if (config->b.misc_infduck)
            cmd->buttons |= IN_BULLRUSH;

        if (config->b.vis_penecross) {
            c_angle va;
            csgo->engine->get_view_angles(&va);

            csgo->aw_damage = features->autowall->get_approx_point_damage(
                    csgo->local_player->origin() +
                    csgo->local_player->view_offset() + va.vector() * 8912.f);

        }
    } else {
        csgo->allow_sp = true;
        csgo->in_camera = false;
    }
    if (config->b.anti_untrusted)
        cmd->viewangles.normalize();

    if (csgo->allow_sp) {
        csgo->sent = cmd->viewangles;
        csgo->choke = 0;
        csgo->packets.emplace_back(cmd->commandnumber);
    } else {
        csgo->unsent = cmd->viewangles;
        csgo->choke++;
    }
    scripting->run();

    csgo->last_allow_sp = csgo->allow_sp;

    uintptr_t *frame_ptr;
    __asm mov frame_ptr, ebp;
    *(bool *) (*frame_ptr - 0x1c) = csgo->allow_sp;

    return false;
}