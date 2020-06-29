#include "../cheat.h"

bool& c_gamerules::match_paused() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bMatchWaitingForResume")];
	return g_offset(bool, this, n.offset);
}

bool& c_gamerules::freeze_period() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bFreezePeriod")];
	return g_offset(bool, this, n.offset);
}

bool& c_gamerules::warmup_period() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bWarmupPeriod")];
	return g_offset(bool, this, n.offset);
}

bool& c_gamerules::bomb_planted() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bBombPlanted")];
	return g_offset(bool, this, n.offset);
}

bool& c_gamerules::is_valve_ds() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bIsValveDS")];
	return g_offset(bool, this, n.offset);
}

bool& c_gamerules::is_matchmaking() {
	static auto n = csgo->netvars[str("DT_CSGameRulesProxy")][str("cs_gamerules_data")][str("m_bIsQueuedMatchmaking")];
	return g_offset(bool, this, n.offset);
}