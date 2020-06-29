#include "../cheat.h"

e_econindex& c_weapon::econ_index() {
    static auto n = csgo->netvars[str("DT_BaseAttributableItem")][str("m_AttributeManager")][str("m_Item")][str("m_iItemDefinitionIndex")];
    return g_offset(e_econindex, this, n.offset);
}

int& c_weapon::clip() {
    static auto n = csgo->netvars[str("DT_BaseCombatWeapon")][str("m_iClip1")];
    return g_offset(int, this, n.offset);
}

float& c_weapon::next_attack() {
    static auto n = csgo->netvars[str("DT_BaseCombatWeapon")][str("LocalActiveWeaponData")][str("m_flNextPrimaryAttack")];
    return g_offset(float, this, n.offset);
}

float& c_weapon::next_secondary_attack() {
    static auto n = csgo->netvars[str("DT_BaseCombatWeapon")][str("LocalActiveWeaponData")][str("m_flNextSecondaryAttack")];
    return g_offset(float, this, n.offset);
}

int& c_weapon::paintkit() {
    static auto n = csgo->netvars[str("DT_BaseAttributableItem")][str("m_nFallbackPaintKit")];
    return g_offset(int, this, n.offset);
}

c_ehandle& c_weapon::owner() {
    static auto n = csgo->netvars[str("DT_BaseCombatWeapon")][str("m_hOwner")];
    return g_offset(c_ehandle, this, n.offset);
}

c_weaponinfo* c_weapon::get_info() {
    return g_vfunc(c_weaponinfo*(__thiscall*)(void*), this, 460)(this);
}

float c_weapon::get_inaccuracy() {
    return g_vfunc(float(__thiscall*)(void*), this, 482)(this);
}

float c_weapon::get_spread() {
    return g_vfunc(float(__thiscall*)(void*), this, 452)(this);
}

float c_weapon::update_spread() {
    return g_vfunc(float(__thiscall*)(void*), this, 483)(this);
}

float& c_weapon::fire_time() {
    static auto n = csgo->netvars[str("DT_WeaponBaseItem")][str("baseclass")][str("m_flPostponeFireReadyTime")];
    return g_offset(float, this, n.offset);
}

