#pragma once

struct s_player {
	ImDrawList* d = 0;
	c_player* p = 0;
	ImVec2 mins, maxs;
	int alpha = 0;
	int health = 0;
	float dormancy_entry_time = 0.f;
	bool is_offscreen = false;
	bool is_dormant = false;
	int real_health = 0;
	c_vector origin;
	s_player_info info;
	bool has_helmet;
	bool has_heavy_armor;
	int armor;
	int money;
	bool is_scoped;
	bool is_fake_ducking;
	int index;
	c_ehandle weapon;
};

struct s_weapon {
	ImDrawList* d = 0;
	c_weapon* w = 0;
	ImVec2 mins, maxs;
	int alpha = 0;
};

struct s_projectile {
	ImDrawList* d = 0;
	c_projectile* p = 0;
	ImVec2 mins, maxs;
};

struct s_hitmarker {
	int alpha = 0;

	float lt[4]{ 0, 0, 0, 0 };
	float rt[4]{ 0, 0, 0, 0 };
	float rb[4]{ 0, 0, 0, 0 };
	float lb[4]{ 0, 0, 0, 0 };
};

class c_visuals {
public:
	void draw(ImDrawList* d);
	void work();
	void update_positions();
	
	void hitmarker();
	void clear();

	ImColor percent_to_color(int p, int alpha = 255);

    std::unordered_map<int, s_player> players;

private:
	void draw_players(ImDrawList* d);
	void draw_hitmarker(ImDrawList* d);
	void draw_weapons(ImDrawList* d);
	void draw_projectiles(ImDrawList* d);
	void draw_hostages(ImDrawList* d);

	void draw_player_box(s_player p);
	void draw_player_health(s_player p);
	void draw_player_name(s_player p);
	void draw_player_sidebar(s_player p);
	void draw_player_bottom(s_player p);

	void draw_aw_damage(ImDrawList* d);

	void draw_weapon_box(s_weapon w);
	void draw_weapon_bottom(s_weapon w);
	void draw_weapon_sidebar(s_weapon w);

	void draw_projectile_box(s_projectile w);
	void draw_projectile_bottom(s_projectile w);
	void draw_molotov(ImDrawList* d);

	std::unordered_map<int, c_vector> player_origin_data;
	std::unordered_map<int, s_weapon> weapons;
	std::unordered_map<int, s_projectile> projectiles;

	s_hitmarker hm;
};