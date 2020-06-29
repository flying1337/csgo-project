//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __stdcall hkLockCursor() {
    if (menu->is_initialized && menu->wnd->get_active()) {
        csgo->surface->unlock_cursor();
        return;
    }

    oLockCursor(csgo->surface);
}