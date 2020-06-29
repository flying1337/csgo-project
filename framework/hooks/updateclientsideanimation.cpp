//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __fastcall hkUpdateClientsideAnimation(c_player* e, void*) {
    if (!e || !csgo->local_player || e->health() <= 0)
        return oUpdateClientsideAnimation(e);

    if (e->index() == csgo->local_player->index()) {
        if (csgo->allow_anims)
            oUpdateClientsideAnimation(e);
        else
            return;
    } else
        oUpdateClientsideAnimation(e);
}