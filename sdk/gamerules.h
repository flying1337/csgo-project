#pragma once

class c_gamerules {
public:
	bool& match_paused();
	bool& freeze_period();
	bool& warmup_period();
	bool& bomb_planted();
	bool& is_valve_ds();
	bool& is_matchmaking();
};