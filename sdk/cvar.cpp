#include "../cheat.h"

// 55 8B EC 81 EC ? ? ? ? 80 3D ? ? ? ? ? 56 8B 35

void c_cvar::devconsole_color_printf(const c_color& clr, const char* fmt, ...) {
	char buffer[2048] = { 0 };

	va_list va;
	va_start(va, fmt);
	vsprintf(buffer, fmt, va);
	va_end(va);

	using Fn = void(__stdcall*)(const c_color&, char const*);
	static auto fn = (Fn)utils->scan(str("engine.dll"), str("55 8B EC 81 EC ? ? ? ? 80 3D ? ? ? ? ? 56 8B 35"));

	fn(clr, buffer);
}

void c_cvar::devconsole_printf(const char* fmt, ...) {
	char buffer[2048] = { 0 };

	va_list va;
	va_start(va, fmt);
	vsprintf(buffer, fmt, va);
	va_end(va);

	devconsole_color_printf(c_color(255, 255, 255), buffer);
}