e_weapontype c_weapon::get_type() {
    auto idx = econ_index();
    switch (idx) {
        case WEAPON_NONE:
            return WEAPONTYPE_UNKNOWN;
        case WEAPON_DEAGLE:
            return WEAPONTYPE_HEAVYPISTOL;
        case WEAPON_ELITE:
            return WEAPONTYPE_PISTOL;
        case WEAPON_FIVESEVEN:
            return WEAPONTYPE_PISTOL;
        case WEAPON_GLOCK:
            return WEAPONTYPE_PISTOL;
        case WEAPON_AK47:
            return WEAPONTYPE_RIFLE;
        case WEAPON_AUG:
            return WEAPONTYPE_RIFLE;
        case WEAPON_AWP:
            return WEAPONTYPE_BOLTACTION;
        case WEAPON_FAMAS:
            return WEAPONTYPE_RIFLE;
        case WEAPON_G3SG1:
            return WEAPONTYPE_SNIPER_RIFLE;
        case WEAPON_GALILAR:
            return WEAPONTYPE_RIFLE;
        case WEAPON_M249:
            return WEAPONTYPE_MACHINEGUN;
        case WEAPON_M4A1:
            return WEAPONTYPE_RIFLE;
        case WEAPON_MAC10:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_P90:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_MP5SD:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_UMP45:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_XM1014:
            return WEAPONTYPE_SHOTGUN;
        case WEAPON_BIZON:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_MAG7:
            return WEAPONTYPE_SHOTGUN;
        case WEAPON_NEGEV:
            return WEAPONTYPE_MACHINEGUN;
        case WEAPON_SAWEDOFF:
            return WEAPONTYPE_SHOTGUN;
        case WEAPON_TEC9:
            return WEAPONTYPE_PISTOL;
        case WEAPON_TASER:
            return WEAPONTYPE_UNKNOWN;
        case WEAPON_HKP2000:
            return WEAPONTYPE_PISTOL;
        case WEAPON_MP7:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_MP9:
            return WEAPONTYPE_SUBMACHINEGUN;
        case WEAPON_NOVA:
            return WEAPONTYPE_SHOTGUN;
        case WEAPON_P250:
            return WEAPONTYPE_PISTOL;
        case WEAPON_SHIELD:
            return WEAPONTYPE_UNKNOWN;
        case WEAPON_SCAR20:
            return WEAPONTYPE_SNIPER_RIFLE;
        case WEAPON_SG556:
            return WEAPONTYPE_RIFLE;
        case WEAPON_SSG08:
            return WEAPONTYPE_BOLTACTION;
        case WEAPON_KNIFEGG:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE:
            return WEAPONTYPE_KNIFE;
        case WEAPON_FLASHBANG:
            return WEAPONTYPE_GRENADE;
        case WEAPON_HEGRENADE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_SMOKEGRENADE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_MOLOTOV:
            return WEAPONTYPE_GRENADE;
        case WEAPON_DECOY:
            return WEAPONTYPE_GRENADE;
        case WEAPON_INCGRENADE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_C4:
            return WEAPONTYPE_C4;
        case WEAPON_HEALTHSHOT:
            return WEAPONTYPE_UNKNOWN;
        case WEAPON_KNIFE_T:
            return WEAPONTYPE_KNIFE;
        case WEAPON_M4A1_SILENCER:
            return WEAPONTYPE_RIFLE;
        case WEAPON_USP_SILENCER:
            return WEAPONTYPE_PISTOL;
        case WEAPON_CZ75A:
            return WEAPONTYPE_PISTOL;
        case WEAPON_REVOLVER:
            return WEAPONTYPE_HEAVYPISTOL;
        case WEAPON_TAGRENADE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_FISTS:
            return WEAPONTYPE_KNIFE;
        case WEAPON_BREACHCHARGE:
            return WEAPONTYPE_C4;
        case WEAPON_TABLET:
            return WEAPONTYPE_UNKNOWN;
        case WEAPON_MELEE:
            return WEAPONTYPE_KNIFE;
        case WEAPON_AXE:
            return WEAPONTYPE_KNIFE;
        case WEAPON_HAMMER:
            return WEAPONTYPE_KNIFE;
        case WEAPON_SPANNER:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_GHOST:
            return WEAPONTYPE_KNIFE;
        case WEAPON_FIREBOMB:
            return WEAPONTYPE_GRENADE;
        case WEAPON_DIVERSION:
            return WEAPONTYPE_GRENADE;
        case WEAPON_FRAG_GRENADE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_SNOWBALL:
            return WEAPONTYPE_GRENADE;
        case WEAPON_BUMPMINE:
            return WEAPONTYPE_GRENADE;
        case WEAPON_BAYONET:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_CSS:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_FLIP:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_GUT:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_KARAMBIT:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_M9_BAYONET:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_TACTICAL:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_FALCHION:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_SURVIVAL_BOWIE:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_BUTTERFLY:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_PUSH:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_CORD:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_CANIS:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_URSUS:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_GYPSY_JACKKNIFE:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_OUTDOOR:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_STILETTO:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_WIDOWMAKER:
            return WEAPONTYPE_KNIFE;
        case WEAPON_KNIFE_SKELETON:
            return WEAPONTYPE_KNIFE;
        default:
            return WEAPONTYPE_UNKNOWN;
    }
}

#define it(i, t) strcpy(txt, b ? str(i) : str(t))

