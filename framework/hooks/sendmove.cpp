//
// Created by ruppet on 2/3/2020.
//

#include "../../cheat.h"

void __stdcall hkSendMove() {
//    char data[4000] = {0};
//
//    auto nextcommandnr = g_offset(int, csgo->clientstate, 0x4D24) + g_offset(int, csgo->clientstate, 0x4D28) + 1;

    oCL_SendMove();
}