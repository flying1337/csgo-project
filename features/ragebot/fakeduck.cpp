//
// Created by ruppet on 30.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"

void c_fakeduck::work(c_usercmd* c) {
    if (keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style)) {
        features->fakelag->force_choke(14);

        if (csgo->local_player->view_offset().z < 60.f)
            c->buttons &= ~IN_ATTACK;



        if (csgo->clientstate->choked_commands > 6)
            c->buttons |= IN_DUCK;
        else
            c->buttons &= ~IN_DUCK;

    }
}