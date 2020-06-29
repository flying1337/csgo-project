#include "../cheat.h"

#define it(i, t) strcpy(txt, b ? str(i) : str(t))

const char* c_projectile::get_icon(bool b) {
	auto txt = (char*)malloc(33);

	switch (get_client_class()->class_id) {
	case DT_BaseCSGrenadeProjectile: it("j", "High Explosive Grenade"); break;
	case DT_DecoyProjectile: it("m", "Decoy Grenade"); break;
	case DT_MolotovProjectile: it("l", "Molotov Cocktail"); break;
	case DT_SensorGrenadeProjectile: it("i", "Flashbang"); break;
	case DT_SmokeGrenadeProjectile: it("k", "Smoke Grenade"); break;
	case DT_TEClientProjectile: it("i", "Tactical Grenade"); break;
	case DT_SnowballProjectile: it("i", "Snowball"); break;
	}

	return txt;
}

#undef it