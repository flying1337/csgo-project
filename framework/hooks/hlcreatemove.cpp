//
// Created by ruppet on 3/21/2020.
//

#include "../../cheat.h"

void __fastcall hkHlCreateMove(void* o, void*, int seq, float inp, bool active) {
    oHlCreateMove(o, seq, inp, csgo->recharge > 0 ? false : active);
}