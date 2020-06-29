//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include <intrin.h>
int __fastcall hkListLeavesInBox(void* bsp, void*, c_vector& mins, c_vector& maxs, unsigned short* list, int size) {
    if (!config->b.vis_noocclusion || *(unsigned int*)_ReturnAddress() != 0x14244489) return oListLeavesInBox(bsp, mins, maxs, list, size);

    auto info = *(c_renderableinfo**)((uintptr_t)_AddressOfReturnAddress() + 0x14);
    if (!info || !info->renderable) return oListLeavesInBox(bsp, mins, maxs, list, size);

    auto ent = info->renderable->get_unknown()->get_entity();
    if (!ent || !ent->is_player()) return oListLeavesInBox(bsp, mins, maxs, list, size);

    info->_0 &= ~0x100;
    info->_1 |= 0xC0;

    static auto mi = c_vector(-16384.0f, -16384.0f, -16384.0f);
    static auto ma = c_vector(16384.0f, 16384.0f, 16384.0f);

    return oListLeavesInBox(bsp, mi, ma, list, size);
}