#include "../cheat.h"
#include "entity.h"



c_vector& c_entity::origin() {
	static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_vecOrigin")];
	return g_offset(c_vector, this, n.offset);
}

bool c_entity::is_projectile() {
	if (!this || IsBadReadPtr(this, sizeof(this))) return false;

	auto c = get_client_class();
	if (!c) return false;

	return 
		c->class_id == DT_BaseCSGrenadeProjectile ||
		c->class_id == DT_DecoyProjectile ||
		c->class_id == DT_MolotovProjectile ||
		c->class_id == DT_SensorGrenadeProjectile ||
		c->class_id == DT_SmokeGrenadeProjectile ||
		c->class_id == DT_TEClientProjectile ||
		c->class_id == DT_SnowballProjectile;
}


bool c_entity::is_hostage() {
    auto c = get_client_class();
    if (!c) return false;

    return c->class_id == DT_CHostage;
}

c_vector &c_entity::abs_velocity() {
    static auto o = utils->datamap_find(get_datamap(), str("m_vecAbsVelocity"));
    return g_offset(c_vector, this, o);
}