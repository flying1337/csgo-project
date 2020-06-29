//
// Created by megapc on 26.04.2020.
//

#include "../../cheat.h"
int __stdcall hkCamera(c_cvar* var, void* return_address, void* eax)
{
    static auto return_to_modify_eye_pos = utils->scan(str("client_panorama.dll"), str("85 C0 0F 84 ? ? ? ? 8B 45 08 8B 0D"));

    const auto local = csgo->local_player;

    if (return_address == (void*)return_to_modify_eye_pos)
        return !csgo->in_camera || (local && !local->get_anim_state()->is_in_hit_ground_anim);

    return 0;
}