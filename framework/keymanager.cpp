#include "../cheat.h"

bool c_keymanager::is_key_down(int key) {
    if (GetForegroundWindow() != csgo->window) return prev[key];
    if (utils->is_chat_opened()) return prev[key];

    prev[key] = HIWORD(GetKeyState(key));
	return prev[key];
}

bool c_keymanager::is_key_up(int key) {
    if (GetForegroundWindow() != csgo->window) return prev[key];
    if (utils->is_chat_opened()) return prev[key];

    prev[key] = !HIWORD(GetKeyState(key));
	return prev[key];
}

bool c_keymanager::is_key_toggled(int key) {
    if (GetForegroundWindow() != csgo->window) return prev[key];
    if (utils->is_chat_opened()) return prev[key];

    prev[key] = LOWORD(GetKeyState(key));
	return prev[key];
}

bool c_keymanager::check(int key, int style) {
    if (GetForegroundWindow() != csgo->window) return prev[key];
    if (utils->is_chat_opened()) return prev[key];

    prev[key] = true;
	if (style == 0)
		return prev[key];
	if (style == 1)
		return is_key_down(key);
	if (style == 2)
		return is_key_toggled(key);
	if (style == 3)
		return is_key_up(key);
	return false;
}