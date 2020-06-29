//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __fastcall hkPacketStart(void* o, void*, int inc, int out) {
    if (!csgo->local_player) return oPacketStart(o, inc, out);

    for (auto it = csgo->packets.begin(); it != csgo->packets.end(); it++) {
        if (*it == out) {
            csgo->packets.erase(it);
            return oPacketStart(o, inc, out);
        }
    }
}