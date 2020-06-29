#pragma once

class c_netvar {
public:
	int offset = -1;
	e_sendproptype type;

	std::unordered_map<std::string, c_netvar> child;

	c_netvar& operator[](std::string key) {
		return child[key];
	}
};

struct s_shot {
    c_player* player;
    c_vector point;
    int tick = 0;
    bool hit = false;
    bool hurt = false;
    int damage = 0;
    int side = 0;
    float lby = 0;
    bool hc = false;
    c_matrix3x4 bones[128];
    c_vector abspos;
    c_angle absyaw;
    int hitgroup = 0;
    int hurtgroup = 0;
    c_vector shotpos;
    int hitbox = 0;
};

struct s_aimpoint {
    c_vector point;
    int alpha = 255;

    bool operator==(s_aimpoint v) { return v.point == point && v.alpha == alpha; }
};

class c_csgo {
public:
	c_hl* hl = nullptr;
	c_engine* engine = nullptr;
	c_cvar* cvar = nullptr;
	c_eventmanager* events = nullptr;
	c_entitylist* entities = nullptr;
	c_modelrender* modelrender = nullptr;
	c_materialsystem* materialsystem = nullptr;
	c_renderview* renderview = nullptr;
	c_debugoverlay* debugoverlay = nullptr;
	c_globals* globals = nullptr;
	c_modelinfo* modelinfo = nullptr;
	c_enginetrace* enginetrace = nullptr;
	c_physics* physics = nullptr;
	c_movement* movement = nullptr;
	c_prediction* prediction = nullptr;
	c_surface* surface = nullptr;
	c_glowmanager* glowmanager = nullptr;
    c_move* movehelper = nullptr;
	c_beamrenderer* beamrenderer = nullptr;
	c_enginememory* memory = nullptr;
	c_clientstate* clientstate = nullptr;
	c_localize* localize = nullptr;
	c_input* input = nullptr;

	void* panel = nullptr;
	void* enginesound = nullptr;
	void* clientmode = nullptr;

public:
	c_matrix4x4 w2s_matrix;

public:
	void cache_entities();
	void force_recache() { old_num_entities = 0; }

	c_entity* find_cached(int num);
	c_entity* find_cached(c_ehandle& const h);
	
	c_weaponinfo* weapon_infos[4096] = { 0 };

	c_player* local_player = nullptr;
	c_tonemap_controller* tonemap_controller = nullptr;
	c_bomb* bomb = nullptr;
	c_gamerules* gamerules = nullptr;
	c_playerresources* playerresources = nullptr;
	c_fogcontroller* fog_controller = nullptr;

	bool is_in_tp = false;
	bool allow_sp = true;
	bool want_sp = true;
	bool aa_worked = false;
	bool aa_desync = false;
	bool allow_anims = true;
	bool is_bomb_ticking = false;
	bool switch_sidemove = false;
	bool last_allow_sp = true;
	bool is_freezetime = false;
	bool is_in_sendmove = false;
	bool is_first_sendmove = false;
	bool mitigate_fps = false;

	bool goes_thru[65] = {false};
    bool in_camera = false;
	int choke = 0;
	int aw_damage = -1;
	int next_sidemove = 0;
	int shift = 0;
	int last_slot = 0;
	int recharge = 0;
	int next_shift = 0;
    int last_vel_mod_tick = 0;
	int brute_stage[65] = {0};
	int baim_stage[65] = {0};
	int resolver_mode[65] = {0};

	float gfy_delta = 0;
	float proper_abs = 0;
	float next_update = 0;
	float body_delta = 0;
	float servertime = 0;
    float last_vel_mod = 0.f;

	float resolver_simetimes[65] = {0};

	c_usercmd* cmd;

	c_angle unsent;
	c_angle sent;
	c_angle real;
	c_vector aimpoint;
	c_vector sentpos;
	c_vector velocity;

	s_shot shot;

	c_matrix3x4 body_bones[128] = {};
	c_matrix3x4 real_bones[128] = {};
	c_matrix3x4 scan_bones[128] = {};

	std::vector<int> packets;
	std::vector<s_aimpoint> aimpoints;

	ptr sendpacket = 0;

	HWND window = 0;

private:
	int old_num_entities = 0;

public:
	std::unordered_map<std::string, std::unordered_map<std::string, c_netvar>> netvars;
};