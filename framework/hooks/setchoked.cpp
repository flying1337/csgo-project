//
// Created by ruppet on 2/3/2020.
//

#include "../../cheat.h"
#include <intrin.h>

void __fastcall hkSetChoked(c_netchannel* o, void*) {
    static auto retn = utils->scan(str("engine.dll"), str("FF 90 ? ? ? ? FF 87 ? ? ? ?")) + 6;
    if ((ptr)_ReturnAddress() != retn)
        return oSetChoked(o);

    oSetChoked(o);
}