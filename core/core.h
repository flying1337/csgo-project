#pragma once

class c_core {
public:
	void initialize();
	void crash(const char* message, ...);
	void dump_netvars();

	bool is_fully_loaded = false;
	bool is_dx_loaded = false;
	bool is_unload_triggered = false;
	bool is_loadanim_shown = false;
	bool is_config_working = false;
	bool is_updating_entites = false;

	bool reset_fsn = false;
	bool reset_bloom = false;

	float loadanim_alpha = 0.f;
	float loadanim_lasttime = 0.f;

	int hide_from_obs = 0;
	int loadanim_spinner = 0;
	
	std::string loadanim_text = "";

	char username[64] = { 0 };
	char password[64] = { 0 };

private:
	bool verify();
	bool load_files();
	bool load_interfaces();
	bool load_interfaces_dx();
	bool load_hooks();
	bool load_dx();
	bool load_netvars();
	bool load_others();
};