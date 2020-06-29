#include "../cheat.h"

c_hookable::c_hookable(void* b) {
	original = *(int**)b;
	methodCount = count(b);
	if (methodCount <= 0)
		return;
	current = new int[methodCount + 1]();
	current[0] = original[-1];
	memcpy(&current[1], original, methodCount * sizeof(int));
	base = b;
	DWORD oldprotect = 0, newprotect = 0;
	VirtualProtect(base, methodCount * sizeof(int), PAGE_READWRITE, &oldprotect);
	*(int**)base = &current[1];
	VirtualProtect(base, methodCount * sizeof(int), oldprotect, &newprotect);
}

void c_hookable::hook(int idx, void* hk) {
	DWORD oldprotect = 0, newprotect = 0;
	VirtualProtect(current, methodCount * sizeof(int), PAGE_READWRITE, &oldprotect);
	current[idx + 1] = (int)hk;
	VirtualProtect(current, methodCount * sizeof(int), oldprotect, &newprotect);
}

void* c_hookable::orig(int idx) {
	return (void*)original[idx];
}

void c_hookable::unhook(int idx) {
	DWORD oldprotect = 0, newprotect = 0;
	VirtualProtect(current, methodCount * sizeof(int), PAGE_READWRITE, &oldprotect);
	current[idx + 1] = original[idx];
	VirtualProtect(current, methodCount * sizeof(int), oldprotect, &newprotect);
}

int c_hookable::count(void* base) {
	auto length = 0;
	while ((*(int**)base)[length])
		length++;

	return length;
}