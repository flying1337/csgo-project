#pragma once

class c_tonemap_controller : public c_entity {
public:
	bool& use_custom_autoexposure_min();
	bool& use_custom_autoexposure_max();
	bool& use_custom_bloom_scale();
	float& custom_autoexposure_max();
	float& custom_autoexposure_min();
	float& custom_bloom_scale();
};