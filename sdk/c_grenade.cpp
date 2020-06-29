//
// Created by ruppet on 2/25/2020.
//

#include "../cheat.h"
#include "c_grenade.h"

float &c_grenade::throwtime() {
    static auto n = csgo->netvars[str("DT_BaseCSGrenade")][str("m_fThrowTime")];
    return g_offset(float, this, n.offset);
}

bool &c_grenade::pinpulled() {
    static auto n = csgo->netvars[str("DT_BaseCSGrenade")][str("m_bPinPulled")];
    return g_offset(bool, this, n.offset);
}
