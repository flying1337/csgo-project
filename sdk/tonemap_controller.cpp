#include "../cheat.h"

bool& c_tonemap_controller::use_custom_autoexposure_min() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_bUseCustomAutoExposureMin")];
	return g_offset(bool, this, n.offset);
}

bool& c_tonemap_controller::use_custom_autoexposure_max() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_bUseCustomAutoExposureMax")];
	return g_offset(bool, this, n.offset);
}

bool& c_tonemap_controller::use_custom_bloom_scale() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_bUseCustomBloomScale")];
	return g_offset(bool, this, n.offset);
}

float& c_tonemap_controller::custom_autoexposure_max() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_flCustomAutoExposureMax")];
	return g_offset(float, this, n.offset);
}

float& c_tonemap_controller::custom_autoexposure_min() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_flCustomAutoExposureMin")];
	return g_offset(float, this, n.offset);
}

float& c_tonemap_controller::custom_bloom_scale() {
	static auto n = csgo->netvars[str("DT_EnvTonemapController")][str("m_flCustomBloomScale")];
	return g_offset(float, this, n.offset);
}