const char* c_weapon::get_icon(bool b) {
	auto txt = (char*)malloc(33);

	switch (econ_index())
	{
    case WEAPON_NONE:
        it("", "");
        break;
    case WEAPON_DEAGLE:
        it("A", "Desert Eagle");
        break;
    case WEAPON_ELITE:
        it("B", "Dual Berettas");
        break;
    case WEAPON_FIVESEVEN:
        it("C", "Five-SeveN");
        break;
    case WEAPON_GLOCK:
        it("D", "Glock 18");
        break;
    case WEAPON_AK47:
        it("W", "AK-47");
        break;
    case WEAPON_AUG:
        it("U", "AUG");
        break;
    case WEAPON_AWP:
        it("Z", "AWP");
        break;
    case WEAPON_FAMAS:
        it("R", "FAMAS");
        break;
    case WEAPON_G3SG1:
        it("X", "G3SG1");
        break;
    case WEAPON_GALILAR:
        it("Q", "Galil AR");
        break;
    case WEAPON_M249:
        it("g", "M249");
        break;
    case WEAPON_M4A1:
        it("S", "M4A4");
        break;
    case WEAPON_MAC10:
        it("K", "MAC-10");
        break;
    case WEAPON_P90:
        it("P", "P90");
        break;
    case WEAPON_MP5SD:
        it("N", "MP5-SD");
        break;
    case WEAPON_UMP45:
        it("L", "UMP-45");
        break;
    case WEAPON_XM1014:
        it("b", "XM1014");
        break;
    case WEAPON_BIZON:
        it("M", "PP-Bizon");
        break;
    case WEAPON_MAG7:
        it("d", "MAG-7");
        break;
    case WEAPON_NEGEV:
        it("f", "Negev");
        break;
    case WEAPON_SAWEDOFF:
        it("c", "Sawed-Off");
        break;
    case WEAPON_TEC9:
        it("H", "Tec-9");
        break;
    case WEAPON_TASER:
        it("h", "Zeus x27");
        break;
    case WEAPON_HKP2000:
        it("E", "P2000");
        break;
    case WEAPON_MP7:
        it("N", "MP7");
        break;
    case WEAPON_MP9:
        it("O", "MP9");
        break;
    case WEAPON_NOVA:
        it("e", "Nova");
        break;
    case WEAPON_P250:
        it("F", "P250");
        break;
    case WEAPON_SHIELD:
        it("p", "Shield");
        break;
    case WEAPON_SCAR20:
        it("Y", "SCAR 20");
        break;
    case WEAPON_SG556:
        it("V", "SG-553");
        break;
    case WEAPON_SSG08:
        it("a", "SSG-08");
        break;
    case WEAPON_KNIFEGG:
        it("[", "Final Knife");
        break;
    case WEAPON_KNIFE:
        it("]", "Knife");
        break;
    case WEAPON_FLASHBANG:
        it("i", "Flashbang");
        break;
    case WEAPON_HEGRENADE:
        it("j", "HE Grenade");
        break;
    case WEAPON_SMOKEGRENADE:
        it("k", "Smoke");
        break;
    case WEAPON_MOLOTOV:
        it("l", "Molotov");
        break;
    case WEAPON_DECOY:
        it("m", "Decoy");
        break;
    case WEAPON_INCGRENADE:
        it("n", "Incediary");
        break;
    case WEAPON_C4:
        it("o", "C4");
        break;
    case WEAPON_HEALTHSHOT:
        it("u", "Healthshot");
        break;
    case WEAPON_KNIFE_T:
        it("[", "Knife");
        break;
    case WEAPON_M4A1_SILENCER:
        it("T", "M4A1-S");
        break;
    case WEAPON_USP_SILENCER:
        it("G", "USP-S");
        break;
    case WEAPON_CZ75A:
        it("I", "CZ75-Auto");
        break;
    case WEAPON_REVOLVER:
        it("J", "R8 Revolver");
        break;
    case WEAPON_TAGRENADE:
        it("i", "Tactical Grenade");
        break;
    case WEAPON_FISTS:
        it("[", "Fists");
        break;
    case WEAPON_BREACHCHARGE:
        it("o", "Breach Charge");
        break;
    case WEAPON_TABLET:
        it("o", "Tablet");
        break;
    case WEAPON_MELEE:
        it("[", "Melee");
        break;
    case WEAPON_AXE:
        it("[", "Axe");
        break;
    case WEAPON_HAMMER:
        it("[", "Hammer");
        break;
    case WEAPON_SPANNER:
        it("[", "Spanner");
        break;
    case WEAPON_KNIFE_GHOST:
        it("[", "Ghost Knife");
        break;
    case WEAPON_FIREBOMB:
        it("m", "Firebomb");
        break;
    case WEAPON_DIVERSION:
        it("m", "Diversion Grenade");
        break;
    case WEAPON_FRAG_GRENADE:
        it("j", "Frag Grenade");
        break;
    case WEAPON_SNOWBALL:
        it("i", "Snowball");
        break;
    case WEAPON_BUMPMINE:
        it("u", "Bumpmine");
        break;
    case WEAPON_BAYONET:
        it("1", "Knife");
        break;
    case WEAPON_KNIFE_CSS:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_FLIP:
        it("2", "Knife");
        break;
    case WEAPON_KNIFE_GUT:
        it("3", "Knife");
        break;
    case WEAPON_KNIFE_KARAMBIT:
        it("4", "Knife");
        break;
    case WEAPON_KNIFE_M9_BAYONET:
        it("5", "Knife");
        break;
    case WEAPON_KNIFE_TACTICAL:
        it("6", "Knife");
        break;
    case WEAPON_KNIFE_FALCHION:
        it("0", "Knife");
        break;
    case WEAPON_KNIFE_SURVIVAL_BOWIE:
        it("7", "Knife");
        break;
    case WEAPON_KNIFE_BUTTERFLY:
        it("8", "Knife");
        break;
    case WEAPON_KNIFE_PUSH:
        it("9", "Knife");
        break;
    case WEAPON_KNIFE_CORD:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_CANIS:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_URSUS:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_GYPSY_JACKKNIFE:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_OUTDOOR:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_STILETTO:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_WIDOWMAKER:
        it("[", "Knife");
        break;
    case WEAPON_KNIFE_SKELETON:
        it("[", "Knife");
        break;
    default:
        it("", "");
        break;
	}

    return txt;
}

#undef it