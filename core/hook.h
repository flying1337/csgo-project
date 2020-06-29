#pragma once

class c_hookable {
public:
	c_hookable(void* b);

	void hook(int idx, void* hk);
	void* orig(int idx);
	void unhook(int idx);

	int count(void* base);

	void* base = 0;

	int methodCount = 0;
	int* original = 0;
	int* current = 0;
};