//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

float __fastcall hkGetScreenAspectRatio(void* e, void*, int w, int h) {
    if (config->i.misc_aspectratiomd > 0) {
        auto v = config->i.misc_aspectratiomd;
        if (v == 1) return 1.f/1.f;
        if (v == 2) return 5.f/4.f;
        if (v == 3) return 4.f/3.f;
        if (v == 4) return 3.f/2.f;
        if (v == 5) return 14.f/9.f;
        if (v == 6) return 16.f/10.f;
        if (v == 7) return 16.f/9.f;
        if (v == 8) return 21.f/9.f;
        if (v == 9) return 11.f/4.f;
        if (v == 10) return 64.f/45.f;
        if (v == 11) return config->i.misc_aspectratio * 0.01f;
    }

    return oGetScreenAspectRatio(e, w, h);
}