#include "../cheat.h"

int c_enginetrace::get_point_contents(const c_vector& vecAbsPosition, int contentsMask, c_handle** ppEntity)
{
	return g_vfunc(int(__thiscall*)(void*, const c_vector&, int, c_handle**), this, 0)(this, vecAbsPosition, contentsMask, ppEntity);
}

void c_enginetrace::trace_ray(const c_ray& ray, unsigned int fMask, c_tracefilter* filter, c_trace* trace)
{
	g_vfunc(void(__thiscall*)(void*, const c_ray&, unsigned int, c_tracefilter*, c_trace*), this, 5)(this, ray, fMask, filter, trace);
}

void c_enginetrace::clip_ray_to_entity(const c_ray& ray, unsigned int fMask, c_entity* pEnt, c_trace* pTrace)
{
	return g_vfunc(void(__thiscall*)(void*, const c_ray&, unsigned int, c_entity*, c_trace*), this, 3)(this, ray, fMask, pEnt, pTrace);
}

void c_enginetrace::clip_ray_to_collideable(const c_ray& ray, unsigned int fMask, c_collider* pEnt, c_trace* pTrace) {
    return g_vfunc(void(__thiscall*)(void*, const c_ray&, unsigned int, c_collider*, c_trace*), this, 4)(this, ray, fMask, pEnt, pTrace);
}

bool standart_filter_rules( c_entity *pCollide, int fContentsMask )
{
    // Static prop case...
    if ( !pCollide )
        return true;

    if ( !pCollide->is_player() )
    {
        if ( (fContentsMask & CONTENTS_MONSTER) == 0 )
            return false;
    }

    return true;
}