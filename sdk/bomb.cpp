#include "../cheat.h"

float& c_bomb::timer() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_flTimerLength")];
	return g_offset(float, this, n.offset);
}

bool& c_bomb::is_ticking() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_bBombTicking")];
	return g_offset(bool, this, n.offset);
}

int& c_bomb::site() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_nBombSite")];
	return g_offset(int, this, n.offset);
}

float& c_bomb::defusetime() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_flDefuseCountDown")];
	return g_offset(float, this, n.offset);
}

float& c_bomb::blowtime() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_flC4Blow")];
	return g_offset(float, this, n.offset);
}

c_ehandle& c_bomb::defuser() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_hBombDefuser")];
	return g_offset(c_ehandle, this, n.offset);
}

float& c_bomb::defuse() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_flDefuseLength")];
	return g_offset(float, this, n.offset);
}

bool& c_bomb::is_defused() {
	static auto n = csgo->netvars[str("DT_PlantedC4")][str("m_bBombDefused")];
	return g_offset(bool, this, n.offset);
}

int c_bomb::get_damage(c_player* p) {
	if (!p)
		return 0;

	float a1 = 450.7f;
	float a2 = 75.68f;
	float a3 = 789.2f;
	float a4 = (p->origin().dist(origin()) - a2) / a3;
	float damage = a1 * expf(-a4 * a4);

	static auto get_armor_health = [](float flDamage, int ArmorValue)
	{
		float flCurDamage = flDamage;

		if (flCurDamage == 0.0f || ArmorValue == 0)
			return flCurDamage;

		float flArmorRatio = 0.5f;
		float flArmorBonus = 0.5f;
		float flNew = flCurDamage * flArmorRatio;
		float flArmor = (flCurDamage - flNew) * flArmorBonus;

		if (flArmor > ArmorValue)
		{
			flArmor = ArmorValue * (1.0f / flArmorBonus);
			flNew = flCurDamage - flArmor;
		}

		return flNew;
	};

	float fdamage = max((int)ceilf(get_armor_health(damage, p->armor())), 0);
	return std::clamp(floorf(fdamage) - 1.f, 0.f, 999.f);
}