#pragma once

class c_bomb : public c_entity {
public:
	float& timer();
	bool& is_ticking();
	int& site();
	float& defusetime();
	float& blowtime();
	c_ehandle& defuser();
	float& defuse();
	bool& is_defused();

	int get_damage(c_player* p);
};