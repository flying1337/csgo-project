//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __fastcall hkPostDataUpdate(c_player* p, void* edx, int updatetype) {
    if (csgo->local_player && p && p->index() == csgo->local_player->index()) {
        auto wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());
        if (wep) {
            wep->paintkit() = config->i.skins_weapon_paintkit[wep->econ_index()];
        }
    }

    return oPostDataUpdate(p, updatetype);
}