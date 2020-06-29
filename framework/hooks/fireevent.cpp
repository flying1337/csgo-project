//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __fastcall hkFireEvent(void* o, void*) {
    if (!csgo->local_player) return oFireEvent(o);

    for (auto e = csgo->clientstate->events; e; e = *(c_eventinfo**)((unsigned long long)e + 0x38))
        e->fire_delay = 0.f;

    oFireEvent(o);
}