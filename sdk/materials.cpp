#include "../cheat.h"

c_material* c_materialsystem::find_material(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	using fn = c_material*(__thiscall*)(PVOID, char const*, const char*, bool, const char*);
	return g_vfunc(fn, this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}

unsigned short c_materialsystem::first_material()
{
	using fn = unsigned short(__thiscall*)(void*);
	return g_vfunc(fn, this, 86)(this);
}

unsigned short c_materialsystem::next_material(unsigned short h)
{
	using fn = unsigned short(__thiscall*)(void*, unsigned short);
	return g_vfunc(fn, this, 87)(this, h);
}

unsigned short c_materialsystem::invalid_material()
{
	using fn = unsigned short(__thiscall*)(void*);
	return g_vfunc(fn, this, 88)(this);
}

c_material* c_materialsystem::get_material(unsigned short h)
{
	using fn = c_material*(__thiscall*)(void*, unsigned short);
	return g_vfunc(fn, this, 89)(this, h);
}