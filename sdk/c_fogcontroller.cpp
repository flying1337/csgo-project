//
// Created by ruppet on 2/12/2020.
//

#include "c_fogcontroller.h"
#include "../cheat.h"

int &c_fogcontroller::color_secondary() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.colorSecondary")];
    return g_offset(int, this, n.offset);
}

int &c_fogcontroller::color_primary() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.colorPrimary")];
    return g_offset(int, this, n.offset);
}

bool &c_fogcontroller::enable() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.enable")];
    return g_offset(bool, this, n.offset);
}

int &c_fogcontroller::blend() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.blend")];
    return g_offset(int, this, n.offset);
}

float &c_fogcontroller::start() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.start")];
    return g_offset(float, this, n.offset);
}

float &c_fogcontroller::end() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.end")];
    return g_offset(float, this, n.offset);
}

float &c_fogcontroller::maxdensity() {
    static auto n = csgo->netvars[str("DT_FogController")][str("m_fog.maxdensity")];
    return g_offset(float, this, n.offset);
}
