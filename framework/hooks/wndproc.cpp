//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

long __stdcall hkWndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	if (!core->is_dx_loaded || !core->is_fully_loaded || !menu->is_initialized) return CallWindowProcA(oWndProc, hwnd, msg, w, l);

	if (msg == WM_KEYDOWN) {
		if (w == config->i.menukey)
			menu->wnd->set_active(!menu->wnd->get_active());
	}

	if (menu->wnd->get_active()) {
		auto ret = ImGui_ImplWin32_WndProcHandler(hwnd, msg, w, l);

		if (w != 'W' && w != 'A' && w != 'S' && w != 'D' && w != VK_SHIFT && w != VK_CONTROL && w != VK_TAB && w != VK_SPACE || ImGui::GetIO().WantTextInput) {
            if (!ret)
                return CallWindowProcA(oWndProc, hwnd, msg, w, l);

            return true;
        }
	} else {
        auto io = &ImGui::GetIO();
        memset(io->MouseDown, 0, 5);
        memset(io->MouseClicked, 0, 5);
        memset(io->MouseUpDuration, 0, sizeof(float) * 5);
        memset(io->MouseDownDuration, 0, sizeof(float) * 5);
        memset(io->KeysDown, 0, 512);
        memset(io->KeysDownDuration, 0, sizeof(float) * 512);
	}

	return CallWindowProcA(oWndProc, hwnd, msg, w, l);
}