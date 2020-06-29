//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

bool __stdcall hkDoPostScreenSpaceEffects(void* a) {
    if (!core->is_fully_loaded || !csgo->local_player || core->hide_from_obs) return oDoPostScreenSpaceEffects(csgo->clientmode, a);

    features->glow->work();
    return oDoPostScreenSpaceEffects(csgo->clientmode, a);
}