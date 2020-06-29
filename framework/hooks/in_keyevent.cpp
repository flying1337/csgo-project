//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

int __fastcall hkIN_KeyEvent(void* o, void*, int code, int key, const char* binding) {
//    if (binding && !strcmp(binding, str("drop")) && csgo->aa_worked) {
//        features->antiaims->skip = 2;
//        features->antiaims->shoulduse = true;
//
//        features->antiaims->code = code;
//        features->antiaims->key = key;
//        features->antiaims->binding = binding;
//
//        return 0;
//    }

    return oIN_KeyEvent(o, code, key, binding);
}