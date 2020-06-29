//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include <intrin.h>

c_angle* __fastcall hkEyeAngles(c_player* e, void*) {
    if (!e) return oEyeAngles(e);
    if (csgo->local_player && e->index() == csgo->local_player->index()) {
        static auto retn_pitch = utils->scan(str("client_panorama.dll"), str("8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55 ?"));
        static auto retn_yaw = utils->scan(str("client_panorama.dll"), str("F3 0F 10 55 ? 51 8B 8E ? ? ? ?"));
        static auto retn_grenade_preview = utils->scan(str("client_panorama.dll"), str("8B 0D ? ? ? ? F3 0F 7E 00 8B 40 08 89 44 24 24"));


        auto ra = (unsigned long long)_ReturnAddress();

        if (config->i.aa_pitch && ra == retn_grenade_preview) {
            c_angle va;
            csgo->engine->get_view_angles(&va);
            return &va;
        }
        if (!retn_pitch || !retn_yaw) return oEyeAngles(e);
        if (!csgo->is_in_tp) return oEyeAngles(e);



        if (ra == retn_pitch || ra == retn_yaw)
            return &csgo->sent;
    }

    return oEyeAngles(e);
}