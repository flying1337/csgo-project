//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* a) {
    if (!core->is_dx_loaded || !menu->is_initialized) return oReset(device, a);

    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto result = oReset(device, a);
    ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}