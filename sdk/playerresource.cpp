#include "../cheat.h"

int& c_playerresources::c4_carrier() {
	static auto n = csgo->netvars[str("DT_CSPlayerResource")][str("m_iPlayerC4")];
	return g_offset(int, this, n.offset);
}

int& c_playerresources::ping(int entnum) {
	static auto n = csgo->netvars[str("DT_CSPlayerResource")][str("baseclass")][str("m_iPing")];
	return g_offset(int, this, n.offset + (entnum * sizeof(int)));
}