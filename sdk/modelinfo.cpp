#include "../cheat.h"

c_studiohdr* c_modelinfo::get_studio_model(const c_model* model)
{
	return g_vfunc(c_studiohdr * (__thiscall*)(void*, const c_model*), this, 32)(this, model);
}

int c_modelinfo::get_model_index(const char* name) {
	return g_vfunc(int(__thiscall*)(void*, const char*), this, 2)(this, name);
}

char* c_modelinfo::get_model_name(const c_model* model)
{
	return g_vfunc(char* (__thiscall*)(void*, const c_model*), this, 3)(this, model);
}

void c_modelinfo::get_model_materials(const c_model* model, int count, c_material** mats)
{
	g_vfunc(char* (__thiscall*)(void*, const c_model*, int, c_material**), this, 19)(this, model, count, mats);
}