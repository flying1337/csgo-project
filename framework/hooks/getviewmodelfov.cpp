//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

float __fastcall hkGetViewModelFOV(void* o, void*) {
    return oGetViewModelFOV(o) + config->i.misc_viewmodelfov;
}